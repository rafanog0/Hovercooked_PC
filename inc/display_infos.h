#ifndef DISPLAY_INFOS_H
#define DISPLAY_INFOS_H

#define MAX_TITLE_LINES 10
#define MAX_TITLE_LENGTH 100
#define TITLE_FILE "assets/title.txt"

typedef struct choice_t Choice_t;

void initialize_screen();
int display_menu();
void end_program();
void load_title(char title[][MAX_TITLE_LENGTH], int *num_lines);

#endif // DISPLAY_INFOS_H

