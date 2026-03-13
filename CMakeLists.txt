cmake_minimum_required(VERSION 3.10)
project(my_tetris C)

set(CMAKE_C_STANDARD 99)

# 1. Tell CLion to look in the current folder for header files like curses.h
include_directories(${CMAKE_CURRENT_SOURCE_DIR})

target_link_directories(${PROJECT_NAME} winmm)

# 2. List all of your .c files here so CLion knows to compile them
add_executable(my_tetris
        game/game.c
        game/game.h
        saveScore/saveScore.c
        saveScore/saveScore.h
        game/tetromino.c
        game/tetromino.h
        game/curses.h
        main.c
)

# 3. Tell CLion to link the PDCurses engine to your game!
# (Make sure pdcurses.a is in your main project folder)
target_link_libraries(my_tetris ${CMAKE_CURRENT_SOURCE_DIR}/pdcurses.a)