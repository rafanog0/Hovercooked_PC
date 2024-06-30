#ifndef DISPLAY_INFOS_H
#define DISPLAY_INFOS_H

#include "game.h"
#include <ncurses.h>

#define MAX_TITLE_LINES 10
#define MAX_TITLE_LENGTH 100
#define TITLE_FILE "assets/title.txt"
#define MAX_DISPLAYED_ORDERS 5
#define COOKS_MENU 0
#define ORDERS_MENU 1
#define EASY 1
#define MEDIUM 2
#define HARD 3
#define QUIT 4

extern int menu_n;
extern int highlight_manager;
extern int choice_manager;

typedef struct difficulty_t {
  char difficulty[MAX_TITLE_LENGTH];
  int difficulty_id;
  int time;
  int benches;
  int cooks;
} Difficulty_t;

void initialize_screen();
Difficulty_t display_menu();
void end_program();
void load_title(char title[][MAX_TITLE_LENGTH], int *num_lines);
void display_game(List_t *orders_list);
void display_orders(int y, int x, Order_t p, WINDOW *offscreen);
void display_match_info(int y, int x, int time_left, WINDOW *offscreen);
void *match_clock(void *orders_list);
void draw_center_box_with_orders(WINDOW *win, List_t* orders_list, int num_options);


#endif // DISPLAY_INFOS_H
