#include "ComputerDFSPlayer.h"
#include <iostream>
#include <algorithm>
#include <stack>

void ComputerDFSPlayer::initSearch(Maze &maze) {
    int rows = maze.getRows();
    int cols = maze.getCols();

    visited.assign(rows, std::vector<bool>(cols, false));
    parentMove.assign(rows, std::vector<char>(cols, 0));

    std::stack<DFSNode> empty;
    std::swap(dfsStack, empty);



    // POPRAWKA 1: Startujemy fizycznie tam gdzie stoi gracz
    startRow = this->row;
    endRow = (startSideRowNumber == 0) ? rows - 1 : 0;

    dfsStack.push({startRow, column});
    visited[startRow][column] = true;
    maze.markCell(startRow, column);
    std::cout << "Mark cell init:" << startRow << ' ' << column << std::endl;

    searchStarted = true;
}

void ComputerDFSPlayer::performDFSStep(Maze &maze) {
    if (dfsStack.empty()) {
        std::cout << "DFS: brak wyjścia lub stos pusty\n";
        return;
    }

    DFSNode curr = dfsStack.top();

    // --- SPRAWDZENIE CELU ---
    if (curr.r == endRow) {
        path.clear();
        int r = curr.r;
        int c = curr.c;

        // Rekonstrukcja ścieżki
        while (r != startRow || c != column) {
            char mv = parentMove[r][c];
            if (mv == 0) break;
            path.push_back(mv);

            if (mv == 'G') r++;
            else if (mv == 'D') r--;
            else if (mv == 'L') c++;
            else if (mv == 'P') c--;
        }

        std::reverse(path.begin(), path.end());
        char entryChar = (startRow == 0) ? 'D' : 'G';

        size_t entryPos = path.find(entryChar);
        if (entryPos != std::string::npos) {
            // Wstawiamy duplikat w tym samym miejscu
            // Np. z "PPPD..." zrobi się "PPPDD..."
            // Np. z "D..." zrobi się "DD..."
            path.insert(entryPos, 1, entryChar);
        }
        pathFound = true;
        pathIndex = 0;

        std::stack<DFSNode> empty;
        std::swap(dfsStack, empty);
        return;
    }

    // --- DEFINICJA RUCHÓW (Priorytet: Dół) ---

    // Kolejność: Dół, Lewo, Prawo, Góra
    // WAŻNE: Ostatnia wartość dr (dla G) to -1!
    int dr[4] = {1, 0, 0, -1};
    int dc[4] = {0, -1, 1, 0};
    char mv[4] = {'D', 'L', 'P', 'G'};

    bool moved = false;

    for (int i = 0; i < 4; i++) {
        if (curr.r == startRow && i == 0 && maze.isBlocked(curr.r,curr.c)) continue;
        int nr = curr.r + dr[i];
        int nc = curr.c + dc[i];

        if (!maze.isBlocked(curr.r, curr.c))std::cout << "aktualny rzad i kolumna: (" << curr.r << " " << curr.c
                  << ") nastepny rzad i kolumna: (" << nr << " " << nc << ")" << std::endl;

        // 1. Sprawdzanie granic
        if (nr < 0 || nr >= maze.getRows() || nc < 0 || nc >= maze.getCols())
            continue;

        // 2. Jeśli już odwiedzone - pomiń
        if (visited[nr][nc])
            continue;

        // 3. Sprawdzanie ścian
        bool allowed = false;

        // WYJĄTEK DLA STARTU:
        // Jeśli jesteśmy w rzędzie startowym i idziemy w bok (nr == startRow),
        // to ignorujemy ściany. Pozwala to znaleźć wejście oddalone od gracza.
        // Jeśli jednak próbujemy zejść w dół (nr != startRow), to szanujemy ściany.
        if (curr.r == startRow && nr == startRow) {
            allowed = true;
        } else {
            allowed = !maze.isBlocked(nr, nc);
        }

        if (allowed) {
            visited[nr][nc] = true;
            parentMove[nr][nc] = mv[i];

            dfsStack.push({nr, nc});
            int sup = maze.markCell(nr, nc); // Wizualizacja (może zamalować ścianę w rzędzie 0, jeśli szuka wejścia)
            if ( sup)std::cout << "Mark cell performStep:" << nr << ' ' << nc << std::endl;

            moved = true;
            break; // Przerywamy pętlę - w DFS idziemy w głąb pierwszego znalezionego
        }
    }

    // --- BACKTRACKING ---
    if (!moved) {
        dfsStack.pop();
        // Jeśli masz funkcję do czyszczenia koloru, możesz ją tu dodać:
        // maze.restoreCell(curr.r, curr.c);
    }
}

void ComputerDFSPlayer::update(Maze &maze) {
    if (!isActivated) return;

    if (!searchStarted) {
        initSearch(maze);
        return;
    }

    if (!pathFound) {
        // Możesz zwiększyć pętlę, żeby DFS szedł szybciej (np. k < 5)
        for (int k = 0; k < 1; k++) {
            if (pathFound || dfsStack.empty()) break;
            performDFSStep(maze);
        }
        return;
    }

    static int moveDelay = 0;
    if (++moveDelay < 5) return;
    moveDelay = 0;

    if (pathIndex < path.length()) {
        makeMove(maze, path[pathIndex]);
        pathIndex++;
    }
}

void ComputerDFSPlayer::resetPosition() {
    Player::resetPosition();
    searchStarted = false;
    pathFound = false;
    pathIndex = 0;
    path.clear();

    std::stack<DFSNode> empty;
    std::swap(dfsStack, empty);

    visited.clear();
    parentMove.clear();
    isActivated = false;
}
