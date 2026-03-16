#ifndef GAME_H
#define GAME_H

/**
 * @file Game.h
 * @brief Main game class and state management
 */

#include <SFML/Graphics.hpp>
#include "Grid.h"
#include "Snake.h"
#include "Food.h"
#include "ParticleSystem.h"
#include "GradientBackground.h"
#include "PowerUp.h"
#include "Obstacle.h"
#include "GameMode.h"
#include "SoundManager.h"
#include "AISnake.h"
#include "FunnyMessages.h"

#include <vector>
#include <memory>

enum class GameState { Menu, ModeSelect, Playing, GameOver, Paused, Settings };

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
    PowerUp* powerUp;
    Obstacle* obstacle;
    ParticleSystem particles;
    GradientBackground background;
    SoundManager soundManager;
    FunnyMessages funnyMessages;
    
    std::vector<std::unique_ptr<AISnake>> aiSnakes;
    
    GameMode gameMode;
    GameState state;
    
    float powerUpTimer;
    float baseUpdateInterval;
    bool doublePointsActive;
    float modeTimer;
    int modeLevel;
    bool playerWon;
    
    sf::Font font;
    std::vector<sf::Text> menuItems;
    std::vector<sf::Text> modeItems;
    int selectedMenuItem;
    int selectedModeItem;
    
    sf::Text titleText{font, "", 64};
    sf::Text scoreText{font, "", 24};
    sf::Text gameOverText{font, "", 48};
    sf::Text gameOverStats{font, "", 20};
    sf::Text highScoreIndicator{font, "", 18};
    sf::Text pauseText{font, "", 48};
    sf::Text controlsText{font, "", 16};
    sf::Text timerText{font, "", 28};
    sf::Text modeInfoText{font, "", 18};
    sf::Text fpsText{font, "", 12};
    sf::Text funnyMessageText{font, "", 24};
    float funnyMessageTimer;
    
    int score;
    int highScore;
    int sessionScore;
    float updateTimer;
    float updateInterval;
    float animationTimer;
    bool keyProcessed;
    float screenShake;
    float screenShakeIntensity;
    float menuBlinkTimer;
    
    void processEvents();
    void update(float deltaTime);
    void render();
    
    void handleKeyPress(sf::Keyboard::Scancode key);
    void resetGame();
    void resetSession();
    void updateScore(int points);
    void loadHighScore();
    void saveHighScore();
    void updateWindowTitle();
    void setupMenu();
    void setupModeSelect();
    void startGameWithMode(GameMode mode);
    void showFunnyMessage(const std::string& msg, sf::Color color, float duration);
};

#endif
