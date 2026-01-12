#include "ComputerBFTPlayer.h"
#include <iostream>
#include <algorithm>


void ComputerBFTPlayer::initSearch(Maze &maze) {

    int rows = maze.getRows();
    int cols = maze.getCols();
    // switchSide(maze);
    // switchSide(maze);


    visited.assign(rows, std::vector<bool>(cols, false));
    parentMove.assign(rows, std::vector<char>(cols, 0));


    std::queue<Node> empty;
    std::swap(bfsQueue, empty);

    startRow = (startSideRowNumber == 0) ? 0 : rows - 1;
    endRow = (startSideRowNumber == 0) ? rows - 1 : 0;
    std::cout << "start row: " << startSideRowNumber << std::endl;


    for (int c = 0; c < cols; c++) {

        if (!maze.isBlocked(startRow, c)) {
            bfsQueue.push({startRow, c});
            visited[startRow][c] = true;


            maze.markCell(startRow, c);
        }
    }

    searchStarted = true;
}

void ComputerBFTPlayer::performBFSStep(Maze &maze) {

    if (bfsQueue.empty()) {
        std::cout << "Koniec przeszukiwania - brak wyjscia!" << std::endl;

        return;
    }


    int stepsPerFrame = 1;

    for (int k = 0; k < stepsPerFrame && !bfsQueue.empty(); k++) {
        Node curr = bfsQueue.front();
        bfsQueue.pop();

        if (curr.r == endRow) {
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

            std::queue<Node> empty;
            std::swap(bfsQueue, empty);
            return;
        }

        int dr[] = {0, 0, 1, -1};
        int dc[] = {1, -1, 0, 0};
        char moveChars[] = {'P', 'L', 'D', 'G'};

        for (int i = 0; i < 4; i++) {
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
                maze.markCell(nr, nc);
            }
        }
    }
}

void ComputerBFTPlayer::update(Maze &maze) {
    if (!isActivated) {
        return;
    }
    if (!searchStarted) {
        initSearch(maze);
        return;
    }

    if (!pathFound) {
        performBFSStep(maze);
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


void ComputerBFTPlayer::resetPosition() {
    Player::resetPosition();

    searchStarted = false;
    pathFound = false;
    pathIndex = 0;
    path = "";

    std::queue<Node> empty;
    std::swap(bfsQueue, empty);


    visited.clear();
    parentMove.clear();
    isActivated = false;

    // isActivated = true;
}
