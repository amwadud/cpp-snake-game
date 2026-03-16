#ifndef PARTICLE_SYSTEM_H
#define PARTICLE_SYSTEM_H

/**
 * @file ParticleSystem.h
 * @brief Particle effect system
 */

#include <SFML/Graphics.hpp>
#include <vector>
#include <cmath>
#include <random>

struct Particle {
    sf::Vector2f position;
    sf::Vector2f velocity;
    sf::Color color;
    float lifetime;
    float maxLifetime;
    float size;
};

class ParticleSystem {
public:
    ParticleSystem();
    
    void emit(const sf::Vector2f& position, sf::Color color, int count = 20);
    void update(float deltaTime);
    void render(sf::RenderWindow& window);
    void clear();
    bool isActive() const;

private:
    std::vector<Particle> particles;
    std::mt19937 rng;
};

#endif