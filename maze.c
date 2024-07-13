#include "maze.h"

struct maze_t *init_maze(int width, int height) {
    struct maze_t *maze_struct = malloc(sizeof(struct maze_t));
    if (maze_struct == NULL) {
        return NULL;
    }

    maze_struct->width = width;
    maze_struct->height = height;
    maze_struct->top_left_x = 0;
    maze_struct->top_left_y = 0;

    maze_struct->maze = malloc(height * sizeof(int*));
    if (maze_struct->maze == NULL) {
        return NULL;
    }

    for (int i = 0; i < height; i++) {
        *(maze_struct->maze + i) = malloc(width * sizeof(int));
        if (*(maze_struct->maze + i) == NULL) {
            for (int j = 0; j < i; j++) {
                free(*(maze_struct->maze + j));
            }
            free(maze_struct->maze);
        }
        for (int j = 0; j < width; j++) {
            if (i == 0 || i == height - 1 || j % 2 == 0 || i % 2 == 0) {
                *(*(maze_struct->maze + i) + j) = WALL_I;
            } else {
                *(*(maze_struct->maze + i) + j) = SPACE_I;
            }
        }
    }

    return maze_struct;
}

int create_maze(struct maze_t *maze, int x, int y) {
    if (x < 0 || y < 0 || x >= maze->width || y >= maze->height) {
        return 0;
    }

    if (*(*(maze->maze + y) + x) == 2) {
        return 0;
    }

    *(*(maze->maze + y) + x) = 2;

    struct location_t neighbours[4] = {{2, 0}, {-2, 0}, {0, 2}, {0, -2}};

    for (int i = 0; i < 4; i++) {
        int random = rand() % 4;
        struct location_t temp = *(neighbours + i);
        *(neighbours + i) = *(neighbours + random);
        *(neighbours + random) = temp;
    }

    for (int i = 0; i < 4; i++) { 
        if (create_maze(maze, x + (*(neighbours + i)).x, y + (*(neighbours + i)).y) == 1) {
            if ((*(neighbours + i)).x == 2) {
                *(*(maze->maze + y) + x + 1) = TEMP_WALL_I;
            } else if ((*(neighbours + i)).x == -2) {
                *(*(maze->maze + y) + x - 1) = TEMP_WALL_I;
            } else if ((*(neighbours + i)).y == 2) {
                 *(*(maze->maze + y + 1) + x) = TEMP_WALL_I;
            } else {
                *(*(maze->maze + y - 1) + x) = TEMP_WALL_I;
            }
        }
    }

    return 1;
}

void repair_maze(struct maze_t *maze) {
    if (maze == NULL) {
        return;
    }

    for (int y = 0; y < maze->height; y++) {
        for (int x = 0; x < maze->width; x++) {
            if (*(*(maze->maze + y) + x) == TEMP_WALL_I) {
                *(*(maze->maze + y) + x) = SPACE_I;
            }
        }
    }
}

void generate_coins(struct maze_t *maze, int count) {
    if (maze == NULL) {
        return;
    }

    for (int i = 0; i < count; i++) {
        int coin_placed = 0;
        while (!coin_placed) {
            int random = rand() % (maze->width * maze->height);
            int x = random - random / maze->width * maze->width;
            int y = random / maze->width;
            if (*(*(maze->maze + y) + x) == 0) {
                *(*(maze->maze + y) + x) = 3;
                coin_placed = 1;
            }
        }
    }
}

void print_maze(struct maze_t *maze, int scr_width, int scr_height) {
    if (maze == NULL) {
        return;
    }

    maze->top_left_x = (scr_width - maze->width) / 2;
    maze->top_left_y = (scr_height - maze->height) / 2;

    for (int y = 0; y < maze->height; y++) {
        for (int x = 0; x < maze->width; x++) {
            switch(*(*(maze->maze + y) + x)) {
                case SPACE_I:
                    attron(COLOR_PAIR(SPACE_I));
                    mvprintw(maze->top_left_y + y, maze->top_left_x + x, "%s", SPACE_S);
                    attroff(COLOR_PAIR(SPACE_I));
                    break;
                case WALL_I:
                    attron(COLOR_PAIR(WALL_I));
                    mvprintw(maze->top_left_y + y, maze->top_left_x + x, "%s", WALL_S);
                    attroff(COLOR_PAIR(WALL_I));
                    break;
                case TEMP_WALL_I:
                    attron(COLOR_PAIR(SPACE_I));
                    mvprintw(maze->top_left_y + y, maze->top_left_x + x, "%s", SPACE_S);
                    attroff(COLOR_PAIR(SPACE_I));
                    break;
                case COIN_I:
                    attron(COLOR_PAIR(COIN_I));
                    mvprintw(maze->top_left_y + y, maze->top_left_x + x, "%s", COIN_S);
                    attroff(COLOR_PAIR(COIN_I));
                    break;
                default:
                    attron(COLOR_PAIR(SPACE_I));
                    mvprintw(maze->top_left_y + y, maze->top_left_x + x, "%s", SPACE_S);
                    attroff(COLOR_PAIR(SPACE_I));
            }
        }
        printw("\n");
    }

    move(0, 0);
}

void free_maze(struct maze_t *maze) {
    if (maze == NULL) {
        return;
    }

    if (maze->maze != NULL) {
        for (int i = 0; i < maze->height; i++) {
            free(*(maze->maze + i));
        }
        free(maze->maze);
    }
    free(maze);
}