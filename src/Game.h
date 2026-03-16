#ifndef GAME_H
#define GAME_H

#include <SFML/Graphics.hpp>
#include "Grid.h"
#include "Snake.h"
#include "Food.h"

enum class GameState { Start, Playing, GameOver, Paused };

class Game {
public:
    Game();
    ~Game();
    
    void run();

private:
    sf::RenderWindow window;
    Grid grid;
    Snake* snake;
    Food* food;
    
    sf::Font font;
    sf::Text scoreText{font, "", 30};
    sf::Text gameOverText{font, "", 30};
    sf::Text pauseText{font, "", 30};
    sf::Text startText{font, "", 30};
    sf::Text controlsText{font, "", 16};
    
    GameState state;
    int score;
    int highScore;
    float updateTimer;
    float updateInterval;
    float animationTimer;
    
    void processEvents();
    void update(float deltaTime);
    void render();
    
    void handleKeyPress(sf::Keyboard::Scancode key);
    void resetGame();
    void updateScore(int points);
    void loadHighScore();
    void saveHighScore();
    void updateWindowTitle();
};

#endif