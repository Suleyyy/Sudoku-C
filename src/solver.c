#include <stdio.h>
#include <stdlib.h>
#include <../include/play.h>

void solver(GameBoard *board) {
    double T_start = 1000.0; // Wyższa temp. początkowa dla problemów z większą skalą wartości
    double T_end = 0.01;
    double alpha = 0.99; // Współczynnik chłodzenia
    int max_iterations = 200000; // Więcej iteracji może być potrzebne

    solve_knapsack_sa(board->boardPuzzle, board->, capacity, T_start, T_end, alpha, max_iterations);
}