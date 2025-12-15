//
// Created by Sebastian on 14/12/2025.
//

#ifndef MAZEPROJECT_COMPUTERPLAYER_H
#define MAZEPROJECT_COMPUTERPLAYER_H

#include "Player.h"
#include <queue>
#include <vector>

struct Node {
    int r, c;
};

class ComputerBFTPlayer : public Player {
private:
    bool pathFound = false;
    bool isActivated = false;
    bool searchStarted = false; // Flaga: czy już zaczęliśmy szukać?
    std::string path;
    size_t pathIndex = 0; // Zmieniono na size_t dla bezpieczeństwa porównań

    // --- ZMIENNE DO ALGORYTMU BFS (Stan między klatkami) ---
    std::queue<Node> bfsQueue;
    std::vector<std::vector<bool>> visited;
    std::vector<std::vector<char>> parentMove;
    int startRow;
    int endRow;
    // -------------------------------------------------------

    void initSearch(Maze &maze); // Nowa funkcja tylko do startu
    void performBFSStep(Maze &maze); // Funkcja wykonująca jeden krok

public:
    void update(Maze &maze) override;
    bool isHuman() override { return false; } // const i override dla poprawności
    void activate() override { isActivated = !isActivated; }
    void resetPosition() override;
};

#endif // MAZEPROJECT_COMPUTERPLAYER_H

 // MAZEPROJECT_COMPUTERPLAYER_H
