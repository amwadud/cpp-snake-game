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
    Challenge,
    Battle,
    Marathon,
    IronSnake
};

struct GameModeInfo {
    const char* name;
    const char* description;
    bool hasDeath;
    bool hasTimer;
    int timerDuration;
    bool hasObstacles;
    bool hasAI;
    int aiCount;
};

inline GameModeInfo getModeInfo(GameMode mode) {
    switch (mode) {
        case GameMode::Classic:
            return {"Classic", "Traditional snake gameplay", true, false, 0, false, false, 0};
        case GameMode::TimeAttack:
            return {"Time Attack", "Score as much as possible in 3 minutes", true, true, 180, false, false, 0};
        case GameMode::Zen:
            return {"Zen", "Relax - no death, just grow", false, false, 0, false, false, 0};
        case GameMode::Challenge:
            return {"Challenge", "Obstacles appear as you grow", true, false, 0, true, false, 0};
        case GameMode::Battle:
            return {"Battle", "Compete against AI snakes!", true, false, 0, false, true, 3};
        case GameMode::Marathon:
            return {"Marathon", "Endless mode - survive as long as you can", true, false, 0, false, false, 0};
        case GameMode::IronSnake:
            return {"Iron Snake", "One life - don't mess up!", true, false, 0, false, false, 0};
        default:
            return {"Classic", "Traditional snake gameplay", true, false, 0, false, false, 0};
    }
}

#endif
