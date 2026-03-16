#ifndef GRID_H
#define GRID_H

/**
 * @file Grid.h
 * @brief Grid system for game board management
 */

#include <SFML/Graphics.hpp>

class Grid {
public:
    Grid(int cellSize, int width, int height);
    
    int getCellSize() const;
    int getWidth() const;
    int getHeight() const;
    int getCols() const;
    int getRows() const;
    
    bool isWithinBounds(int x, int y) const;
    sf::Vector2i snapToGrid(const sf::Vector2f& position) const;
    
private:
    int cellSize;
    int width;
    int height;
    int cols;
    int rows;
};

#endif