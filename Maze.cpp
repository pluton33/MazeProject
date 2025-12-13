//
// Created by Sebastian on 10/12/2025.
//

#include "Maze.h"

#include <fstream>
#include <iostream>

Maze::Maze(const std::string &filename, int rows, int cols) : filename(filename), rows(rows), cols(cols) {}

Maze::Maze(int rows, int cols) : filename(""), rows(rows), cols(cols) {};

bool Maze::readBoard() {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Nie mozna otworzyc pliku: " << filename << std::endl;
        return false;
    }

    file >> rows >> cols;
    board.clear();
    // Ważne: po wczytaniu liczb, trzeba "zjeść" znak nowej linii, zanim użyjemy getline
    std::string dummy;
    std::getline(file, dummy);

    std::string line;
    for (int i = 0; i < rows; ++i) {
        std::getline(file, line);
        // Zabezpieczenie na wypadek pustych linii na końcu pliku
        if (line.empty()) { i--; continue; }

        if (line.length() != static_cast<size_t>(cols)) {
            // Czasami edytory dodają niewidoczne znaki \r na końcu (Windows vs Mac), ignorujemy to dla prostoty
            // w bardziej zaawansowanym kodzie należałoby to oczyścić.
            if (line.length() < static_cast<size_t>(cols)) {
                 std::cerr << "Bledny rzad w linii " << i+2 << " Dlugosc: " << line.length() << std::endl;
                 return false;
            }
        }
        board.push_back(line.substr(0, cols)); // Bierzemy tylko tyle znaków ile trzeba
    }
    return true;
}

bool Maze::createBoard() {
    board.clear(); // Czyścimy na wszelki wypadek
    std::string line;
    for (int i = 0; i < rows; ++i) {
        line = randomLine(this->cols);
        board.push_back(line);
    }
    return true; // POPRAWKA: Zwracamy true po sukcesie
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
        // 'b' - ściana (block), 'c' - korytarz (corridor)
        result += (rand() % 3 != 0) ? 'B' : 'C';
    }

    return result;
}

int Maze::activateCell(int row, int col) {
    char& cell = board[row][col];
    if (cell == 'B') {
        cell = 'G';
        return 1;
    } else if (cell == 'G' || cell == 'C') {
        return 0;
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

std::vector<std::string> Maze::getBoard() const {
    return board;
}

int Maze::getRows() const { return rows; }
int Maze::getCols() const { return cols; }
