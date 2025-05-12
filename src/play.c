#include "../include/play.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <ctype.h>

int checkForWin(const GameBoard *board) {
    for (int i = 0; i < board->total; i++) {
        if (board->boardPuzzle[i] != board->board[i])
            return 0;
    }
    return 1;
}

void playGame(GameBoard *gameBoard, Player *player) {
    resumeClock(&gameBoard->clock);
    while (1) {
        printf("\nAktualna plansza:\n");
        printSimpleSudoku(gameBoard);
        showTime(&gameBoard->clock);
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
                printf("Podaj wspolrzedne (np. A1): ");
                char rowChar;
                if (scanf(" %c %d", &rowChar, &player->cordX) != 2) {
                    while (getchar() != '\n') {}
                    printf("Nieprawidlowy format! Uzyj np. A1\n");
                    break;
                }
                while (getchar() != '\n') {}

                // Konwersja litery na indeks wiersza (A=0, B=1, ...)
                player->cordY = toupper(rowChar) - 'A';
                player->cordX--; // Konwersja na indeks (1->0, 2->1, ...)

                if (player->cordY < 0 || player->cordY >= gameBoard->size ||
                    player->cordX < 0 || player->cordX >= gameBoard->size) {
                    printf("Wspolrzedne poza zakresem! Dostepne: %c-%c i 1-%d\n",
                           'A', 'A' + gameBoard->size - 1, gameBoard->size);
                    break;
                    }

                if (gameBoard->boardPuzzle[player->cordY * gameBoard->size + player->cordX] != 0) {
                    printf("To pole zawiera stala liczbe - nie mozna jej zmienic!\n");
                    break;
                }

                printf("Podaj liczbe (1-%d): ", gameBoard->size);
                if (scanf("%d", &player->number) != 1) {
                    while (getchar() != '\n') {}
                    printf("Nieprawidlowa liczba!\n");
                    break;
                }
                while (getchar() != '\n') {}

                if (player->number < 1 || player->number > gameBoard->size) {
                    printf("Liczba poza zakresem! Dopuszczalne: 1-%d\n", gameBoard->size);
                    break;
                }

                gameBoard->boardPuzzle[player->cordY * gameBoard->size + player->cordX] = player->number;

                player->win = checkForWin(gameBoard);
                if (player->win) {
                    printf("\nGratulacje! Rozwiazales Sudoku!\n");
                    sleep(3);
                    gameBoard->gameOn = 0;
                }
                break;
            }
            case 2: {
                printf("Podaj wspolrzedne do usuniecia (np. A1): ");
                char rowChar;
                if (scanf(" %c %d", &rowChar, &player->cordX) != 2) {
                    while (getchar() != '\n') {}
                    printf("Nieprawidlowy format! Uzyj np. A1\n");
                    sleep(3);
                    break;
                }
                while (getchar() != '\n') {}

                player->cordY = toupper(rowChar) - 'A';
                player->cordX--;

                if (player->cordY < 0 || player->cordY >= gameBoard->size ||
                    player->cordX < 0 || player->cordX >= gameBoard->size) {
                    printf("Wspolrzedne poza zakresem! Dostepne: %c-%c i 1-%d\n",
                           'A', 'A' + gameBoard->size - 1, gameBoard->size);
                    sleep(3);
                    break;
                    }

                if (gameBoard->boardPuzzle[player->cordY * gameBoard->size + player->cordX] == 0) {
                    printf("To pole jest juz puste!\n");
                    sleep(3);
                    break;
                }

                if (gameBoard->frozenCords[player->cordY * gameBoard->size + player->cordX] == 1) {
                    printf("Nie wolno usunac pola inicjacji!\n");
                    sleep(3);
                    break;
                }

                gameBoard->boardPuzzle[player->cordY * gameBoard->size + player->cordX] = 0;
                printf("Usunieto liczbe z pola %c%d\n", rowChar, player->cordX + 1);
                sleep(3);
                break;
            }
            case 3: {
                char *nazwa = malloc(100 * sizeof(char));
                if (nazwa == NULL) {
                    printf("Blad alokacji pamieci!\n");
                    break;
                }

                printf("Podaj nazwe zapisu: ");
                scanf("%99s", nazwa);  

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

                pauseClock(&gameBoard->clock);
                saveGameBoard(sciezka, gameBoard);
                sleep(3);
                printf("Gra zostala zapisana do pliku: %s\n", sciezka);

                free(nazwa);
                free(sciezka);
                break;
            }
            case 4: {
                pauseClock(&gameBoard->clock);
                break;
            }
            default: {
                printf("Nieprawidlowy wybor!\n");
                break;
            }
        }
        system("CLS");
        if (gameChoice == 4)
            break;
    }
}