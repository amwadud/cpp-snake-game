cmake_minimum_required(VERSION 3.16)
project(SnakeGame VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

if(CMAKE_BUILD_TYPE STREQUAL "")
    set(CMAKE_BUILD_TYPE Release)
endif()

option(ENABLE_WERROR "Treat warnings as errors" OFF)
option(ENABLE_TESTING "Build tests" OFF)

find_package(SFML 3.0 COMPONENTS Graphics Window System)

if(NOT SFML_FOUND)
    message(FATAL_ERROR "SFML not found. Please install libsfml-dev")
endif()

message(STATUS "Found SFML: ${SFML_VERSION}")

include_directories(${SFML_INCLUDE_DIR})

set(SOURCES
    src/main.cpp
    src/Game.cpp
    src/Game.h
    src/Snake.cpp
    src/Snake.h
    src/Food.cpp
    src/Food.h
    src/Grid.cpp
    src/Grid.h
    src/Constants.h
)

add_executable(snake_game ${SOURCES})

target_include_directories(snake_game PRIVATE ${CMAKE_SOURCE_DIR}/src)

target_link_libraries(snake_game PRIVATE
    sfml-graphics
    sfml-window
    sfml-system
)

target_compile_options(snake_game PRIVATE
    $<$<CXX_COMPILER_ID:GNU,Clang>:-Wall -Wextra -Wpedantic>
    $<$<CXX_COMPILER_ID:MSVC>:/W4>
)

set_target_properties(snake_game PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
    CXX_STANDARD 17
)

install(TARGETS snake_game DESTINATION bin)

message(STATUS "Snake Game ${PROJECT_VERSION}")
message(STATUS "Build type: ${CMAKE_BUILD_TYPE}")
message(STATUS "SFML version: ${SFML_VERSION}")