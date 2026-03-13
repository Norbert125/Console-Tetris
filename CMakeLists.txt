cmake_minimum_required(VERSION 3.26)
project(Console_Tetris C)

set(CMAKE_C_STANDARD 11)

# 1. Source files (Only .c and .h files)
set(SOURCE_FILES
        main.c
        game/game.c
        game/game.h
        game/tetromino.c
        game/tetromino.h
        saveScore/saveScore.c
        saveScore/saveScore.h
)

# 2. Create the executable
add_executable(my_tetris ${SOURCE_FILES})

# 3. Include Directories (Where the compiler looks for headers)
target_include_directories(my_tetris PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/game)
target_include_directories(my_tetris PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})

# 4. Linking (The critical fix)
# We use target_link_libraries and point directly to the .a file.
# This assumes pdcurses.a is in your main project folder.
target_link_libraries(my_tetris PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/pdcurses.a")