#include "game/curses.h"
#include "game/game.h"
// #include "saveScore/saveScore.h" // Uncomment when ready

#define BOARD_WIDTH 10
#define BOARD_HEIGHT 20

void drawMenu(int selected, int bagSystem) {
    erase();

    /// Giant Retro ASCII Art Title
    attron(COLOR_PAIR(3) | A_BOLD);
    mvprintw(2, 5, "  _______ ______ _______ _____  _____  _____ ");
    mvprintw(3, 5, " |__   __|  ____|__   __|  __ \\|_   _|/ ____|");
    mvprintw(4, 5, "    | |  | |__     | |  | |__) | | | | (___  ");
    mvprintw(5, 5, "    | |  |  __|    | |  |  _  /  | |  \\___ \\ ");
    mvprintw(6, 5, "    | |  | |____   | |  | | \\ \\ _| |_ ____) |");
    mvprintw(7, 5, "    |_|  |______|  |_|  |_|  \\_\\_____|_____/ ");
    attroff(COLOR_PAIR(3) | A_BOLD);

    /// Menu Options
    attron(COLOR_PAIR(7));
    mvprintw(12, 12, selected == 0 ? "==> Play Game <==" : "    Play Game    ");
    mvprintw(14, 12, selected == 1 ? "==> Bag System: [%s] <==" : "    Bag System: [%s]    ", bagSystem ? "ON " : "OFF");
    mvprintw(16, 12, selected == 2 ? "==> Exit <==" : "    Exit    ");

    mvprintw(21, 5, "Use UP/DOWN to navigate, ENTER to select.");
    attroff(COLOR_PAIR(7));

    refresh();
}

int main() {
    /// --- PDCurses Initialization ---
    initscr();
    noecho();
    curs_set(0);
    keypad(stdscr, TRUE);

    start_color();
    init_pair(1, COLOR_BLUE, COLOR_BLACK);
    init_pair(2, COLOR_GREEN, COLOR_BLACK);
    init_pair(3, COLOR_RED, COLOR_BLACK);
    init_pair(4, COLOR_YELLOW, COLOR_BLACK);
    init_pair(5, COLOR_CYAN, COLOR_BLACK);
    init_pair(6, COLOR_MAGENTA, COLOR_BLACK);
    init_pair(7, COLOR_WHITE, COLOR_BLACK);

    int bagSystem = 1; /// Default to ON
    int selected = 0;
    int running = 1;

    /// Wait for input in the menu (blocking)
    nodelay(stdscr, FALSE);

    while (running) {
        drawMenu(selected, bagSystem);
        int ch = getch();

        /// Menu Navigation Logic
        if (ch == KEY_UP && selected > 0) {
            selected--;
        } else if (ch == KEY_DOWN && selected < 2) {
            selected++;
        } else if (ch == '\n' || ch == KEY_ENTER || ch == 10 || ch == 13) {

            if (selected == 0) {
                /// PLAY GAME
                int board[BOARD_HEIGHT][BOARD_WIDTH] = {0};
                int score = 0;

                nodelay(stdscr, TRUE); /// Switch to non-blocking for gameplay
                gameLoop(board, &score, bagSystem);
                nodelay(stdscr, FALSE); /// Switch back to blocking for the menu

                // saveScore(score); // Uncomment when ready

            } else if (selected == 1) {
                /// TOGGLE BAG SYSTEM
                bagSystem = !bagSystem;

            } else if (selected == 2) {
                /// EXIT
                running = 0;
            }
        }
    }

    /// --- PDCurses Cleanup ---
    endwin();
    return 0;
}