#include "../include/play.h"
#include <stdio.h>
#include <sys/time.h>

void initClock(GameClock *clock) {
  clock->is_paused = 1;
  clock->total_paused = 0.0;
}

void startClock(GameClock *clock) {
  gettimeofday(&clock->start_time, NULL);
  clock->is_paused = 0;
  clock->total_paused = 0.0;
}

void pauseClock(GameClock *clock) {
  if (!clock->is_paused) {
    gettimeofday(&clock->pause_time, NULL);
    clock->is_paused = 1;
  }
}

void resumeClock(GameClock *clock) {
  if (clock->is_paused) {
    struct timeval now;
    gettimeofday(&now, NULL);
    clock->total_paused += (now.tv_sec - clock->pause_time.tv_sec) +
                         (now.tv_usec - clock->pause_time.tv_usec) / 1000000.0;
    clock->is_paused = 0;
  }
}

double getElapsedTime(const GameClock *clock) {
  struct timeval now;
  gettimeofday(&now, NULL);

  if (clock->is_paused) {
    return (clock->pause_time.tv_sec - clock->start_time.tv_sec) +
           (clock->pause_time.tv_usec - clock->start_time.tv_usec) / 1000000.0 -
           clock->total_paused;
  }

  return (now.tv_sec - clock->start_time.tv_sec) +
         (now.tv_usec - clock->start_time.tv_usec) / 1000000.0 -
         clock->total_paused;
}

void showTime(const GameClock *clock) {
  double elapsed = getElapsedTime(clock);
  printf("Czas gry: %.2f sekund\n", elapsed);
}