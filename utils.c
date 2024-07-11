#include "utils.h"

void set_color() {
    start_color();
    init_pair(WALL_I, COLOR_GREEN, COLOR_BLACK);
    init_pair(SPACE_I, COLOR_BLACK, COLOR_BLACK);
    init_pair(TEMP_WALL_I, COLOR_BLACK, COLOR_BLACK);
    init_pair(COIN_I, COLOR_YELLOW, COLOR_BLACK);

    init_pair(PLAYER_I, COLOR_MAGENTA, COLOR_BLACK);
}