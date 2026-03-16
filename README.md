# Snake Game

A classic Snake game built with SFML (Simple and Fast Multimedia Library).

## Features

- Smooth snake movement with keyboard controls
- Score tracking with high score persistence
- Game states: Playing, Paused, Game Over
- Progressive difficulty (speed increases as you eat)

## Requirements

- C++17 compatible compiler
- SFML 2.5+ (or 3.0+)

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

## Controls

- Arrow Keys or WASD: Move snake
- P or Escape: Pause game
- Enter: Restart after game over

## Project Structure

```
cpp-snake-game/
├── CMakeLists.txt      # Build configuration
├── src/
│   ├── main.cpp        # Entry point
│   ├── Game.cpp/h      # Main game loop and logic
│   ├── Snake.cpp/h     # Snake entity
│   ├── Food.cpp/h      # Food entity
│   ├── Grid.cpp/h      # Grid system
│   └── Constants.h     # Game constants
└── .gitignore
```

## License

MIT License
