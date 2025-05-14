#include "game.h"
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include "tetromino.h"

void setColor(int level) {
    int color = 7;
    switch (level) {
        case 1:
            color = 9;  /// Blue
            break;
        case 2:
            color = 10; /// Green
            break;
        case 3:
            color = 12; /// Red
            break;
        case 4:
            color = 14; /// Yellow
            break;
        case 5:
            color = 11; /// Cyan
            break;
        case 6:
            color = 13; /// Magenta
            break;
        default:
            color = 15; /// White
            break;
    }

    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole, color);
}

void gameLoop(int board[20][10], int *score) {
    srand(time(NULL));
    int gameOver = 0;
    int linesClear = 0;
    int level = 1;
    int currentShape[4][4] = {0}; /// current shape
    int rotatedShape[4][4] = {0}; /// rotated shape
    int x = 3, y = 0;
    int nextShape = rand() % NUM_SHAPES;
    int currentPiece = nextShape;
    memcpy(currentShape, tetrominos[nextShape], sizeof(tetrominos[0]));
    nextShape = rand() % NUM_SHAPES;
    while (!gameOver) {
        /// clear console
        system("cls");

        int displayBoard[20][10];

        /// display board - game board
        for (int row = 0; row < 20; row++) {
            for (int col = 0; col < 10; col++) {
                displayBoard[row][col] = board[row][col];
            }
        }

        /// render current piece
        for (int row = 0; row < 4; row++) {
            for (int col = 0; col < 4; col++) {
                if (currentShape[row][col] == 1) {
                    int boardY = y + row;
                    int boardX = x + col;
                    if (boardY >= 0 && boardY < 20 && boardX >= 0 && boardX < 10) {
                        displayBoard[boardY][boardX] = 1;
                    }
                }
            }
        }
        setColor(level); /// setting starting level color
        /// Drawing:
        for (int row = 0; row < 20; ++row) {
            for (int col = 0; col < 10; ++col) {
                if (displayBoard[row][col] == 1) {
                    printf("# ");
                } else {
                    printf(". ");
                }
            }
            printf("\n");
        }
        setColor(7);

        printf("-------------------\n"); /// separator!

        printf("Score: %d\n", *score); /// printing score

        printf("Level: %d\n", level); /// printing current level

        /// drawing next shape
        printf("\nNext piece:\n");

        for (int row = 0; row < 4; row++) {
            for (int col = 0; col < 4; col++) {
                printf("%c ", tetrominos[nextShape][row][col] ? '#' : ' ');
            }
            printf("\n");
        }

        /// Get Input
        if (GetAsyncKeyState(VK_LEFT) & 0x8000) {
            int canMove = 1;
            for (int row = 0; row < 4; ++row) {
                for (int col = 0; col < 4; ++col) {
                    if (currentShape[row][col]) {
                        int newX = x + col - 1;
                        int newY = y + row;
                        if (newX < 0 || board[newY][newX]) {
                            canMove = 0;
                        }
                    }
                }
            }
            if (canMove) x--;
        }
        if (GetAsyncKeyState(VK_RIGHT) & 0x8000) {
            int canMove = 1;
            for (int row = 0; row < 4; ++row) {
                for (int col = 0; col < 4; ++col) {
                    if (currentShape[row][col]) {
                        int newX = x + col + 1;
                        int newY = y + row;
                        if (newX >= 10 || board[newY][newX]) {
                            canMove = 0;
                        }
                    }
                }
            }
            if (canMove) x++;
        }

        /// push down
        if (GetAsyncKeyState(VK_DOWN) & 0x8000) {
            if (canMoveDown(board, x, y, currentShape)) {
                y++;
                score += 10;
            }
        }
        static int zPressed = 0;
        static int xPressed = 0;

        /// rotate counter clockwise
        if ((GetAsyncKeyState('Z') & 0x8000) && !zPressed) {
            rotateCounterClockwise(currentShape, rotatedShape);
            if (canRotate(board, x, y, rotatedShape)) {
                memcpy(currentShape, rotatedShape, sizeof(rotatedShape));
            }
            zPressed = 1;
        }
        if (!(GetAsyncKeyState('Z') & 0x8000)) {
            zPressed = 0;
        }
        /// rotate clockwise
        if ((GetAsyncKeyState('X') & 0x8000) && !xPressed) {
            rotateClockwise(currentShape, rotatedShape);
            if (canRotate(board, x, y, rotatedShape)) {
                memcpy(currentShape, rotatedShape, sizeof(rotatedShape));
            }
            xPressed = 1;
        }
        if (!(GetKeyState('X') & 0x8000)) {
            xPressed = 0;
        }

        if (canMoveDown(board, x, y, currentShape)) {
            y++;
        } else {
            /// Lock piece into the board
            for (int row = 0; row < 4; ++row) {
                for (int col = 0; col < 4; ++col) {
                    if (currentShape[row][col]) {
                        int boardY = y + row;
                        int boardX = x + col;
                        if (boardY >= 0 && boardY < 20 && boardX >= 0 && boardX < 10) {
                            board[boardY][boardX] = 1;
                        }
                    }
                }
            }

            for (int row = 19; row >= 0; row--) {
                int full = 1;
                for (int col = 0; col < 10; col++) {
                    if (board[row][col] == 0) {
                        full = 0;
                        break;
                    }
                }
                if (full) {
                    (*score) += 100 * level;
                    Beep(600, 100);
                    linesClear++;
                    if (linesClear % 10 == 0) {
                        Beep(1000, 150);
                        Beep(1200, 150);
                        level++;
                    }
                    for (int r = row; r > 0; r--) {
                        for (int col = 0; col < 10; col++) {
                            board[r][col] = board[r - 1][col];
                        }
                    }
                    for (int col = 0; col < 10; col++) {
                        board[0][col] = 0;
                    }
                    row++;  /// re-check this row
                }
            }

            /// Spawn next piece AFTER checking for lines
            x = 3;
            y = 0;
            memcpy(currentShape, tetrominos[nextShape], sizeof(currentShape));
            currentPiece = nextShape;
            nextShape = rand() % NUM_SHAPES;

            /// Game over check
            if (!canMoveDown(board, x, y, currentShape)) { gameOver = 1; }
        }

        /// Game over
        if (y >= 20) {
            Beep(400, 300);
            Beep(300, 300);
            Beep(200, 400);
            gameOver = 1;
        }
        /// Falling speed
        int speed = 250 - level * 50;
        if (level > 10) {speed = 1;}
        else if (speed < 100) speed = 100;
        Sleep(speed);
    }
    system("cls");
    printf("Game OVER!!\n");
    printf("Final score: %i\n", *score);
    printf("Final level reached: %i\n", level);
}