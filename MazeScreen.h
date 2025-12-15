//
// Created by Sebastian on 12/12/2025.
//

#ifndef MAZEPROJECT_MAZESCREEN_H
#define MAZEPROJECT_MAZESCREEN_H
#include "Maze.h"
#include "Player.h"
#include "SFML/Graphics/RenderWindow.hpp"

enum class GameState {
    PAUSED,
    RUNNING,
};

class MazeScreen {
public:
    MazeScreen(Maze &maze, sf::RenderWindow &window, Player &player);

    void draw() const;

    void handleEvents(const sf::Event &event);

    sf::View getView() const {
        return view;
    }

    void updateGame();
    void startGame();

private:
    Maze &maze;
    Player &player;
    sf::RenderWindow &window;
    sf::View view;
    sf::Color pathColor = sf::Color::Magenta;
    const sf::Vector2f virtualSize;
    float cellSize;
    GameState gameState;


    void toggleBlock(int row, int col);

    void winGame();

    void restartGame();

    void handleKeyPressed(const sf::Event::KeyPressed &keyPressed);

    void resizeView();
};


#endif //MAZEPROJECT_MAZESCREEN_H
