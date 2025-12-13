//
// Created by Sebastian on 13/12/2025.
//

#include "Player.h"

#include <iostream>
#include <ostream>

Player::~Player() {
}

void Player::makeMove(Maze &maze, char moveType) {
    if (moves.empty()) {
        switch (moveType) {
            case 'P':
                if (column != maze.getCols() - 1) {
                    column++;
                }
                break;
            case 'L':
                if (column != 0) {
                    column--;
                }
                break;
            case 'D':
                if (maze.activateCell(row, column)) {
                    moves += 'D';
                }
                break;
        }
    } else if (moves.empty() && moveType == 'D') {
    } else {
        switch (moveType) {
            case 'P':
                if (maze.activateCell(row, column + 1)) {
                    column++;

                    moves += 'P';
                }
                break;
            case 'L':
                if (maze.activateCell(row, column - 1)) {
                    column--;

                    moves += 'L';
                }
                break;
            case 'D':
                if (maze.activateCell(row + 1, column)) {
                    row++;

                    moves += 'D';
                }
                break;
            case 'G':
                if (maze.activateCell(row - 1, column)) {
                    row--;
                    moves += 'G';
                }
                break;
            default:
                break;
        }
    }
    std::cout << "aktualny rząd: " << row << "aktualna kolumna: " << column << "ruchy: " + moves << std::endl;
}

void Player::undoMove(Maze &maze) {
    char lastMove = moves.back();

    if (maze.deactivateCell(row, column)) {
        switch (lastMove) {
            case 'P':
                column--;
                break;
            case 'L':
                column++;
                break;
            case 'D':
                if (row == 0) {
                    break;
                }
                row--;
                break;
            case 'G':
                if (row == maze.getRows() - 1) {
                    break;
                }
                row++;
                break;
            default:
                break;
        }
    }
    if (!moves.empty()) moves.pop_back();
    std::cout << "aktualny rząd: " << row << "aktualna kolumna: " << column << "ruchy: " + moves << std::endl;
}
