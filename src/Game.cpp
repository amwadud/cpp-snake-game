/**
 * @file Game.cpp
 * @brief Main game implementation
 */

#include "Game.h"
#include "Constants.h"
#include <fstream>
#include <iostream>
#include <cmath>

Game::Game()
    : grid(Constants::Grid::CellSize, Constants::Window::Width, Constants::Window::Height),
      snake(nullptr),
      food(nullptr),
      powerUp(nullptr),
      obstacle(nullptr),
      background(Constants::Window::Width, Constants::Window::Height),
      gameMode(GameMode::Classic),
      state(GameState::Menu),
      powerUpTimer(0.f),
      baseUpdateInterval(Constants::Gameplay::InitialUpdateInterval),
      doublePointsActive(false),
      modeTimer(0.f),
      modeLevel(1),
      selectedMenuItem(0),
      selectedModeItem(0),
      score(0),
      highScore(0),
      sessionScore(0),
      updateTimer(0.f),
      updateInterval(Constants::Gameplay::InitialUpdateInterval),
      animationTimer(0.f),
      keyProcessed(false),
      screenShake(0.f),
      screenShakeIntensity(0.f),
      menuBlinkTimer(0.f) {
    
    window.create(sf::VideoMode(sf::Vector2u(Constants::Window::Width, Constants::Window::Height), 32), 
                  Constants::Window::Title, sf::Style::Close);
    window.setFramerateLimit(60);
    
    if (!font.openFromFile("/usr/share/fonts/liberation/LiberationSans-Regular.ttf") &&
        !font.openFromFile("/usr/share/fonts/liberation/LiberationSans-Bold.ttf") &&
        !font.openFromFile("C:/Windows/Fonts/arial.ttf")) {
        if (font.openFromFile("/System/Library/Fonts/Helvetica.ttc")) {
            std::cerr << "Warning: Using system Helvetica font" << std::endl;
        }
    }
    
    loadHighScore();
    setupMenu();
    setupModeSelect();
    
    scoreText.setFont(font);
    scoreText.setCharacterSize(24);
    scoreText.setFillColor(Constants::Colors::TextWhite);
    scoreText.setPosition(sf::Vector2f(10, 10));
    
    timerText.setFont(font);
    timerText.setCharacterSize(28);
    timerText.setFillColor(Constants::Colors::TextYellow);
    timerText.setPosition(sf::Vector2f(Constants::Window::Width / 2.f - 40.f, 10.f));
    
    gameOverText.setFont(font);
    gameOverText.setCharacterSize(48);
    gameOverText.setFillColor(Constants::Colors::TextRed);
    gameOverText.setString("GAME OVER");
    sf::FloatRect bounds = gameOverText.getLocalBounds();
    gameOverText.setOrigin(sf::Vector2f(bounds.position.x + bounds.size.x / 2.f, bounds.position.y + bounds.size.y / 2.f));
    gameOverText.setPosition(sf::Vector2f(Constants::Window::Width / 2.f, Constants::Window::Height / 2.f - 60.f));
    
    gameOverStats.setFont(font);
    gameOverStats.setCharacterSize(20);
    gameOverStats.setFillColor(Constants::Colors::TextWhite);
    bounds = gameOverStats.getLocalBounds();
    gameOverStats.setOrigin(sf::Vector2f(bounds.position.x + bounds.size.x / 2.f, bounds.position.y + bounds.size.y / 2.f));
    gameOverStats.setPosition(sf::Vector2f(Constants::Window::Width / 2.f, Constants::Window::Height / 2.f + 10.f));
    
    pauseText.setFont(font);
    pauseText.setCharacterSize(48);
    pauseText.setFillColor(Constants::Colors::TextYellow);
    pauseText.setString("PAUSED");
    bounds = pauseText.getLocalBounds();
    pauseText.setOrigin(sf::Vector2f(bounds.position.x + bounds.size.x / 2.f, bounds.position.y + bounds.size.y / 2.f));
    pauseText.setPosition(sf::Vector2f(Constants::Window::Width / 2.f, Constants::Window::Height / 2.f));
    
    highScoreIndicator.setFont(font);
    highScoreIndicator.setCharacterSize(18);
    highScoreIndicator.setFillColor(Constants::Colors::TextYellow);
    highScoreIndicator.setString("NEW HIGH SCORE!");
    highScoreIndicator.setPosition(sf::Vector2f(Constants::Window::Width / 2.f - 80.f, 50.f));
    
    controlsText.setFont(font);
    controlsText.setCharacterSize(16);
    controlsText.setFillColor(Constants::Colors::TextWhite);
    controlsText.setString("Arrow Keys / WASD / Vim (h,j,k,l) | P pause");
    controlsText.setPosition(sf::Vector2f(10.f, static_cast<float>(Constants::Window::Height - 30)));
    
    modeInfoText.setFont(font);
    modeInfoText.setCharacterSize(18);
    modeInfoText.setFillColor(Constants::Colors::TextWhite);
    
    fpsText.setFont(font);
    fpsText.setCharacterSize(12);
    fpsText.setFillColor(sf::Color(100, 100, 100));
    fpsText.setPosition(sf::Vector2f(static_cast<float>(Constants::Window::Width - 60), 10.f));
    
    titleText.setFont(font);
    titleText.setCharacterSize(64);
    titleText.setFillColor(Constants::Colors::SnakeHead);
    titleText.setString("SNAKE");
    bounds = titleText.getLocalBounds();
    titleText.setOrigin(sf::Vector2f(bounds.position.x + bounds.size.x / 2.f, bounds.position.y + bounds.size.y / 2.f));
    titleText.setPosition(sf::Vector2f(Constants::Window::Width / 2.f, 120.f));
}

