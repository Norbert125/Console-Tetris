//
// Created by Zsolti on 5/9/2025.
//

#include "tetromino.h"


/// definition of tetris shapes:
int T_shape[4][4];
int I_shape[4][4];
int J_shape[4][4];
int L_shape[4][4];
int O_shape[4][4];
int S_shape[4][4];
int Z_shape[4][4];
int tetrominos[NUM_SHAPES][4][4] = {
        {
                {0, 1, 0, 0},
                {1, 1, 1, 0},
                {0, 0, 0, 0},
                {0, 0, 0, 0}
        }, /// T shape
        {
                {0, 0, 0, 0},
                {1, 1, 1, 1},
                {0, 0, 0, 0},
                {0, 0, 0, 0}
        }, /// i shiape
        {
                {1, 0, 0, 0},
                {1, 1, 1, 0},
                {0, 0, 0, 0},
                {0, 0, 0, 0}
        }, /// J shape
        {
                {0, 0, 1, 0},
                {1, 1, 1, 0},
                {0, 0, 0, 0},
                {0, 0, 0, 0}
        }, /// L shape
        {
                {1, 1, 0, 0},
                {1, 1, 0, 0},
                {0, 0, 0, 0},
                {0, 0, 0, 0}
        }, /// O shape
        {
                {0, 1, 1, 0},
                {1, 1, 0, 0},
                {0, 0, 0, 0},
                {0, 0, 0, 0}
        }, /// S shape
        {
                {1, 1, 0, 0},
                {0, 1, 1, 0},
                {0, 0, 0, 0},
                {0, 0, 0, 0}
        }, /// Z shape
};

int canMoveDown(int board[20][10], int x, int y, int shape[4][4]) {
    for (int row = 0; row < 4; ++row) {
        for (int col = 0; col < 4; ++col) {
            if (shape[row][col]) {
                int newY = y + row + 1;
                int newX = x + col;
                if (newY >= 20 || (board[newY][newX] == 1)) {
                    return 0; /// stop
                }
            }
        }
    }
    return 1;  ///safe
}

void rotateClockwise(int shape[4][4], int result[4][4]) {
    for (int row = 0; row < 4; ++row) {
        for (int col = 0; col < 4; ++col) {
            result[col][3 - row] = shape[row][col];
        }
    }
}

void rotateCounterClockwise(int shape[4][4], int result[4][4]) {
    for (int row = 0; row < 4; ++row) {
        for (int col = 0; col < 4; ++col) {
            result[3 - col][row] = shape[row][col];
        }
    }
}

int canRotate(int board[20][10], int x, int y, int rotatedShape[4][4]) {
    for (int row = 0; row < 4; ++row) {
        for (int col = 0; col < 4; ++col) {
            if (rotatedShape[row][col]) {
                int newX = x + col;
                int newY = y + row;
                if (newX < 0 || newX >= 10 || newY < 0 || newY >= 20 || board[newY][newX]) {
                    return 0; /// Rotation is not possible
                }
            }
        }
    }
    return 1;  /// Rotation is possible
}