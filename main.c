#include <stdio.h>
#include <stdlib.h>
#include <time.h>

//TODO: Trzeba dodać sprawdzanie czy po usuwaniu jest jedno rozwiązanie i usuwać dopiero kiedy tak faktycznie jest,
//      Zrobić interfejs użytkownika
//      Zrobić zapis :/



void printSimpleSudoku(const int *board, int n) {
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
                printf(" - ");
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
int countSolutionsHelper(int *board, int i, int j, int n, int count) {
    int size = n * n;

    // Znajdź następne puste pole
    while (i < size && board[i * size + j] != 0) {
        j++;
        if (j >= size) {
            j = 0;
            i++;
        }
    }

    // Jeśli doszliśmy do końca, zwiększ licznik rozwiązań
    if (i == size) {
        return count + 1;
    }

    // Próbuj wszystkie możliwe liczby
    for (int num = 1; num <= size && count < 2; num++) {
        if (checkIfSafe(board, i, j, num, n)) {
            board[i * size + j] = num;

            // Rekurencyjnie sprawdź następne pole
            int next_j = j + 1;
            int next_i = i;
            if (next_j >= size) {
                next_j = 0;
                next_i++;
            }

            count = countSolutionsHelper(board, next_i, next_j, n, count);

            // Backtrack
            board[i * size + j] = 0;
        }
    }

    return count;
}

// Główna funkcja sprawdzająca liczbę rozwiązań
int countSolutions(const int *board, int n) {
    // Tworzymy kopię planszy, aby nie modyfikować oryginału
    int size = n * n;
    int tempBoard[size * size];
    for (int i = 0; i < size * size; i++) {
        tempBoard[i] = board[i];
    }

    return countSolutionsHelper(tempBoard, 0, 0, n, 0);
}

// Funkcja która zwraca 0 gdy jest więcej niż 1 rozwiązanie
int hasUniqueSolution(const int *board, int n) {
    int solutions = countSolutions(board, n);
    return solutions == 1;
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
void removeKDigitsWithCheck(int *board, int n, int k) {
    int size = n * n;
    int cells_removed = 0;
    int attempts = 0;
    const int max_attempts = size * size * 2; // Zabezpieczenie przed nieskończoną pętlą

    while (cells_removed < k && attempts < max_attempts) {
        // Losujemy komórkę do usunięcia
        int cellId = rand() % (size * size);
        int i = cellId / size;
        int j = cellId % size;

        // Sprawdzamy czy komórka nie jest już pusta
        if (board[i * size + j] != 0) {
            // Zapamiętujemy wartość przed usunięciem
            int temp = board[i * size + j];
            board[i * size + j] = 0;

            // Tworzymy kopię planszy do sprawdzenia
            int tempBoard[size * size];
            for (int x = 0; x < size * size; x++) {
                tempBoard[x] = board[x];
            }

            // Sprawdzamy czy plansza nadal ma dokładnie 1 rozwiązanie
            if (countSolutions(tempBoard, n) == 1) {
                cells_removed++;
            } else {
                // Przywracamy wartość jeśli usunięcie spowodowałoby wiele rozwiązań
                board[i * size + j] = temp;
            }
        }
        attempts++;
    }

    if (cells_removed < k) {
        printf("Uwaga: Usunięto tylko %d z %d liczb (zachowując unikalność rozwiązania)\n", cells_removed, k);
    }
}

void sudokuGenerator(int *board, int k, int n) {
    int success = 0;
    int size = n * n;
    int total_cells = size * size;

    do {
        // Wyczyść planszę
        for (int i = 0; i < total_cells; i++) {
            board[i] = 0;
        }

        // Wypełnij bloki diagonalne
        fillDiagonal(board, n);

        // Wypełnij resztę planszy
        success = fillRemaining(board, 0, 0, n);

        if (success) {
            // Usuń K liczb z sprawdzaniem unikalności
            removeKDigitsWithCheck(board, n, k);

            // Sprawdź czy plansza nadal ma rozwiązanie
            int tempBoard[size * size];
            for (int i = 0; i < size * size; i++) {
                tempBoard[i] = board[i];
            }
            success = (countSolutions(tempBoard, n) >= 1);
        }
    } while (n == 2 && !success);
}

int main(void) {
    srand(time(NULL));

    int k = 120;
    int n = 4;

    int gameBoard[n*n][n*n];
    sudokuGenerator(gameBoard, k, n);

    printSimpleSudoku(gameBoard, n);

    int solutions = countSolutions(gameBoard, n);
    if (solutions == 1) {
        printf("Plansza ma dokładnie 1 rozwiązanie.\n");
    } else {
        printf("Plansza ma %d rozwiązań.\n", solutions);
    }

    return 0;
}