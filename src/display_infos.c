#include "../inc/display_infos.h"
#include <locale.h>
#include <ncurses.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/// Estrutura que representa uma escolha do menu, com título e tempo
struct choice_t {
  char title[MAX_TITLE_LENGTH];
  int time;
};

/// Inicializa a tela ncurses e configurações de entrada
void initialize_screen() {
  setlocale(LC_ALL, "");
  initscr();
  start_color();
  init_pair(1, COLOR_RED, COLOR_BLACK);
  noecho();
  cbreak();
  keypad(stdscr, TRUE);
}

/// Exibe o menu e retorna a escolha do usuário
int display_menu() {
  int highlight = 1;
  int choice = 0;
  int c;
  int num_title_lines = 0;
  char title[MAX_TITLE_LINES][MAX_TITLE_LENGTH];

  load_title(title, &num_title_lines);

  Choice_t choices[] = {
    {"Facil", 5},
    {"Medio", 10},
    {"Dificil", 15},
    {"Sair", 0}
  };

  int n_choices = sizeof(choices) / sizeof(Choice_t);

  while (1) {
    clear();
    int start_y = (LINES - num_title_lines - (n_choices - 1) * 2 - 1) / 2;
    for (int i = 0; i < num_title_lines; ++i) {
      attron(COLOR_PAIR(1) | A_BOLD);
      mvprintw(start_y + i, (COLS - strlen(title[i])) / 2, "%s", title[i]);
      attroff(COLOR_PAIR(1) | A_BOLD);
    }
    for (int i = 0; i < n_choices; ++i) {
      if (highlight == i + 1) {
	attron(A_REVERSE);
	mvprintw(start_y + num_title_lines + 2 + i * 2,
		 (COLS - strlen(choices[i].title)) / 2, "%s", choices[i].title);
	attroff(A_REVERSE);
      } else {
	mvprintw(start_y + num_title_lines + 2 + i * 2,
		 (COLS - strlen(choices[i].title)) / 2, "%s", choices[i].title);
      }
    }
    c = getch();
    switch (c) {
    case KEY_UP:
      if (highlight == 1)
	highlight = n_choices;
      else
	--highlight;
      break;
    case KEY_DOWN:
      if (highlight == n_choices)
	highlight = 1;
      else
	++highlight;
      break;
    case 10: // Enter
      choice = highlight;
      break;
    default:
      break;
    }
    if (choice != 0) {
      break;
    }
  }

  return choice - 1;
}

/// Carrega o título do arquivo e armazena nas linhas de título
void load_title(char title[][MAX_TITLE_LENGTH], int *num_lines) {
  FILE *file = fopen(TITLE_FILE, "r");
  if (!file) {
    perror("Erro ao abrir o arquivo de título");
    exit(EXIT_FAILURE);
  }

  char line[MAX_TITLE_LENGTH];
  *num_lines = 0;
  while (fgets(line, sizeof(line), file) && *num_lines < MAX_TITLE_LINES) {
    strcpy(title[*num_lines], line);
    title[*num_lines][strcspn(title[*num_lines], "\n")] = '\0';
    (*num_lines)++;
  }

  fclose(file);
}

/// Finaliza o uso do ncurses e restaura o terminal ao estado normal
void end_program() {
  endwin();
}
