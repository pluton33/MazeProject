//
// Created by Sebastian on 10/12/2025.
//

#ifndef MAZEPROJECT_MAZE_H
#define MAZEPROJECT_MAZE_H
#include <vector>


class Maze {
public:
    Maze(int rows = 0, int cols = 0);

    bool readBoard(const std::string& filename);
    bool createBoard(int x, int y);
    bool createBoard() {
        createBoard(cols, rows);
        return true;
    };

    void printBoard() const;

    std::vector<std::string> getBoard() const;
    int getRows() const;
    int getCols() const;

    std::string randomLine(int length);

    bool isBlocked(int row, int col);
    int activateCell(int row, int col);

    int markCell(int row, int col);

    int deactivateCell(int row, int col);

    void clearPaths();
private:
    std::vector<std::string> board;
    int rows;
    int cols;

};


#endif //MAZEPROJECT_MAZE_H