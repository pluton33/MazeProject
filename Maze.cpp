//
// Created by Sebastian on 10/12/2025.
//

#include "Maze.h"

#include <fstream>
#include <iostream>


Maze::Maze(int rows, int cols) : rows(rows), cols(cols) {};

bool Maze::readBoard(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Nie mozna otworzyc pliku: " << filename << std::endl;
        return false;
    }

    file >> rows >> cols;
    board.clear();
    std::string dummy;
    std::getline(file, dummy);

    std::string line;
    for (int i = 0; i < rows; ++i) {
        std::getline(file, line);

        if (line.empty()) { i--; continue; }

        if (line.length() != static_cast<size_t>(cols)) {

            if (line.length() < static_cast<size_t>(cols)) {
                 std::cerr << "Bledny rzad w linii " << i+2 << " Dlugosc: " << line.length() << std::endl;
                 return false;
            }
        }
        board.push_back(line.substr(0, cols));
    }
    return true;
}

bool Maze::createBoard(int x, int y) {
    rows = y;
    cols = x;
    board.clear();
    std::string line;
    for (int i = 0; i < y; ++i) {
        line = randomLine(x);
        board.push_back(line);
    }
    return true;
}

void Maze::changeWall(int row, int col) {
    char& cell = board[row][col];
    if (cell == 'B') cell = 'C'; else if (cell == 'C') cell = 'B';
}

void Maze::printBoard() const {
    std::cout << "-------------" << std::endl;
    for (std::string row : board) {
        std::cout << row << std::endl;
    }
}

std::string Maze::randomLine(int length) {
    std::string result;
    result.reserve(length);

    for (size_t i = 0; i < length; ++i) {

        result += (rand() % 3 != 0) ? 'B' : 'C';
    }

    return result;
}

bool Maze::isBlocked(int row, int col) {
    char& cell = board[row][col];
    if (cell == 'B' || cell == 'M') return false;
    return true;
}

int Maze::activateCell(int row, int col) {
    char& cell = board[row][col];
    if (cell == 'B' || cell == 'M') {
        cell = 'G';
        return 1;
    }
    return 0;
}

int Maze::markCell(int row, int col) {
    char& cell = board[row][col];
    if (cell == 'B') {
        cell = 'M';
        return 1;
    }
    return 0;
}

int Maze::deactivateCell(int row, int col) {
    char& cell = board[row][col];
    if (cell == 'G') {
        cell = 'B';
        return 1;
    }
    return 0;
}

void Maze::clearPaths() {
    for (int row = 0; row < rows; ++row) {
        for (int col = 0; col < cols; ++col) {
            if (board[row][col] != 'C') {
                board[row][col] = 'B';
            }
        }
    }
}

std::vector<std::string> Maze::getBoard() const {
    return board;
}

int Maze::getRows() const { return rows; }
int Maze::getCols() const { return cols; }
