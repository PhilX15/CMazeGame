#include <ncurses.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

#include "maze.h"
#include "utils.h"
#include "player.h"

int level = 0;
const int max_level = 5;
int maze_width = 13;
int maze_height = 13;

int level_coins = 5;
int collected_coins = 0;

enum game_state_t game_state = GAME;
struct maze_t *maze = NULL;
struct player_t *player = NULL;

void init_level() {
    wclear(stdscr);
    if (maze != NULL) {
        free_maze(maze);
        maze = NULL;
    }
    if (player != NULL) {
        free(player);
        player = NULL;
    }

    if (level > max_level) {
        game_state = END;
        return;
    }

    collected_coins = 0;
    level++;
    maze_width += 2;
    maze_height += 2;

    if (level % 2 == 0) {
        level_coins++;
    }

    maze = init_maze(maze_width, maze_height);
    int scr_width, scr_height;
    getmaxyx(stdscr, scr_height, scr_width);
    maze->top_left_x = (scr_width - maze->width) / 2;
    maze->top_left_y = (scr_height - maze->height) / 2;

    player = init_player(1, 1);

    create_maze(maze, 1, 1);
    repair_maze(maze);
    generate_coins(maze, level_coins);
}

void print_hud() {
    mvprintw(1, 1, "Level: %d/%d\tCoins: %d/%d", level, max_level, collected_coins, level_coins);
}

int main() {
    initscr();
    set_color();
    keypad(stdscr, TRUE);
    savetty();
    cbreak();
    noecho();
    timeout(100);
    leaveok(stdscr, TRUE);
    curs_set(0);
    
    srand(time(0));

    init_level();

    int scr_width = 0;
    int scr_height = 0;
    getmaxyx(stdscr, scr_height, scr_width);
    
    while (game_state == GAME) {
        int last_width = scr_width;
        int last_height = scr_height;
        getmaxyx(stdscr, scr_height, scr_width);
        if (last_width != scr_width || last_height != scr_height) {
            wclear(stdscr);
        }

        print_hud();
        print_maze(maze, scr_width, scr_height);
        mvprintw(maze->top_left_y + player->location.y, maze->top_left_x + player->location.x, "%s", PLAYER_S);
        box(stdscr, 0, 0);

        int key = wgetch(stdscr);
        move_player(maze, player, key);

        if (*(*(maze->maze + player->location.y) + player->location.x) == COIN_I) {
            collected_coins++;
            *(*(maze->maze + player->location.y) + player->location.x) = SPACE_I;
        }
        if (collected_coins == level_coins) {
            init_level();
        }

        refresh();
    }

    endwin();
    return 0;
}