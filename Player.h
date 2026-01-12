//
// Created by Sebastian on 13/12/2025.
//

#ifndef MAZEPROJECT_PLAYER_H
#define MAZEPROJECT_PLAYER_H
#include <string>

#include "Maze.h"


class Player {
public:

    virtual ~Player();

    virtual void update(Maze &maze) = 0;

    virtual bool isHuman() = 0;

    virtual void activate() {};

    virtual void resetPosition();
    void makeMove(Maze &maze, char moveType);

    void undoMove(Maze &maze);

    void switchSide(const Maze &maze);

    int getStartSideRowNumber() const {
        return startSideRowNumber;
    }

    int checkForWin(const Maze &maze) const;

    std::string getMoves() const {
        return moves;
    }

protected:
    std::string moves;
    int column = 0;
    int row = 0;
    int startSideRowNumber = 0;
};


#endif //MAZEPROJECT_PLAYER_H
