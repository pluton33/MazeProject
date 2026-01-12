#include "ComputerRandomPlayer.h"
#include <iostream>
#include <ctime>
#include <algorithm>

void ComputerRandomPlayer::initSearch(Maze &maze) {
    int rows = maze.getRows();
    int cols = maze.getCols();
    // switchSide(maze);

    visited.assign(rows, std::vector<bool>(cols, false));

    startRow = this->row;
    startCol = this->column;
    endRow = (startSideRowNumber == 0) ? rows - 1 : 0;

    parentMove.assign(rows, std::vector<char>(cols, 0));

    randomStack.push({startRow, column});

    visited[startRow][column] = true;
    maze.markCell(startRow, column);

    searchStarted = true;

}

void ComputerRandomPlayer::performStep(Maze &maze) {
    std::uniform_int_distribution<int> dist(0, maze.getCols() -1);
    if (randomStack.empty()) {
        return;
    }

    Node curr = randomStack.top();

    if (curr.r == endRow) {
        {
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
            if (startCol > 0) {
                path.insert(0, startCol, 'P');
            }
            char entryChar = (startRow == 0) ? 'D' : 'G';

            size_t entryPos = path.find(entryChar);
            if (entryPos != std::string::npos) {
                path.insert(entryPos, 1, entryChar);
            }
            pathFound = true;
            pathIndex = 0;

            std::stack<Node> empty;
            std::swap(randomStack, empty);
            return;
        }
    }

    int dr[4] = {1, 0, 0, -1};
    int dc[4] = {0, -1, 1, 0};
    char mv[4] = {'D', 'L', 'P', 'G'};

    std::vector<int> directions = {0, 1, 2, 3};
    std::shuffle(directions.begin(), directions.end(), rng);

    bool moved = false;

    for (int dirIndex : directions) {
        int nr = curr.r + dr[dirIndex];
        int nc = curr.c + dc[dirIndex];

        if (nr >= 0 && nr < maze.getRows() && nc >= 0 && nc < maze.getCols() &&
            !maze.isBlocked(nr, nc) && !visited[nr][nc]) {
            randomStack.push({nr, nc});
            visited[nr][nc] = true;
            parentMove[nr][nc] = mv[dirIndex];

            int sup = maze.markCell(nr, nc);
            if (sup) std::cout << "Ruch na: " << nr << " " << nc << std::endl;

            moved = true;
            break;
            }
    }

    if (!moved) {
        std::cout << "Zablokowany w (" << curr.r << "," << curr.c << ")! Reset do startu..." << std::endl;
        std::stack<Node> empty;
        std::swap(randomStack, empty);
        // resetPosition();
        maze.clearPaths();
        for(auto &row : visited) {
            std::fill(row.begin(), row.end(), false);
        }
        for(auto &row : parentMove) std::fill(row.begin(), row.end(), 0);
        do {
            startCol = dist(rng);
        } while (maze.isBlocked(startRow, startCol));

        randomStack.push({startRow, startCol});
        visited[startRow][startCol] = true;
        maze.markCell(startRow, startCol);
    }
}

void ComputerRandomPlayer::update(Maze &maze) {
    if (!isActivated) return;

    if (!searchStarted) {
        initSearch(maze);
        return;
    }

    if (!pathFound) {
        for (int k = 0; k < 1; k++) {
            if (pathFound || randomStack.empty()) break;
            performStep(maze);
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

void ComputerRandomPlayer::resetPosition() {
    Player::resetPosition();
    searchStarted = false;
    pathFound = false;
    pathIndex = 0;
    path.clear();
    startRow = 0;
    column = 0;

    std::stack<Node> empty;
    std::swap(randomStack, empty);

    visited.clear();
    parentMove.clear();
    isActivated = false;
}
