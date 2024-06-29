#ifndef DISPLAY_INFOS_H
#define DISPLAY_INFOS_H

#include "game.h"
#include<ncurses.h>

#define MAX_TITLE_LINES 10
#define MAX_TITLE_LENGTH 100
#define TITLE_FILE "assets/title.txt"

typedef struct choice_t Choice_t;

void initialize_screen();
int display_menu();
void end_program();
void load_title(char title[][MAX_TITLE_LENGTH], int *num_lines);
void display_game(List_t *orders_list);
void display_orders(int y, int x, Order_t p, WINDOW *offscreen);
void display_match_info(int y, int x, int time_left, WINDOW *offscreen);
void *match_clock(void *orders_list);

#endif // DISPLAY_INFOS_H