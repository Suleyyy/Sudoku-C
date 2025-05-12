#include "../include/play.h"
#include <stdio.h>
#include <stdlib.h>

int unUsedInBox(const int *board, int size, int n, int rowStart, int colStart, int num) {
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            if (board[(rowStart + i) * size + (colStart + j)] == num) {
                return 0;
            }
        }
    }
    return 1;
}

int unUsedInCol(const int *board, int size, int j, int num) {
    for (int i = 0; i < size; i++) {
        if (board[i * size + j] == num) {
            return 0;
        }
    }
    return 1;
}

int unUsedInRow(const int *board, int size, int i, int num) {
    for (int j = 0; j < size; j++) {
        if (board[i * size + j] == num) {
            return 0;
        }
    }
    return 1;
}

int checkIfSafe(const int *board, int size, int n, int i, int j, int num) {
    return (unUsedInBox(board, size, n, i - i % n, j - j % n, num) &&
           unUsedInCol(board, size, j, num) &&
           unUsedInRow(board, size, i, num));
}

int countSolutionsHelper(int *board, int size, int n, int i, int j, int count) {
    if (board == NULL)
        return 0;
    while (i < size && board[i * size + j] != 0) { //sprawdza, czy pole jest puste
        j++;
        if (j >= size) {
            j = 0;
            i++;
        }
    }

    if (i == size) {
        return count + 1;
    }

    for (int num = 1; num <= size && count < 2; num++) { //rekurencyjne sprawdzanie liczby rozwiązań i stopowanie jak jest więcej niż 2
        if (checkIfSafe(board, size, n, i, j, num)) {
            board[i * size + j] = num;

            int next_j = j + 1;
            int next_i = i;
            if (next_j >= size) {
                next_j = 0;
                next_i++;
            }

            count = countSolutionsHelper(board, size, n, next_i, next_j, count);
            board[i * size + j] = 0;
        }
    }
    return count;
}

int countSolutions(const GameBoard *board) {
    int *tempBoard = malloc(sizeof(int) * board->total);
    for (int i = 0; i < board->total; i++) {
        tempBoard[i] = board->boardPuzzle[i];
    }

    int solutions = countSolutionsHelper(tempBoard, board->size, board->n, 0, 0, 0);
    free(tempBoard);
    return solutions;
}

int fillRemaining(GameBoard *board, int i, int j) {
    if (i == board->size) return 1;
    if (j == board->size) return fillRemaining(board, i + 1, 0);
    if (board->board[i * board->size + j] != 0) return fillRemaining(board, i, j + 1);

    for (int num = 1; num <= board->size; num++) {
        if (checkIfSafe(board->board, board->size, board->n, i, j, num)) {
            board->board[i * board->size + j] = num;
            board->frozenCords[i * board->size + j] = 1;
            if (fillRemaining(board, i, j + 1)) {
                return 1;
            }
            board->board[i * board->size + j] = 0;
        }
    }
    return 0;
}

void fillBox(const GameBoard *board, int row, int col) {
    int num;
    for (int i = 0; i < board->n; i++) {
        for (int j = 0; j < board->n; j++) {
            do {
                num = (rand() % board->size) + 1;
            } while (!unUsedInBox(board->board, board->size, board->n, row, col, num));
            board->board[(row + i) * board->size + (col + j)] = num;
            board->frozenCords[(row + i) * board->size + (col + j)] = 1;
        }
    }
}

void fillDiagonal(const GameBoard *board) {
    for (int i = 0; i < board->size; i += board->n) {
        fillBox(board, i, i);
    }
}

void removeKDigitsWithCheck(const GameBoard *board) {
    int cells_removed = 0;
    int attempts = 0;
    const int max_attempts = board->total * 2;

    while (cells_removed < board->level && attempts < max_attempts) {
        int cellId = rand() % board->total;
        int i = cellId / board->size;
        int j = cellId % board->size;

        if (board->boardPuzzle[i * board->size + j] != 0) {
            int temp = board->boardPuzzle[i * board->size + j];
            board->boardPuzzle[i * board->size + j] = 0;
            board->frozenCords[i * board->size + j] = 0;

            if (countSolutions(board) == 1) {
                cells_removed++;
            } else {
                board->boardPuzzle[i * board->size + j] = temp;
                board->frozenCords[i * board->size + j] = 1;
            }
        }
        attempts++;
    }

}

void sudokuGenerator(GameBoard *board) {
    int success = 0;

    do {
        for (int i = 0; i < board->total; i++) {
            board->board[i] = 0;
        }

        fillDiagonal(board);

        success = fillRemaining(board, 0, 0);

        if (success) {
            for (int i = 0; i < board->total; i++)
                board->boardPuzzle[i] = board->board[i];
            removeKDigitsWithCheck(board);
            success = (countSolutions(board) >= 1);
        }
    } while (board->n == 2 && !success);
}