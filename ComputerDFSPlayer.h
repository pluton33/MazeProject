//
// Created by Sebastian on 29/12/2025.
//

#ifndef MAZEPROJECT_COMPUTERDFSPLAYER_H
#define MAZEPROJECT_COMPUTERDFSPLAYER_H

#include "Player.h"
#include <stack>
#include <vector>

struct DFSNode {
    int r, c;
};

class ComputerDFSPlayer : public Player {
private:
    bool pathFound = false;
    bool isActivated = false;
    bool searchStarted = false;
    std::string path;
    size_t pathIndex = 0;

    std::stack<DFSNode> dfsStack;
    std::vector<std::vector<bool>> visited;
    std::vector<std::vector<char>> parentMove;
    int startRow;
    int endRow;
    int startCol;
    // -------------------------------------------------------

    void initSearch(Maze &maze);
    void performDFSStep(Maze &maze, int stepsPerFrame);

public:
    void update(Maze &maze, int stepsPerFrame) override;
    bool isHuman() override { return false; }
    void activate() override { isActivated = !isActivated; }
    void resetPosition() override;
};

#endif //MAZEPROJECT_COMPUTERDFSPLAYER_H