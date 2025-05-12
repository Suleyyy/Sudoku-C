#include "../include/play.h"
#include <stdlib.h>

void setBoard(GameBoard *board) {
    board->size = board->n*board->n;
    board->total = board->size*board->size;
    board->board = malloc(sizeof(int) * board->total);
    board->boardPuzzle = malloc(sizeof(int) * board->total);
    board->frozenCords = malloc(sizeof(int) * board->total);
    const int values[3][3] = {{7,8,9},{37,45,52},{100,110,120}};
    board->level = values[(board->n) - 2][(board->level)-1];
    board->gameOn = 1;
    initClock(&board->clock);
}