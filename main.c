#include "game/game.h"
#include "saveScore/saveScore.h"
#include <stdio.h>

#define BOARD_WIDTH 10
#define BOARD_HEIGHT 20

int main() {
    int board[BOARD_HEIGHT][BOARD_WIDTH] = {0};
    int score = 0;
    gameLoop(board, &score);
    saveScore(score);
    printf("Press Enter to Exit...\n");
    getchar();
    return 0;
}