#include "AISnake.h"
#include <SFML/Graphics.hpp>
#include <random>
#include <algorithm>

AISnake::AISnake(const Grid& grid, sf::Color color, const std::string& name)
    : grid(grid), color(color), name(name), dead(false), grew(false) {
    
    body.push_back({grid.getCols() / 2 + 5, grid.getRows() / 2});
    body.push_back({grid.getCols() / 2 + 4, grid.getRows() / 2});
    body.push_back({grid.getCols() / 2 + 3, grid.getRows() / 2});
    
    direction = Direction::Left;
    nextDirection = Direction::Left;
    
    float size = static_cast<float>(grid.getCellSize() - 2);
    segmentShape.setSize(sf::Vector2f(size, size));
    segmentShape.setOrigin(sf::Vector2f(size / 2.f, size / 2.f));
    
    float eyeRadius = grid.getCellSize() / 8.0f;
    eyeLeft.setRadius(eyeRadius);
    eyeRight.setRadius(eyeRadius);
    eyeLeft.setOrigin(sf::Vector2f(eyeRadius, eyeRadius));
    eyeRight.setOrigin(sf::Vector2f(eyeRadius, eyeRadius));
    eyeLeft.setFillColor(sf::Color::White);
    eyeRight.setFillColor(sf::Color::White);
    
    updateEyePositions();
}

void AISnake::update(const sf::Vector2i& foodPos, const std::vector<sf::Vector2i>& obstacles,
                     const std::vector<std::vector<sf::Vector2i>>& otherSnakes) {
    if (dead) return;
    
    direction = nextDirection;
    
    sf::Vector2i newHead = getNewHeadPosition();
    
    bool hitObstacle = false;
    for (const auto& obs : obstacles) {
        if (newHead == obs) hitObstacle = true;
    }
    for (const auto& snake : otherSnakes) {
        for (const auto& seg : snake) {
            if (newHead == seg) hitObstacle = true;
        }
    }
    
    if (newHead.x < 0 || newHead.x >= grid.getCols() || 
        newHead.y < 0 || newHead.y >= grid.getRows() || hitObstacle) {
        dead = true;
        return;
    }
    
    body.insert(body.begin(), newHead);
    
    if (!grew) {
        body.pop_back();
    } else {
        grew = false;
    }
    
    Direction bestDir = findBestDirection(foodPos, obstacles, otherSnakes);
    if (bestDir != direction && bestDir != Direction::Up) {
        if (!isSafe(bestDir, obstacles, otherSnakes)) {
            std::vector<Direction> dirs = {Direction::Up, Direction::Down, Direction::Left, Direction::Right};
            std::shuffle(dirs.begin(), dirs.end(), std::mt19937(std::random_device()()));
            for (auto d : dirs) {
                if (!isOppositeDirection(d) && isSafe(d, obstacles, otherSnakes)) {
                    bestDir = d;
                    break;
                }
            }
        }
        nextDirection = bestDir;
    }
    
    updateEyePositions();
}

sf::Vector2i AISnake::getNewHeadPosition() {
    sf::Vector2i head = body.front();
    switch (direction) {
        case Direction::Up:    head.y--; break;
        case Direction::Down:   head.y++; break;
        case Direction::Left:  head.x--; break;
        case Direction::Right: head.x++; break;
    }
    return head;
}

void AISnake::changeDirection(Direction dir) {
    if (!isOppositeDirection(dir)) {
        nextDirection = dir;
    }
}

Direction AISnake::getDirection() const {
    return direction;
}

const std::vector<sf::Vector2i>& AISnake::getBody() const {
    return body;
}

sf::Vector2i AISnake::getHead() const {
    return body.front();
}

int AISnake::getLength() const {
    return static_cast<int>(body.size());
}

bool AISnake::hasCollided() const {
    if (body.empty()) return true;
    
    sf::Vector2i head = body.front();
    if (head.x < 0 || head.x >= grid.getCols() || head.y < 0 || head.y >= grid.getRows()) {
        return true;
    }
    
    for (size_t i = 1; i < body.size(); i++) {
        if (head == body[i]) return true;
    }
    return false;
}

bool AISnake::hasEatenFood(const sf::Vector2i& foodPos) const {
    return !body.empty() && body.front() == foodPos;
}

void AISnake::grow() {
    grew = true;
}

bool AISnake::isDead() const {
    return dead;
}

void AISnake::setDead(bool d) {
    dead = d;
}

bool AISnake::isOppositeDirection(Direction dir) const {
    return (direction == Direction::Up && dir == Direction::Down) ||
           (direction == Direction::Down && dir == Direction::Up) ||
           (direction == Direction::Left && dir == Direction::Right) ||
           (direction == Direction::Right && dir == Direction::Left);
}

