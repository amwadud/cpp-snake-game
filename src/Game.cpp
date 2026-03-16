#include "Game.h"
#include <fstream>

Game::Game()
    : grid(20, 800, 600),
      snake(nullptr),
      food(nullptr),
      state(GameState::Playing),
      score(0),
      highScore(0),
      updateTimer(0.f),
      updateInterval(0.12f) {
    
    window.create(sf::VideoMode(800, 600), "Snake Game", sf::Style::Close);
    window.setFramerate(60);
    
    if (!font.loadFromFile("/usr/share/fonts/truetype/dejavu/DejaVuSans.ttf") &&
        !font.loadFromFile("/usr/share/fonts/dejavu/DejaVuSans.ttf") &&
        !font.loadFromFile("C:/Windows/Fonts/arial.ttf")) {
        if (font.loadFromFile("/System/Library/Fonts/Helvetica.ttc")) {
            std::cerr << "Warning: Using system Helvetica font" << std::endl;
        }
    }
    
    scoreText.setFont(font);
    scoreText.setCharacterSize(24);
    scoreText.setFillColor(sf::Color::White);
    scoreText.setPosition(10, 10);
    
    gameOverText.setFont(font);
    gameOverText.setCharacterSize(48);
    gameOverText.setFillColor(sf::Color::Red);
    gameOverText.setString("GAME OVER\nPress ENTER to restart");
    sf::FloatRect bounds = gameOverText.getLocalBounds();
    gameOverText.setOrigin(bounds.width / 2.f, bounds.height / 2.f);
    gameOverText.setPosition(400.f, 300.f);
    
    pauseText.setFont(font);
    pauseText.setCharacterSize(48);
    pauseText.setFillColor(sf::Color::Yellow);
    pauseText.setString("PAUSED\nPress P to resume");
    bounds = pauseText.getLocalBounds();
    pauseText.setOrigin(bounds.width / 2.f, bounds.height / 2.f);
    pauseText.setPosition(400.f, 300.f);
    
    loadHighScore();
    
    snake = new Snake(grid);
    food = new Food(grid, *snake);
    
    if (eatBuffer.loadFromFile("/usr/share/sounds/snake_eat.wav")) {
        eatSound.setBuffer(eatBuffer);
    }
    if (dieBuffer.loadFromFile("/usr/share/sounds/snake_die.wav")) {
        dieSound.setBuffer(dieBuffer);
    }
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
    sf::Event event;
    while (window.pollEvent(event)) {
        if (event.type == sf::Event::Closed) {
            window.close();
        }
        
        if (event.type == sf::Event::KeyPressed) {
            handleKeyPress(event.key.code);
        }
    }
}

void Game::handleKeyPress(sf::Keyboard::Key key) {
    if (state == GameState::Playing) {
        switch (key) {
            case sf::Keyboard::Up:
            case sf::Keyboard::W:
                snake->changeDirection(Direction::Up);
                break;
            case sf::Keyboard::Down:
            case sf::Keyboard::S:
                snake->changeDirection(Direction::Down);
                break;
            case sf::Keyboard::Left:
            case sf::Keyboard::A:
                snake->changeDirection(Direction::Left);
                break;
            case sf::Keyboard::Right:
            case sf::Keyboard::D:
                snake->changeDirection(Direction::Right);
                break;
            case sf::Keyboard::P:
            case sf::Keyboard::Escape:
                state = GameState::Paused;
                break;
            default:
                break;
        }
    } else if (state == GameState::GameOver) {
        if (key == sf::Keyboard::Enter) {
            resetGame();
        }
    } else if (state == GameState::Paused) {
        if (key == sf::Keyboard::P || key == sf::Keyboard::Escape) {
            state = GameState::Playing;
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
            if (dieSound.getBuffer()) {
                dieSound.play();
            }
            if (score > highScore) {
                highScore = score;
                saveHighScore();
            }
            return;
        }
        
        if (snake->hasEatenFood(food->getPosition())) {
            snake->grow();
            food->respawn(*snake);
            updateScore(10);
            if (eatSound.getBuffer()) {
                eatSound.play();
            }
        }
    }
}

void Game::render() {
    window.clear(sf::Color(20, 20, 30));
    
    sf::RectangleShape gridLine;
    gridLine.setFillColor(sf::Color(40, 40, 50));
    
    for (int x = 0; x <= grid.getWidth(); x += grid.getCellSize()) {
        gridLine.setSize(sf::Vector2f(1, grid.getHeight()));
        gridLine.setPosition(x, 0);
        window.draw(gridLine);
    }
    
    for (int y = 0; y <= grid.getHeight(); y += grid.getCellSize()) {
        gridLine.setSize(sf::Vector2f(grid.getWidth(), 1));
        gridLine.setPosition(0, y);
        window.draw(gridLine);
    }
    
    food->render(window);
    snake->render(window);
    
    scoreText.setString("Score: " + std::to_string(score) + " | High Score: " + std::to_string(highScore));
    window.draw(scoreText);
    
    if (state == GameState::GameOver) {
        window.draw(gameOverText);
    } else if (state == GameState::Paused) {
        window.draw(pauseText);
    }
    
    window.display();
}

void Game::resetGame() {
    delete snake;
    delete food;
    
    score = 0;
    state = GameState::Playing;
    updateTimer = 0.f;
    
    snake = new Snake(grid);
    food = new Food(grid, *snake);
}

void Game::updateScore(int points) {
    score += points;
    if (score > highScore) {
        highScore = score;
    }
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