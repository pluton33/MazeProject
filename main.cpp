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
    // HumanPlayer player;
    //
    // ComputerBFTPlayer seba;
    std::shared_ptr<Player> player = std::make_shared<HumanPlayer>();
    Maze maze = Maze();
    if (!maze.readBoard("input.txt")) {
        std::cout << "Generuje losowy labirynt 15x15." << std::endl;
        maze = Maze(15, 15);
        maze.createBoard();
    }
    maze.printBoard();


    // player.update(maze);
    sf::RenderWindow window(sf::VideoMode({(unsigned int)WINDOW_WIDTH, (unsigned int)WINDOW_HEIGHT}), "Labirynt");
    MazeScreen mazeScreen(maze, window, player);
    mazeScreen.startGame();
    while (window.isOpen()) {
        window.setFramerateLimit(60);
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
