#ifndef PLAY_H
#define PLAY_H
#include <_timeval.h>

typedef struct {
    struct timeval start_time;
    struct timeval pause_time;
    double total_paused;
    int is_paused;
} GameClock;

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
    int gameOn;
    GameClock clock;
} GameBoard;

void title();
void printSimpleSudoku(const GameBoard *board);
int unUsedInBox(const int *board, int size, int n, int rowStart, int colStart, int num);
int unUsedInCol(const int *board, int size, int j, int num);
int unUsedInRow(const int *board, int size, int i, int num);
int checkIfSafe(const int *board, int size, int n, int i, int j, int num);
int countSolutionsHelper(int *board, int size, int n, int i, int j, int count);
int countSolutions(const GameBoard *board);
int fillRemaining(GameBoard *board, int i, int j);
void fillBox(const GameBoard *board, int row, int col);
void fillDiagonal(const GameBoard *board);
void removeKDigitsWithCheck(const GameBoard *board);
void sudokuGenerator(GameBoard *board);
void setBoard(GameBoard *board);
int checkForWin(const GameBoard *board);
void saveGameBoard(const char *filename, const GameBoard *game);
GameBoard* loadGameBoard(const char *filename);
void playGame(GameBoard *board, Player *player);
void initClock(GameClock *clock);
void startClock(GameClock *clock);
void pauseClock(GameClock *clock);
void resumeClock(GameClock *clock);
double getElapsedTime(const GameClock *clock);
void showTime(const GameClock *clock);
void solver(GameBoard *board);

#endif