//
// Created by Sebastian on 12/12/2025.
//

#ifndef MAZEPROJECT_MAZESCREEN_H
#define MAZEPROJECT_MAZESCREEN_H
#include "Maze.h"
#include "SFML/Graphics/RenderWindow.hpp"


class MazeScreen {
public:
    MazeScreen(Maze &maze, sf::RenderWindow &window);

    void draw() const;

    void handleEvents(const sf::Event &event);

private:
    Maze &maze;
    sf::RenderWindow &window;
    float cellSize;

    void toggleBlock(int row, int col);

    void handleKeyPressed(const sf::Event::KeyPressed& keyPressed);
};


#endif //MAZEPROJECT_MAZESCREEN_H
