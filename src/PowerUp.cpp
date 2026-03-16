/**
 * @file PowerUp.cpp
 * @brief Power-up implementation
 */

#include "PowerUp.h"
#include "Constants.h"

PowerUp::PowerUp(const Grid& grid, const Snake&)
    : grid(grid), rng(std::random_device{}()), type(PowerUpType::None), active(false), 
      lifetime(0.f), maxLifetime(10.f) {
    
    float radius = static_cast<float>(grid.getCellSize()) / 2.f - 2.f;
    powerUpShape.setRadius(radius);
    powerUpShape.setOrigin(sf::Vector2f(radius, radius));
    
    innerShape.setRadius(radius * 0.6f);
    innerShape.setOrigin(sf::Vector2f(radius * 0.6f, radius * 0.6f));
}

void PowerUp::respawn(const Snake& snake) {
    std::uniform_int_distribution<int> typeDist(0, 2);
    int typeVal = typeDist(rng);
    
    switch (typeVal) {
        case 0: setType(PowerUpType::SpeedBoost); break;
        case 1: setType(PowerUpType::DoublePoints); break;
        case 2: setType(PowerUpType::SlowDown); break;
        default: setType(PowerUpType::SpeedBoost); break;
    }
    
    generatePosition(snake);
    active = true;
    lifetime = maxLifetime;
}

void PowerUp::setType(PowerUpType newType) {
    type = newType;
    
    switch (type) {
        case PowerUpType::SpeedBoost:
            powerUpShape.setFillColor(sf::Color(250, 189, 47));
            innerShape.setFillColor(sf::Color(184, 187, 38));
            break;
        case PowerUpType::DoublePoints:
            powerUpShape.setFillColor(sf::Color(131, 165, 152));
            innerShape.setFillColor(sf::Color(152, 151, 26));
            break;
        case PowerUpType::SlowDown:
            powerUpShape.setFillColor(sf::Color(204, 36, 159));
            innerShape.setFillColor(sf::Color(143, 151, 74));
            break;
        default:
            break;
    }
}

sf::Vector2i PowerUp::getPosition() const {
    return position;
}

PowerUpType PowerUp::getType() const {
    return type;
}

bool PowerUp::isActive() const {
    return active;
}

void PowerUp::setActive(bool a) {
    active = a;
}

float PowerUp::getRemainingTime() const {
    return lifetime;
}

void PowerUp::update(float deltaTime) {
    if (active) {
        lifetime -= deltaTime;
        if (lifetime <= 0) {
            active = false;
        }
    }
}

void PowerUp::render(sf::RenderWindow& window) {
    if (!active) return;
    
    float x = static_cast<float>(position.x * grid.getCellSize() + grid.getCellSize() / 2);
    float y = static_cast<float>(position.y * grid.getCellSize() + grid.getCellSize() / 2);
    
    float pulse = 0.8f + 0.2f * std::sin(lifetime * 5.f);
    
    float radius = (static_cast<float>(grid.getCellSize()) / 2.f - 2.f) * pulse * 1.3f;
    sf::CircleShape glow(radius);
    glow.setOrigin(sf::Vector2f(radius, radius));
    glow.setPosition(sf::Vector2f(x, y));
    glow.setFillColor(sf::Color(powerUpShape.getFillColor().r, powerUpShape.getFillColor().g, 
                                 powerUpShape.getFillColor().b, 80));
    window.draw(glow);
    
    powerUpShape.setPosition(sf::Vector2f(x, y));
    window.draw(powerUpShape);
    
    innerShape.setPosition(sf::Vector2f(x, y));
    window.draw(innerShape);
}

void PowerUp::generatePosition(const Snake& snake) {
    std::uniform_int_distribution<int> distX(0, grid.getCols() - 1);
    std::uniform_int_distribution<int> distY(0, grid.getRows() - 1);
    
    bool validPosition = false;
    while (!validPosition) {
        position = sf::Vector2i(distX(rng), distY(rng));
        
        validPosition = true;
        for (const auto& segment : snake.getBody()) {
            if (segment == position) {
                validPosition = false;
                break;
            }
        }
        
        if (position == sf::Vector2i(grid.getCols() / 2, grid.getRows() / 2)) {
            validPosition = false;
        }
    }
}