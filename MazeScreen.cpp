#include "MazeScreen.h"
#include <iostream>

MazeScreen::MazeScreen(Maze &maze, sf::RenderWindow &window, std::shared_ptr<Player> player)
    : maze(maze), window(window), virtualSize(512.0f, 512.0f), player(player),
      pathText(font), startSideText(font), promptText(font), inputTextDisplay(font),
      solverSpeed(100), wallRatio(3) {

    virtualSize.x = 512.0f;
    int cols = maze.getCols();
    int rows = maze.getRows();

    if (!font.openFromFile("arial.ttf")) {
        std::cerr << "Nie mozna zaladowac czcionki arial.ttf" << std::endl;
    }

    inputBoxBg.setFillColor(sf::Color(0, 0, 0, 200));
    inputBoxBg.setOutlineColor(sf::Color::White);
    inputBoxBg.setOutlineThickness(2.0f);

    promptText.setFont(font);
    promptText.setCharacterSize(24);
    promptText.setFillColor(sf::Color::Yellow);

    inputTextDisplay.setFont(font);
    inputTextDisplay.setCharacterSize(24);
    inputTextDisplay.setFillColor(sf::Color::White);

    startSideText.setFont(font);
    startSideText.setCharacterSize(18);
    startSideText.setString("");

    pathText.setFont(font);
    pathText.setCharacterSize(18);
    pathText.setFillColor(sf::Color::White);
    pathText.setString("");
    pathText.setPosition({20.0f, float(window.getSize().y - 25.0f)});

    promptText.setFont(font);
    promptText.setCharacterSize(18);
    promptText.setFillColor(sf::Color::White);
    promptText.setString("");

    cellSize = virtualSize.x / static_cast<float>(cols);
    virtualSize.y = cellSize * static_cast<float>(rows);

    loadMainMenu();
    resizeView();
}

void MazeScreen::loadMainMenu() {
    buttons.clear();
    updateStartSideText();

    Button restartBtn(160.f, 40.f, "Nowy", font);
    restartBtn.setOnClick([this]() {
        std::cout << this->maze.getRows() << " " << this->maze.getCols() << std::endl;
        this->maze.createBoard(this->maze.getCols(), this->maze.getRows(), this->wallRatio);
        this->restartGame();
    });
    buttons.push_back(restartBtn);

    Button botBtn(160.f, 40.f, "Rozwiaz...", font);
    botBtn.setOnClick([this]() {
        this->loadSolverMenu();
        this->resizeView();
    });
    buttons.push_back(botBtn);

    Button loadBtn(160.f, 40.f, "Wczytaj z Pliku", font);
    loadBtn.setOnClick([this]() {
        isTyping = true;
        currentInputMode = InputMode::LOAD_FILENAME;
        userInput = "";
        promptText.setString("Podaj nazwe pliku:");
    });
    buttons.push_back(loadBtn);

    Button saveBtn(160.f, 40.f, "Zapisz Labirynt", font);
    saveBtn.setOnClick([this]() {
        isTyping = true;
        currentInputMode = InputMode::SAVE_FILE;
        userInput = "";
        promptText.setString("Podaj nazwe pliku:");
    });
    buttons.push_back(saveBtn);

    Button genBtn(160.f, 40.f, "Generuj Wlasny", font);
    genBtn.setOnClick([this]() {
        isTyping = true;
        currentInputMode = InputMode::GEN_WIDTH;
        userInput = "";
        promptText.setString("Podaj szerokosc (kolumny):");
    });
    buttons.push_back(genBtn);

    Button switchBtn(160.f, 40.f, "Zmien Strone", font);
    switchBtn.setOnClick([this]() {
        if (this->player) {
            this->player->switchSide(this->maze);
            updateStartSideText();
            this->maze.clearPaths();
            this->pathText.setString("");
            this->pathColor = sf::Color::Magenta;
        }
    });
    buttons.push_back(switchBtn);

    Button speedBtn(160.f, 40.f, "Szybkosc: " + std::to_string(solverSpeed), font);
    speedBtn.setOnClick([this]() {
        if (this->solverSpeed == 10) this->solverSpeed = 100;
        else if (this->solverSpeed == 100) this->solverSpeed = 10000;
        else this->solverSpeed = 10;

        this->loadMainMenu();
        this->resizeView();
    });
    buttons.push_back(speedBtn);

    Button wallBtn(160.f, 40.f, "Stosunek pol: " + std::to_string(wallRatio), font);
    wallBtn.setOnClick([this]() {
        this->wallRatio += 1;
        if (this->wallRatio > 10) this->wallRatio = 0;

        this->loadMainMenu();
        this->resizeView();
    });
    buttons.push_back(wallBtn);

    Button resetBtn(160.f, 40.f, "Reset", font);
    resetBtn.setOnClick([this]() {
        this->loadMainMenu();
        this->resizeView();
        this->player = std::make_shared<HumanPlayer>();
        pathColor = sf::Color::Magenta;
        maze.clearPaths();
        player->resetPosition();
        updateStartSideText();
        player->activate();
        gameState = GameState::RUNNING;
    });
    buttons.push_back(resetBtn);
}

