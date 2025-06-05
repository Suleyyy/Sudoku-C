#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include "../include/play.h"


int main(void) {
    srand(time(NULL));
    GameBoard *gameBoard = malloc(sizeof(GameBoard));
    Player *player = malloc(sizeof(Player));
    if (gameBoard == NULL || player == NULL) {
        return -1;
    }
    gameBoard->gameOn = -1;

    while (1) {
        title();
        int choice = 0;
        printf("MENU:\n");
        printf("1. Nowa gra\n");
        printf("2. Wczytaj gre\n");
        printf("3. Kontynuuj gre\n");
        printf("4. Jak grac\n");
        printf("5. Wyjdz z gry\n");
        printf("6. Solver\n");
        printf("7. GA_Solver\n");
        printf("Wybierz: ");
        if (scanf("%d", &choice) != 1) {
            while (getchar() != '\n'){} //czyści bufor z scanf
            printf("Nieprawidlowy wybor!\n");
            continue;
        }
        while (getchar() != '\n'){}
        system("CLS");
        switch (choice) {
            case 1: {
                if (gameBoard->gameOn == 1) {
                    printf("Masz niezakonczona gre. Czy na pewno chcesz rozpoczac nowa (t/n): ");
                    char confirm = getchar();
                    while (getchar() != '\n'){}
                    if (confirm != 't' && confirm != 'T') {
                        break;
                    }
                    free(gameBoard->board);
                    free(gameBoard->boardPuzzle);
                    free(gameBoard->frozenCords);
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
                system("CLS");
                setBoard(gameBoard);
                sudokuGenerator(gameBoard);
                startClock(&gameBoard->clock);
                playGame(gameBoard, player);
                break;
            }
            case 2: {
                if (gameBoard->gameOn == 0) {
                    printf("Masz niezakonczona gre. Czy na pewno chcesz wczytac nowa (t/n): ");
                    char confirm = getchar();
                    while (getchar() != '\n') {}

                    if (confirm != 't' && confirm != 'T') {
                        break;
                    }

                    free(gameBoard->board);
                    free(gameBoard->boardPuzzle);
                    free(gameBoard->frozenCords);
                }

                char *nazwa = malloc(100 * sizeof(char));
                if (nazwa == NULL) {
                    printf("Blad alokacji pamieci!\n");
                    break;
                }

                printf("Podaj nazwe zapisu do wczytania: ");
                scanf("%99s", nazwa);
                while (getchar() != '\n') {}

                // Budowa ścieżki
                size_t pathLen = strlen("../saves/") + strlen(nazwa) + strlen(".bin") + 1;
                char *sciezka = malloc(pathLen);
                if (sciezka == NULL) {
                    printf("Blad alokacji pamieci dla sciezki!\n");
                    free(nazwa);
                    break;
                }

                strcpy(sciezka, "../saves/");
                strcat(sciezka, nazwa);
                strcat(sciezka, ".bin");

                // Próba wczytania gry
                GameBoard* loaded = loadGameBoard(sciezka);
                if (loaded) {
                    *gameBoard = *loaded;
                    free(loaded);
                    printf("\nWczytano gre z pliku: %s\n", sciezka);
                    playGame(gameBoard, player);
                } else {
                    printf("Nie udalo sie wczytac gry z pliku: %s\n", sciezka);
                }

                // Sprzątanie
                free(nazwa);
                free(sciezka);
                break;
            }
            case 3: {
                if (gameBoard->gameOn != -1) {
                    playGame(gameBoard, player);
                } else {
                    printf("Nie masz aktywnej gry do kontynuacji.\n");
                }
                break;
            }
            case 4: {
                system("CLS");
                printf("ZASADY:\n");
                printf("- Kazdy mniejszy kwadracik ma miec unikatowa liczbe\n");
                printf("- W kazdym rzedzie ma byc unikatowa liczba\n");
                printf("- W kazdej kolumnie ma byc unikatowa liczba\n");
                printf("- Nacisnij Enter, aby kontynuowac...\n");
                getchar();
                system("CLS");
                break;
            }
            case 5: {
                if (gameBoard->board) free(gameBoard->board);
                if (gameBoard->boardPuzzle) free(gameBoard->boardPuzzle);
                if (gameBoard->frozenCords) free(gameBoard->frozenCords);
                free(gameBoard);
                free(player);
                printf("Dziekujemy za gre!\n");
                sleep(3);
                return 0;
            }
            case 6: {
                if (gameBoard->gameOn != -1)
                    solver(gameBoard);
                else {
                    printf("Stworz Sudoku 9x9");
                    sleep(3);
                }
                system("CLS");
                break;
            }
            case 7: {
                if (gameBoard->gameOn != -1)
                    gasolver(gameBoard);
                else {
                    printf("Stworz Sudoku 9x9");
                    sleep(3);
                }
                system("CLS");
                break;
            }
            default: {
                printf("Nieprawidlowy wybór!\n");
                break;
            }
        }
    }
}