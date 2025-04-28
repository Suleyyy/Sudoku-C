#include <locale.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>


typedef struct {
    int cordX;
    int cordY;
} Player;

typedef struct {
    int n;
    int size;
    int total;
    int *board;
    int *boardPuzzle;
    int level;
} GameBoard;

void printSimpleSudoku(const GameBoard *board) {
    for (int i = 0; i < board->size; i++) {
        if (i % board->n == 0) {
            for (int j = 0; j < board->size + board->n + 5; j++) {
                printf("--");
            }
            printf("\n");
        }

        for (int j = 0; j < board->size; j++) {
            if (j % board->n == 0) {
                printf("| ");
            }

            if (board->boardPuzzle[i * board->size + j] != 0) {
                printf("%2d ", board->boardPuzzle[i * board->size + j]);
            } else {
                printf(" - ");
            }
        }
        printf("|\n");
    }

    for (int j = 0; j < board->size + board->n + 5; j++) {
        printf("--");
    }
    printf("\n");
}

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
    while (i < size && board[i * size + j] != 0) {
        j++;
        if (j >= size) {
            j = 0;
            i++;
        }
    }

    if (i == size) {
        return count + 1;
    }

    for (int num = 1; num <= size && count < 2; num++) {
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

            if (countSolutions(board) == 1) {
                cells_removed++;
            } else {
                board->boardPuzzle[i * board->size + j] = temp;
            }
        }
        attempts++;
    }

    if (cells_removed < board->level) {
        printf("Uwaga: Usunięto tylko %d z %d liczb (zachowując unikalność rozwiązania)\n", cells_removed, board->level);
    }
}

void sudokuGenerator(GameBoard *board) {
    int success = 0;

    do {
        // Wyczyść planszę
        for (int i = 0; i < board->total; i++) {
            board->board[i] = 0;
        }

        // Wypełnij bloki diagonalne
        fillDiagonal(board);

        // Wypełnij resztę planszy
        success = fillRemaining(board, 0, 0);

        if (success) {
            board->boardPuzzle = board->board;
            removeKDigitsWithCheck(board);
            success = (countSolutions(board) >= 1);
        }
    } while (board->n == 2 && !success);
}
void setBoard(GameBoard *board) {
    board->size = board->n*board->n;
    board->total = board->size*board->size;
    board->board = malloc(sizeof(int) * board->total);
    board->boardPuzzle = malloc(sizeof(int) * board->total);
    const int values[3][3] = {{7,8,9},{37,45,52},{100,110,120}};
    board->level = values[(board->n) - 2][(board->level)-1];

}
int checkForWin(const GameBoard *board) {
    return 0;
}

int main(void) {
    srand(time(NULL));
    GameBoard *gameBoard = malloc(sizeof(GameBoard));
    //Player *player = malloc(sizeof(Player));

    int *checker = malloc(sizeof(int));
    *checker = 0;

    printf("SUDOKU\n");
    printf("-------\n");
    while (*checker == 0) {
        printf("Podaj wielkość planszy (2, 3, 4):");
        scanf("%d", &gameBoard->n);
        printf("Podaj poziom trudności (1-3):");
        scanf("%d", &gameBoard->level);
        if (gameBoard->n >1 && gameBoard->level > 0 &&
            gameBoard->level < 4 && gameBoard->n < 5) {
            *checker = 1;
            break;
        }
        printf("Podałeś Nieprawidłowe wartości\n");
    }
    free(checker);
    setBoard(gameBoard);

    sudokuGenerator(gameBoard);
    printf("Wygenerowana plansza: \n");
    printSimpleSudoku(gameBoard);
    int GameOn = 0;
    int isSaved = 0;
    while (1) {
        printf("SUDOKU\n");
        printf("-------\n");
        int choice;
        printf("1.Nowa gra\n");
        printf("2.Wczytaj gre");
        if (GameOn == 1)
            printf("3.Zapisz gre\n");
        printf("4.Wyjdz");
        while (choice > 1 && choice < 4) {
            scanf("%d", &choice);
        }


        switch (choice) {
            case 1:
            case 2:
            case 3:
            case 4:break;
        }
        if (choice == 4 && isSaved == 0) {
            printf("Twoja gra jest nie zapisana, utracisz cały postęp\n");
        }

    }


    free(gameBoard->board);
    free(gameBoard->boardPuzzle);
    free(gameBoard);
    return 0;
}