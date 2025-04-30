#include "../include/play.h"
#include <stdio.h>
#include <stdlib.h>

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

    if (fread(&game->n, sizeof(int), 1, file) != 1 ||
        fread(&game->size, sizeof(int), 1, file) != 1 ||
        fread(&game->total, sizeof(int), 1, file) != 1 ||
        fread(&game->level, sizeof(int), 1, file) != 1 ||
        fread(&game->gameOn, sizeof(int), 1, file) != 1) {
        free(game);
        fclose(file);
        printf("Blad odczytu parametrow planszy\n");
        return NULL;
    }

    long start_usec, pause_usec;

    if (fread(&game->clock.start_time.tv_sec, sizeof(time_t), 1, file) != 1 ||
        fread(&start_usec, sizeof(long), 1, file) != 1 ||
        fread(&game->clock.pause_time.tv_sec, sizeof(time_t), 1, file) != 1 ||
        fread(&pause_usec, sizeof(long), 1, file) != 1 ||
        fread(&game->clock.total_paused, sizeof(double), 1, file) != 1 ||
        fread(&game->clock.is_paused, sizeof(int), 1, file) != 1) {
        free(game);
        fclose(file);
        printf("Blad odczytu danych zegara\n");
        return NULL;
    }

    game->clock.start_time.tv_usec = start_usec;
    game->clock.pause_time.tv_usec = pause_usec;

    game->board = malloc(sizeof(int) * game->total);
    game->boardPuzzle = malloc(sizeof(int) * game->total);

    if (!game->board || !game->boardPuzzle ||
        fread(game->board, sizeof(int), game->total, file) != (size_t)game->total ||
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