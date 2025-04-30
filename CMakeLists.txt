cmake_minimum_required(VERSION 3.30)
project(Sudoku-C C)

set(CMAKE_C_STANDARD 11)

include_directories(include)

add_executable(Sudoku-C
        src/main.c
        src/play.c
        src/printer.c
        src/generator.c
        src/initBoard.c
        src/saver.c
        src/loader.c
        src/clock.c
)

