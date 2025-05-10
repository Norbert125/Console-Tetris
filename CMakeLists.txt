cmake_minimum_required(VERSION 3.30)
project(my_tetris C)

set(CMAKE_C_STANDARD 23)

add_executable(my_tetris main.c
        game/game.c
        game/game.h
        saveScore/saveScore.c
        saveScore/saveScore.h
        game/tetromino.c
        game/tetromino.h)