void AISnake::updateEyePositions() {
    float cellSize = static_cast<float>(grid.getCellSize());
    sf::Vector2f headPos(body.front().x * cellSize + cellSize / 2.f, 
                         body.front().y * cellSize + cellSize / 2.f);
    
    float eyeOffset = cellSize / 4.0f;
    
    switch (direction) {
        case Direction::Right:
            eyeLeft.setPosition(sf::Vector2f(headPos.x + eyeOffset, headPos.y - eyeOffset));
            eyeRight.setPosition(sf::Vector2f(headPos.x + eyeOffset, headPos.y + eyeOffset));
            break;
        case Direction::Left:
            eyeLeft.setPosition(sf::Vector2f(headPos.x - eyeOffset, headPos.y - eyeOffset));
            eyeRight.setPosition(sf::Vector2f(headPos.x - eyeOffset, headPos.y + eyeOffset));
            break;
        case Direction::Down:
            eyeLeft.setPosition(sf::Vector2f(headPos.x - eyeOffset, headPos.y + eyeOffset));
            eyeRight.setPosition(sf::Vector2f(headPos.x + eyeOffset, headPos.y + eyeOffset));
            break;
        case Direction::Up:
            eyeLeft.setPosition(sf::Vector2f(headPos.x - eyeOffset, headPos.y - eyeOffset));
            eyeRight.setPosition(sf::Vector2f(headPos.x + eyeOffset, headPos.y - eyeOffset));
            break;
    }
}

Direction AISnake::findBestDirection(const sf::Vector2i& foodPos,
                                      const std::vector<sf::Vector2i>& obstacles,
                                      const std::vector<std::vector<sf::Vector2i>>& otherSnakes) {
    sf::Vector2i head = body.front();
    
    int dx = foodPos.x - head.x;
    int dy = foodPos.y - head.y;
    
    std::vector<Direction> preferred;
    
    if (std::abs(dx) >= std::abs(dy)) {
        if (dx > 0) preferred.push_back(Direction::Right);
        else if (dx < 0) preferred.push_back(Direction::Left);
        if (dy > 0) preferred.push_back(Direction::Down);
        else if (dy < 0) preferred.push_back(Direction::Up);
    } else {
        if (dy > 0) preferred.push_back(Direction::Down);
        else if (dy < 0) preferred.push_back(Direction::Up);
        if (dx > 0) preferred.push_back(Direction::Right);
        else if (dx < 0) preferred.push_back(Direction::Left);
    }
    
    for (auto dir : preferred) {
        if (!isOppositeDirection(dir) && isSafe(dir, obstacles, otherSnakes)) {
            return dir;
        }
    }
    
    return direction;
}

bool AISnake::isSafe(Direction dir, const std::vector<sf::Vector2i>& obstacles,
                     const std::vector<std::vector<sf::Vector2i>>& otherSnakes) {
    sf::Vector2i testHead = body.front();
    
    switch (dir) {
        case Direction::Up:    testHead.y--; break;
        case Direction::Down:   testHead.y++; break;
        case Direction::Left:   testHead.x--; break;
        case Direction::Right:  testHead.x++; break;
    }
    
    if (testHead.x < 0 || testHead.x >= grid.getCols() ||
        testHead.y < 0 || testHead.y >= grid.getRows()) {
        return false;
    }
    
    for (const auto& obs : obstacles) {
        if (testHead == obs) return false;
    }
    
    for (const auto& snake : otherSnakes) {
        for (const auto& seg : snake) {
            if (testHead == seg) return false;
        }
    }
    
    for (const auto& seg : body) {
        if (testHead == seg) return false;
    }
    
    return true;
}

bool AISnake::isPositionOccupied(sf::Vector2i pos, const std::vector<sf::Vector2i>& obstacles,
                                  const std::vector<std::vector<sf::Vector2i>>& otherSnakes) {
    for (const auto& obs : obstacles) {
        if (pos == obs) return true;
    }
    for (const auto& snake : otherSnakes) {
        for (const auto& seg : snake) {
            if (pos == seg) return true;
        }
    }
    for (const auto& seg : body) {
        if (pos == seg) return true;
    }
    return false;
}

void AISnake::render(sf::RenderWindow& window) {
    if (dead) return;
    
    float cellSize = static_cast<float>(grid.getCellSize());
    float offset = cellSize / 2.f;
    
    for (size_t i = 0; i < body.size(); i++) {
        float x = static_cast<float>(body[i].x * cellSize) + offset;
        float y = static_cast<float>(body[i].y * cellSize) + offset;
        
        segmentShape.setPosition(sf::Vector2f(x, y));
        
        if (i == 0) {
            segmentShape.setFillColor(color);
        } else {
            sf::Color bodyColor = color;
            bodyColor.a = 200;
            float fade = 1.0f - static_cast<float>(i) / body.size() * 0.5f;
            bodyColor.r = static_cast<uint8_t>(bodyColor.r * fade);
            bodyColor.g = static_cast<uint8_t>(bodyColor.g * fade);
            bodyColor.b = static_cast<uint8_t>(bodyColor.b * fade);
            segmentShape.setFillColor(bodyColor);
        }
        
        window.draw(segmentShape);
    }
    
    if (!body.empty()) {
        window.draw(eyeLeft);
        window.draw(eyeRight);
    }
}
