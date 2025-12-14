#include <SFML/Graphics.hpp>
#include <iostream>
#include <ctime>
#include <optional> // Wymagane w SFML 3

#include "HumanPlayer.h"
#include "Maze.h"
#include "MazeScreen.h"

const int WINDOW_WIDTH = 800;
const int WINDOW_HEIGHT = 600;

int main() {
    std::srand(static_cast<unsigned int>(std::time(nullptr)));
    HumanPlayer seba;
    //
    Maze maze("input.txt");
    if (!maze.readBoard()) {
        std::cout << "Generuje losowy labirynt 20x15." << std::endl;
        maze = Maze(20, 15);
        maze.createBoard();
    }
    maze.printBoard();

    sf::RenderWindow window(sf::VideoMode({(unsigned int)WINDOW_WIDTH, (unsigned int)WINDOW_HEIGHT}), "Labirynt SFML 3");
    MazeScreen mazeScreen(maze, window, seba);
    mazeScreen.startGame();
    while (window.isOpen()) {
        // window.setFramerateLimit(60);
        while (const std::optional event = window.pollEvent()) {

            if (event->is<sf::Event::Closed>()) {
                window.close();
            }

            mazeScreen.handleEvents(*event);
        }

        window.clear(sf::Color(50, 50, 50));

        window.setView(mazeScreen.getView());
        mazeScreen.draw();

        window.display();
    }

    return 0;
}
