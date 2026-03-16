/**
 * @file main.cpp
 * @brief Entry point for Snake Game
 * 
 * A classic Snake game built with SFML.
 * Use arrow keys or WASD to control the snake.
 */

#include <iostream>
#include "Game.h"

/**
 * @brief Main entry point
 * @return 0 on successful exit, 1 on error
 */
int main() {
    try {
        Game game;
        game.run();
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }
    
    return 0;
}