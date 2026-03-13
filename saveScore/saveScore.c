#include "saveScore.h"
#include <stdio.h>
#include <windows.h>

#define FILENAME "highscores.txt"

void saveScore(int score) {
    char option;
    int highScore = 0;
    int ch;

    /// 1. Safely read the high score using exact string matching
    FILE *file = fopen(FILENAME, "r");
    if (file != NULL) {
        fscanf(file, "HIGH SCORE: %d", &highScore);
        fclose(file);
        Sleep(500);
    } else {
        printf("No high scores file found, creating a new one.\n");
        Sleep(500);
    }

    FlushConsoleInputBuffer(GetStdHandle(STD_INPUT_HANDLE));
    Sleep(100);

    while (1) {
        printf("Do you want to save your score? (Y/N): ");
        FlushConsoleInputBuffer(GetStdHandle(STD_INPUT_HANDLE));
        scanf(" %c", &option);

        while ((ch = getchar()) != '\n' && ch != EOF); /// Flush input buffer
        Sleep(50);

        if (option == 'y' || option == 'Y') {
            if (score > highScore) {
                printf("Congratulations!!! NEW HIGH SCORE!!\n");
                Sleep(500);
                highScore = score;
            }

            FILE *tempFile = fopen("temp.txt", "w");
            if (tempFile == NULL) {
                perror("Failed to open temp file for saving.\n");
                return;
            }

            /// Write the new (or existing) high score to the top
            fprintf(tempFile, "HIGH SCORE: %d\n", highScore);

            /// 2. Safely copy old scores, skipping the old high score line
            file = fopen(FILENAME, "r");
            if (file != NULL) {
                char line[100];
                fgets(line, sizeof(line), file); /// Skip the first line
                while (fgets(line, sizeof(line), file)) {
                    fputs(line, tempFile);
                }
                fclose(file);
            }

            /// Append current score to the end
            fprintf(tempFile, "%d\n", score);
            fclose(tempFile);

            /// 3. Safely replace the old file
            if (remove(FILENAME) != 0 && GetLastError() != ERROR_FILE_NOT_FOUND) {
                perror("Failed to remove original file");
                Sleep(3000);
                return;
            }

            if (rename("temp.txt", FILENAME) != 0) {
                perror("Failed to rename temp file");
                Sleep(3000);
                return;
            }

            printf("Score saved successfully.\n");
            Sleep(500);
            return;

        } else if (option == 'n' || option == 'N') {
            printf("Score not saved.\n");
            Sleep(500);
            return;
        }
    }
}