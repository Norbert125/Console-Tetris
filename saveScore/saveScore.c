#include "saveScore.h"
#include <stdio.h>
#include "curses.h" // Use Curses instead of Windows.h/Stdio
#include <windows.h>

#define FILENAME "highscores.txt"

void saveScore(int score) {
    int highScore = 0;

    // 1. Read High Score
    FILE *file = fopen(FILENAME, "r");
    if (file != NULL) {
        fscanf(file, "HIGH SCORE: %d", &highScore);
        fclose(file);
    }

    // 2. Draw the Save Prompt in the TUI
    clear();
    attron(COLOR_PAIR(7));
    mvprintw(10, 10, "CURRENT SCORE: %d", score);
    mvprintw(11, 10, "HIGH SCORE   : %d", highScore);
    mvprintw(14, 10, "Do you want to save your score? (Y/N)");
    refresh();

    int choice = getch();
    if (choice == 'y' || choice == 'Y') {
        if (score > highScore) {
            attron(COLOR_PAIR(2) | A_BOLD);
            mvprintw(16, 10, "NEW HIGH SCORE! CONGRATS!");
            attroff(COLOR_PAIR(2) | A_BOLD);
            highScore = score;
        }

        // Save logic
        FILE *tempFile = fopen("temp.txt", "w");
        if (tempFile) {
            fprintf(tempFile, "HIGH SCORE: %d\n", highScore);
            file = fopen(FILENAME, "r");
            if (file) {
                char line[100];
                fgets(line, sizeof(line), file); // skip old high score
                while (fgets(line, sizeof(line), file)) fputs(line, tempFile);
                fclose(file);
            }
            fprintf(tempFile, "%d\n", score);
            fclose(tempFile);
            remove(FILENAME);
            rename("temp.txt", FILENAME);
        }

        mvprintw(18, 10, "Score saved! Press any key...");
    } else {
        mvprintw(18, 10, "Score discarded. Press any key...");
    }

    refresh();
    while(getch() == ERR); // Wait for confirmation before returning to menu
}