Game::~Game() {
    delete snake;
    delete food;
    delete powerUp;
    delete obstacle;
}

void Game::setupMenu() {
    menuItems.clear();
    std::vector<std::string> items = {"Start Game", "Select Mode", "Quit"};
    
    for (size_t i = 0; i < items.size(); ++i) {
        sf::Text text{font, items[i], 32};
        sf::FloatRect bounds = text.getLocalBounds();
        text.setOrigin(sf::Vector2f(bounds.position.x + bounds.size.x / 2.f, bounds.position.y + bounds.size.y / 2.f));
        text.setPosition(sf::Vector2f(Constants::Window::Width / 2.f, 250.f + i * 60.f));
        menuItems.push_back(text);
    }
}

void Game::setupModeSelect() {
    modeItems.clear();
    std::vector<std::string> modes = {"Classic", "Time Attack", "Zen", "Challenge"};
    
    for (size_t i = 0; i < modes.size(); ++i) {
        sf::Text text{font, modes[i], 28};
        sf::FloatRect bounds = text.getLocalBounds();
        text.setOrigin(sf::Vector2f(bounds.position.x + bounds.size.x / 2.f, bounds.position.y + bounds.size.y / 2.f));
        text.setPosition(sf::Vector2f(Constants::Window::Width / 2.f, 180.f + i * 70.f));
        modeItems.push_back(text);
    }
}

void Game::run() {
    sf::Clock clock;
    sf::Clock fpsClock;
    float fpsTimer = 0.f;
    int frameCount = 0;
    
    while (window.isOpen()) {
        sf::Time elapsed = clock.restart();
        float deltaTime = elapsed.asSeconds();
        animationTimer += deltaTime;
        menuBlinkTimer += deltaTime;
        
        frameCount++;
        fpsTimer += deltaTime;
        if (fpsTimer >= 1.0f) {
            fpsText.setString("FPS: " + std::to_string(frameCount));
            frameCount = 0;
            fpsTimer = 0.f;
        }
        
        processEvents();
        
        if (state == GameState::Playing) {
            update(deltaTime);
            particles.update(deltaTime);
        }
        
        render();
    }
}

void Game::processEvents() {
    while (auto event = window.pollEvent()) {
        if (event->is<sf::Event::Closed>()) {
            window.close();
        }
        
        if (auto keyEvent = event->getIf<sf::Event::KeyPressed>()) {
            handleKeyPress(keyEvent->scancode);
        }
    }
}

