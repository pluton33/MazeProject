#include "ComputerDFSPlayer.h"
#include <iostream>
#include <algorithm>

void ComputerDFSPlayer::initSearch(Maze &maze) {
    int rows = maze.getRows();
    int cols = maze.getCols();

    visited.assign(rows, std::vector<bool>(cols, false));
    parentMove.assign(rows, std::vector<char>(cols, 0));

    std::stack<DFSNode> empty;
    std::swap(dfsStack, empty);

    startRow = (startSideRowNumber == 0) ? 0 : rows - 1;
    endRow = (startSideRowNumber == 0) ? rows - 1 : 0;

    // Dodajemy punkt startowy
    if (!maze.isBlocked(startRow, column)) { // Używamy aktualnej pozycji gracza (column) jako startu X
        dfsStack.push({startRow, column});
        // W DFS oznaczamy jako odwiedzone dopiero po zdjęciu ze stosu lub przy wrzuceniu,
        // tutaj przy wrzuceniu dla spójności wizualnej
        visited[startRow][column] = true;
        maze.markCell(startRow, column);
    } else {
        // Fallback jeśli gracz stoi w ścianie (nie powinno się zdarzyć)
        for (int c = 0; c < cols; c++) {
            if (!maze.isBlocked(startRow, c)) {
                dfsStack.push({startRow, c});
                visited[startRow][c] = true;
                maze.markCell(startRow, c);
                break;
            }
        }
    }

    searchStarted = true;
}

void ComputerDFSPlayer::performDFSStep(Maze &maze) {
    if (dfsStack.empty()) {
        std::cout << "Koniec przeszukiwania (DFS) - brak wyjscia lub stos pusty!" << std::endl;
        return;
    }

    // Wykonujemy kilka kroków na klatkę dla wizualizacji
    int stepsPerFrame = 1;

    for (int k = 0; k < stepsPerFrame && !dfsStack.empty(); k++) {
        DFSNode curr = dfsStack.top();
        dfsStack.pop();

        if (curr.r == endRow) {
            path = "";
            int r = curr.r;
            int c = curr.c;

            while (r != startRow || c != this->column) { // Cofamy się do startu
                char mv = parentMove[r][c];
                if (mv == 0) break;
                path += mv;
                if (mv == 'P') c--;
                else if (mv == 'L') c++;
                else if (mv == 'D') r--;
                else if (mv == 'G') r++;
            }

            // Ustawiamy pozycję gracza na koniec (teoretycznie, wizualnie przejdzie tam animacja)
            // this->column = curr.c; // Opcjonalne, zależy czy chcemy teleportować
            // this->row = curr.r;

            std::reverse(path.begin(), path.end());

            pathFound = true;
            pathIndex = 0;

            std::stack<DFSNode> empty;
            std::swap(dfsStack, empty);
            return;
        }

        // Kolejność: G, D, L, P (odwrotna do wrzucania na stos, by priorytetyzować kierunki)
        int dr[] = {0, 0, 1, -1};
        int dc[] = {1, -1, 0, 0};
        char moveChars[] = {'P', 'L', 'D', 'G'};

        for (int i = 0; i < 4; i++) {
            // Blokada powrotu na start side (opcjonalne, zachowane z logiki BFT)
            if (curr.r == startRow) {
               if (startRow == 0 && moveChars[i] != 'D') continue;
               if (startRow != 0 && moveChars[i] != 'G') continue;
            }

            int nr = curr.r + dr[i];
            int nc = curr.c + dc[i];

            if (!maze.isBlocked(nr, nc) && !visited[nr][nc]) {
                visited[nr][nc] = true;
                parentMove[nr][nc] = moveChars[i];
                dfsStack.push({nr, nc});
                maze.markCell(nr, nc);
            }
        }
    }
}

void ComputerDFSPlayer::update(Maze &maze) {
    if (!isActivated) return;

    if (!searchStarted) {
        initSearch(maze);
        return;
    }

    if (!pathFound) {
        performDFSStep(maze);
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
    path = "";

    std::stack<DFSNode> empty;
    std::swap(dfsStack, empty);

    visited.clear();
    parentMove.clear();
    isActivated = false;
}