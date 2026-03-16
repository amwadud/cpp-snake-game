#include "FunnyMessages.h"
#include <random>
#include <ctime>

FunnyMessages::FunnyMessages() {
    std::srand(static_cast<unsigned int>(std::time(nullptr)));
    initMessages();
}

void FunnyMessages::initMessages() {
    playerMessages = {
        {"YUMMY! Tastes like victory!", sf::Color{250, 189, 47}},
        {"Snack time!", sf::Color{184, 187, 38}},
        {"Mmm... delicious!", sf::Color{254, 128, 25}},
        {"Another one bites the dust!", sf::Color{131, 165, 152}},
        {"Nom nom nom!", sf::Color{184, 187, 38}},
        {"That's my food!", sf::Color{254, 128, 25}},
        {"Food truck acquired!", sf::Color{250, 189, 47}},
        {"Chomp chomp!", sf::Color{184, 187, 38}},
        {"Perfect timing!", sf::Color{131, 165, 152}},
        {"Can't touch this!", sf::Color{254, 128, 25}}
    };
    
    aiMessages = {
        {"Oops! I ate it first!", sf::Color{251, 73, 52}},
        {"Better luck next time!", sf::Color{204, 136, 98}},
        {"Speed is key, my friend!", sf::Color{211, 134, 155}},
        {"Was that your food? Oops!", sf::Color{251, 73, 52}},
        {"Too slow!", sf::Color{204, 136, 98}},
        {"I'm just better at this!", sf::Color{211, 134, 155}},
        {"Snake 2, You 0", sf::Color{251, 73, 52}},
        {"Who needs skills when you have AI?", sf::Color{204, 136, 98}},
        {"GG EZ!", sf::Color{211, 134, 155}},
        {"Mind = Blown!", sf::Color{251, 73, 52}}
    };
    
    deathMessages = {
        {"Oof! You hit a wall!", sf::Color{251, 73, 52}},
        {"Watch where you're going!", sf::Color{204, 136, 98}},
        {"That's gotta hurt!", sf::Color{211, 134, 155}},
        {"Did you see that coming?", sf::Color{251, 73, 52}},
        {"Your snake has left the chat!", sf::Color{204, 136, 98}},
        {"RIP Snakey McSnakeface!", sf::Color{211, 134, 155}},
        {"Try again, champ!", sf::Color{250, 189, 47}},
        {"Almost had it!", sf::Color{254, 128, 25}},
        {"So close, yet so far!", sf::Color{131, 165, 152}},
        {"Don't give up!", sf::Color{184, 187, 38}}
    };
    
    winMessages = {
        {"VICTORY! You're the snake master!", sf::Color{184, 187, 38}},
        {"Incredible! You won!", sf::Color{250, 189, 47}},
        {"Winner winner snake dinner!", sf::Color{254, 128, 25}},
        {"LEGENDARY gameplay!", sf::Color{211, 134, 155}},
        {"They never stood a chance!", sf::Color{184, 187, 38}},
        {"Absolute domination!", sf::Color{250, 189, 47}}
    };
}

FunnyMessages::Message FunnyMessages::getRandomPlayerMessage() {
    int idx = std::rand() % playerMessages.size();
    return {playerMessages[idx].first, playerMessages[idx].second, 2.0f};
}

FunnyMessages::Message FunnyMessages::getRandomAIMessage(const std::string& aiName) {
    int idx = std::rand() % aiMessages.size();
    std::string msg = aiName + ": " + aiMessages[idx].first;
    return {msg, aiMessages[idx].second, 2.0f};
}

FunnyMessages::Message FunnyMessages::getRandomDeathMessage(const std::string& killerName) {
    int idx = std::rand() % deathMessages.size();
    std::string msg = killerName + " says: " + deathMessages[idx].first;
    return {msg, deathMessages[idx].second, 2.5f};
}

FunnyMessages::Message FunnyMessages::getRandomWinMessage() {
    int idx = std::rand() % winMessages.size();
    return {winMessages[idx].first, winMessages[idx].second, 3.0f};
}
