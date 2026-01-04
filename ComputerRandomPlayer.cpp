#include "ComputerRandomPlayer.h"
#include <iostream>
#include <ctime>
#include <algorithm>

void ComputerRandomPlayer::initSearch(Maze &maze) {
    int rows = maze.getRows();
    int cols = maze.getCols();

    // Inicjalizacja tablicy odwiedzin
    visited.assign(rows, std::vector<bool>(cols, false));

    // Inicjalizacja generatora losowego
    rng.seed(std::time(nullptr));

    // Ustalamy start tam, gdzie stoi gracz (tak jak w DFS)
    startRow = this->row;

    // Ustalamy koniec na podstawie strony startowej (zmienna z klasy Player)
    endRow = (startSideRowNumber == 0) ? rows - 1 : 0;

    // Oznaczamy start jako odwiedzony i malujemy
    visited[startRow][this->column] = true;
    maze.markCell(startRow, this->column);

    std::cout << "RandomPlayer: Init search at " << startRow << "," << this->column << std::endl;

    searchStarted = true;
    foundExit = false;
}

void ComputerRandomPlayer::resetToRandomStart(Maze &maze) {
    int rows = maze.getRows();
    int cols = maze.getCols();

    // 1. Czyścimy pamięć odwiedzin dla nowej próby
    for(auto &r : visited) {
        std::fill(r.begin(), r.end(), false);
    }

    // 2. Wracamy do rzędu startowego
    this->row = startRow;

    // 3. Losujemy nową kolumnę w rzędzie startowym, która NIE jest ścianą
    std::vector<int> validCols;
    for (int c = 0; c < cols; c++) {
        if (!maze.isBlocked(startRow, c)) {
            validCols.push_back(c);
        }
    }

    if (!validCols.empty()) {
        std::uniform_int_distribution<int> dist(0, validCols.size() - 1);
        this->column = validCols[dist(rng)];
    } else {
        // Fallback (teoretycznie niemożliwe w poprawnym labiryncie)
        this->column = 0;
    }

    // 4. Resetujemy stan w nowym miejscu
    visited[this->row][this->column] = true;
    maze.markCell(this->row, this->column);

    // std::cout << "RandomPlayer: Zablokowany! Reset do: " << this->row << "," << this->column << std::endl;
}

void ComputerRandomPlayer::performStep(Maze &maze) {
    // Jeśli już znaleźliśmy wyjście, nic nie robimy
    if (this->row == endRow) {
        if (!foundExit) {
            std::cout << "RandomPlayer: Znalazlem wyjscie!" << std::endl;
            foundExit = true;
        }
        return;
    }

    // Definicja ruchów: Dół, Góra, Lewo, Prawo
    int dr[] = {1, -1, 0, 0};
    int dc[] = {0, 0, -1, 1};
    // char moves[] = {'D', 'G', 'L', 'P'}; // Opcjonalne, jeśli chciałbyś logować ruchy

    std::vector<int> validIndices;

    // Szukamy dostępnych sąsiadów
    for (int i = 0; i < 4; i++) {
        int nr = this->row + dr[i];
        int nc = this->column + dc[i];

        // 1. Granice
        if (nr < 0 || nr >= maze.getRows() || nc < 0 || nc >= maze.getCols()) continue;

        // 2. Ściany
        if (maze.isBlocked(nr, nc)) continue;

        // 3. Unikanie pętli (Self-Avoiding Walk w ramach jednej próby)
        if (visited[nr][nc]) continue;

        validIndices.push_back(i);
    }

    if (!validIndices.empty()) {
        // --- WYKONANIE RUCHU ---
        // Wybieramy losowy kierunek z dostępnych
        std::uniform_int_distribution<int> dist(0, validIndices.size() - 1);
        int randIdx = validIndices[dist(rng)];

        // Aktualizacja pozycji gracza (fizyczny ruch)
        this->row += dr[randIdx];
        this->column += dc[randIdx];

        // Oznaczamy nowe pole jako odwiedzone i malujemy
        visited[this->row][this->column] = true;
        maze.markCell(this->row, this->column);

    } else {
        // --- BLOKADA ---
        // Brak dostępnych ruchów -> Resetujemy próbę
        resetToRandomStart(maze);
    }
}

void ComputerRandomPlayer::update(Maze &maze) {
    if (!isActivated) return;

    if (!searchStarted) {
        initSearch(maze);
        return;
    }

    // Wykonujemy kilka kroków na klatkę dla płynniejszej (szybszej) wizualizacji
    // Możesz zmienić 'k < 5' na inną wartość, aby dostosować prędkość
    for (int k = 0; k < 5; k++) {
        if (foundExit) break;
        performStep(maze);
    }
}

void ComputerRandomPlayer::resetPosition() {
    Player::resetPosition();
    searchStarted = false;
    foundExit = false;
    visited.clear();
    isActivated = false;
}