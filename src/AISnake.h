#ifndef AI_SNAKE_H
#define AI_SNAKE_H

/**
 * @file AISnake.h
 * @brief AI-controlled snake for Battle mode
 */

#include <SFML/Graphics.hpp>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <algorithm>
#include "Grid.h"
#include "Snake.h"

class AISnake {
public:
    AISnake(const Grid& grid, sf::Color color, const std::string& name);
    
    void update(const sf::Vector2i& foodPos, const std::vector<sf::Vector2i>& obstacles,
                const std::vector<std::vector<sf::Vector2i>>& otherSnakes);
    void changeDirection(Direction dir);
    Direction getDirection() const;
    
    const std::vector<sf::Vector2i>& getBody() const;
    sf::Vector2i getHead() const;
    int getLength() const;
    bool hasCollided() const;
    bool hasEatenFood(const sf::Vector2i& foodPos) const;
    void grow();
    bool isDead() const;
    void setDead(bool dead);
    
    void render(sf::RenderWindow& window);
    
    const std::string& getName() const { return name; }

private:
    const Grid& grid;
    std::vector<sf::Vector2i> body;
    Direction direction;
    Direction nextDirection;
    bool grew;
    bool dead;
    sf::Color color;
    std::string name;
    
    sf::RectangleShape segmentShape;
    sf::CircleShape eyeLeft;
    sf::CircleShape eyeRight;
    
    sf::Vector2i getNewHeadPosition();
    bool isOppositeDirection(Direction dir) const;
    void updateEyePositions();
    Direction findBestDirection(const sf::Vector2i& foodPos,
                                const std::vector<sf::Vector2i>& obstacles,
                                const std::vector<std::vector<sf::Vector2i>>& otherSnakes);
    bool isSafe(Direction dir, const std::vector<sf::Vector2i>& obstacles,
                const std::vector<std::vector<sf::Vector2i>>& otherSnakes);
    bool isPositionOccupied(sf::Vector2i pos, const std::vector<sf::Vector2i>& obstacles,
                           const std::vector<std::vector<sf::Vector2i>>& otherSnakes);
};

#endif
