#include "ComputerRandomPlayer.h"
#include <algorithm>
#include <vector>
#include <chrono>

ComputerRandomPlayer::ComputerRandomPlayer() {
    auto seed = std::chrono::system_clock::now().time_since_epoch().count();
    rng.seed(seed);
}

void ComputerRandomPlayer::update(Maze &maze) {
    if (!isActivated) return;
    
    // Spowolnienie ruchu, żeby było widać co robi
    static int moveDelay = 0;
    if (++moveDelay < 5) return; 
    moveDelay = 0;

    if (checkForWin(maze)) return;

    // Lista możliwych ruchów
    struct Move { char key; int dr; int dc; };
    std::vector<Move> moves = {
        {'G', -1, 0}, {'D', 1, 0}, {'L', 0, -1}, {'P', 0, 1}
    };

    // Mieszamy kolejność sprawdzania
    std::shuffle(moves.begin(), moves.end(), rng);

    bool moved = false;
    for (const auto& m : moves) {
        // Sprawdzamy czy ruch jest legalny (nie wchodzimy w ścianę)
        if (!maze.isBlocked(row + m.dr, column + m.dc)) {
            // Dodatkowy warunek "głupoty": 
            // Bot po prostu wykonuje losowy dozwolony ruch. 
            // Może się cofać. To jest brute-force w czystej postaci (błądzenie losowe).
            makeMove(maze, m.key);
            moved = true;
            break;
        }
    }
}

void ComputerRandomPlayer::resetPosition() {
    Player::resetPosition();
    isActivated = false;
}