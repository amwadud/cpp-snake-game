#ifndef STATS_H
#define STATS_H

/**
 * @file Stats.h
 * @brief Game statistics tracking
 */

#include <string>

struct GameStats {
    int gamesPlayed = 0;
    int totalScore = 0;
    int highScore = 0;
    int totalFoodEaten = 0;
    int longestSnake = 0;
    
    void reset() {
        gamesPlayed = 0;
        totalScore = 0;
        totalFoodEaten = 0;
        longestSnake = 0;
    }
};

#endif