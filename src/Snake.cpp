/**
 * @file Snake.cpp
 * @brief Snake implementation
 */

#include "Snake.h"
#include "Constants.h"

Snake::Snake(const Grid& grid)
    : grid(grid), direction(Direction::Right), nextDirection(Direction::Right), grew(false) {
    
    int startX = grid.getCols() / 2;
    int startY = grid.getRows() / 2;
    
    for (int i = 0; i < Constants::Gameplay::StartingSnakeLength; ++i) {
        body.push_back(sf::Vector2i(startX - i, startY));
    }
    
    float size = static_cast<float>(grid.getCellSize() - 6);
    segmentShape.setSize(sf::Vector2f(size, size));
    segmentShape.setOrigin(sf::Vector2f(size/2.f, size/2.f));
    
    float eyeRadius = size / 5.f;
    eyeLeft.setRadius(eyeRadius);
    eyeRight.setRadius(eyeRadius);
    eyeLeft.setOrigin(sf::Vector2f(eyeRadius, eyeRadius));
    eyeRight.setOrigin(sf::Vector2f(eyeRadius, eyeRadius));
}

void Snake::update() {
    sf::Vector2i newHead = getNewHeadPosition();
    body.insert(body.begin(), newHead);
    
    if (!grew) {
        body.pop_back();
    } else {
        grew = false;
    }
}

void Snake::changeDirection(Direction dir) {
    if (!isOppositeDirection(dir)) {
        nextDirection = dir;
    }
}

Direction Snake::getDirection() const {
    return direction;
}

const std::vector<sf::Vector2i>& Snake::getBody() const {
    return body;
}

sf::Vector2i Snake::getHead() const {
    return body.front();
}

int Snake::getLength() const {
    return static_cast<int>(body.size());
}

bool Snake::hasCollided() {
    sf::Vector2i head = body.front();
    
    if (!grid.isWithinBounds(head.x, head.y)) {
        return true;
    }
    
    for (size_t i = 1; i < body.size(); ++i) {
        if (body[i] == head) {
            return true;
        }
    }
    
    direction = nextDirection;
    return false;
}

bool Snake::hasEatenFood(const sf::Vector2i& foodPos) const {
    return body.front() == foodPos;
}

void Snake::grow() {
    grew = true;
}

void Snake::render(sf::RenderWindow& window) {
    float cellSize = static_cast<float>(grid.getCellSize());
    float size = cellSize - 6.f;
    float offset = cellSize / 2.f;
    
    for (size_t i = 0; i < body.size(); ++i) {
        float x = static_cast<float>(body[i].x * cellSize) + offset;
        float y = static_cast<float>(body[i].y * cellSize) + offset;
        
        segmentShape.setPosition(sf::Vector2f(x, y));
        
        if (i == 0) {
            segmentShape.setFillColor(Constants::Colors::SnakeHead);
            
            float headX = body[i].x * cellSize + cellSize / 2.f;
            float headY = body[i].y * cellSize + cellSize / 2.f;
            
            float eyeOffset = size / 3.5f;
            float eyeSize = size / 4.f;
            
            eyeLeft.setRadius(eyeSize / 2.f);
            eyeRight.setRadius(eyeSize / 2.f);
            eyeLeft.setOrigin(sf::Vector2f(eyeSize / 4.f, eyeSize / 4.f));
            eyeRight.setOrigin(sf::Vector2f(eyeSize / 4.f, eyeSize / 4.f));
            
            float eye1X, eye1Y, eye2X, eye2Y;
            
            switch (direction) {
                case Direction::Up:
                    eye1X = headX - eyeOffset; eye1Y = headY - eyeOffset;
                    eye2X = headX + eyeOffset; eye2Y = headY - eyeOffset;
                    break;
                case Direction::Down:
                    eye1X = headX - eyeOffset; eye1Y = headY + eyeOffset;
                    eye2X = headX + eyeOffset; eye2Y = headY + eyeOffset;
                    break;
                case Direction::Left:
                    eye1X = headX - eyeOffset; eye1Y = headY - eyeOffset;
                    eye2X = headX - eyeOffset; eye2Y = headY + eyeOffset;
                    break;
                case Direction::Right:
                default:
                    eye1X = headX + eyeOffset; eye1Y = headY - eyeOffset;
                    eye2X = headX + eyeOffset; eye2Y = headY + eyeOffset;
                    break;
            }
            
            eyeLeft.setPosition(sf::Vector2f(eye1X, eye1Y));
            eyeRight.setPosition(sf::Vector2f(eye2X, eye2Y));
            eyeLeft.setFillColor(sf::Color(30, 30, 30));
            eyeRight.setFillColor(sf::Color(30, 30, 30));
            
            sf::CircleShape pupilLeft(eyeSize / 5.f);
            sf::CircleShape pupilRight(eyeSize / 5.f);
            pupilLeft.setOrigin(sf::Vector2f(eyeSize / 5.f, eyeSize / 5.f));
            pupilRight.setOrigin(sf::Vector2f(eyeSize / 5.f, eyeSize / 5.f));
            pupilLeft.setPosition(sf::Vector2f(eye1X, eye1Y));
            pupilRight.setPosition(sf::Vector2f(eye2X, eye2Y));
            pupilLeft.setFillColor(sf::Color(10, 10, 10));
            pupilRight.setFillColor(sf::Color(10, 10, 10));
            
            window.draw(segmentShape);
            window.draw(eyeLeft);
            window.draw(eyeRight);
            window.draw(pupilLeft);
            window.draw(pupilRight);
        } else {
            segmentShape.setFillColor(Constants::Colors::SnakeBody);
            window.draw(segmentShape);
        }
    }
}

sf::Vector2i Snake::getNewHeadPosition() {
    sf::Vector2i head = body.front();
    direction = nextDirection;
    
    switch (direction) {
        case Direction::Up:    return sf::Vector2i(head.x, head.y - 1);
        case Direction::Down:  return sf::Vector2i(head.x, head.y + 1);
        case Direction::Left:  return sf::Vector2i(head.x - 1, head.y);
        case Direction::Right: return sf::Vector2i(head.x + 1, head.y);
    }
    return head;
}

bool Snake::isOppositeDirection(Direction dir) const {
    if (direction == Direction::Up && dir == Direction::Down) return true;
    if (direction == Direction::Down && dir == Direction::Up) return true;
    if (direction == Direction::Left && dir == Direction::Right) return true;
    if (direction == Direction::Right && dir == Direction::Left) return true;
    return false;
}