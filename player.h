#ifndef PLAYER_H
#define PLAYER_H

#include <stdlib.h>

#include "utils.h"

struct player_t {
    struct location_t location;

    void (*move_up)(struct player_t *);
    void (*move_down)(struct player_t *);
    void (*move_right)(struct player_t *);
    void (*move_left)(struct player_t *);
};

struct player_t *init_player(int x, int y);

void move_up(struct player_t *player);
void move_down(struct player_t *player);
void move_right(struct player_t *player);
void move_left(struct player_t *player);


#endif