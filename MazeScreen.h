//
// Created by Sebastian on 12/12/2025.
//

#ifndef MAZEPROJECT_MAZESCREEN_H
#define MAZEPROJECT_MAZESCREEN_H
#include "Maze.h"
#include "Player.h"
#include "SFML/Graphics/RenderWindow.hpp"


class MazeScreen {
public:
    MazeScreen(Maze &maze, sf::RenderWindow &window, Player &player);

    void draw() const;

    void handleEvents(const sf::Event &event);

    sf::View getView() const {
        return view;
    }

private:
    Maze &maze;
    Player &player;
    sf::RenderWindow &window;
    sf::View view;
    const sf::Vector2f virtualSize;
    float cellSize;

    void toggleBlock(int row, int col);

    void handleKeyPressed(const sf::Event::KeyPressed& keyPressed);

    void resizeView();
};


#endif //MAZEPROJECT_MAZESCREEN_H
