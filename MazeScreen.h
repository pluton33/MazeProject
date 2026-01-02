//
// Created by Sebastian on 12/12/2025.
//

#ifndef MAZEPROJECT_MAZESCREEN_H
#define MAZEPROJECT_MAZESCREEN_H

#include <SFML/Graphics.hpp>
#include <vector>
#include <memory> // Dodano dla shared_ptr
#include "Maze.h"
#include "Player.h"
#include "Button.h"

// Dołączamy nagłówki, by móc tworzyć instancje
#include "ComputerBFTPlayer.h"
#include "ComputerDFSPlayer.h"
#include "ComputerRandomPlayer.h"
#include "HumanPlayer.h"

class MazeScreen {
private:
    Maze &maze;
    sf::RenderWindow &window;
    sf::Vector2f virtualSize;

    // ZMIANA: Zamiast referencji, używamy shared_ptr, by móc podmieniać obiekt
    std::shared_ptr<Player> player;

    sf::Font font;
    std::vector<Button> buttons;
    sf::Text pathText;
    float cellSize;
    sf::View view;
    sf::Color pathColor = sf::Color::Magenta;

    enum class GameState {
        RUNNING,
        PAUSED,
        WIN
    };
    GameState gameState = GameState::RUNNING;

    void handleKeyPressed(const sf::Event::KeyPressed &keyPressed);
    void winGame();

    // Metody pomocnicze do UI
    void loadMainMenu();
    void loadSolverMenu();

public:
    sf::View getView() const {
        return view;
    }

    // ZMIANA: Konstruktor przyjmuje shared_ptr
    MazeScreen(Maze &maze, sf::RenderWindow &window, std::shared_ptr<Player> player);

    void draw() const;
    void handleEvents(const sf::Event &event);
    void updateGame();
    void resizeView();
    void updateMazeLayout();
    void restartGame();
    void toggleBlock(int row, int col);
    void startGame();
};

#endif //MAZEPROJECT_MAZESCREEN_H