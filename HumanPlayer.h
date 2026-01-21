//
// Created by Sebastian on 13/12/2025.
//

#ifndef MAZEPROJECT_HUMANPLAYER_H
#define MAZEPROJECT_HUMANPLAYER_H
#include "Player.h"


class HumanPlayer : public Player {
public:
    void update(Maze &maze, int stepsPerFrame) override {
    }
    bool isHuman() override {return true;}

    void resetPosition() override;
};


#endif //MAZEPROJECT_HUMANPLAYER_H
