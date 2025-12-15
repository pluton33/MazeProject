//
// Created by Sebastian on 12/12/2025.
//

#include "MazeScreen.h"

#include "SFML/Graphics/RectangleShape.hpp"

MazeScreen::MazeScreen(Maze &maze, sf::RenderWindow &window, Player &player) : maze(maze), window(window),
                                                                               virtualSize(512.0f, 512.0f),
                                                                               player(player) {
    int cols = maze.getCols();
    cellSize = virtualSize.x / static_cast<float>(cols);
    resizeView();
}

void MazeScreen::draw() const {
    int rows = maze.getRows();
    int cols = maze.getCols();

    // float cellWidth = static_cast<float>(WINDOW_WIDTH) / cols;
    // float cellHeight = static_cast<float>(WINDOW_HEIGHT) / rows;
    // player.update(maze);

    for (int row = 0; row < rows; ++row) {
        for (int col = 0; col < cols; ++col) {
            sf::RectangleShape cellShape(sf::Vector2f(cellSize - 2.0f, cellSize - 2.0f));
            cellShape.setPosition(sf::Vector2f(row * cellSize, col * cellSize));

            char cell = maze.getBoard()[col][row];

            if (cell == 'B') cellShape.setFillColor(sf::Color::White);
            else if (cell == 'C') cellShape.setFillColor(sf::Color::Black);
            else if (cell == 'G') cellShape.setFillColor(pathColor);
            else if (cell == 'M') cellShape.setFillColor(sf::Color::Blue);
            else cellShape.setFillColor(sf::Color::Cyan);
            // window.setView(window.getDefaultView());
            window.draw(cellShape);
        }
    }
}

void MazeScreen::handleEvents(const sf::Event &event) {
    if (const sf::Event::KeyPressed *key = event.getIf<sf::Event::KeyPressed>()) {
        handleKeyPressed(*key);
    } else if (event.is<sf::Event::Resized>()) {
        resizeView();
    }
}

void MazeScreen::toggleBlock(int row, int col) {
}

void MazeScreen::winGame() {
    gameState = GameState::PAUSED;
    pathColor = sf::Color::Green;
    //wypisz ruchy(player.moves)
}

void MazeScreen::restartGame() {
    player.resetPosition();
    maze.createBoard();
    maze.printBoard();
    gameState = GameState::RUNNING;
    pathColor = sf::Color::Magenta;
}

void MazeScreen::updateGame() {
    if (gameState == GameState::RUNNING) {
        player.update(maze);
        if (player.checkForWin(maze)) {
            winGame();
        }
    }
}

void MazeScreen::handleKeyPressed(const sf::Event::KeyPressed &keyPressed) {
    if (keyPressed.code == sf::Keyboard::Key::Space) {
        restartGame();
    }

    if (gameState == GameState::RUNNING) {
        if (player.isHuman()) {
            if (keyPressed.code == sf::Keyboard::Key::Left) {
                player.makeMove(maze, 'L');
            } else if (keyPressed.code == sf::Keyboard::Key::Right) {
                player.makeMove(maze, 'P');
            } else if (keyPressed.code == sf::Keyboard::Key::Down) {
                player.makeMove(maze, 'D');
            }
        }
        if (keyPressed.code == sf::Keyboard::Key::Up) {
            player.makeMove(maze, 'G');
        } else if (keyPressed.code == sf::Keyboard::Key::Backspace) {
            player.undoMove(maze);
        } else if (keyPressed.code == sf::Keyboard::Key::B) {
            player.activate();
        }
    }
    draw();
    maze.printBoard();
    if (player.checkForWin(maze)) {
        winGame();
    }
}

void MazeScreen::resizeView() {
    float targetWidth = static_cast<float>(window.getSize().x) * 0.75f;
    float targetHeight = static_cast<float>(window.getSize().y);
    float squareSide = std::min(targetWidth, targetHeight);
    float offsetX = (targetWidth - squareSide) / 2.0f;
    float offsetY = (targetHeight - squareSide) / 2.0f;
    view.setSize(sf::Vector2f(virtualSize.x, virtualSize.y));
    view.setCenter(sf::Vector2f(virtualSize.x / 2.0f, virtualSize.y / 2.0f));
    view.setViewport(sf::FloatRect({offsetX / window.getSize().x, offsetY / window.getSize().y},
                                   {squareSide / window.getSize().x, squareSide / window.getSize().y}));
}

void MazeScreen::startGame() {
    gameState = GameState::RUNNING;
}
