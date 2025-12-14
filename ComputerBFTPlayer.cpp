#include "ComputerBFTPlayer.h"
#include <iostream>
#include <algorithm>

// Funkcja pomocnicza isSafe musi być widoczna (może być static w klasie lub globalna w cpp)
bool isSafe(int r, int c, int rows, int cols, const std::vector<std::string>& board) {
    if (r < 0 || r >= rows || c < 0 || c >= cols) return false;
    if (board[c][r] == 'B') return false; // Pamiętaj o [c][r] vs [r][c] zależnie od Twojej Maze
    return true;
}

void ComputerBFTPlayer::initSearch(Maze &maze) {
    int rows = maze.getRows();
    int cols = maze.getCols();

    // Inicjalizacja struktur
    visited.assign(rows, std::vector<bool>(cols, false));
    parentMove.assign(rows, std::vector<char>(cols, 0));

    // Czyścimy kolejkę na wypadek restartu
    std::queue<Node> empty;
    std::swap(bfsQueue, empty);

    startRow = (startSideRowNumber == 0) ? 0 : rows - 1;
    endRow = (startSideRowNumber == 0) ? rows - 1 : 0;

    // KROK 1: Wrzucamy WSZYSTKIE wejścia do kolejki
    for (int c = 0; c < cols; c++) {
        // Używamy isBlocked lub isSafe
        if (!maze.isBlocked(startRow, c)) {
            bfsQueue.push({startRow, c});
            visited[startRow][c] = true;

            // Opcjonalnie: od razu zaznaczamy wejścia
            maze.markCell(startRow, c);
        }
    }

    searchStarted = true;
}

void ComputerBFTPlayer::performBFSStep(Maze &maze) {
    // Jeśli kolejka pusta, to znaczy że przeszukaliśmy wszystko i nie ma wyjścia
    if (bfsQueue.empty()) {
        std::cout << "Koniec przeszukiwania - brak wyjscia!" << std::endl;
        // Można tu dodać flagę zatrzymującą symulację
        return;
    }

    // Wykonujemy np. 1 lub 5 kroków algorytmu na jedną klatkę gry.
    // Im większa liczba pętli, tym szybsza animacja "rozlewania".
    int stepsPerFrame = 1;

    for(int k=0; k<stepsPerFrame && !bfsQueue.empty(); k++) {
        Node curr = bfsQueue.front();
        bfsQueue.pop();

        // Znaleziono wyjście!
        if (curr.r == endRow) {
            // Odtwarzanie ścieżki (tak jak miałeś)
            path = "";
            int r = curr.r;
            int c = curr.c;

            while (r != startRow) {
                char mv = parentMove[r][c];
                path += mv;
                if (mv == 'P') c--;
                else if (mv == 'L') c++;
                else if (mv == 'D') r--;
                else if (mv == 'G') r++;
            }

            this->column = c;
            this->row = r;

            std::reverse(path.begin(), path.end());
            if (!path.empty()) path = path[0] + path; // Duplikacja pierwszego ruchu

            pathFound = true;
            pathIndex = 0;

            // Czyścimy kolejkę żeby zatrzymać szukanie
            std::queue<Node> empty;
            std::swap(bfsQueue, empty);
            return;
        }

        // Sprawdzanie sąsiadów
        int dr[] = {0, 0, 1, -1};
        int dc[] = {1, -1, 0, 0};
        char moveChars[] = {'P', 'L', 'D', 'G'};

        for(int i=0; i<4; i++) {
            // Twój warunek kierunkowy na starcie
            if (curr.r == startRow) {
                if (startRow == 0 && moveChars[i] != 'D') continue;
                if (startRow != 0 && moveChars[i] != 'G') continue;
            }

            int nr = curr.r + dr[i];
            int nc = curr.c + dc[i];

            if (!maze.isBlocked(nr, nc) && !visited[nr][nc]) {
                visited[nr][nc] = true;
                parentMove[nr][nc] = moveChars[i];
                bfsQueue.push({nr, nc});

                // --- TUTAJ DZIEJE SIĘ MAGIA ANIMACJI ---
                // Oznaczamy komórkę jako odwiedzoną wizualnie.
                // Ponieważ funkcja zaraz wróci do update->draw, zobaczymy to na ekranie!
                maze.markCell(nr, nc);
            }
        }
    }
}

void ComputerBFTPlayer::update(Maze &maze) {
    // 1. Jeśli jeszcze nie zaczęliśmy szukać - zainicjuj
    if (!searchStarted) {
        initSearch(maze);
        return;
    }

    // 2. Jeśli szukamy (pathFound == false), wykonuj kolejne kroki BFS
    if (!pathFound) {
        performBFSStep(maze);
        return;
        // Wracamy z update, system rysuje klatkę z nowymi 'markCell',
        // a w następnej klatce kontynuujemy pętlę.
    }

    // 3. Jeśli ścieżka znaleziona - poruszaj się
    // Opóźnienie ruchu bota (opcjonalne)
    static int moveDelay = 0;
    if (++moveDelay < 5) return;
    moveDelay = 0;

    if (pathIndex < path.length()) {
        makeMove(maze, path[pathIndex]);
        pathIndex++;
    }
}