void MazeScreen::loadSolverMenu() {
    buttons.clear();
    Button bfsBtn(160.f, 40.f, "Algorytm palenie lasu", font);
    bfsBtn.setOnClick([this]() {
        bool wasBottomSide = (this->player->getStartSideRowNumber() != 0);
        this->player = std::make_shared<ComputerBFTPlayer>();
        this->player->resetPosition();
        if (wasBottomSide) player->switchSide(maze); else player->resetPosition();
        this->player->activate();
        this->loadMainMenu();
        this->resizeView();
    });
    buttons.push_back(bfsBtn);

    Button dfsBtn(160.f, 40.f, "Algorytm DFS", font);
    dfsBtn.setOnClick([this]() {
        bool wasBottomSide = (this->player->getStartSideRowNumber() != 0);
        this->player = std::make_shared<ComputerDFSPlayer>();
        if (wasBottomSide) player->switchSide(maze); else player->resetPosition();
        this->player->activate();
        this->loadMainMenu();
        this->resizeView();
    });
    buttons.push_back(dfsBtn);

    Button rndBtn(160.f, 40.f, "Algorytm Random", font);
    rndBtn.setOnClick([this]() {
        bool wasBottomSide = (this->player->getStartSideRowNumber() != 0);
        this->player = std::make_shared<ComputerRandomPlayer>();
        if (wasBottomSide) player->switchSide(maze); else player->resetPosition();
        this->player->activate();
        this->loadMainMenu();
        this->resizeView();
    });
    buttons.push_back(rndBtn);

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
    window.draw(startSideText);
    for (const auto &btn: buttons) {
        btn.render((window));
    }

    if (isTyping) {
        sf::Vector2u windowSize = window.getSize();

        float boxW = 400.0f;
        float boxH = 150.0f;
        sf::RectangleShape bg = inputBoxBg;
        bg.setSize({boxW, boxH});
        bg.setPosition({(windowSize.x - boxW) / 2.0f, (windowSize.y - boxH) / 2.0f});

        sf::Text prompt = promptText;
        prompt.setPosition({bg.getPosition().x + 20.0f, bg.getPosition().y + 20.0f});

        sf::Text input = inputTextDisplay;
        input.setString(userInput + "_");
        input.setPosition({bg.getPosition().x + 20.0f, bg.getPosition().y + 70.0f});

        window.draw(bg);
        window.draw(prompt);
        window.draw(input);
    }
}

