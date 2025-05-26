#include "saveScore.h"
#include <stdio.h>
#include <windows.h>

#define FILENAME "highscores.txt"

void saveScore(int score) {
    char option = '\0';
    int highScore = 0;
    int newHigh;
    FILE *file = fopen(FILENAME, "r");
    if (file != NULL) {
        char label[10];
        fscanf(file, "%s %d", label, &highScore);
        fclose(file);
    } else {
        printf("No high scores file found, creating a new one.\n");
    }

    FlushConsoleInputBuffer(GetStdHandle(STD_INPUT_HANDLE)); /// Flushing console of garbage

    printf("Do you want to save your score? (Y/N): ");

    option = getchar();

    while (getchar() != '\n'){

        if (option == 'y' || option == 'Y') { /// if wanted saving score!
            if (score > highScore) {
                printf("Congratulations!!! NEW HIGH SCORE!!\n");
                Sleep(2000);
                highScore = score;
                newHigh = 1;
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
            remove(FILENAME);
            rename("temp.txt", FILENAME);

            printf("Score saved.\n");
        } else {
            printf("Score not saved.\n");
        }
    }

}