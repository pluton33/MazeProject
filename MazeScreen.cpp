//
// Created by Sebastian on 12/12/2025.
//

#include "MazeScreen.h"

#include <iostream>

#include "Button.h"
#include "SFML/Graphics/RectangleShape.hpp"

MazeScreen::MazeScreen(Maze &maze, sf::RenderWindow &window, Player &player) : maze(maze), window(window),
                                                                               virtualSize(512.0f, 512.0f),
                                                                               player(player),
                                                                               pathText(font) {
    virtualSize.x = 512.0f;

    int cols = maze.getCols();
    int rows = maze.getRows();

    Button restartBtn(160.f, 40.f, "Reset", font);
    restartBtn.setOnClick([this]() {
        std::cout << "Restart klikniety!" << std::endl;
        this->maze.createBoard();
        this->restartGame();
    });
    buttons.push_back(restartBtn);
    Button botBtn(160.f, 40.f, "Rozwiaz", font);
    botBtn.setOnClick([this]() {
        std::cout << "Bot aktywowany!" << std::endl;
        this->player.activate();
    });
    buttons.push_back(botBtn);
    Button loadBtn(160.f, 40.f, "Wczytaj z Pliku", font);
    loadBtn.setOnClick([this]() {
        std::string filename;
        std::cout << "\n--- WCZYTYWANIE LABIRYNTU ---" << std::endl;
        std::cout << "Podaj nazwe pliku (np. input.txt): ";
        std::cin >> filename;


        if (this->maze.readBoard(filename)) {
             std::cout << "Udalo sie wczytac!" << std::endl;
             this->restartGame();
            updateMazeLayout();
        } else {
             std::cout << "Blad: Nie udalo sie otworzyc pliku." << std::endl;
        }
    });
    buttons.push_back(loadBtn);


    Button genBtn(160.f, 40.f, "Generuj Wlasny", font);
    genBtn.setOnClick([this]() {
        int w, h;
        std::cout << "\n--- GENEROWANIE LABIRYNTU ---" << std::endl;
        std::cout << "Podaj liczbe kolumn (szerokosc): ";
        std::cin >> h;
        std::cout << "Podaj liczbe wierszy (wysokosc): ";
        std::cin >> w;


        if (w < 5 || h < 5) {
            std::cout << "Za male wymiary! Minimum 5x5." << std::endl;
            return;
        }

        this->maze.createBoard(h, w);

        std::cout << "Wygenerowano nowy labirynt " << w << "x" << h << std::endl;
        this->restartGame();
        updateMazeLayout();
    });
    buttons.push_back(genBtn);


    cellSize = virtualSize.x / static_cast<float>(cols);


    virtualSize.y = cellSize * static_cast<float>(rows);


    if (!font.openFromFile("arial.ttf")) {
        throw std::runtime_error("Nie można załadować czcionki");
    }

    pathText.setFont(font);
    pathText.setCharacterSize(18);
    pathText.setFillColor(sf::Color::White);
    pathText.setString("");
    pathText.setPosition({20.0f, float(window.getSize().y-25.0f)});
    resizeView();


}

void MazeScreen::draw() const {
    int rows = maze.getRows();
    int cols = maze.getCols();

    // float cellWidth = static_cast<float>(WINDOW_WIDTH) / cols;
    // float cellHeight = static_cast<float>(WINDOW_HEIGHT) / rows;
    // player.update(maze);

    sf::RectangleShape cellShape(sf::Vector2f(cellSize - 2.0f, cellSize - 2.0f));
    for (int row = 0; row < rows; ++row) {
        for (int col = 0; col < cols; ++col) {
            cellShape.setPosition(sf::Vector2f(col * cellSize, row * cellSize));

            char cell = maze.getBoard()[row][col];

            if (cell == 'B') cellShape.setFillColor(sf::Color::White);
            else if (cell == 'C') cellShape.setFillColor(sf::Color::Black);
            else if (cell == 'G') cellShape.setFillColor(pathColor);
            else if (cell == 'M') cellShape.setFillColor(sf::Color::Blue);
            else cellShape.setFillColor(sf::Color::Cyan);
            // window.setView(window.getDefaultView());
            window.draw(cellShape);

        }

    }
    window.setView(window.getDefaultView());
    window.draw(pathText);
    for (const auto& btn : buttons) {
        btn.render((window));
    }
}

