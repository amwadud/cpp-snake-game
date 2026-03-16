#ifndef FUNNY_MESSAGES_H
#define FUNNY_MESSAGES_H

/**
 * @file FunnyMessages.h
 * @brief Funny messages displayed when AI eats food
 */

#include <string>
#include <vector>
#include <random>
#include <SFML/Graphics.hpp>

class FunnyMessages {
public:
    struct Message {
        std::string text;
        sf::Color color;
        float duration;
    };
    
    FunnyMessages();
    
    Message getRandomPlayerMessage();
    Message getRandomAIMessage(const std::string& aiName);
    Message getRandomDeathMessage(const std::string& killerName);
    Message getRandomWinMessage();
    
private:
    std::vector<std::pair<std::string, sf::Color>> playerMessages;
    std::vector<std::pair<std::string, sf::Color>> aiMessages;
    std::vector<std::pair<std::string, sf::Color>> deathMessages;
    std::vector<std::pair<std::string, sf::Color>> winMessages;
    
    std::mt19937 rng;
    
    void initMessages();
};

#endif
