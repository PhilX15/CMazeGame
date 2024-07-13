#include <ncurses.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <string.h>

#include "maze.h"
#include "utils.h"
#include "player.h"

int level = 0;
const int max_level = 5;
int maze_width = 13;
int maze_height = 13;

int level_coins = 5;
int collected_coins = 0;

enum game_state_t game_state = MENU;
struct maze_t *maze = NULL;
struct player_t *player = NULL;

int scr_width = 0;
int scr_height = 0;

const char *logo[6] = {
    "___  ___                _____                      ",
    "|  \\/  |               |  __ \\                     ",
    "| .  . | __ _ _______  | |  \\/ __ _ _ __ ___   ___ ",
    "| |\\/| |/ _` |_  / _ \\ | | __ / _` | '_ ` _ \\ / _ \\",
    "| |  | | (_| |/ |  __/ | |_\\ | (_| | | | | | |  __/",
    "\\_|  |_/\\__,_/___\\___|  \\____/\\__,_|_| |_| |_|\\___|"
};

const char *play_text[2] = {
    "[ PLAY ]",
    "  PLAY  "
};

const char *exit_text[2] = {
    "[ EXIT ]",
    "  EXIT  "
};

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

    collected_coins = 0;
    level++;
    maze_width += 2;
    maze_height += 2;

    if (level % 2 == 0) {
        level_coins++;
    }

    if (level > max_level) {
        game_state = END;
        return;
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
    mvprintw(scr_height - 2, scr_width / 2 - strlen("Press 'q' to exit") / 2, "Press 'q' to exit");
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

    getmaxyx(stdscr, scr_height, scr_width);

    int option = 0;
    
    while (game_state != END) {
        int last_width = scr_width;
        int last_height = scr_height;
        getmaxyx(stdscr, scr_height, scr_width);
        if (last_width != scr_width || last_height != scr_height) {
            wclear(stdscr);
        }
        int key = -1;
        switch (game_state) {
            case MENU:
                for (int i = 0; i < 6 && scr_width > strlen(*(logo + i)) && scr_height > 20; i++) {
                    mvprintw(i + 2, scr_width / 2 - strlen(*(logo + i)) / 2, "%s", *(logo + i));
                }
                box(stdscr, 0, 0);
                if (option == 0) {
                    mvprintw(scr_height / 2 - 1, scr_width / 2 - strlen(*play_text) / 2, "%s", *play_text);
                    mvprintw(scr_height / 2 + 1, scr_width / 2 - strlen(*exit_text) / 2, "%s",*(exit_text + 1));
                } else {
                    mvprintw(scr_height / 2 - 1, scr_width / 2 - strlen(*play_text) / 2, "%s", *(play_text + 1));
                    mvprintw(scr_height / 2 + 1, scr_width / 2 - strlen(*exit_text) / 2, "%s", *exit_text);
                }

                key = wgetch(stdscr);

                switch (key) {
                    case KEY_DOWN:
                        option = 1;
                        break;
                    case KEY_UP:
                        option = 0;
                        break;
                    case '\n':
                        if (option == 0) {
                            game_state = GAME;
                            init_level();
                        } else {
                            game_state = END;
                        }
                        break;
                }
                break;
            case GAME:
                print_hud();
                print_maze(maze, scr_width, scr_height);
                mvprintw(maze->top_left_y + player->location.y, maze->top_left_x + player->location.x, "%s", PLAYER_S);
                box(stdscr, 0, 0);

                key = wgetch(stdscr);
                if (key == 'q') {
                    game_state = END;
                    break;
                }
                move_player(maze, player, key);

                if (*(*(maze->maze + player->location.y) + player->location.x) == COIN_I) {
                    collected_coins++;
                    *(*(maze->maze + player->location.y) + player->location.x) = SPACE_I;
                }
                if (collected_coins == level_coins) {
                    init_level();
                }
        }

        refresh();
    }

    endwin();
    return 0;
}