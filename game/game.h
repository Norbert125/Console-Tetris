//
// Created by Zsolti on 5/9/2025.
//

#ifndef MY_TETRIS_GAME_H
#define MY_TETRIS_GAME_H

int canMoveDown(int board[20][10], int x, int y, int shape[4][4]);

void shuffleBag(int *bag, int *bagIndex);

int getNextPiece(int *bag, int *bagIndex, int useBagSystem);

void gameLoop(int board[20][10], int *score);

void setColor(int level);

#endif //MY_TETRIS_GAME_H