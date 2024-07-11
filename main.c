#include <ncurses.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

#include "maze.h"
#include "utils.h"
#include "player.h"

int level = 1;
int maze_width = 13;
int maze_height = 13;

int level_coins = 5;
int collected_coins = 0;

enum game_state_t game_state = GAME;

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

    struct maze_t *maze = init_maze(maze_width, maze_height);
    create_maze(maze, 1, 1);
    repair_maze(maze);
    generate_coins(maze, level_coins);
    print_maze(maze);

    struct player_t *player = init_player(1, 1);
    
    while (game_state == GAME) {
        int key = wgetch(stdscr);
        switch (key) {
            case KEY_UP:
                if (*(*(maze->maze + player->location.y - 1) + player->location.x) == WALL_I) break;
                player->move_up(player);
                break;
            case KEY_DOWN:
                if (*(*(maze->maze + player->location.y + 1) + player->location.x) == WALL_I) break;
                player->move_down(player);
                break;
            case KEY_RIGHT:
                if (*(*(maze->maze + player->location.y) + player->location.x + 1) == WALL_I) break;
                player->move_right(player);
                break;
            case KEY_LEFT:
                if (*(*(maze->maze + player->location.y) + player->location.x - 1) == WALL_I) break;
                player->move_left(player);
                break;
            default:
                break;
        }

        mvprintw(player->location.y, player->location.x, "%s", PLAYER_S);
        if (*(*(maze->maze + player->location.y) + player->location.x) == COIN_I) {
            collected_coins++;
            *(*(maze->maze + player->location.y) + player->location.x) = SPACE_I;
        }
        if (collected_coins == level_coins) {
            collected_coins = 0;

            wclear(stdscr);

            if (maze_width >= 21) {
                game_state = END;
                break;
            }

            free_maze(maze);
            free(player);

            level++;
            maze_width += 2;
            maze_height += 2;

            if (level % 2 == 1) {
                level_coins++;
            }

            maze = init_maze(maze_width, maze_height);
            create_maze(maze, 1, 1);
            repair_maze(maze);
            generate_coins(maze, level_coins);
            print_maze(maze);
            player = init_player(1, 1);
        }

        refresh();
    }

    free_maze(maze);
    free(player);
    endwin();
    return 0;
}