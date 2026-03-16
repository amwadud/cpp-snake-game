/**
 * @file Game.cpp
 * @brief Main game implementation
 */

#include "Game.h"
#include "Constants.h"
#include <fstream>
#include <iostream>

Game::Game()
    : grid(Constants::Grid::CellSize, Constants::Window::Width, Constants::Window::Height),
      snake(nullptr),
      food(nullptr),
      state(GameState::Start),
      score(0),
      highScore(0),
      updateTimer(0.f),
      updateInterval(Constants::Gameplay::InitialUpdateInterval),
      animationTimer(0.f),
      keyProcessed(false) {
    
    window.create(sf::VideoMode(sf::Vector2u(Constants::Window::Width, Constants::Window::Height), 32), 
                  Constants::Window::Title, sf::Style::Close);
    window.setFramerateLimit(Constants::Window::FrameRateLimit);
    
    if (!font.openFromFile("/usr/share/fonts/liberation/LiberationSans-Regular.ttf") &&
        !font.openFromFile("/usr/share/fonts/liberation/LiberationSans-Bold.ttf") &&
        !font.openFromFile("C:/Windows/Fonts/arial.ttf")) {
        if (font.openFromFile("/System/Library/Fonts/Helvetica.ttc")) {
            std::cerr << "Warning: Using system Helvetica font" << std::endl;
        }
    }
    
    scoreText.setFont(font);
    scoreText.setCharacterSize(24);
    scoreText.setFillColor(Constants::Colors::TextWhite);
    scoreText.setPosition(sf::Vector2f(10, 10));
    
    gameOverText.setFont(font);
    gameOverText.setCharacterSize(48);
    gameOverText.setFillColor(Constants::Colors::TextRed);
    gameOverText.setString("GAME OVER\nPress ENTER to restart");
    sf::FloatRect bounds = gameOverText.getLocalBounds();
    gameOverText.setOrigin(sf::Vector2f(bounds.position.x + bounds.size.x / 2.f, bounds.position.y + bounds.size.y / 2.f));
    gameOverText.setPosition(sf::Vector2f(Constants::Window::Width / 2.f, Constants::Window::Height / 2.f - 60.f));
    
    gameOverStats.setFont(font);
    gameOverStats.setCharacterSize(20);
    gameOverStats.setFillColor(Constants::Colors::TextWhite);
    gameOverStats.setString("");
    sf::FloatRect statsBounds = gameOverStats.getLocalBounds();
    gameOverStats.setOrigin(sf::Vector2f(statsBounds.position.x + statsBounds.size.x / 2.f, statsBounds.position.y + statsBounds.size.y / 2.f));
    gameOverStats.setPosition(sf::Vector2f(Constants::Window::Width / 2.f, Constants::Window::Height / 2.f + 30.f));
    
    pauseText.setFont(font);
    pauseText.setCharacterSize(48);
    pauseText.setFillColor(Constants::Colors::TextYellow);
    pauseText.setString("PAUSED\nPress P to resume");
    bounds = pauseText.getLocalBounds();
    pauseText.setOrigin(sf::Vector2f(bounds.position.x + bounds.size.x / 2.f, bounds.position.y + bounds.size.y / 2.f));
    pauseText.setPosition(sf::Vector2f(Constants::Window::Width / 2.f, Constants::Window::Height / 2.f));
    
    controlsText.setFont(font);
    controlsText.setCharacterSize(16);
    controlsText.setFillColor(Constants::Colors::TextWhite);
    controlsText.setString("Controls: Arrow Keys / WASD / Vim (h,j,k,l) | P to pause");
    controlsText.setPosition(sf::Vector2f(10.f, static_cast<float>(Constants::Window::Height - 30)));
    
    startText.setFont(font);
    startText.setCharacterSize(48);
    startText.setFillColor(Constants::Colors::TextWhite);
    startText.setString("SNAKE\nPress ENTER to start");
    sf::FloatRect startBounds = startText.getLocalBounds();
    startText.setOrigin(sf::Vector2f(startBounds.position.x + startBounds.size.x / 2.f, startBounds.position.y + startBounds.size.y / 2.f));
    startText.setPosition(sf::Vector2f(Constants::Window::Width / 2.f, Constants::Window::Height / 2.f - 50.f));
    
    fpsText.setFont(font);
    fpsText.setCharacterSize(12);
    fpsText.setFillColor(sf::Color(100, 100, 100));
    fpsText.setPosition(sf::Vector2f(static_cast<float>(Constants::Window::Width - 60), 10.f));
    
    highScoreIndicator.setFont(font);
    highScoreIndicator.setCharacterSize(18);
    highScoreIndicator.setFillColor(Constants::Colors::TextYellow);
    highScoreIndicator.setString("NEW HIGH SCORE!");
    highScoreIndicator.setPosition(sf::Vector2f(Constants::Window::Width / 2.f - 80.f, 50.f));
    
    loadHighScore();
    
    snake = new Snake(grid);
    food = new Food(grid, *snake);
}

