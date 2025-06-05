#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <math.h>
#include <../include/play.h>


void fillBoxHere(GameBoard *board, int row, int col) {
    int num;
    for (int i = 0; i < board->n; i++) {
        for (int j = 0; j < board->n; j++) {
            if (board->boardPuzzle[(row + i) * board->size + (col + j)] == 0) {
                do {
                    num = (rand() % board->size) + 1;
                } while (unUsedInBox(board->boardPuzzle, board->size, board->n, row, col, num) == 0);
                board->boardPuzzle[(row + i) * board->size + (col + j)] = num;
            }
        }
    }
}

void initializeSudoku(GameBoard *board){
    for (int i = 0; i < board->size; i += board->n) {
        for (int j = 0; j < board->size; j += board->n) {
          fillBoxHere(board, i, j);
        }
    }
}

int calculateCost(const GameBoard *board) {
    int cost = 0;

    for (int i = 0; i < board->size; i++) {
        int count[board->size + 1];
        memset(count, 0, sizeof(count));

        for (int j = 0; j < board->size; j++) {
            int num = board->boardPuzzle[i * board->size + j];
            count[num]++;
        }

        for (int n = 1; n <= board->size; n++) {
            if (count[n] > 1) {
                cost += count[n] - 1;
            }
        }
    }

    for (int j = 0; j < board->size; j++) {
        int count[board->size + 1];
        memset(count, 0, sizeof(count));

        for (int i = 0; i < board->size; i++) {
            int num = board->boardPuzzle[i * board->size + j];
            count[num]++;
        }

        for (int n = 1; n <= board->size; n++) {
            if (count[n] > 1) {
                cost += count[n] - 1;
            }
        }
    }

    return cost;
}

void generateNeighbor(GameBoard *original, GameBoard *neighbor) {
    memcpy(neighbor->boardPuzzle, original->boardPuzzle, original->total * sizeof(int));

    int attempts = 0;
    const int max_attempts = 100;

    while (attempts < max_attempts) {
        int blockRow = rand() % original->n;
        int blockCol = rand() % original->n;

        int row1 = rand() % original->n;
        int col1 = rand() % original->n;
        int row2 = rand() % original->n;
        int col2 = rand() % original->n;

        int cell1 = (blockRow * original->n + row1) * original->size + (blockCol * original->n + col1);
        int cell2 = (blockRow * original->n + row2) * original->size + (blockCol * original->n + col2);

        if (cell1 != cell2 &&
            original->frozenCords[cell1] == 0 &&
            original->frozenCords[cell2] == 0) {

            int temp = neighbor->boardPuzzle[cell1];
            neighbor->boardPuzzle[cell1] = neighbor->boardPuzzle[cell2];
            neighbor->boardPuzzle[cell2] = temp;
            return;
            }
        attempts++;
    }
}

void solve_sudoku_sa(GameBoard *board, double T_start, double T_end, double alpha) {
    srand(time(NULL));
    double T = T_start;
    int currentCost = calculateCost(board);
    int bestCost = currentCost;
    int iterations = 0;

    int *bestSolution = malloc(board->total * sizeof(int));
    memcpy(bestSolution, board->boardPuzzle, board->total * sizeof(int));

    printf("-----------Parametry startowe----------\n");
    printf("Iter: %d, Temp: %f, CurrCost:, %d, BestCost: %d\n", iterations, T, currentCost, bestCost);
    printf("---------------------------------------\n");

    while (T > T_end && currentCost > 0) {
        GameBoard neighborBoard = *board;
        neighborBoard.boardPuzzle = malloc(board->total * sizeof(int));
        neighborBoard.frozenCords = board->frozenCords;

        generateNeighbor(board,&neighborBoard);

        int neighborCost = calculateCost(&neighborBoard);
        int deltaE = neighborCost - currentCost;

        if (deltaE < 0) {
            memcpy(board->boardPuzzle, neighborBoard.boardPuzzle, board->total * sizeof(int));
            currentCost = neighborCost;
        }
        else {
            double probability = exp(-deltaE / T);
            if ((double)rand() / RAND_MAX < probability) {
                memcpy(board->boardPuzzle, neighborBoard.boardPuzzle, board->total * sizeof(int));
                currentCost = neighborCost;
            }
        }
        if (currentCost < bestCost) {
            bestCost = currentCost;
            memcpy(bestSolution, board->boardPuzzle, board->total * sizeof(int));
        }
        T *= alpha;
        iterations++;
        if (iterations % 100000 == 0 || bestCost == 0)
            printf("Iter: %d, Temp: %f, CurrCost:, %d, BestCost: %d\n", iterations, T, currentCost, bestCost);
    }
    memcpy(board->boardPuzzle, bestSolution, board->total * sizeof(int));
    free(bestSolution);
    printSimpleSudoku(board);
}

void solver(GameBoard *board) {
    double T_start = 100.0;
    double T_end = 0.00001;
    double alpha = 0.99999;

    initializeSudoku(board);

    solve_sudoku_sa(board, T_start, T_end, alpha);
    getchar();
}