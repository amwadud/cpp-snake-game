#ifndef POWERUP_H
#define POWERUP_H

/**
 * @file PowerUp.h
 * @brief Power-up system
 */

#include <SFML/Graphics.hpp>
#include <random>
#include "Grid.h"
#include "Snake.h"

enum class PowerUpType { None, SpeedBoost, DoublePoints, SlowDown };

class PowerUp {
public:
    PowerUp(const Grid& grid, const Snake& snake);
    
    void respawn(const Snake& snake);
    sf::Vector2i getPosition() const;
    PowerUpType getType() const;
    bool isActive() const;
    void setActive(bool active);
    float getRemainingTime() const;
    void update(float deltaTime);
    
    void render(sf::RenderWindow& window);

private:
    const Grid& grid;
    sf::Vector2i position;
    PowerUpType type;
    bool active;
    float lifetime;
    float maxLifetime;
    
    sf::CircleShape powerUpShape;
    sf::CircleShape innerShape;
    std::mt19937 rng;
    
    void generatePosition(const Snake& snake);
    void setType(PowerUpType newType);
};

#endif