Game::~Game() {
    delete snake;
    delete food;
}

void Game::run() {
    sf::Clock clock;
    sf::Clock fpsClock;
    float fpsUpdateTimer = 0.f;
    int frameCount = 0;
    
    while (window.isOpen()) {
        sf::Time elapsed = clock.restart();
        float deltaTime = elapsed.asSeconds();
        animationTimer += deltaTime;
        
        frameCount++;
        fpsUpdateTimer += deltaTime;
        if (fpsUpdateTimer >= 1.0f) {
            fpsText.setString("FPS: " + std::to_string(frameCount));
            frameCount = 0;
            fpsUpdateTimer = 0.f;
        }
        
        processEvents();
        
        if (state == GameState::Playing) {
            update(deltaTime);
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
    if (state == GameState::Start) {
        if (key == sf::Keyboard::Scancode::Enter) {
            state = GameState::Playing;
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
            resetGame();
        }
    } else if (state == GameState::Paused) {
        if (key == sf::Keyboard::Scancode::P || key == sf::Keyboard::Scancode::Escape) {
            state = GameState::Playing;
        }
    } else if (state == GameState::Start) {
        if (key == sf::Keyboard::Scancode::Escape) {
            window.close();
        }
    } else if (state == GameState::GameOver) {
        if (key == sf::Keyboard::Scancode::Escape) {
            state = GameState::Start;
        }
    }
}

void Game::update(float deltaTime) {
    updateTimer += deltaTime;
    
    if (updateTimer >= updateInterval) {
        updateTimer = 0.f;
        
        snake->update();
        
        if (snake->hasCollided()) {
            state = GameState::GameOver;
            gameOverStats.setString("Score: " + std::to_string(score) + " | Length: " + std::to_string(snake->getLength()));
            if (score > highScore) {
                highScore = score;
                saveHighScore();
            }
            return;
        }
        
        if (snake->hasEatenFood(food->getPosition())) {
            snake->grow();
            food->respawn(*snake);
            updateScore(Constants::Gameplay::PointsPerFood);
            
            if (updateInterval > Constants::Gameplay::MinUpdateInterval) {
                updateInterval -= Constants::Gameplay::SpeedIncrease;
            }
            
            if (score > highScore) {
                highScore = score;
            }
        }
    }
    keyProcessed = false;
}

void Game::render() {
    window.clear(Constants::Colors::Background);
    
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
    
    if (state != GameState::Start) {
        float foodPulse = 0.8f + 0.4f * (std::sin(animationTimer * Constants::Animation::FoodPulseSpeed) + 1.0f) / 2.0f;
        food->setScale(foodPulse);
        food->render(window);
        snake->render(window);
        
        scoreText.setString("Score: " + std::to_string(score) + 
                            " | Length: " + std::to_string(snake->getLength()) +
                            " | High: " + std::to_string(highScore));
        window.draw(scoreText);
        
        if (score > 0 && score == highScore) {
            float hsPulse = (std::sin(animationTimer * Constants::Animation::HighScoreSpeed) + 1.0f) / 2.0f;
            int alpha = static_cast<int>(180 + 75 * hsPulse);
            highScoreIndicator.setFillColor(sf::Color(255, 255, 0, alpha));
            window.draw(highScoreIndicator);
        }
    }
    
    if (state == GameState::Start) {
        float pulse = (std::sin(animationTimer * Constants::Animation::StartTextSpeed) + 1.0f) / 2.0f;
        int alpha = static_cast<int>(128 + 127 * pulse);
        startText.setFillColor(sf::Color(255, 255, 255, alpha));
        window.draw(startText);
    } else if (state == GameState::GameOver) {
        window.draw(gameOverText);
        window.draw(gameOverStats);
    } else if (state == GameState::Paused) {
        window.draw(pauseText);
    }
    
    if (state != GameState::Start) {
        window.draw(controlsText);
    }
    
    window.draw(fpsText);
    
    window.display();
}

void Game::resetGame() {
    delete snake;
    delete food;
    
    score = 0;
    state = GameState::Playing;
    updateTimer = 0.f;
    updateInterval = Constants::Gameplay::InitialUpdateInterval;
    
    snake = new Snake(grid);
    food = new Food(grid, *snake);
}

void Game::updateScore(int points) {
    score += points;
    if (score > highScore) {
        highScore = score;
    }
    updateWindowTitle();
}

void Game::updateWindowTitle() {
    std::string title = Constants::Window::Title;
    title += " - Score: " + std::to_string(score);
    if (score == highScore && score > 0) {
        title += " (High Score!)";
    }
    window.setTitle(title);
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