#ifndef GAME_MODE_H
#define GAME_MODE_H

/**
 * @file GameMode.h
 * @brief Game mode definitions
 */

enum class GameMode {
    Classic,
    TimeAttack,
    Zen,
    Challenge
};

struct GameModeInfo {
    const char* name;
    const char* description;
    bool hasDeath;
    bool hasTimer;
    int timerDuration;
    bool hasObstacles;
};

inline GameModeInfo getModeInfo(GameMode mode) {
    switch (mode) {
        case GameMode::Classic:
            return {"Classic", "Traditional snake gameplay", true, false, 0, false};
        case GameMode::TimeAttack:
            return {"Time Attack", "Score as much as possible in 3 minutes", true, true, 180, false};
        case GameMode::Zen:
            return {"Zen", "Relax - no death, just grow", false, false, 0, false};
        case GameMode::Challenge:
            return {"Challenge", "Obstacles appear as you grow", true, false, 0, true};
        default:
            return {"Classic", "Traditional snake gameplay", true, false, 0, false};
    }
}

#endif