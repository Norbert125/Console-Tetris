#include "game/curses.h"
#include "game/game.h"
#include "saveScore/saveScore.h"

#define BOARD_WIDTH 10
#define BOARD_HEIGHT 20

int main() {
    /// --- PDCurses Initialization ---
    initscr();
    noecho();
    curs_set(0);
    keypad(stdscr, TRUE);
    nodelay(stdscr, TRUE);

    /// FIX: Initialize PDCurses color engine
    start_color();
    init_pair(1, COLOR_BLUE, COLOR_BLACK);
    init_pair(2, COLOR_GREEN, COLOR_BLACK);
    init_pair(3, COLOR_RED, COLOR_BLACK);
    init_pair(4, COLOR_YELLOW, COLOR_BLACK);
    init_pair(5, COLOR_CYAN, COLOR_BLACK);
    init_pair(6, COLOR_MAGENTA, COLOR_BLACK);
    init_pair(7, COLOR_WHITE, COLOR_BLACK);

    int board[BOARD_HEIGHT][BOARD_WIDTH] = {0};
    int score = 0;

    gameLoop(board, &score);
    // saveScore(score); // Uncomment this when you are ready

    /// --- PDCurses Cleanup ---
    endwin();

    return 0;
}