//
// Created by Sebastian on 12/12/2025.
//

#include "MazeScreen.h"

#include "SFML/Graphics/RectangleShape.hpp"

MazeScreen::MazeScreen(Maze &maze, sf::RenderWindow &window) : maze(maze), window(window), virtualSize(512.0f, 512.0f) {
    int cols = maze.getCols();
    cellSize = virtualSize.x / static_cast<float>(cols);
    resizeView();
}

void MazeScreen::draw() const {
    int rows = maze.getRows();
    int cols = maze.getCols();

    // float cellWidth = static_cast<float>(WINDOW_WIDTH) / cols;
    // float cellHeight = static_cast<float>(WINDOW_HEIGHT) / rows;


    for (int row = 0; row < rows; ++row) {
        for (int col = 0; col < cols; ++col) {
            sf::RectangleShape cellShape(sf::Vector2f(cellSize, cellSize));
            cellShape.setPosition(sf::Vector2f(row * cellSize, col * cellSize));

            char cell = maze.getBoard()[col][row];

            if (cell == 'B') cellShape.setFillColor(sf::Color::White);
            else if (cell == 'C') cellShape.setFillColor(sf::Color::Black);
            else cellShape.setFillColor(sf::Color::Red);
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

void MazeScreen::handleKeyPressed(const sf::Event::KeyPressed &keyPressed) {
    if (keyPressed.code == sf::Keyboard::Key::Space) {
        maze.createBoard();
        maze.printBoard();
        draw();
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
