#include "game/game.h"
#include "saveScore/saveScore.h"

#define BOARD_WIDTH 10
#define BOARD_HEIGHT 20

int main(void) {
    int board[BOARD_HEIGHT][BOARD_WIDTH] = {0};
    int score = 0;
    gameLoop(board, &score);
    saveScore(score);
    return 0;
}