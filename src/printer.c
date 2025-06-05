#include "../include/play.h"
#include <stdio.h>

void printSimpleSudoku(const GameBoard *board) {

    printf("\n");
    printf("    ");
    for (int j = 0; j < board->size; j++) {
        if (j % board->n == 0) printf("| ");
        printf("%2d ", j + 1);
    }
    printf("|");
    printf("\n");

    printf("    ");
    for (int j = 0; j < board->size; j++) {
        if (j % board->n == 0) printf("+-");
        printf("---");
    }
    printf("+");
    printf("\n");

    for (int i = 0; i < board->size; i++) {

        printf("%2c  ", 'A' + i);

        for (int j = 0; j < board->size; j++) {
            if (j % board->n == 0) printf("| ");

            if (board->boardPuzzle[i * board->size + j] != 0) {
                printf("%2d ", board->boardPuzzle[i * board->size + j]);
            } else {
                printf(" - ");
            }
        }
        printf("|\n");

        if ((i + 1) % board->n == 0 && (i + 1) != board->size) {
            printf("    ");
            for (int j = 0; j < board->size; j++) {
                if (j % board->n == 0) printf("+-");
                printf("---");
            }
            printf("+");
            printf("\n");
        }
    }

    printf("    ");
    for (int j = 0; j < board->size; j++) {
        if (j % board->n == 0) printf("+-");
        printf("---");
    }
    printf("+");
    printf("\n");
}
void printSimpleSudokuv2(const int *clone ,const GameBoard *board) {

    printf("\n");
    printf("    ");
    for (int j = 0; j < board->size; j++) {
        if (j % board->n == 0) printf("| ");
        printf("%2d ", j + 1);
    }
    printf("|");
    printf("\n");

    printf("    ");
    for (int j = 0; j < board->size; j++) {
        if (j % board->n == 0) printf("+-");
        printf("---");
    }
    printf("+");
    printf("\n");

    for (int i = 0; i < board->size; i++) {

        printf("%2c  ", 'A' + i);

        for (int j = 0; j < board->size; j++) {
            if (j % board->n == 0) printf("| ");

            if (clone[i * board->size + j] != 0) {
                printf("%2d ", clone[i * board->size + j]);
            } else {
                printf(" - ");
            }
        }
        printf("|\n");

        if ((i + 1) % board->n == 0 && (i + 1) != board->size) {
            printf("    ");
            for (int j = 0; j < board->size; j++) {
                if (j % board->n == 0) printf("+-");
                printf("---");
            }
            printf("+");
            printf("\n");
        }
    }

    printf("    ");
    for (int j = 0; j < board->size; j++) {
        if (j % board->n == 0) printf("+-");
        printf("---");
    }
    printf("+");
    printf("\n");
}
void title() {
    printf("                                           ,----..          ,--.               \n");
    printf("  .--.--.                     ,---,       /   /   \\     ,--/  /|               \n");
    printf(" /  /    '.          ,--,   .'  .' `\\    /   .     : ,---,': / '         ,--,  \n");
    printf("|  :  /`. /        ,'_ /| ,---.'     \\  .   /   ;.  \\:   : '/ /        ,'_ /|  \n");
    printf(";  |  |--`    .--. |  | : |   |  .`\\  |.   ;   /  ` ;|   '   ,    .--. |  | :  \n");
    printf("|  :  ;_    ,'_ /| :  . | :   : |  '  |;   |  ; \\ ; |'   |  /   ,'_ /| :  . |  \n");
    printf(" \\  \\    `. |  ' | |  . . |   ' '  ;  :|   :  | ; | '|   ;  ;   |  ' | |  . .  \n");
    printf("  `----.   \\|  | ' |  | | '   | ;  .  |.   |  ' ' ' ::   '   \\  |  | ' |  | |  \n");
    printf("  __ \\  \\  |:  | | :  ' ; |   | :  |  ''   ;  \\; /  ||   |    ' :  | | :  ' ;  \n");
    printf(" /  /`--'  /|  ; ' |  | ' '   : | /  ;  \\   \\  ',  / '   : |.  \\|  ; ' |  | '  \n");
    printf("'--'.     / :  | : ;  ; | |   | '` ,/    ;   :    /  |   | '_\\.':  | : ;  ; |  \n");
    printf("  `--'---'  '  :  `--'   \\;   :  .'       \\   \\ .'   '   : |    '  :  `--'   \\ \n");
    printf("            :  ,      .-./|   ,.'          `---`     ;   |,'    :  ,      .-./\n");
    printf("             `--`----'    '---'                      '---'       `--`----'    \n");
}