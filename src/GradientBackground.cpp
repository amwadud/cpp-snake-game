/**
 * @file GradientBackground.cpp
 * @brief Gradient background implementation
 */

#include "GradientBackground.h"
#include "Constants.h"

GradientBackground::GradientBackground(unsigned int width, unsigned int height)
    : width(width), height(height) {
    
    vertices = sf::VertexArray(sf::PrimitiveType::TriangleStrip, 4);
    
    sf::Color topColor(28, 32, 33);
    sf::Color bottomColor(40, 50, 60);
    
    sf::Vertex v0, v1, v2, v3;
    v0.position = sf::Vector2f(0, 0);
    v0.color = topColor;
    v1.position = sf::Vector2f(width, 0);
    v1.color = topColor;
    v2.position = sf::Vector2f(0, height);
    v2.color = bottomColor;
    v3.position = sf::Vector2f(width, height);
    v3.color = bottomColor;
    
    vertices[0] = v0;
    vertices[1] = v1;
    vertices[2] = v2;
    vertices[3] = v3;
}

void GradientBackground::render(sf::RenderWindow& window) {
    window.draw(vertices);
}