void Game::handleKeyPress(sf::Keyboard::Scancode key) {
    if (state == GameState::Menu) {
        if (key == sf::Keyboard::Scancode::Up || key == sf::Keyboard::Scancode::W) {
            selectedMenuItem = (selectedMenuItem - 1 + menuItems.size()) % menuItems.size();
        } else if (key == sf::Keyboard::Scancode::Down || key == sf::Keyboard::Scancode::S) {
            selectedMenuItem = (selectedMenuItem + 1) % menuItems.size();
        } else if (key == sf::Keyboard::Scancode::Enter) {
            if (selectedMenuItem == 0) {
                state = GameState::Playing;
                startGameWithMode(gameMode);
            } else if (selectedMenuItem == 1) {
                state = GameState::ModeSelect;
            } else if (selectedMenuItem == 2) {
                window.close();
            }
        }
    } else if (state == GameState::ModeSelect) {
        if (key == sf::Keyboard::Scancode::Up || key == sf::Keyboard::Scancode::W) {
            selectedModeItem = (selectedModeItem - 1 + modeItems.size()) % modeItems.size();
        } else if (key == sf::Keyboard::Scancode::Down || key == sf::Keyboard::Scancode::S) {
            selectedModeItem = (selectedModeItem + 1) % modeItems.size();
        } else if (key == sf::Keyboard::Scancode::Enter) {
            gameMode = static_cast<GameMode>(selectedModeItem);
            state = GameState::Playing;
            startGameWithMode(gameMode);
        } else if (key == sf::Keyboard::Scancode::Escape) {
            state = GameState::Menu;
        }
    } else if (state == GameState::Playing) {
        switch (key) {
            case sf::Keyboard::Scancode::Up:
            case sf::Keyboard::Scancode::W:
            case sf::Keyboard::Scancode::K:
                snake->changeDirection(Direction::Up);
                break;
            case sf::Keyboard::Scancode::Down:
            case sf::Keyboard::Scancode::S:
            case sf::Keyboard::Scancode::J:
                snake->changeDirection(Direction::Down);
                break;
            case sf::Keyboard::Scancode::Left:
            case sf::Keyboard::Scancode::A:
            case sf::Keyboard::Scancode::H:
                snake->changeDirection(Direction::Left);
                break;
            case sf::Keyboard::Scancode::Right:
            case sf::Keyboard::Scancode::D:
            case sf::Keyboard::Scancode::L:
                snake->changeDirection(Direction::Right);
                break;
            case sf::Keyboard::Scancode::P:
            case sf::Keyboard::Scancode::Escape:
                state = GameState::Paused;
                break;
            default:
                break;
        }
    } else if (state == GameState::GameOver) {
        if (key == sf::Keyboard::Scancode::Enter) {
            state = GameState::Menu;
        } else if (key == sf::Keyboard::Scancode::Escape) {
            state = GameState::Menu;
        }
    } else if (state == GameState::Paused) {
        if (key == sf::Keyboard::Scancode::P || key == sf::Keyboard::Scancode::Escape) {
            state = GameState::Playing;
        }
    }
}

void Game::startGameWithMode(GameMode mode) {
    delete snake;
    delete food;
    delete powerUp;
    delete obstacle;
    
    snake = new Snake(grid);
    food = new Food(grid, *snake);
    powerUp = new PowerUp(grid, *snake);
    obstacle = new Obstacle(grid);
    
    GameModeInfo info = getModeInfo(mode);
    
    score = 0;
    sessionScore = 0;
    modeTimer = static_cast<float>(info.timerDuration);
    modeLevel = 1;
    updateTimer = 0.f;
    updateInterval = Constants::Gameplay::InitialUpdateInterval;
    baseUpdateInterval = Constants::Gameplay::InitialUpdateInterval;
    screenShake = 0.f;
    particles.clear();
    powerUpTimer = 0.f;
    doublePointsActive = false;
    
    if (info.hasObstacles) {
        obstacle->generate(*snake, modeLevel);
    }
}

