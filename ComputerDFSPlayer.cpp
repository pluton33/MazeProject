#include "ComputerDFSPlayer.h"
#include <iostream>
#include <algorithm>
#include <stack>

void ComputerDFSPlayer::initSearch(Maze &maze) {
    // switchSide(maze);
    int rows = maze.getRows();
    int cols = maze.getCols();

    visited.assign(rows, std::vector<bool>(cols, false));
    parentMove.assign(rows, std::vector<char>(cols, 0)); // Wyzerowanie

    std::stack<DFSNode> empty;
    std::swap(dfsStack, empty);

    startRow = this->row;
    endRow = (startSideRowNumber == 0) ? rows - 1 : 0;

    // --- SKANOWANIE WSZYSTKICH WEJŚĆ ---
    bool foundEntrance = false;
    for (int c = cols - 1 ; c >= 0; c--) {
        // Jeśli pole jest wolne, dodajemy je jako potencjalny punkt startowy
        if (!maze.isBlocked(startRow, c)) {
            dfsStack.push({startRow, c});
            visited[startRow][c] = true;
            maze.markCell(startRow, c); // Oznaczamy wizualnie
            foundEntrance = true;
        }
    }

    if (!foundEntrance) {
        std::cout << "UWAGA: Rzad startowy jest calkowicie zamurowany!" << std::endl;
        // Opcjonalnie: można tu zakończyć grę lub wymusić start w ścianie (jak wolisz)
    }

    searchStarted = true;
}

void ComputerDFSPlayer::performDFSStep(Maze &maze) {
    if (dfsStack.empty()) {
        std::cout << "DFS: brak wyjścia (sprawdzono wszystkie opcje)\n";
        return;
    }

    DFSNode curr = dfsStack.top();

    // --- SPRAWDZENIE CELU ---
    if (curr.r == endRow) {
        path.clear();
        int r = curr.r;
        int c = curr.c;

        // --- REKONSTRUKCJA ---
        // Cofamy się do momentu, aż parentMove == 0.
        // Ponieważ w initSearch nie ustawiliśmy parentMove dla punktów startowych,
        // pętla zatrzyma się dokładnie na tym polu startowym, z którego wyszła zwycięska ścieżka.
        while (true) {
            char mv = parentMove[r][c];
            if (mv == 0) break; // STOP - dotarliśmy do startu

            path.push_back(mv);

            if (mv == 'G') r++;
            else if (mv == 'D') r--;
            else if (mv == 'L') c++;
            else if (mv == 'P') c--;
        }

        std::reverse(path.begin(), path.end());

        // --- SYNCHRONIZACJA ---
        // Zmienne r i c wskazują teraz na ZWYCIĘSKIE wejście.
        // Przenosimy tam gracza.
        this->row = r;
        this->column = c;

        // Dodajemy ruch wejściowy (D/G)
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
    std::array<int, 4> dr{};
    std::array<int, 4> dc{};
    std::array<char, 4> mv{};

    if (startRow == 0) {
        // 2. Przypisanie wartości (działa dzięki std::array)
        dr = {1, 0, 0, -1};
        dc = {0, -1, 1, 0};
        mv = {'D', 'L', 'P', 'G'};
    } else {
        dr = {-1, 0, 0, 1};
        dc = {0, -1, 1, 0};
        mv = {'G', 'L', 'P', 'D'};
    }

    bool moved = false;

    for (int i = 0; i < 4; i++) {
        int nr = curr.r + dr[i];
        int nc = curr.c + dc[i];

        if (nr < 0 || nr >= maze.getRows() || nc < 0 || nc >= maze.getCols()) continue;
        if (visited[nr][nc]) continue;
        if (maze.isBlocked(nr, nc)) continue;

        visited[nr][nc] = true;
        parentMove[nr][nc] = mv[i];

        dfsStack.push({nr, nc});
        maze.markCell(nr, nc);

        moved = true;
        break;
    }

    if (!moved) {
        dfsStack.pop();
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
