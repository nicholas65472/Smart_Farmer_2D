#include "Game.h"
#include <iostream>

int main() {
    try {
        Game game;
        game.run();
    } catch (const std::exception& exception) {
        std::cerr << exception.what() << '\n';
        return 1;
    }

    return 0;
}