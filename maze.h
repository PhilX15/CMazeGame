#ifndef MAZE_H
#define MAZE_H

#include <math.h>
#include <stdlib.h>

#include "utils.h"

struct maze_t {
    int **maze;
    int width;
    int height;
};

struct maze_t *init_maze(int width, int height);
int create_maze(struct maze_t *maze, int x, int y);
void repair_maze(struct maze_t *maze);

void generate_coins(struct maze_t *maze, int count);

void print_maze(struct maze_t *maze);

void free_maze(struct maze_t *maze);

#endif