#ifndef SNAKE_H
#define SNAKE_H

/**
 * @file Snake.h
 * @brief Snake entity and movement logic
 */

#include <SFML/Graphics.hpp>
#include <vector>
#include "Grid.h"

/** Cardinal direction for snake movement */
enum class Direction { Up, Down, Left, Right };

class Snake {
public:
    Snake(const Grid& grid);
    
    void update();
    void changeDirection(Direction dir);
    Direction getDirection() const;
    
    const std::vector<sf::Vector2i>& getBody() const;
    sf::Vector2i getHead() const;
    int getLength() const;
    bool hasCollided();
    bool hasEatenFood(const sf::Vector2i& foodPos) const;
    void grow();
    
    void render(sf::RenderWindow& window);

private:
    const Grid& grid;
    std::vector<sf::Vector2i> body;
    Direction direction;
    Direction nextDirection;
    bool grew;
    
    sf::RectangleShape segmentShape;
    sf::CircleShape eyeLeft;
    sf::CircleShape eyeRight;
    
    sf::Vector2i getNewHeadPosition();
    bool isOppositeDirection(Direction dir) const;
    void updateEyePositions();
};

#endif