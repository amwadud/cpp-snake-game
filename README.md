# Snake Game

A modern, feature-rich Snake game built with SFML (Simple and Fast Multimedia Library) featuring multiple game modes, particle effects, power-ups, and a polished Gruvbox-inspired visual style.

**Version:** 1.0.0

## Look!

<img width="809" height="607" alt="Main Menu" src="https://github.com/user-attachments/assets/466ba91f-8dc8-4eda-9e07-464b754383d0" />

<img width="809" height="607" alt="Gameplay" src="https://github.com/user-attachments/assets/53e94aac-2ff7-4060-86cb-d260ddf9df85" />

## Features

### Game Modes
- **Classic** - Traditional snake gameplay with increasing difficulty
- **Time Attack** - Race against the clock to eat as much as possible
- **Zen** - Relaxed mode with no game over (practice your skills)
- **Challenge** - Navigate through obstacles to test your limits

### Visual Effects
- Animated snake with eyes that follow movement direction
- Glowing food with pulsing multi-layer rendering
- Gradient background with smooth color transitions
- Particle burst effects when eating food
- Screen shake on collision
- Smooth animations throughout

### Power-Ups
- **Speed Boost** - Move faster for a limited time
- **Double Points** - Earn 2x points on food
- **Slow Down** - Slow time to plan your next move

### Controls
- Arrow Keys, WASD, or Vim keys (h,j,k,l): Move snake
- P or Escape: Pause game
- Enter: Select menu options / Restart after game over
- 1-4: Quick select game modes

## Requirements

- C++17 compatible compiler
- SFML 2.5+ (tested with SFML 3.0)

## Building

### Linux (Ubuntu/Debian)

```bash
sudo apt-get install libsfml-dev
mkdir build && cd build
cmake ..
make
./bin/snake_game
```

### Arch Linux

```bash
sudo pacman -S sfml
mkdir build && cd build
cmake ..
make
./bin/snake_game
```

### macOS

```bash
brew install sfml
mkdir build && cd build
cmake ..
make
./bin/snake_game
```

### Windows

1. Download SFML from https://www.sfml-dev.org/
2. Configure CMake with SFML_DIR pointing to your SFML installation
3. Build and run

## Quick Start

```bash
cmake --preset default
cmake --build build
./build/bin/snake_game
```

## Controls

| Key | Action |
|-----|--------|
| Arrow Keys / WASD / h,j,k,l | Move snake |
| P / Escape | Pause game |
| Enter | Select / Restart |
| 1-4 | Quick select game mode |

## Project Structure

```
cpp-snake-game/
├── CMakeLists.txt           # Build configuration
├── src/
│   ├── main.cpp             # Entry point
│   ├── Game.cpp/h           # Main game loop, menus, modes
│   ├── Snake.cpp/h          # Snake entity with animations
│   ├── Food.cpp/h           # Glowing food entity
│   ├── Grid.cpp/h           # Grid system
│   ├── Constants.h          # Game constants & colors
│   ├── ParticleSystem.cpp/h # Particle effects
│   ├── GradientBackground.h # Background renderer
│   ├── PowerUp.cpp/h        # Power-up system
│   ├── Obstacle.cpp/h       # Challenge mode obstacles
│   ├── GameMode.h           # Game mode definitions
│   └── SoundManager.h       # Sound system (placeholder)
└── .gitignore
```

## Configuration

Edit `src/Constants.h` to customize:
- Window size and grid cell size
- Gameplay speed and timing
- Color scheme
- Difficulty parameters

## License

MIT License
