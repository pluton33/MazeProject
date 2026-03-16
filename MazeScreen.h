#ifndef MAZEPROJECT_MAZESCREEN_H
#define MAZEPROJECT_MAZESCREEN_H

#include <SFML/Graphics.hpp>
#include <vector>
#include <memory>
#include <string>

#include "Maze.h"
#include "Player.h"
#include "Button.h"

#include "ComputerBFTPlayer.h"
#include "ComputerDFSPlayer.h"
#include "ComputerRandomPlayer.h"
#include "HumanPlayer.h"

enum class InputMode {
    NONE,
    LOAD_FILENAME,
    SAVE_FILE,
    GEN_WIDTH,
    GEN_HEIGHT
};

class MazeScreen {
private:
    int solverSpeed;
    int wallRatio;
    Maze &maze;
    sf::RenderWindow &window;
    sf::Vector2f virtualSize;

    std::shared_ptr<Player> player;

    sf::Font font;
    std::vector<Button> buttons;
    sf::Text pathText;
    sf::Text startSideText;
    float cellSize;
    sf::View view;
    sf::Color pathColor = sf::Color::Magenta;

    bool isTyping = false;
    std::string userInput;
    sf::Text promptText;
    sf::Text inputTextDisplay;
    sf::RectangleShape inputBoxBg;
    InputMode currentInputMode = InputMode::NONE;
    int tempGenWidth = 0;

    enum class GameState {
        RUNNING,
        PAUSED,
        WIN
    };
    GameState gameState = GameState::RUNNING;

    void handleKeyPressed(const sf::Event::KeyPressed &keyPressed);
    void winGame();

    void loadMainMenu();
    void loadSolverMenu();

    void processInputConfirmation();

public:
    sf::View getView() const {
        return view;
    }

    MazeScreen(Maze &maze, sf::RenderWindow &window, std::shared_ptr<Player> player);

    void draw() const;
    void handleEvents(const sf::Event &event);
    void updateGame();
    void resizeView();
    void updateMazeLayout();
    void restartGame();
    void toggleBlock(int row, int col);
    void startGame();
    void updateStartSideText();
};

#endif