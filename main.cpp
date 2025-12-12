#include <SFML/Graphics.hpp>
#include <iostream>
#include <ctime>
#include <optional> // Wymagane w SFML 3
#include "Maze.h"
#include "MazeScreen.h"

const int WINDOW_WIDTH = 800;
const int WINDOW_HEIGHT = 600;

int main() {
    std::srand(static_cast<unsigned int>(std::time(nullptr)));

    Maze maze("input.txt");
    if (!maze.readBoard()) {
        std::cout << "Generuje losowy labirynt 20x15." << std::endl;
        maze = Maze(20, 15);
        maze.createBoard();
    }

    auto board = maze.getBoard();
    int rows = maze.getRows();
    int cols = maze.getCols();

    float cellWidth = static_cast<float>(WINDOW_WIDTH) / cols;
    float cellHeight = static_cast<float>(WINDOW_HEIGHT) / rows;

    sf::RenderWindow window(sf::VideoMode({(unsigned int)WINDOW_WIDTH, (unsigned int)WINDOW_HEIGHT}), "Labirynt SFML 3");
    MazeScreen mazeScreen(maze, window);

    while (window.isOpen()) {
        // ZMIANA W SFML 3: pollEvent zwraca std::optional
        // window.setFramerateLimit(60);
        while (const std::optional event = window.pollEvent()) {

            // ZMIANA: Sprawdzanie typu zdarzenia
            if (event->is<sf::Event::Closed>()) {
                window.close();
            }
            // Obsługa klawiszy w SFML 3
            // else if (const auto* keyPressed = event->getIf<sf::Event::KeyPressed>()) {
            //     if (keyPressed->code == sf::Keyboard::Key::Space) {
            //         maze.createBoard();
            //         board = maze.getBoard();
            //     }
            // }
            mazeScreen.handleEvents(*event);
        }

        window.clear(sf::Color(50, 50, 50));

        // for (int i = 0; i < rows; ++i) {
        //     for (int j = 0; j < cols; ++j) {
        //         sf::RectangleShape cell(sf::Vector2f(cellWidth - 1.0f, cellHeight - 1.0f));
        //         cell.setPosition(sf::Vector2f(j * cellWidth, i * cellHeight));
        //
        //         char field = board[i][j];
        //
        //         if (field == 'B') cell.setFillColor(sf::Color::Black);
        //         else if (field == 'C') cell.setFillColor(sf::Color::White);
        //         else cell.setFillColor(sf::Color::Red);
        //
        //         window.draw(cell);
        //     }
        // }
        mazeScreen.draw();

        window.display();
    }

    return 0;
}
