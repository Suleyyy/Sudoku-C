#include <../include/play.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>


#define POPULATION_SIZE 100
#define NUM_GENERATIONS 100000
#define MUTATION_RATE 0.025
#define CROSSOVER_RATE 0.76


void fillBoxHerev2(int *board, int row, int col, int n, int size) {
  int num;
  for (int i = 0; i < n; i++) {
    for (int j = 0; j < n; j++) {
      if (board[(row + i) * size + (col + j)] == 0) {
        do {
          num = (rand() % size) + 1;
        } while (!unUsedInBox(board, size, n, row, col, num));
        board[(row + i) * size + (col + j)] = num;
      }
    }
  }
}

void initializePopulation(Individual population[], GameBoard *board) {
  for (int z = 0; z < POPULATION_SIZE; z++) {
    population[z].board = malloc(board->total * sizeof(int));
    memcpy(population[z].board, board->boardPuzzle, board->total * sizeof(int));
    for (int i = 0; i < board->size; i += board->n) {
      for (int j = 0; j < board->size; j += board->n) {
        fillBoxHerev2(population[z].board, i, j, board->n, board->size);
      }
    }
    population[z].fitness = 100;
  }
}

void calculateFitness(Individual *individual, GameBoard *board) {
  int cost = 0;

  for (int i = 0; i < board->size; i++) {
    int count[board->size + 1];
    memset(count, 0, sizeof(count));

    for (int j = 0; j < board->size; j++) {
      int num = individual->board[i * board->size + j];
      count[num]++;
    }

    for (int n = 1; n <= board->size; n++) {
      if (count[n] > 1) {
        cost += count[n] - 1;
      }
    }
  }

  for (int j = 0; j < board->size; j++) {
    int count[board->size + 1];
    memset(count, 0, sizeof(count));

    for (int i = 0; i < board->size; i++) {
      int num = individual->board[i * board->size + j];
      count[num]++;
    }

    for (int n = 1; n <= board->size; n++) {
      if (count[n] > 1) {
        cost += count[n] - 1;
      }
    }
  }
  individual->fitness = cost;
}

void evaluatePopulation(Individual population[], GameBoard *board) {
  for (int i = 0; i < POPULATION_SIZE; i++) {
    calculateFitness(&population[i], board);
  }
}

Individual selectTournament(Individual population[]) {
  int tournament_size = 3;

  Individual best_in_tournament = population[rand() % POPULATION_SIZE];

  for (int i = 1; i < tournament_size; i++) {
    Individual current = population[rand() % POPULATION_SIZE];
    if (current.fitness < best_in_tournament.fitness) {
      best_in_tournament = current;
    }
  }
  return best_in_tournament;
}
void uniformCrossover(Individual parent1, Individual parent2, Individual *child1, Individual *child2, int size) {
  int total = size * size;

  for (int i = 0; i < total; i++) {
    if ((double)rand() / RAND_MAX < 0.5) {
      child1->board[i] = parent1.board[i];
      child2->board[i] = parent2.board[i];
    } else {
      child1->board[i] = parent2.board[i];
      child2->board[i] = parent1.board[i];
    }
  }

  child1->fitness = 0;
  child2->fitness = 0;
}

void singleCrossover(Individual parent1, Individual parent2, Individual *child1, Individual *child2, int size) {
  for (int i = 0; i < size; i++) {
    for (int j = 0; j < size; j++) {
      child1->board[i*size+j] = parent1.board[i*size+j];
      child2->board[i*size+j] = parent2.board[i*size+j];
    }
  }
  if ((double)rand() / RAND_MAX < CROSSOVER_RATE) {
    int crossover_point1 = rand() % size;
    int crossover_point2 = rand() % size;

    for (int i = crossover_point1; i < size; i++) {
      for (int j = crossover_point2; j < size; j++) {
        int temp = child1->board[i*size+j];
        child1->board[i*size+j] = child2->board[i*size+j];
        child2->board[i*size+j] = temp;
      }
    }
  }

  child1->fitness = 0;
  child2->fitness = 0;
}

void mutate(Individual *individual, GameBoard *board) {
  for (int i = 0; i < board->size; i++) {
    for (int j = 0; j < board->size; j++) {
      if (board->frozenCords[i * board->size + j] == 0 && ((double)rand() / RAND_MAX < MUTATION_RATE)) {
        individual->board[i*board->size+j] = (rand() % board->size) + 1;
      }
    }
  }
}


void gasolver(GameBoard *board){
  srand(time(NULL));
  int choice = 0;
  while (choice == 0 || choice > 2) {
    printf("Wybierz cross\n");
    printf("1. single\n");
    printf("2. uniform\n");
    printf("Wybierz: ");
    if (scanf("%d", &choice) != 1 || choice > 2) {
      while (getchar() != '\n'){}
      printf("Nieprawidlowy wybor!\n");
    }
  }
  while (getchar() != '\n'){}
  Individual population[POPULATION_SIZE];
  Individual next_population[POPULATION_SIZE];

  initializePopulation(population, board);

  Individual best_solution_ever;
  best_solution_ever.fitness = 100;
  best_solution_ever.board = malloc(board->total * sizeof(int));

  for (int generation = 0; generation < NUM_GENERATIONS; generation++) {
    evaluatePopulation(population, board);

    Individual *current_best = &population[0];
    for (int i = 1; i < POPULATION_SIZE; i++) {
      if (population[i].fitness < current_best->fitness) {
        current_best = &population[i];
      }
    }

    if (current_best->fitness < best_solution_ever.fitness) {
      if (best_solution_ever.board == NULL) {
        best_solution_ever.board = malloc(board->total * sizeof(int));
      }
      memcpy(best_solution_ever.board, current_best->board, board->total * sizeof(int));
      best_solution_ever.fitness = current_best->fitness;

      printf("Pokolenie %d: Znaleziono lepsze rozwiazanie - wartosc = %d\n",
             generation, best_solution_ever.fitness);
    } else if (generation % 10000 == 0) {
      printf("Pokolenie %d: Najlepsza wartosc = %d\n", generation, current_best->fitness);
    }

    int current_population_idx = 0;

    while (current_population_idx < POPULATION_SIZE) {

        Individual parent1 = selectTournament(population);
        Individual parent2 = selectTournament(population);


      Individual child1, child2;

      child1.board = malloc(board->total * sizeof(int));
      child2.board = malloc(board->total * sizeof(int));

      if (choice == 1)
        singleCrossover(parent1, parent2, &child1, &child2, board->size);
      else
        uniformCrossover(parent1, parent2, &child1, &child2, board->size);

      mutate(&child1, board);
      mutate(&child2, board);

      next_population[current_population_idx++] = child1;
      if (current_population_idx < POPULATION_SIZE) {
        next_population[current_population_idx++] = child2;
      }
    }
    for (int i = 0; i < POPULATION_SIZE; i++) {
      free(population[i].board);
    }
    for(int i = 0; i < POPULATION_SIZE; i++){
      population[i] = next_population[i];
    }
    if (best_solution_ever.fitness == 0)
      break;
  }
  printf("\n--- Koniec Algorytmu Genetycznego ---\n");
  printf("Najlepsze znalezione rozwiazanie:\n");
  printf("Wartosc: %d\n", best_solution_ever.fitness);
  printSimpleSudokuv2(best_solution_ever.board, board);

  free(best_solution_ever.board);
  getchar();
}
