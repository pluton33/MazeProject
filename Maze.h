//
// Created by Sebastian on 10/12/2025.
//

#ifndef MAZEPROJECT_MAZE_H
#define MAZEPROJECT_MAZE_H
#include <vector>


class Maze {
public:
    Maze(const std::string& filename = "", int rows = 0, int cols = 0);
    Maze(int rows = 0, int cols = 0);

    bool readBoard();
    bool createBoard();

    std::vector<std::string> getBoard() const;
    int getRows() const;
    int getCols() const;

    // Metoda pomocnicza, mogła być prywatna, ale zostawiam publiczną zgodnie z oryginałem
    std::string randomLine(int length);

private:
    std::string filename;
    std::vector<std::string> board;
    int rows;
    int cols;
};


#endif //MAZEPROJECT_MAZE_H