#ifndef OBSTACLE_H
#define OBSTACLE_H

/**
 * @file Obstacle.h
 * @brief Obstacle system for Challenge mode
 */

#include <SFML/Graphics.hpp>
#include <vector>
#include <random>
#include "Grid.h"
#include "Snake.h"

class Obstacle {
public:
    Obstacle(const Grid& grid);
    
    void generate(const Snake& snake, int level);
    void render(sf::RenderWindow& window);
    bool checkCollision(const sf::Vector2i& pos) const;
    void clear();
    int getCount() const;
    const std::vector<sf::Vector2i>& getObstacles() const;

private:
    const Grid& grid;
    std::vector<sf::Vector2i> positions;
    sf::RectangleShape obstacleShape;
    std::mt19937 rng;
    
    void generatePosition(const Snake& snake);
};

#endif