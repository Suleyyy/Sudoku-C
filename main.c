#include <stdio.h>
#include <stdlib.h>
#include <time.h>

void printSimpleSudoku(int *board, int n) {
    int size = n * n;

    for (int i = 0; i < size; i++) {
        // Linia pozioma co n wierszy
        if (i % n == 0) {
            for (int j = 0; j < size + n + 5; j++) {
                printf("--");
            }
            printf("\n");
        }

        for (int j = 0; j < size; j++) {
            // Linia pionowa co n kolumn
            if (j % n == 0) {
                printf("| ");
            }

            // Wyświetl liczbę lub kropkę jeśli 0
            if (board[i * size + j] != 0) {
                printf("%2d ", board[i * size + j]);
            } else {
                printf(" . ");
            }
        }
        printf("|\n");
    }

    // Dolna linia
    for (int j = 0; j < size + n + 5; j++) {
        printf("--");
    }
    printf("\n");
}

int unUsedInBox(const int *board, int n, int rowStart, int colStart, int num) {
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            if (board[(rowStart + i) * (n*n) + (colStart + j)] == num) {
                return 0;
            }
        }
    }
    return 1;
}

int unUsedInCol(const int *board, int j, int num, int n) {
    for (int i = 0; i < n*n; i++) {
        if (board[i*(n*n)+j] == num) {
            return 0;
        }
    }
    return 1;
}

int unUsedInRow(const int *board, int i, int num, int n) {
    for (int j = 0; j < n*n; j++) {
        if (board[i*(n*n)+j] == num) {
            return 0;
        }
    }
    return 1;
}

int checkIfSafe(const int *board, int i, int j, int num, int n) {
    return (unUsedInBox(board, n, i-i%n, j-j%n, num) &&
            unUsedInCol(board, j, num, n) &&
            unUsedInRow(board, i, num, n));
}

int fillRemaining(int *board, int i, int j, int n) {
    if (i == n*n) return 1;
    if (j == n*n) return fillRemaining(board, i+1, 0, n);
    if (board[i*(n*n)+j] != 0) return fillRemaining(board, i, j+1, n);

    for (int num = 1; num <= n*n; num++) {
        if (checkIfSafe(board, i, j, num, n)) {
            board[i*(n*n)+j] = num;
            if (fillRemaining(board, i, j+1, n)) {
                return 1;
            }
            board[i*(n*n)+j] = 0;
        }
    }
    return 0;
}

void fillBox(int *board, int n, int row, int col) {
    int num;
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            do {
                num = (rand() % (n*n)) + 1;
            } while (!unUsedInBox(board, n, row, col, num));
            board[(row + i) * (n*n) + (col + j)] = num;
        }
    }
}

void fillDiagonal(int *board, int n) {
    for (int i = 0; i < n*n; i += n) {
        fillBox(board, n, i, i);
    }
}

void sudokuGenerator(int *board, int k, int n) {
    // Clear the board
    for (int i = 0; i < n*n * n*n; i++) {
        board[i] = 0;
    }

    fillDiagonal(board, n);
    fillRemaining(board, 0, 0, n);
}

int main(void) {
    srand(time(NULL));

    int k = 20;
    int n = 4; // For standard 9x9 Sudoku

    int gameBoard[n*n][n*n]; // Fixed size for standard Sudoku
    sudokuGenerator(gameBoard, k, n);

    printSimpleSudoku(gameBoard, n);

    return 0;
}