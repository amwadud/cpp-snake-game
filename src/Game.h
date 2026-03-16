#ifndef GAME_H
#define GAME_H

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include "Grid.h"
#include "Snake.h"
#include "Food.h"

enum class GameState { Playing, GameOver, Paused };

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
    sf::Text scoreText;
    sf::Text gameOverText;
    sf::Text pauseText;
    
    GameState state;
    int score;
    int highScore;
    float updateTimer;
    float updateInterval;
    
    sf::SoundBuffer eatBuffer;
    sf::SoundBuffer dieBuffer;
    sf::Sound eatSound;
    sf::Sound dieSound;
    
    void processEvents();
    void update(float deltaTime);
    void render();
    
    void handleKeyPress(sf::Keyboard::Key key);
    void resetGame();
    void updateScore(int points);
    void loadHighScore();
    void saveHighScore();
};

#endif