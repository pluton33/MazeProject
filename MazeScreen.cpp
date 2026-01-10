//
// Created by Sebastian on 12/12/2025.
//

#include "MazeScreen.h"

#include <iostream>
#include "Button.h"
#include "SFML/Graphics/RectangleShape.hpp"

// Konstruktor zaktualizowany o shared_ptr
MazeScreen::MazeScreen(Maze &maze, sf::RenderWindow &window, std::shared_ptr<Player> player)
    : maze(maze), window(window), virtualSize(512.0f, 512.0f), player(player), pathText(font) {

    virtualSize.x = 512.0f;
    int cols = maze.getCols();
    int rows = maze.getRows();

    if (!font.openFromFile("arial.ttf")) {
        // W razie błędu, spróbujmy załadować domyślną, żeby nie wywalić programu od razu
        // (opcjonalne zabezpieczenie)
        std::cerr << "Nie mozna zaladowac czcionki arial.ttf" << std::endl;
    }

    pathText.setFont(font);
    pathText.setCharacterSize(18);
    pathText.setFillColor(sf::Color::White);
    pathText.setString("");
    pathText.setPosition({20.0f, float(window.getSize().y-25.0f)});

    cellSize = virtualSize.x / static_cast<float>(cols);
    virtualSize.y = cellSize * static_cast<float>(rows);

    // Inicjalizacja głównego menu przycisków
    loadMainMenu();

    resizeView();
}

