#ifndef UTILS_H
#define UTILS_H

#include "ncurses.h"

// GAME STATES
enum game_state_t {
    MENU,
    GAME,
    END
};

// REPRESENTATION
#define SPACE_S " "
#define WALL_S "#"
#define TEMP_WALL_S " "
#define COIN_S "o"
#define PLAYER_S "@"

// INDEXES
#define SPACE_I 0
#define WALL_I 1
#define TEMP_WALL_I 2
#define COIN_I 3
#define PLAYER_I 4

struct location_t {
    int x;
    int y;
};

void set_color();

#endif