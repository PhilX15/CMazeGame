#include "player.h"

struct player_t *init_player(int x, int y) {
    struct player_t *player = malloc(sizeof(struct player_t));

    player->location.x = x;
    player->location.y = y;

    player->move_up = move_up;
    player->move_down = move_down;
    player->move_right = move_right;
    player->move_left = move_left;
}

void move_up(struct player_t *player) {
    if (player == NULL) {
        return;
    }
    player->location.y--;
}

void move_down(struct player_t *player) {
    if (player == NULL) {
        return;
    }
    player->location.y++;
}

void move_right(struct player_t *player) {
    if (player == NULL) {
        return;
    }
    player->location.x++;
}

void move_left(struct player_t *player) {
    if (player == NULL) {
        return;
    }
    player->location.x--;
}

void move_player(struct maze_t *maze, struct player_t *player, int key) {
    if (player == NULL) {
        return;
    }
    switch (key) {
        case KEY_UP:
            if (*(*(maze->maze + player->location.y - 1) + player->location.x) == WALL_I) break;
            mvprintw(player->location.y + maze->top_left_y, player->location.x + maze->top_left_x, "%s", SPACE_S);
            player->move_up(player);
            break;
        case KEY_DOWN:
            if (*(*(maze->maze + player->location.y + 1) + player->location.x) == WALL_I) break;
            mvprintw(player->location.y + maze->top_left_y, player->location.x + maze->top_left_x, "%s", SPACE_S);
            player->move_down(player);
            break;
        case KEY_RIGHT:
            if (*(*(maze->maze + player->location.y) + player->location.x + 1) == WALL_I) break;
            mvprintw(player->location.y + maze->top_left_y, player->location.x + maze->top_left_x, "%s", SPACE_S);
            player->move_right(player);
            break;
        case KEY_LEFT:
            if (*(*(maze->maze + player->location.y) + player->location.x - 1) == WALL_I) break;
            mvprintw(player->location.y + maze->top_left_y, player->location.x + maze->top_left_x, "%s", SPACE_S);
            player->move_left(player);
            break;
        default:
            break;
    }
}