void MazeScreen::loadMainMenu() {
    buttons.clear();

    Button restartBtn(160.f, 40.f, "Reset", font);
    restartBtn.setOnClick([this]() {
        std::cout << "Restart klikniety!" << std::endl;
        this->maze.createBoard();
        this->restartGame();
    });
    buttons.push_back(restartBtn);

    // ZMIANA: Przycisk Rozwiaz teraz otwiera menu wyboru
    Button botBtn(160.f, 40.f, "Rozwiaz...", font);
    botBtn.setOnClick([this]() {
        std::cout << "Otwieranie menu algorytmow..." << std::endl;
        this->loadSolverMenu();
        this->resizeView(); // Odswiez pozycje przyciskow
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

    Button resetBtn(160.f, 40.f, "Reset", font);
    resetBtn.setOnClick([this]() {
        this->loadMainMenu();
        this->resizeView();
        this->player = std::make_shared<HumanPlayer>();
        pathColor = sf::Color::Magenta;
        maze.clearPaths();
        player->resetPosition();
        player->activate();
        gameState = GameState::RUNNING;

    });
    buttons.push_back(resetBtn);
}

void MazeScreen::loadSolverMenu() {
    buttons.clear();

    // 1. Algorytm BFS (istniejący)
    Button bfsBtn(160.f, 40.f, "Algorytm BFS", font);
    bfsBtn.setOnClick([this]() {
        std::cout << "Wybrano BFS" << std::endl;
        // Podmiana gracza
        this->player = std::make_shared<ComputerBFTPlayer>();
        this->player->resetPosition();
        this->player->activate();
        this->loadMainMenu(); // Powrót do menu
        this->resizeView();
    });
    buttons.push_back(bfsBtn);

    // 2. Algorytm DFS (nowy)
    Button dfsBtn(160.f, 40.f, "Algorytm DFS", font);
    dfsBtn.setOnClick([this]() {
        std::cout << "Wybrano DFS" << std::endl;
        this->player = std::make_shared<ComputerDFSPlayer>();
        this->player->resetPosition();
        this->player->activate();
        this->loadMainMenu();
        this->resizeView();
    });
    buttons.push_back(dfsBtn);

    // 3. Algorytm Random (nowy)
    Button rndBtn(160.f, 40.f, "Algorytm Random", font);
    rndBtn.setOnClick([this]() {
        std::cout << "Wybrano Random" << std::endl;
        this->player = std::make_shared<ComputerRandomPlayer>();
        this->player->resetPosition();
        this->player->activate();
        this->loadMainMenu();
        this->resizeView();
    });
    buttons.push_back(rndBtn);

    // 4. Anuluj
    Button cancelBtn(160.f, 40.f, "Anuluj", font);
    cancelBtn.setOnClick([this]() {
        this->loadMainMenu();
        this->resizeView();
    });
    buttons.push_back(cancelBtn);
}

void MazeScreen::draw() const {
    int rows = maze.getRows();
    int cols = maze.getCols();

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
            // 1. Sprawdzamy kliknięcia w przyciski (na domyślnym widoku UI)
            sf::Vector2f mousePosUI = window.mapPixelToCoords(mouse->position);
            bool buttonClicked = false;

            for (const auto& btn : buttons) {
                if (btn.checkClick(mousePosUI)) {
                    buttonClicked = true;
                    break;
                }
            }

            // 2. Jeśli nie kliknięto przycisku, sprawdzamy kliknięcie w labirynt
            if (!buttonClicked) {
                // Mapujemy piksele na współrzędne widoku labiryntu (view)
                sf::Vector2f mousePosWorld = window.mapPixelToCoords(mouse->position, view);

                int col = static_cast<int>(mousePosWorld.x / cellSize);
                int row = static_cast<int>(mousePosWorld.y / cellSize);

                // Sprawdzamy, czy kliknięcie mieści się w granicach tablicy
                if (row >= 0 && row < maze.getRows() && col >= 0 && col < maze.getCols()) {
                    maze.changeWall(row, col);
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
    pathText.setString("Ruchy rozwiazania: " + player->getMoves());
    std::cout << "ruchy gracza: " + player->getMoves() << std::endl;
}

void MazeScreen::restartGame() {
    // Resetujemy gracza do ludzkiego przy pełnym restarcie,
    // albo resetujemy pozycje obecnego bota?
    // Zwykle restart gry to powrót do początku.
    // Jeśli chcemy wrócić do sterowania ręcznego:
    this->player = std::make_shared<HumanPlayer>();

    player->resetPosition();
    maze.printBoard();
    gameState = GameState::RUNNING;
    pathColor = sf::Color::Magenta;
    pathText.setString("");
}

void MazeScreen::updateGame() {
    if (gameState == GameState::RUNNING) {
        player->update(maze); // Użycie strzałki zamiast kropki (wskaźnik)
        if (player->checkForWin(maze)) {
            winGame();
        }
    }
}

void MazeScreen::handleKeyPressed(const sf::Event::KeyPressed &keyPressed) {
    if (keyPressed.code == sf::Keyboard::Key::Space) {
        restartGame();
    }

    if (gameState == GameState::RUNNING) {
        if (player->isHuman()) { // Użycie wskaźnika
            if (keyPressed.code == sf::Keyboard::Key::Left) {
                player->makeMove(maze, 'L');
            } else if (keyPressed.code == sf::Keyboard::Key::Right) {
                player->makeMove(maze, 'P');
            } else if (keyPressed.code == sf::Keyboard::Key::Down) {
                player->makeMove(maze, 'D');
            } else if (keyPressed.code == sf::Keyboard::Key::Up) {
                player->makeMove(maze, 'G');
            } else if (keyPressed.code == sf::Keyboard::Key::Backspace) {
                player->undoMove(maze);
            }
        }

        // Klawisz B może aktywować obecnego bota (jeśli jest ustawiony)
        if (keyPressed.code == sf::Keyboard::Key::B) {
            player->activate();
        }
    }
    draw();
    maze.printBoard();
    if (player->checkForWin(maze)) {
        winGame();
    }
}

void MazeScreen::resizeView() {
    float windowWidth = static_cast<float>(window.getSize().x);
    float windowHeight = static_cast<float>(window.getSize().y);

    float targetWidth = windowWidth * 0.75f;
    float targetHeight = windowHeight - 60.0f;

    float mazeAspectRatio = virtualSize.x / virtualSize.y;
    float windowAspectRatio = targetWidth / targetHeight;

    float viewWidth, viewHeight;
    window.setView(window.getDefaultView());

    if (mazeAspectRatio > windowAspectRatio) {
        viewWidth = targetWidth;
        viewHeight = targetWidth / mazeAspectRatio;
    } else {
        viewHeight = targetHeight;
        viewWidth = targetHeight * mazeAspectRatio;
    }

    float offsetX = (targetWidth - viewWidth) / 2.0f;
    float offsetY = (targetHeight - viewHeight) / 2.0f + 30.0f;

    view.setSize(virtualSize);
    view.setCenter(sf::Vector2f(virtualSize.x / 2.0f, virtualSize.y / 2.0f));

    view.setViewport(sf::FloatRect(
        { offsetX / windowWidth, offsetY / windowHeight },
        { viewWidth / windowWidth, viewHeight / windowHeight }
    ));

    float btnMargin = 20.0f;
    float startY = 50.0f;
    float gap = 50.0f;

    // Pozycjonowanie przycisków dynamicznie, niezależnie ile ich jest (Menu vs Solver)
    for (size_t i = 0; i < buttons.size(); ++i) {
         buttons[i].setPosition({window.getSize().y - 40.f - btnMargin, startY + (i * gap)});
    }
}

void MazeScreen::updateMazeLayout() {
    int cols = maze.getCols();
    int rows = maze.getRows();
    if (cols == 0) cols = 1;
    cellSize = virtualSize.x / static_cast<float>(cols);
    virtualSize.y = cellSize * static_cast<float>(rows);
    resizeView();
}

void MazeScreen::startGame() {
    gameState = GameState::RUNNING;
}