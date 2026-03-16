#ifndef FOOD_H
#define FOOD_H

/**
 * @file Food.h
 * @brief Food entity and spawning logic
 */

#include <SFML/Graphics.hpp>
#include <random>
#include "Grid.h"
#include "Snake.h"

class Food {
public:
    Food(const Grid& grid, const Snake& snake);
    
    void respawn(const Snake& snake);
    sf::Vector2i getPosition() const;
    void setScale(float scale);
    
    void render(sf::RenderWindow& window);

private:
    const Grid& grid;
    sf::Vector2i position;
    sf::CircleShape foodShape;
    std::mt19937 rng;
    
    void generatePosition(const Snake& snake);
};

#endif