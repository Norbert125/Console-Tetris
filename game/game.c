#include "game.h"
#include "tetromino.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

/// FIX: Prevent the collision between windows.h and curses.h
#include <windows.h>
#undef MOUSE_MOVED
#include "game/curses.h"

// Helper to map your levels to PDCurses color pairs
int getColorPair(int level) {
    switch (level) {
        case 1: return 1; /// Blue
        case 2: return 2; /// Green
        case 3: return 3; /// Red
        case 4: return 4; /// Yellow
        case 5: return 5; /// Cyan
        case 6: return 6; /// Magenta
        default: return 7; /// White
    }
}

void shuffleBag(int *bag, int *bagIndex) {
    for (int i = 0; i < NUM_SHAPES; ++i) {
        bag[i] = i;
    }
    for (int i = NUM_SHAPES - 1; i > 0; --i) {
        int j = rand() % (i + 1);
        int temp = bag[i];
        bag[i] = bag[j];
        bag[j] = temp;
    }
    *bagIndex = 0;
}

int getNextPiece(int *bag, int *bagIndex, int useBagSystem) {
    if (useBagSystem) {
        if (*bagIndex >= NUM_SHAPES) {
            shuffleBag(bag, bagIndex);
        }
        return bag[(*bagIndex)++];
    } else {
        return rand() % NUM_SHAPES;
    }
}

