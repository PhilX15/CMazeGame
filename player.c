#include "player.h"

struct player_t *init_player(int x, int y) {
    struct player_t *player = malloc(sizeof(struct player_t));

    player->location.x = 1;
    player->location.y = 1;

    player->move_up = move_up;
    player->move_down = move_down;
    player->move_right = move_right;
    player->move_left = move_left;
}

void move_up(struct player_t *player) {
    mvprintw(player->location.y, player->location.x, "%s", SPACE_S);
    player->location.y--;
}

void move_down(struct player_t *player) {
    mvprintw(player->location.y, player->location.x, "%s", SPACE_S);
    player->location.y++;
}

void move_right(struct player_t *player) {
    mvprintw(player->location.y, player->location.x, "%s", SPACE_S);
    player->location.x++;
}

void move_left(struct player_t *player) {
    mvprintw(player->location.y, player->location.x, "%s", SPACE_S);
    player->location.x--;
}