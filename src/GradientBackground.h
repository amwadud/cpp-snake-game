#ifndef GRADIENT_BACKGROUND_H
#define GRADIENT_BACKGROUND_H

/**
 * @file GradientBackground.h
 * @brief Gradient background renderer
 */

#include <SFML/Graphics.hpp>

class GradientBackground {
public:
    GradientBackground(unsigned int width, unsigned int height);
    
    void render(sf::RenderWindow& window);

private:
    sf::VertexArray vertices;
    unsigned int width;
    unsigned int height;
};

#endif