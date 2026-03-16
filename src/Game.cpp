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
      updateInterval(Constants::Gameplay::InitialUpdateInterval) {
    
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
    gameOverText.setPosition(sf::Vector2f(Constants::Window::Width / 2.f, Constants::Window::Height / 2.f));
    
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
    controlsText.setString("Controls: Arrow Keys / WASD to move | P to pause");
    controlsText.setPosition(sf::Vector2f(10.f, static_cast<float>(Constants::Window::Height - 30)));
    
    startText.setFont(font);
    startText.setCharacterSize(48);
    startText.setFillColor(Constants::Colors::TextWhite);
    startText.setString("SNAKE\nPress ENTER to start");
    sf::FloatRect startBounds = startText.getLocalBounds();
    startText.setOrigin(sf::Vector2f(startBounds.position.x + startBounds.size.x / 2.f, startBounds.position.y + startBounds.size.y / 2.f));
    startText.setPosition(sf::Vector2f(Constants::Window::Width / 2.f, Constants::Window::Height / 2.f - 50.f));
    
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
    
    while (window.isOpen()) {
        sf::Time elapsed = clock.restart();
        float deltaTime = elapsed.asSeconds();
        
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
                snake->changeDirection(Direction::Up);
                break;
            case sf::Keyboard::Scancode::Down:
            case sf::Keyboard::Scancode::S:
                snake->changeDirection(Direction::Down);
                break;
            case sf::Keyboard::Scancode::Left:
            case sf::Keyboard::Scancode::A:
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
        }
    }
}

void Game::render() {
    window.clear(Constants::Colors::Background);
    
    sf::RectangleShape gridLine;
    gridLine.setFillColor(Constants::Colors::GridLine);
    
    for (int x = 0; x <= grid.getWidth(); x += grid.getCellSize()) {
        gridLine.setSize(sf::Vector2f(1, static_cast<float>(grid.getHeight())));
        gridLine.setPosition(sf::Vector2f(static_cast<float>(x), 0));
        window.draw(gridLine);
    }
    
    for (int y = 0; y <= grid.getHeight(); y += grid.getCellSize()) {
        gridLine.setSize(sf::Vector2f(static_cast<float>(grid.getWidth()), 1));
        gridLine.setPosition(sf::Vector2f(0, static_cast<float>(y)));
        window.draw(gridLine);
    }
    
    if (state != GameState::Start) {
        food->render(window);
        snake->render(window);
        
        scoreText.setString("Score: " + std::to_string(score) + 
                            " | Length: " + std::to_string(snake->getLength()) +
                            " | High: " + std::to_string(highScore));
        window.draw(scoreText);
    }
    
    if (state == GameState::Start) {
        window.draw(startText);
    } else if (state == GameState::GameOver) {
        window.draw(gameOverText);
    } else if (state == GameState::Paused) {
        window.draw(pauseText);
    }
    
    if (state != GameState::Start) {
        window.draw(controlsText);
    }
    
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
    std::ifstream file("highscore.txt");
    if (file.is_open()) {
        file >> highScore;
        file.close();
    }
}

void Game::saveHighScore() {
    std::ofstream file("highscore.txt");
    if (file.is_open()) {
        file << highScore;
        file.close();
    }
}