void MazeScreen::handleEvents(const sf::Event &event) {
    if (const sf::Event::KeyPressed *key = event.getIf<sf::Event::KeyPressed>()) {
        handleKeyPressed(*key);
    } else if (event.is<sf::Event::Resized>()) {
        resizeView();
    }
    if (const sf::Event::MouseButtonPressed* mouse = event.getIf<sf::Event::MouseButtonPressed>()) {
        if (mouse->button == sf::Mouse::Button::Left) {
            sf::Vector2f mousePos = window.mapPixelToCoords(mouse->position);
            for (const auto& btn : buttons) {
                if (btn.checkClick(mousePos)) {
                    break; // Kliknięto, przerywamy sprawdzanie reszty
                }
            }

        }
    }
}

void MazeScreen::toggleBlock(int row, int col) {
}

void MazeScreen::winGame() {
    gameState = GameState::PAUSED;
    pathColor = sf::Color::Green;
    pathText.setString("Ruchy rozwiazania: " + player.getMoves());
    std::cout << "ruchy gracza: " + player.getMoves() << std::endl;
}

void MazeScreen::restartGame() {
    player.resetPosition();
    // maze.createBoard();
    maze.printBoard();
    gameState = GameState::RUNNING;
    pathColor = sf::Color::Magenta;
    pathText.setString("");
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
        if (/*player.isHuman()*/ true) {
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
    float windowWidth = static_cast<float>(window.getSize().x);
    float windowHeight = static_cast<float>(window.getSize().y);

    // Obszar dostępny dla labiryntu (np. 75% szerokości okna)
    float targetWidth = windowWidth * 0.75f;
    float targetHeight = windowHeight - 60.0f;

    // Oblicz proporcje labiryntu i dostępnego miejsca
    float mazeAspectRatio = virtualSize.x / virtualSize.y;
    float windowAspectRatio = targetWidth / targetHeight;

    float viewWidth, viewHeight;
    window.setView(window.getDefaultView());
    // pathText.setPosition();

    // Algorytm "Letterboxing" - dopasowanie z zachowaniem proporcji
    if (mazeAspectRatio > windowAspectRatio) {
        // Labirynt jest szerszy niż okno - dopasuj do szerokości
        viewWidth = targetWidth;
        viewHeight = targetWidth / mazeAspectRatio;
    } else {
        // Labirynt jest wyższy niż okno - dopasuj do wysokości
        viewHeight = targetHeight;
        viewWidth = targetHeight * mazeAspectRatio;
    }

    // Centrowanie widoku w oknie
    float offsetX = (targetWidth - viewWidth) / 2.0f;
    float offsetY = (targetHeight - viewHeight) / 2.0f + 30.0f;

    // Ustawienie widoku
    view.setSize(virtualSize);
    view.setCenter(sf::Vector2f(virtualSize.x / 2.0f, virtualSize.y / 2.0f));

    // Ustawienie viewportu (współrzędne znormalizowane 0.0 - 1.0)
    view.setViewport(sf::FloatRect(
        {
            offsetX / windowWidth,
            offsetY / windowHeight
        },
        {
            viewWidth / windowWidth,
            viewHeight / windowHeight
        }
    ));
    float btnMargin = 20.0f;
    float startY = 50.0f;

    // Sprawdzamy czy mamy przyciski, żeby nie wyjść poza zakres
    if (buttons.size() >= 2) {

        buttons[0].setPosition({window.getSize().y - 40.f - btnMargin, startY});


        buttons[1].setPosition({window.getSize().y - 40.f - btnMargin, startY + 50.0f});

        buttons[2].setPosition({window.getSize().y - 40.f - btnMargin, startY + 100.0f});

        // Przycisk 3: Generuj Własny
        buttons[3].setPosition({window.getSize().y - 40.f - btnMargin, startY + 150.0f});
    }
}
void MazeScreen::updateMazeLayout() {
    int cols = maze.getCols();
    int rows = maze.getRows();

    // Zabezpieczenie przed dzieleniem przez zero
    if (cols == 0) cols = 1;

    // 1. Przeliczamy rozmiar komórki na podstawie nowej liczby kolumn
    // virtualSize.x jest stałe (512.0f)
    cellSize = virtualSize.x / static_cast<float>(cols);

    // 2. Aktualizujemy wysokość wirtualnego świata
    virtualSize.y = cellSize * static_cast<float>(rows);

    // 3. Wymuszamy odświeżenie widoku (kamery), żeby uwzględnił nowe proporcje
    resizeView();
}
void MazeScreen::startGame() {
    gameState = GameState::RUNNING;
}
