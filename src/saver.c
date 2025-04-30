#include "../include/play.h"
#include <stdio.h>
#include <stdlib.h>

void saveGameBoard(const char *filename, const GameBoard *game) {
    FILE *file = fopen(filename, "wb");
    if (!file) {
        perror("Blad otwarcia pliku");
        return;
    }

    if (fwrite(&game->n, sizeof(int), 1, file) != 1 ||
        fwrite(&game->size, sizeof(int), 1, file) != 1 ||
        fwrite(&game->total, sizeof(int), 1, file) != 1 ||
        fwrite(&game->level, sizeof(int), 1, file) != 1 ||
        fwrite(&game->gameOn, sizeof(int), 1, file) != 1) {
        fclose(file);
        printf("Blad zapisu parametrow planszy\n");
        return;
    }

    long start_usec = game->clock.start_time.tv_usec;
    long pause_usec = game->clock.pause_time.tv_usec;

    if (fwrite(&game->clock.start_time.tv_sec, sizeof(time_t), 1, file) != 1 ||
        fwrite(&start_usec, sizeof(long), 1, file) != 1 ||
        fwrite(&game->clock.pause_time.tv_sec, sizeof(time_t), 1, file) != 1 ||
        fwrite(&pause_usec, sizeof(long), 1, file) != 1 ||
        fwrite(&game->clock.total_paused, sizeof(double), 1, file) != 1 ||
        fwrite(&game->clock.is_paused, sizeof(int), 1, file) != 1) {
        fclose(file);
        printf("Blad zapisu danych zegara\n");
        return;
    }

    if (fwrite(game->board, sizeof(int), game->total, file) != (size_t)game->total ||
        fwrite(game->boardPuzzle, sizeof(int), game->total, file) != (size_t)game->total) {
        fclose(file);
        printf("Blad zapisu danych planszy\n");
        return;
    }

    fclose(file);
}
