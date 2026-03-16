/**
 * @file Grid.cpp
 * @brief Grid implementation
 */

#include "Grid.h"

Grid::Grid(int cellSize, int width, int height)
    : cellSize(cellSize), width(width), height(height) {
    cols = width / cellSize;
    rows = height / cellSize;
}

int Grid::getCellSize() const { return cellSize; }
int Grid::getWidth() const { return width; }
int Grid::getHeight() const { return height; }
int Grid::getCols() const { return cols; }
int Grid::getRows() const { return rows; }

bool Grid::isWithinBounds(int x, int y) const {
    return x >= 0 && x < cols && y >= 0 && y < rows;
}

sf::Vector2i Grid::snapToGrid(const sf::Vector2f& position) const {
    return sf::Vector2i(
        static_cast<int>(position.x) / cellSize,
        static_cast<int>(position.y) / cellSize
    );
}