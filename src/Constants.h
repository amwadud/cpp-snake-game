#ifndef CONSTANTS_H
#define CONSTANTS_H

/**
 * @file Constants.h
 * @brief Centralized game constants and configuration
 */

#include <SFML/Graphics.hpp>

namespace Constants {
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
        constexpr sf::Color Background{20, 20, 30};
        constexpr sf::Color GridLine{40, 40, 50};
        constexpr sf::Color SnakeHead{0, 200, 100};
        constexpr sf::Color SnakeBody{0, 150, 80};
        constexpr sf::Color Food{255, 80, 80};
        constexpr sf::Color TextWhite{255, 255, 255};
        constexpr sf::Color TextRed{255, 80, 80};
        constexpr sf::Color TextYellow{255, 255, 80};
    }
}

#endif