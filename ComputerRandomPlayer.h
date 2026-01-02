//
// Created by Sebastian on 29/12/2025.
//

#ifndef MAZEPROJECT_COMPUTERRANDOMPLAYER_H
#define MAZEPROJECT_COMPUTERRANDOMPLAYER_H

#include "Player.h"
#include <random>

class ComputerRandomPlayer : public Player {
private:
    bool isActivated = false;
    std::default_random_engine rng;

public:
    ComputerRandomPlayer();
    void update(Maze &maze) override;
    bool isHuman() override { return false; }
    void activate() override { isActivated = !isActivated; }
    void resetPosition() override;
};

#endif //MAZEPROJECT_COMPUTERRANDOMPLAYER_H