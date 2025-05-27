#include "saveScore.h"
#include <stdio.h>
#include <windows.h>

#define FILENAME "highscores.txt"

void saveScore(int score) {
    char option;
    int highScore = 0;
    int ch;
    FILE *file = fopen(FILENAME, "r");
    if (file != NULL) {
        char label[20];
        fscanf(file, "%s %d", label, &highScore);
        fclose(file);
        Sleep(500);
    } else {
        printf("No high scores file found, creating a new one.\n");
        Sleep(500);
    }

    FlushConsoleInputBuffer(GetStdHandle(STD_INPUT_HANDLE)); /// Flushing console of garbage
    Sleep(100);

    while (1) {

        printf("Do you want to save your score? (Y/N): ");
        FlushConsoleInputBuffer(GetStdHandle(STD_INPUT_HANDLE)); /// Flushing console of garbage
        scanf(" %c", &option);


        while ((ch = getchar()) != '\n' && ch != EOF); /// Flushing input buffer

        Sleep(50);


        if (option == 'y' || option == 'Y') { /// if wanted saving score!
            if (score > highScore) {
                printf("Congratulations!!! NEW HIGH SCORE!!\n");
                Sleep(500);
                highScore = score;
            }

            FILE *tempFile = fopen("temp.txt", "w");
            if (tempFile == NULL) {
                perror("Failed to save Score.\n");
                return;
            }

            fprintf(tempFile, "HIGH SCORE: %d\n", highScore);

            if ((file = fopen(FILENAME, "r")) != NULL) {
                char line[100];
                fgets(line, sizeof(line), file); /// Skip the first line
                while (fgets(line, sizeof(line), file)) {
                    fputs(line, tempFile); /// Copy the remaining lines
                }
                fclose(file);
            }

            /// Append current score to the end
            fprintf(tempFile, "%d\n", score);
            fclose(tempFile);

            /// Replace original file with updated temp file
            FILE *checkFile = fopen(FILENAME, "r");
            if (checkFile) {
                fclose(checkFile);
                if (remove(FILENAME) != 0) {
                    perror("Failed to remove original file");
                    Sleep(3000);
                    DWORD err = GetLastError();
                    printf("Windows Error Code: %lu\n", err);
                    Sleep(3000);
                    return;
                }
            }

            /// Rename temporal file to FILENAME
            if (rename("temp.txt", FILENAME) != 0) {
                perror("Failed to rename temp file\n");
                DWORD err = GetLastError();
                printf("Windows Error Code: %lu\n", err);
                Sleep(3000);
                return;
            }

            printf("Score saved.\n");
            Sleep(500);
            return;
        } else {
            printf("Score not saved.\n");
            Sleep(500);
            return;
        }
    }
}