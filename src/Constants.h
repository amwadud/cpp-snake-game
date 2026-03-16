#ifndef CONSTANTS_H
#define CONSTANTS_H

/**
 * @file Constants.h
 * @brief Centralized game constants and configuration
 */

#include <SFML/Graphics.hpp>

namespace Constants {
    /** Default high score file path */
    constexpr const char* HighScoreFile = "highscore.txt";
    
    namespace Version {
        constexpr int Major = 1;
        constexpr int Minor = 0;
        constexpr int Patch = 0;
        constexpr const char* String = "1.0.0";
    }
    
    namespace Window {
        constexpr unsigned int Width = 800;
        constexpr unsigned int Height = 600;
        constexpr const char* Title = "Snake Game";
        constexpr unsigned int FrameRateLimit = 60;
    }

    namespace Grid {
        constexpr int CellSize = 20;
        constexpr int Cols = Window::Width / CellSize;
        constexpr int Rows = Window::Height / CellSize;
    }

    namespace Gameplay {
        constexpr float InitialUpdateInterval = 0.12f;
        constexpr float MinUpdateInterval = 0.05f;
        constexpr float SpeedIncrease = 0.002f;
        constexpr int PointsPerFood = 10;
        constexpr int StartingSnakeLength = 3;
    }

    namespace Debug {
        constexpr bool ShowFPS = true;
        constexpr bool ShowDebugInfo = false;
    }
    
    namespace Animation {
        constexpr float StartTextSpeed = 3.0f;
        constexpr float FoodPulseSpeed = 5.0f;
        constexpr float HighScoreSpeed = 4.0f;
    }
    
    namespace Colors {
        // Gruvbox Material Dark
        constexpr sf::Color Background{28, 32, 33};      // #1d2021
        constexpr sf::Color GridLine{40, 40, 40};          // Dark gray
        constexpr sf::Color SnakeHead{184, 187, 38};       // #b8bb26 (Green)
        constexpr sf::Color SnakeBody{151, 156, 12};       // #979c0c (Dark Green)
        constexpr sf::Color Food{254, 128, 25};            // #fe8019 (Orange)
        constexpr sf::Color TextWhite{235, 219, 178};     // #ebdbb2
        constexpr sf::Color TextRed{251, 73, 52};         // #fb4934
        constexpr sf::Color TextYellow{250, 189, 47};     // #fabd2f
    }
}

#endif