void Game::update(float deltaTime) {
    GameModeInfo info = getModeInfo(gameMode);
    
    if (info.hasTimer) {
        modeTimer -= deltaTime;
        if (modeTimer <= 0) {
            state = GameState::GameOver;
            soundManager.playGameOver();
            if (score > highScore) {
                highScore = score;
                saveHighScore();
            }
            return;
        }
    }
    
    updateTimer += deltaTime;
    
    if (updateTimer >= updateInterval) {
        updateTimer = 0.f;
        
        snake->update();
        
        bool hasDeath = info.hasDeath;
        
        if (hasDeath && snake->hasCollided()) {
            state = GameState::GameOver;
            screenShake = 0.5f;
            screenShakeIntensity = 15.f;
            soundManager.playCollision();
            if (score > highScore) {
                highScore = score;
                saveHighScore();
            }
            return;
        }
        
        if (info.hasObstacles && obstacle->checkCollision(snake->getHead())) {
            state = GameState::GameOver;
            screenShake = 0.5f;
            screenShakeIntensity = 15.f;
            soundManager.playCollision();
            if (score > highScore) {
                highScore = score;
                saveHighScore();
            }
            return;
        }
        
        if (snake->hasEatenFood(food->getPosition())) {
            snake->grow();
            
            float foodX = food->getPosition().x * Constants::Grid::CellSize + Constants::Grid::CellSize / 2.f;
            float foodY = food->getPosition().y * Constants::Grid::CellSize + Constants::Grid::CellSize / 2.f;
            particles.emit(sf::Vector2f(foodX, foodY), Constants::Colors::Food, 25);
            soundManager.playEat();
            
            food->respawn(*snake);
            updateScore(Constants::Gameplay::PointsPerFood);
            sessionScore += 1;
            
            if (updateInterval > Constants::Gameplay::MinUpdateInterval && gameMode != GameMode::Zen) {
                updateInterval -= Constants::Gameplay::SpeedIncrease;
            }
            
            if (info.hasObstacles && sessionScore % 5 == 0) {
                modeLevel++;
                obstacle->generate(*snake, modeLevel);
            }
            
            if (score > highScore) {
                highScore = score;
            }
        }
        
        if (powerUp->isActive() && snake->getHead() == powerUp->getPosition()) {
            powerUp->setActive(false);
            
            float puX = powerUp->getPosition().x * Constants::Grid::CellSize + Constants::Grid::CellSize / 2.f;
            float puY = powerUp->getPosition().y * Constants::Grid::CellSize + Constants::Grid::CellSize / 2.f;
            particles.emit(sf::Vector2f(puX, puY), sf::Color(250, 189, 47), 30);
            soundManager.playPowerUpCollect();
            
            switch (powerUp->getType()) {
                case PowerUpType::SpeedBoost:
                    updateInterval = baseUpdateInterval * 0.6f;
                    break;
                case PowerUpType::DoublePoints:
                    doublePointsActive = true;
                    break;
                case PowerUpType::SlowDown:
                    updateInterval = baseUpdateInterval * 1.5f;
                    break;
                default:
                    break;
            }
        }
        
        powerUpTimer += deltaTime;
        if (powerUpTimer >= 15.f && !powerUp->isActive() && gameMode != GameMode::Zen) {
            powerUp->respawn(*snake);
            powerUpTimer = 0.f;
        }
        
        powerUp->update(deltaTime);
    }
    keyProcessed = false;
}

