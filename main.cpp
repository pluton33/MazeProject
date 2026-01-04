#include <SFML/Graphics.hpp>
#include <iostream>
#include <ctime>
#include <optional> // Wymagane w SFML 3

#include "ComputerBFTPlayer.h"
#include "HumanPlayer.h"
#include "Maze.h"
#include "MazeScreen.h"

const int WINDOW_WIDTH = 800;
const int WINDOW_HEIGHT = 600;

int main() {
    std::srand(static_cast<unsigned int>(std::time(nullptr)));
    // HumanPlayer seba;
    //
    // ComputerBFTPlayer seba;
    // Tworzymy inteligentny wskaźnik:
    std::shared_ptr<Player> seba = std::make_shared<HumanPlayer>();
    Maze maze = Maze();
    if (!maze.readBoard("input.txt")) {
        std::cout << "Generuje losowy labirynt 20x15." << std::endl;
        maze = Maze(20, 15);
        maze.createBoard();
    }
    maze.printBoard();


    // seba.update(maze);
    sf::RenderWindow window(sf::VideoMode({(unsigned int)WINDOW_WIDTH, (unsigned int)WINDOW_HEIGHT}), "Labirynt SFML 3");
    MazeScreen mazeScreen(maze, window, seba);
    mazeScreen.startGame();
    while (window.isOpen()) {
        // window.setFramerateLimit(6);
        while (const std::optional event = window.pollEvent()) {

            if (event->is<sf::Event::Closed>()) {
                window.close();
            }

            mazeScreen.handleEvents(*event);
        }
        mazeScreen.updateGame();

        window.clear(sf::Color(50, 50, 50));

        window.setView(mazeScreen.getView());
        mazeScreen.draw();

        window.display();
    }

    return 0;
}
