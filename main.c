#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int unUsedInBox(int *board, int n, int rowStart, int colStart, int num) {
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            if (board[(rowStart + i) * n + (colStart + j)] == num) {
                return 0;
            }
        }
    }
    return 1;
}

void fillBox(int *board, int n, int row, int col) {
    int num;
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            do {
                num = (rand() % n) + 1;
            } while (unUsedInBox(board, n, row, col, num) == 0);
            board[(row + i) * n + (col + j)] = num;
        }
    }
}

void fillDiagonal(int *board, int n) {
    for (int i = 0; i < n; i += 3) {
        fillBox(board, n, i, i);
    }
}

void sudokuGenerator(int *board, int k, int n) {
    for (int i = 0; i < n * n; i++) {
        board[i] = 0;
    }
    fillDiagonal(board, n);
}

int main(void) {
    srand(time(NULL));

    int k = 20;
    int n = 9;

    int gameBoard[n][n];
    sudokuGenerator(gameBoard, k, n);

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            printf("%d ", gameBoard[i][j]);
        }
        printf("\n");
    }

    return 0;
}