void Game::render() {
    sf::View view = window.getDefaultView();
    
    if (screenShake > 0 && state == GameState::Playing) {
        float offsetX = (rand() % 100 / 100.f - 0.5f) * screenShakeIntensity * screenShake;
        float offsetY = (rand() % 100 / 100.f - 0.5f) * screenShakeIntensity * screenShake;
        view.setCenter(sf::Vector2f(Constants::Window::Width / 2.f + offsetX, Constants::Window::Height / 2.f + offsetY));
        window.setView(view);
        screenShake -= 0.02f;
    }
    
    window.clear(sf::Color::Transparent);
    background.render(window);
    
    if (state == GameState::Menu || state == GameState::ModeSelect) {
        float pulse = (std::sin(animationTimer * 2.f) + 1.f) / 2.f;
        int alpha = static_cast<int>(200 + 55 * pulse);
        titleText.setFillColor(sf::Color(184, 187, 38, alpha));
        window.draw(titleText);
        
        for (size_t i = 0; i < menuItems.size(); ++i) {
            if (state == GameState::Menu) {
                if (i == selectedMenuItem) {
                    menuItems[i].setFillColor(Constants::Colors::SnakeHead);
                } else {
                    menuItems[i].setFillColor(Constants::Colors::TextWhite);
                }
                window.draw(menuItems[i]);
            }
        }
        
        if (state == GameState::ModeSelect) {
            for (size_t i = 0; i < modeItems.size(); ++i) {
                if (i == selectedModeItem) {
                    modeItems[i].setFillColor(Constants::Colors::Food);
                } else {
                    modeItems[i].setFillColor(Constants::Colors::TextWhite);
                }
                window.draw(modeItems[i]);
                
                GameModeInfo mi = getModeInfo(static_cast<GameMode>(i));
                sf::Text descText{font, mi.description, 14};
                descText.setFillColor(sf::Color(150, 150, 150));
                descText.setPosition(sf::Vector2f(Constants::Window::Width / 2.f + 120.f, 180.f + i * 70.f - 7.f));
                window.draw(descText);
            }
            
            sf::Text backText{font, "Press ESC to go back", 18};
            backText.setFillColor(sf::Color(100, 100, 100));
            backText.setPosition(sf::Vector2f(Constants::Window::Width / 2.f - 80.f, Constants::Window::Height - 50.f));
            window.draw(backText);
        }
    }
    
    if (state == GameState::Playing || state == GameState::GameOver || state == GameState::Paused) {
        static sf::RectangleShape gridLineV(sf::Vector2f(1.f, static_cast<float>(Constants::Window::Height)));
        static sf::RectangleShape gridLineH(sf::Vector2f(static_cast<float>(Constants::Window::Width), 1.f));
        gridLineV.setFillColor(Constants::Colors::GridLine);
        gridLineH.setFillColor(Constants::Colors::GridLine);
        
        for (unsigned int x = 0; x <= Constants::Window::Width; x += Constants::Grid::CellSize) {
            gridLineV.setPosition(sf::Vector2f(static_cast<float>(x), 0));
            window.draw(gridLineV);
        }
        
        for (unsigned int y = 0; y <= Constants::Window::Height; y += Constants::Grid::CellSize) {
            gridLineH.setPosition(sf::Vector2f(0, static_cast<float>(y)));
            window.draw(gridLineH);
        }
        
        if (obstacle) {
            obstacle->render(window);
        }
        
        float foodPulse = 0.8f + 0.4f * (std::sin(animationTimer * Constants::Animation::FoodPulseSpeed) + 1.0f) / 2.0f;
        food->setScale(foodPulse);
        food->render(window);
        powerUp->render(window);
        snake->render(window);
        particles.render(window);
        
        scoreText.setString("Score: " + std::to_string(score));
        window.draw(scoreText);
        
        GameModeInfo info = getModeInfo(gameMode);
        if (info.hasTimer) {
            int minutes = static_cast<int>(modeTimer) / 60;
            int seconds = static_cast<int>(modeTimer) % 60;
            timerText.setString(std::to_string(minutes) + ":" + (seconds < 10 ? "0" : "") + std::to_string(seconds));
            window.draw(timerText);
        }
        
        if (score > 0 && score == highScore) {
            float hsPulse = (std::sin(animationTimer * Constants::Animation::HighScoreSpeed) + 1.0f) / 2.0f;
            int alpha = static_cast<int>(180 + 75 * hsPulse);
            highScoreIndicator.setFillColor(sf::Color(255, 255, 0, alpha));
            window.draw(highScoreIndicator);
        }
        
        window.draw(controlsText);
    }
    
    if (state == GameState::GameOver) {
        float pulse = (std::sin(animationTimer * 3.f) + 1.f) / 2.f;
        int alpha = static_cast<int>(180 + 75 * pulse);
        gameOverText.setFillColor(sf::Color(251, 73, 52, alpha));
        
        window.draw(gameOverText);
        
        std::string statsStr = "Score: " + std::to_string(score);
        if (gameMode == GameMode::TimeAttack) {
            statsStr += " | Time's Up!";
        } else if (gameMode == GameMode::Zen) {
            statsStr = "Length: " + std::to_string(snake->getLength());
        }
        gameOverStats.setString(statsStr);
        window.draw(gameOverStats);
        
        sf::Text restartText{font, "Press ENTER to continue", 20};
        float blinkAlpha = static_cast<int>(180 + 75 * pulse);
        restartText.setFillColor(sf::Color(235, 219, 178, blinkAlpha));
        sf::FloatRect bounds = restartText.getLocalBounds();
        restartText.setOrigin(sf::Vector2f(bounds.position.x + bounds.size.x / 2.f, bounds.position.y + bounds.size.y / 2.f));
        restartText.setPosition(sf::Vector2f(Constants::Window::Width / 2.f, Constants::Window::Height / 2.f + 80.f));
        window.draw(restartText);
    }
    
    if (state == GameState::Paused) {
        window.draw(pauseText);
    }
    
    window.draw(fpsText);
    
    window.setView(window.getDefaultView());
    window.display();
}

void Game::resetGame() {
    startGameWithMode(gameMode);
}

void Game::resetSession() {
    sessionScore = 0;
    modeLevel = 1;
}

void Game::updateScore(int points) {
    int actualPoints = doublePointsActive ? points * 2 : points;
    score += actualPoints;
    if (score > highScore) {
        highScore = score;
    }
    updateWindowTitle();
}

void Game::loadHighScore() {
    std::ifstream file(Constants::HighScoreFile);
    if (file.is_open()) {
        file >> highScore;
        file.close();
    }
}

void Game::saveHighScore() {
    std::ofstream file(Constants::HighScoreFile);
    if (file.is_open()) {
        file << highScore;
        file.close();
    }
}

void Game::updateWindowTitle() {
    std::string title = Constants::Window::Title;
    title += " - Score: " + std::to_string(score);
    if (score == highScore && score > 0) {
        title += " (High Score!)";
    }
    window.setTitle(title);
}