void MazeScreen::handleEvents(const sf::Event &event) {
    if (isTyping) {
        if (const sf::Event::TextEntered* textEvent = event.getIf<sf::Event::TextEntered>()) {
            if (textEvent->unicode == 8) {
                if (!userInput.empty()) userInput.pop_back();
            }
            else if (textEvent->unicode >= 32 && textEvent->unicode < 128) {
                userInput += static_cast<char>(textEvent->unicode);
            }
        }
        else if (const sf::Event::KeyPressed* keyEvent = event.getIf<sf::Event::KeyPressed>()) {
            if (keyEvent->code == sf::Keyboard::Key::Enter) {
                processInputConfirmation();
            }
            else if (keyEvent->code == sf::Keyboard::Key::Escape) {
                isTyping = false;
                currentInputMode = InputMode::NONE;
            }
        }
        return;
    }

    if (const sf::Event::KeyPressed *key = event.getIf<sf::Event::KeyPressed>()) {
        handleKeyPressed(*key);
    } else if (event.is<sf::Event::Resized>()) {
        resizeView();
    }
    if (const sf::Event::MouseButtonPressed *mouse = event.getIf<sf::Event::MouseButtonPressed>()) {
        if (mouse->button == sf::Mouse::Button::Left) {
            sf::Vector2f mousePosUI = window.mapPixelToCoords(mouse->position);
            bool buttonClicked = false;

            for (const auto &btn: buttons) {
                if (btn.checkClick(mousePosUI)) {
                    buttonClicked = true;
                    break;
                }
            }
            if (!buttonClicked) {
                sf::Vector2f mousePosWorld = window.mapPixelToCoords(mouse->position, view);

                int col = static_cast<int>(mousePosWorld.x / cellSize);
                int row = static_cast<int>(mousePosWorld.y / cellSize);

                if (row >= 0 && row < maze.getRows() && col >= 0 && col < maze.getCols()) {
                    maze.changeWall(row, col);
                }
            }
        }
    }
}

void MazeScreen::processInputConfirmation() {
    try {
        if (currentInputMode == InputMode::LOAD_FILENAME) {
            if (maze.readBoard(userInput)) {
                restartGame();
                updateMazeLayout();
            } else {
                std::cout << "Blad wczytywania pliku: " << userInput << std::endl;
            }
            isTyping = false;
        }
        else if (currentInputMode == InputMode::SAVE_FILE) {
            if (!maze.saveBoard(userInput)) {
                std::cout << "Błąd zapisu pliku" << std::endl;
            }
            std::cout << "Zapisano pomyślnie" << std::endl;
            isTyping = false;
        }
        else if (currentInputMode == InputMode::GEN_WIDTH) {
            tempGenWidth = std::stoi(userInput);
            if (tempGenWidth < 5) tempGenWidth = 5;

            userInput = "";
            currentInputMode = InputMode::GEN_HEIGHT;
            promptText.setString("Podaj wysokosc (wiersze):");
        }
        else if (currentInputMode == InputMode::GEN_HEIGHT) {
            int h = std::stoi(userInput);
            if (h < 5) h = 5;

            maze.createBoard(tempGenWidth, h, wallRatio);
            restartGame();
            updateMazeLayout();
            isTyping = false;
        }
    } catch (...) {
        std::cout << "Blad danych wejsciowych" << std::endl;
        isTyping = false;
    }
}

void MazeScreen::toggleBlock(int row, int col) {}

void MazeScreen::winGame() {
    gameState = GameState::PAUSED;
    pathColor = sf::Color::Green;
    pathText.setString("Ruchy rozwiazania: " + player->getMoves());
    std::cout << "ruchy gracza: " + player->getMoves() << std::endl;
}

void MazeScreen::restartGame() {
    this->player = std::make_shared<HumanPlayer>();
    updateStartSideText();

    player->resetPosition();
    gameState = GameState::RUNNING;
    pathColor = sf::Color::Magenta;
    pathText.setString("");
}

void MazeScreen::updateGame() {
    if (gameState == GameState::RUNNING && !isTyping) {
        player->update(maze, solverSpeed);
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
        if (player->isHuman()) {
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
        if (keyPressed.code == sf::Keyboard::Key::B) {
            player->activate();
        }
    }
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
        {offsetX / windowWidth, offsetY / windowHeight},
        {viewWidth / windowWidth, viewHeight / windowHeight}
    ));

    float btnMargin = 20.0f;
    float startY = 50.0f;
    float gap = 50.0f;

    for (size_t i = 0; i < buttons.size(); ++i) {
        buttons[i].setPosition({window.getSize().y - btnMargin, startY + (i * gap)});
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

void MazeScreen::updateStartSideText() {
    if (player->getStartSideRowNumber() == 0) {
        startSideText.setString("Start: GORA");
        startSideText.setFillColor(sf::Color::Green);
    } else {
        startSideText.setString("Start: DOL");
        startSideText.setFillColor(sf::Color::Red);
    }
}