void gameLoop(int board[20][10], int *score) {
    srand(time(0));
    int useBagSys = 0;
    int bagIndex;
    int bag[NUM_SHAPES];

    /// Use Curses for the prompt so it doesn't break the terminal buffer
    nodelay(stdscr, FALSE);
    mvprintw(0, 0, "Enable bag system?");
    mvprintw(1, 0, "(by enabling it bag system provides the fact that don't get duplicates, allways separate pieces) (y/n): ");
    refresh();

    int input = getch();
    if (input == 'y' || input == 'Y') { useBagSys = 1; }
    if (useBagSys){ shuffleBag(bag, &bagIndex); }

    nodelay(stdscr, TRUE);
    clear();

    int gameOver = 0;
    int linesClear = 0;
    int level = 1;
    int currentShape[4][4] = {0};
    int rotatedShape[4][4] = {0};
    int x = 3, y = 0;
    int nextShape = rand() % NUM_SHAPES;
    int currentPiece = nextShape;
    memcpy(currentShape, tetrominos[nextShape], sizeof(tetrominos[0]));
    nextShape = getNextPiece(bag, &bagIndex, useBagSys);

    while (!gameOver) {
        erase();

        int displayBoard[20][10];

        for (int row = 0; row < 20; row++) {
            for (int col = 0; col < 10; col++) {
                displayBoard[row][col] = board[row][col];
            }
        }

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

        int colorPair = getColorPair(level);

        /// --- 1. DRAW THE RETRO BORDER ---
        attron(COLOR_PAIR(7));
        mvprintw(0, 1, "--------------------");
        for (int row = 0; row < 20; ++row) {
            mvprintw(row + 1, 0, "|");
            mvprintw(row + 1, 21, "|");
        }
        mvprintw(21, 1, "--------------------");
        attroff(COLOR_PAIR(7));

        /// --- 2. DRAW THE PLAYFIELD ---
        for (int row = 0; row < 20; ++row) {
            for (int col = 0; col < 10; ++col) {
                if (displayBoard[row][col] == 1) {
                    attron(COLOR_PAIR(colorPair));
                    mvprintw(row + 1, (col * 2) + 1, "[]");
                    attroff(COLOR_PAIR(colorPair));
                } else {
                    mvprintw(row + 1, (col * 2) + 1, "  ");
                }
            }
        }

        /// --- 3. DRAW THE ARCADE UI ---
        int ui_x = 26;
        attron(COLOR_PAIR(7));
        mvprintw(1, ui_x, "SCORE: %06d", *score);
        mvprintw(3, ui_x, "LEVEL: %02d", level);
        mvprintw(5, ui_x, "LINES: %03d", linesClear);
        mvprintw(8, ui_x, "NEXT:");
        attroff(COLOR_PAIR(7));

        attron(COLOR_PAIR(colorPair));
        for (int row = 0; row < 4; row++) {
            for (int col = 0; col < 4; col++) {
                if (tetrominos[nextShape][row][col]) {
                    mvprintw(10 + row, ui_x + (col * 2), "[]");
                } else {
                    mvprintw(10 + row, ui_x + (col * 2), "  ");
                }
            }
        }
        attroff(COLOR_PAIR(colorPair));

        refresh();

        /// Get Input (Still using Windows API for now)
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

        if (GetAsyncKeyState(VK_DOWN) & 0x8000) {
            if (canMoveDown(board, x, y, currentShape)) {
                y++;
                *score += 5;
            }
        }
        static int zPressed = 0;
        static int xPressed = 0;

        if ((GetAsyncKeyState('Z') & 0x8000) && !zPressed) {
            rotateCounterClockwise(currentShape, rotatedShape);
            if (canRotate(board, x, y, rotatedShape)) {
                memcpy(currentShape, rotatedShape, sizeof(rotatedShape));
            } else {
                int kick = 0;
                for (int dx = -1; dx <= 1; dx++) {
                    if (canRotate(board,x+dx,y,rotatedShape)) {
                        x += dx;
                        memcpy(currentShape,rotatedShape,sizeof(rotatedShape));
                        kick = 1;
                        break;
                    }
                }
            }
            zPressed = 1;
        }
        if (!(GetAsyncKeyState('Z') & 0x8000)) {
            zPressed = 0;
        }

        if ((GetAsyncKeyState('X') & 0x8000) && !xPressed) {
            rotateClockwise(currentShape, rotatedShape);
            if (canRotate(board, x, y, rotatedShape)) {
                memcpy(currentShape, rotatedShape, sizeof(rotatedShape));
            } else {
                int kick = 0;
                for (int dx = -1; dx <= 1; dx++) {
                    if (canRotate(board,x+dx,y,rotatedShape)) {
                        x += dx;
                        memcpy(currentShape,rotatedShape,sizeof(rotatedShape));
                        kick = 1;
                        break;
                    }
                }
            }
            xPressed = 1;
        }
        if (!(GetKeyState('X') & 0x8000)) {
            xPressed = 0;
        }

        if (canMoveDown(board, x, y, currentShape)) {
            y++;
        } else {
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

            int linesClearedNow = 0;

            for (int row = 19; row >= 0; row--) {
                int full = 1;
                for (int col = 0; col < 10; col++) {
                    if (board[row][col] == 0) {
                        full = 0;
                        break;
                    }
                }
                if (full) {
                    linesClearedNow++;

                    for (int r = row; r > 0; r--) {
                        for (int col = 0; col < 10; col++) {
                            board[r][col] = board[r - 1][col];
                        }
                    }
                    for (int col = 0; col < 10; col++) {
                        board[0][col] = 0;
                    }
                    row++;
                }
            }

            switch (linesClearedNow) {
                case 1: *score += 100 * level; Beep(600,100); break;
                case 2: *score += 300 * level; Beep(700,100); break;
                case 3: *score += 500 * level; Beep(800,100); break;
                case 4:
                    *score += 800 * level;
                    Beep(900,100);
                    mvprintw(23, 0, ">>> TETRIS!! <<<");
                    refresh();
                    Sleep(500);
                    break;
                default:break;
            }

            linesClear += linesClearedNow;

            if (linesClear >= (level+1) * 10) {
                level++;
                Beep(1000,150);
                Beep(1200,150);
            }

            x = 3;
            y = 0;
            memcpy(currentShape, tetrominos[nextShape], sizeof(currentShape));
            currentPiece = nextShape;
            nextShape = getNextPiece(bag,&bagIndex,useBagSys);

            if (!canMoveDown(board, x, y, currentShape)) { gameOver = 1; }
        }

        if (y >= 20) {
            Beep(400, 300);
            Beep(300, 300);
            Beep(200, 400);
            gameOver = 1;
        }

        int speed = 250 - level * 50;
        if (level > 10) {speed = 10;}
        else if (speed < 100) speed = 100;
        Sleep(speed);
    }

    clear();
    mvprintw(0, 0, "Game OVER!!");
    mvprintw(1, 0, "Final score: %i", *score);
    mvprintw(2, 0, "Final level reached: %i", level);
    refresh();
    Sleep(2000);
    FlushConsoleInputBuffer(GetStdHandle(STD_INPUT_HANDLE));
}