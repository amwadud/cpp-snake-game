/**
 * @file Obstacle.cpp
 * @brief Obstacle implementation
 */

#include "Obstacle.h"
#include "Constants.h"

Obstacle::Obstacle(const Grid& grid)
    : grid(grid), rng(std::random_device{}()) {
    
    obstacleShape.setFillColor(sf::Color(100, 100, 100));
}

void Obstacle::generate(const Snake& snake, int level) {
    int obstacleCount = 3 + level * 2;
    positions.clear();
    
    for (int i = 0; i < obstacleCount; ++i) {
        generatePosition(snake);
    }
}

void Obstacle::render(sf::RenderWindow& window) {
    float cellSize = static_cast<float>(grid.getCellSize());
    float size = cellSize - 4.f;
    
    for (const auto& pos : positions) {
        float x = pos.x * cellSize + cellSize / 2.f;
        float y = pos.y * cellSize + cellSize / 2.f;
        
        obstacleShape.setSize(sf::Vector2f(size, size));
        obstacleShape.setOrigin(sf::Vector2f(size/2.f, size/2.f));
        obstacleShape.setPosition(sf::Vector2f(x, y));
        
        float pulse = 0.7f + 0.3f * std::sin(static_cast<float>(pos.x + pos.y) * 0.5f);
        obstacleShape.setFillColor(sf::Color(
            static_cast<int>(120 * pulse),
            static_cast<int>(120 * pulse),
            static_cast<int>(130 * pulse)
        ));
        
        window.draw(obstacleShape);
    }
}

bool Obstacle::checkCollision(const sf::Vector2i& pos) const {
    for (const auto& obstacle : positions) {
        if (obstacle == pos) {
            return true;
        }
    }
    return false;
}

void Obstacle::clear() {
    positions.clear();
}

int Obstacle::getCount() const {
    return static_cast<int>(positions.size());
}

const std::vector<sf::Vector2i>& Obstacle::getObstacles() const {
    return positions;
}

void Obstacle::generatePosition(const Snake& snake) {
    std::uniform_int_distribution<int> distX(1, grid.getCols() - 2);
    std::uniform_int_distribution<int> distY(1, grid.getRows() - 2);
    
    bool validPosition = false;
    int attempts = 0;
    
    while (!validPosition && attempts < 100) {
        sf::Vector2i pos(distX(rng), distY(rng));
        
        validPosition = true;
        
        for (const auto& segment : snake.getBody()) {
            if (segment == pos) {
                validPosition = false;
                break;
            }
        }
        
        sf::Vector2i center(grid.getCols() / 2, grid.getRows() / 2);
        if (pos.x > center.x - 3 && pos.x < center.x + 3 &&
            pos.y > center.y - 3 && pos.y < center.y + 3) {
            validPosition = false;
        }
        
        for (const auto& existing : positions) {
            if (existing == pos) {
                validPosition = false;
                break;
            }
        }
        
        if (validPosition) {
            positions.push_back(pos);
        }
        
        attempts++;
    }
}