cmake_minimum_required(VERSION 3.16)
project(SnakeGame VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(SFML 2.5 COMPONENTS graphics window system REQUIRED)

add_executable(snake_game
    src/main.cpp
    src/Game.cpp
    src/Game.h
    src/Snake.cpp
    src/Snake.h
    src/Food.cpp
    src/Food.h
    src/Grid.cpp
    src/Grid.h
)

target_include_directories(snake_game PRIVATE src)

target_link_libraries(snake_game PRIVATE
    sfml-graphics
    sfml-window
    sfml-system
)

set_target_properties(snake_game PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
)

install(TARGETS snake_game DESTINATION bin)