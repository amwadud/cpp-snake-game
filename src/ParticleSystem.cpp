/**
 * @file ParticleSystem.cpp
 * @brief Particle effect implementation
 */

#include "ParticleSystem.h"
#include "Constants.h"

ParticleSystem::ParticleSystem() : rng(std::random_device{}()) {}

void ParticleSystem::emit(const sf::Vector2f& position, sf::Color color, int count) {
    std::uniform_real_distribution<float> angleDist(0.f, 6.28318f);
    std::uniform_real_distribution<float> speedDist(50.f, 150.f);
    std::uniform_real_distribution<float> lifetimeDist(0.3f, 0.8f);
    std::uniform_real_distribution<float> sizeDist(3.f, 8.f);
    
    for (int i = 0; i < count; ++i) {
        float angle = angleDist(rng);
        float speed = speedDist(rng);
        
        Particle p;
        p.position = position;
        p.velocity = sf::Vector2f(std::cos(angle) * speed, std::sin(angle) * speed);
        p.color = color;
        p.maxLifetime = lifetimeDist(rng);
        p.lifetime = p.maxLifetime;
        p.size = sizeDist(rng);
        
        particles.push_back(p);
    }
}

void ParticleSystem::update(float deltaTime) {
    for (auto it = particles.begin(); it != particles.end();) {
        it->lifetime -= deltaTime;
        
        if (it->lifetime <= 0) {
            it = particles.erase(it);
            continue;
        }
        
        it->position += it->velocity * deltaTime;
        it->velocity *= 0.98f;
        
        float lifeRatio = it->lifetime / it->maxLifetime;
        it->color.a = static_cast<int>(255 * lifeRatio);
        
        ++it;
    }
}

void ParticleSystem::render(sf::RenderWindow& window) {
    for (const auto& p : particles) {
        sf::CircleShape shape(p.size * (p.lifetime / p.maxLifetime));
        shape.setOrigin(sf::Vector2f(shape.getRadius(), shape.getRadius()));
        shape.setPosition(p.position);
        shape.setFillColor(p.color);
        window.draw(shape);
    }
}

void ParticleSystem::clear() {
    particles.clear();
}

bool ParticleSystem::isActive() const {
    return !particles.empty();
}