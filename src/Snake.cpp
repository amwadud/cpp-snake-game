#include "Snake.h"

Snake::Snake(const Grid& grid)
    : grid(grid), direction(Direction::Right), nextDirection(Direction::Right), grew(false) {
    
    int startX = grid.getCols() / 2;
    int startY = grid.getRows() / 2;
    
    body.push_back(sf::Vector2i(startX, startY));
    body.push_back(sf::Vector2i(startX - 1, startY));
    body.push_back(sf::Vector2i(startX - 2, startY));
    
    segmentShape.setSize(sf::Vector2f(grid.getCellSize() - 2, grid.getCellSize() - 2));
    segmentShape.setOrigin(sf::Vector2f(1, 1));
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
    for (size_t i = 0; i < body.size(); ++i) {
        float x = static_cast<float>(body[i].x * grid.getCellSize());
        float y = static_cast<float>(body[i].y * grid.getCellSize());
        
        segmentShape.setPosition(sf::Vector2f(x + grid.getCellSize() / 2.f, y + grid.getCellSize() / 2.f));
        
        if (i == 0) {
            segmentShape.setFillColor(sf::Color(0, 200, 100));
        } else {
            segmentShape.setFillColor(sf::Color(0, 150, 80));
        }
        
        window.draw(segmentShape);
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