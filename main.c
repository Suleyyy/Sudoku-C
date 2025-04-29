#include <locale.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>


typedef struct {
    int cordX;
    int cordY;
    int number;
    int win;
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
    if (board == NULL)
        return 0;
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

}

void sudokuGenerator(GameBoard *board) {
    int success = 0;

    do {
        // Wyczy?? plansz?
        for (int i = 0; i < board->total; i++) {
            board->board[i] = 0;
        }

        // Wype?nij bloki diagonalne
        fillDiagonal(board);

        // Wype?nij reszt? planszy
        success = fillRemaining(board, 0, 0);

        if (success) {
            for (int i = 0; i < board->total; i++)
                board->boardPuzzle[i] = board->board[i];
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
    for (int i = 0; i < board->total; i++) {
        if (board->boardPuzzle[i] != board->board[i])
            return 0;
    }
    return 1;
}
void saveGameBoard(const char *filename, const GameBoard *game) {
    FILE *file = fopen(filename, "wb");
    if (!file) {
        perror("Blad otwarcia pliku");
        return;
    }

    // Zapisujemy podstawowe parametry
    if (fwrite(&game->n, sizeof(int), 1, file) != 1 ||
        fwrite(&game->size, sizeof(int), 1, file) != 1 ||
        fwrite(&game->total, sizeof(int), 1, file) != 1 ||
        fwrite(&game->level, sizeof(int), 1, file) != 1) {
        fclose(file);
        printf("Blad zapisu parametrow planszy\n");
        return;
        }

    // Zapisujemy zawarto?? tablic
    if (fwrite(game->board, sizeof(int), game->total, file) != (size_t)game->total ||
        fwrite(game->boardPuzzle, sizeof(int), game->total, file) != (size_t)game->total) {
        fclose(file);
        printf("Blad zapisu danych planszy\n");
        return;
        }

    fclose(file);
}
GameBoard* loadGameBoard(const char *filename) {
    FILE *file = fopen(filename, "rb");
    if (!file) {
        perror("Blad otwarcia pliku");
        return NULL;
    }

    GameBoard *game = malloc(sizeof(GameBoard));
    if (!game) {
        fclose(file);
        return NULL;
    }

    // Wczytujemy podstawowe parametry
    if (fread(&game->n, sizeof(int), 1, file) != 1 ||
        fread(&game->size, sizeof(int), 1, file) != 1 ||
        fread(&game->total, sizeof(int), 1, file) != 1 ||
        fread(&game->level, sizeof(int), 1, file) != 1) {
        free(game);
        fclose(file);
        printf("Blad odczytu parametrow planszy\n");
        return NULL;
        }

    // Alokujemy pami?? dla tablic
    game->board = malloc(sizeof(int) * game->total);
    game->boardPuzzle = malloc(sizeof(int) * game->total);
    if (!game->board || !game->boardPuzzle) {
        free(game->board);
        free(game->boardPuzzle);
        free(game);
        fclose(file);
        printf("Blad alokacji pamieci dla planszy\n");
        return NULL;
    }

    // Wczytujemy zawarto?? tablic
    if (fread(game->board, sizeof(int), game->total, file) != (size_t)game->total ||
        fread(game->boardPuzzle, sizeof(int), game->total, file) != (size_t)game->total) {
        free(game->board);
        free(game->boardPuzzle);
        free(game);
        fclose(file);
        printf("Blad odczytu danych planszy\n");
        return NULL;
        }

    fclose(file);
    return game;
}

void playGame(const GameBoard *gameBoard, Player *player, int *gameOn) {
    while (*gameOn) {
        printf("\nAktualna plansza:\n");
        printSimpleSudoku(gameBoard);

        int gameChoice = 0;
        printf("\n1. Wprowadz liczbe\n");
        printf("2. Usun liczbe\n");
        printf("3. Zapisz gre\n");
        printf("4. Wroc do menu\n");
        printf("Wybor: ");

        if (scanf("%d", &gameChoice) != 1) {
            while (getchar() != '\n'){}
            printf("Nieprawidlowy wybor!\n");
            continue;
        }
        while (getchar() != '\n'){}

        switch (gameChoice) {
            case 1: {
                printf("Podaj wspolrzedne (wiersz kolumna): ");
                if (scanf("%d %d", &player->cordY, &player->cordX) != 2) {
                    while (getchar() != '\n') {}
                    printf("Nieprawidlowe wspolrzedne!\n");
                    break;
                }
                while (getchar() != '\n') {}

                if (player->cordY < 0 || player->cordY >= gameBoard->size ||
                    player->cordX < 0 || player->cordX >= gameBoard->size) {
                    printf("Wspolrzedne poza zakresem!\n");
                    break;
                }

                if (gameBoard->boardPuzzle[player->cordY * gameBoard->size + player->cordX] != 0) {
                    printf("To pole zawiera stala liczbe - nie mozna jej zmienic!\n");
                    break;
                }

                printf("Podaj liczb? (1-%d): ", gameBoard->size);
                if (scanf("%d", &player->number) != 1) {
                    while (getchar() != '\n'){}
                    printf("Nieprawidlowa liczba!\n");
                    break;
                }
                while (getchar() != '\n'){}

                if (player->number < 1 || player->number > gameBoard->size) {
                    printf("Liczba poza zakresem!\n");
                    break;
                }

                gameBoard->boardPuzzle[player->cordY * gameBoard->size + player->cordX] = player->number;

                player->win = checkForWin(gameBoard);
                if (player->win) {
                    printf("\nGratulacje! Rozwiazales Sudoku!\n");
                    *gameOn = 0;
                }
                break;
            }
            case 2: {
                printf("Podaj wspolrzedne do usuniecia (wiersz kolumna): ");
                if (scanf("%d %d", &player->cordY, &player->cordX) != 2) {
                    while (getchar() != '\n'){}
                    printf("Nieprawidlowe wspolrzedne!\n");
                    break;
                }
                while (getchar() != '\n') {}

                if (player->cordY < 0 || player->cordY >= gameBoard->size ||
                    player->cordX < 0 || player->cordX >= gameBoard->size) {
                    printf("Wspolrzedne poza zakresem!\n");
                    break;
                }

                if (gameBoard->boardPuzzle[player->cordY * gameBoard->size + player->cordX] == 0) {
                    printf("To pole jest juz puste!\n");
                    break;
                }

                gameBoard->boardPuzzle[player->cordY * gameBoard->size + player->cordX] = 0;
                break;
            }
            case 3: {
                saveGameBoard("../saves/save1.bin", gameBoard);
                printf("Gra zostala zapisana.\n");
                break;
            }
            case 4: {
                *gameOn = 0;
                break;
            }
            default: {
                printf("Nieprawidlowy wybor!\n");
                break;
            }
        }
    }
}

int main(void) {
    srand(time(NULL));
    GameBoard *gameBoard = malloc(sizeof(GameBoard));
    Player *player = malloc(sizeof(Player));
    if (gameBoard == NULL || player == NULL) {
        return -1;
    }
    int gameOn = 0; // Flaga czy gra jest w toku

    printf("SUDOKU\n");
    printf("-------");

    while (1) {
        int choice = 0;
        printf("\nMENU:\n");
        printf("1. Nowa gra\n");
        printf("2. Wczytaj gre\n");
        printf("3. Kontynuuj gre\n");
        printf("4. Wyjdz z gry\n");
        printf("Wybierz: ");

        if (scanf("%d", &choice) != 1) {
            while (getchar() != '\n'){}
            printf("Nieprawidlowy wybor!\n");
            continue;
        }
        while (getchar() != '\n'){}

        switch (choice) {
            case 1: { // Nowa gra
                if (gameOn) {
                    printf("Masz niezakonczona gre. Czy na pewno chcesz rozpoczac nowa (t/n): ");
                    char confirm = getchar();
                    while (getchar() != '\n'){}
                    if (confirm != 't' && confirm != 'T') {
                        break;
                    }
                    // Zwolnienie pami?ci starej gry
                    free(gameBoard->board);
                    free(gameBoard->boardPuzzle);
                }

                player->win = 0;
                gameBoard->n = 0;
                gameBoard->level = 0;

                while (gameBoard->n < 2 || gameBoard->n > 4) {
                    printf("Podaj wielkosc planszy (2, 3, 4): ");
                    if (scanf("%d", &gameBoard->n) != 1) {
                        while (getchar() != '\n'){}
                        continue;
                    }
                    while (getchar() != '\n');
                }

                while (gameBoard->level < 1 || gameBoard->level > 3) {
                    printf("Podaj poziom trudnosci (1-3): ");
                    if (scanf("%d", &gameBoard->level) != 1) {
                        while (getchar() != '\n'){}
                        continue;
                    }
                    while (getchar() != '\n');
                }

                setBoard(gameBoard);
                sudokuGenerator(gameBoard);
                gameOn = 1;
                playGame(gameBoard, player, &gameOn);
                break;
            }
            case 2: { // Wczytaj gr?
                if (gameOn) {
                    printf("Masz niezakonczona gre. Czy na pewno chcesz wczytac nowa (t/n): ");
                    char confirm = getchar();
                    while (getchar() != '\n'){}
                    if (confirm != 't' && confirm != 'T') {
                        break;
                    }
                    // Zwolnienie pami?ci starej gry
                    free(gameBoard->board);
                    free(gameBoard->boardPuzzle);
                }

                GameBoard* loaded = loadGameBoard("../saves/save1.bin");
                if (loaded) {
                    *gameBoard = *loaded;
                    free(loaded);
                    printf("\nWczytano gre:\n");
                    gameOn = 1;
                    playGame(gameBoard, player, &gameOn);
                } else {
                    printf("Nie udalo sie wczytac gry.\n");
                }
                break;
            }
            case 3: { // Kontynuuj gr?
                if (gameOn) {
                    playGame(gameBoard, player, &gameOn);
                } else {
                    printf("Nie masz aktywnej gry do kontynuacji.\n");
                }
                break;
            }
            case 4: { // Wyjd? z gry
                if (gameBoard->board) free(gameBoard->board);
                if (gameBoard->boardPuzzle) free(gameBoard->boardPuzzle);
                free(gameBoard);
                free(player);
                printf("Dziekujemy za gre!\n");
                return 0;
            }
            default: {
                printf("Nieprawidlowy wybór!\n");
                break;
            }
        }
    }
}