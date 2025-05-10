//
// Created by Zsolti on 5/9/2025.
//

#ifndef MY_TETRIS_TETROMINO_H
#define MY_TETRIS_TETROMINO_H

#define NUM_SHAPES 7

extern int T_shape[4][4];
extern int I_shape[4][4];
extern int L_shape[4][4];
extern int J_shape[4][4];
extern int Z_shape[4][4];
extern int S_shape[4][4];
extern int O_shape[4][4];
extern int tetrominos[NUM_SHAPES][4][4];

int canMoveDown(int board[20][10], int x, int y, int shape[4][4]);

void rotateClockwise(int shape[4][4], int result[4][4]);

void rotateCounterClockwise(int shape[4][4], int result[4][4]);

int canRotate(int board[20][10], int x, int y, int rotatedShape[4][4]);

#endif //MY_TETRIS_TETROMINO_H
