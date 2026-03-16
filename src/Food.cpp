#include "Food.h"
#include "Constants.h"

Food::Food(const Grid& grid, const Snake& snake)
    : grid(grid), rng(std::random_device{}()) {
    
    float radius = static_cast<float>(grid.getCellSize()) / 2.f - 4.f;
    foodShape.setRadius(radius);
    foodShape.setOrigin(sf::Vector2f(radius, radius));
    foodShape.setFillColor(Constants::Colors::Food);
    
    generatePosition(snake);
}

void Food::respawn(const Snake& snake) {
    generatePosition(snake);
}

sf::Vector2i Food::getPosition() const {
    return position;
}

void Food::render(sf::RenderWindow& window) {
    float x = static_cast<float>(position.x * grid.getCellSize() + grid.getCellSize() / 2);
    float y = static_cast<float>(position.y * grid.getCellSize() + grid.getCellSize() / 2);
    foodShape.setPosition(sf::Vector2f(x, y));
    window.draw(foodShape);
}

void Food::generatePosition(const Snake& snake) {
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
    }
}