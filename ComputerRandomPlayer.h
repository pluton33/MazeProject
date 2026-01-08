//
// Created by Sebastian on 29/12/2025.
//

#ifndef MAZEPROJECT_COMPUTERRANDOMPLAYER_H
#define MAZEPROJECT_COMPUTERRANDOMPLAYER_H

#include "Player.h"
#include <vector>
#include <random>
#include <stack>

#include "ComputerBFTPlayer.h"

class ComputerRandomPlayer : public Player {
private:
    bool isActivated = false;
    bool searchStarted = false;
    bool foundExit = false;
    bool pathFound = false;
    std::string path;
    size_t pathIndex = 0;

    // Pamięć odwiedzonych pól w BIEŻĄCEJ próbie (aby wykryć zablokowanie)
    std::vector<std::vector<bool>> visited;
    std::stack<Node> randomStack;
    std::vector<std::vector<char>> parentMove;

    // Generator liczb losowych
    std::mt19937 rng;

    int startRow;
    int endRow;
    int startCol;

    // -------------------------------------------------------

    void initSearch(Maze &maze);
    void performStep(Maze &maze);
    // void resetToRandomStart(Maze &maze);

public:
    void update(Maze &maze) override;

    // Zwracamy false, bo to komputer
    bool isHuman() override { return false; }

    // Przełącznik aktywacji (taki sam jak w DFS)
    void activate() override { isActivated = !isActivated; }

    void resetPosition() override;
};

#endif //MAZEPROJECT_COMPUTERRANDOMPLAYER_H