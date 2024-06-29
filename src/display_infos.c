#include "../inc/display_infos.h"
#include "../inc/game.h"
#include <locale.h>
#include <ncurses.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

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
  curs_set(FALSE);
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

/// Gerencia o tempo da partida
void *match_clock(void *arg)
{
  List_t *orders_list = (List_t *)arg;
  while(1)
  {
    sleep(1);
    orders_list->time_left--;
  }
}

/// Exibe os pedidos na interface
void display_orders(int y, int x, Order_t p, WINDOW *offscreen) {
    const int largura_caixa = 32; // comprimento da linha horizontal
    int altura_caixa = 7; // altura inicial

    // Desenha a linha horizontal superior
    mvwprintw(offscreen, y, x, "+");
    for (int i = 0; i < largura_caixa - 2; i++) {
        mvwprintw(offscreen, y, x + 1 + i, "-");
    }
    mvwprintw(offscreen, y, x + largura_caixa - 1, "+");

    // Desenha o conteúdo da caixa
    mvwprintw(offscreen, y + 1, x, "| Nome: %-23s|", p.name);
    mvwprintw(offscreen, y + 2, x, "| Tempo cozinhar: %-13d|", p.cook_time);
    mvwprintw(offscreen, y + 3, x, "| Tempo ingredientes: %-9d|", p.ingredients_time);
    mvwprintw(offscreen, y + 4, x, "| Pontos: %-21d|", p.points);

    // Desenha a linha horizontal inferior
    mvwprintw(offscreen, y + altura_caixa - 2, x, "+");
    for (int i = 0; i < largura_caixa - 2; i++) {
        mvwprintw(offscreen, y + altura_caixa - 2, x + 1 + i, "-");
    }
    mvwprintw(offscreen, y + altura_caixa - 2, x + largura_caixa - 1, "+");
}

/// Exibe as informações da partida na interface
void display_match_info(int y, int x, int time_left, WINDOW *offscreen)
{
  char time_left_str[20];
  snprintf(time_left_str, sizeof(time_left_str), "Tempo: %ds", time_left);
  mvwprintw(offscreen, y, x + 2, time_left_str);
  snprintf(time_left_str, sizeof(time_left_str), "Score: %d", score);
  mvwprintw(offscreen, y+1, x + 2, time_left_str);
}

/// Exibe as bancadas na interface
void display_bench(int y, int x, char* status, WINDOW *offscreen) {
  int len = strlen(status);
  int start_pos = (x + 5) - len/2;

  mvwprintw(offscreen, y, x,     "+----------+");
  mvwprintw(offscreen, y + 1, x, "|          |");
  mvwprintw(offscreen, y + 2, x, "|          |");
  mvwprintw(offscreen, y + 3, x, "|          |");
  mvwprintw(offscreen, y + 4, x, "+----------+");

  mvwprintw(offscreen, y + 2, start_pos, "%s", status);
}


/// Exibe a interface gráfica para o usuário
void display_game(List_t *orders_list) {

  WINDOW *offscreen = newwin(LINES, COLS, 0, 0); // Cria uma janela fora da tela

  if (offscreen == NULL) {
    endwin();
    strcpy(report_error, "Erro ao criar janela offscreen.");
    exit(1);
  }

  init_pair(5, COLOR_RED, COLOR_BLACK);

  werase(offscreen); // Limpa a janela offscreen

  int orders_y = 2;
  int orders_x = 2;

  int bench_ingredients_y = 16;
  int bench_ingredients_x = 7;

  int bench_kitchen_y = 44;
  int bench_kitchen_x = 35;

  int time_x = 180;
  int time_y = 2;

  int mercado_y = LINES - 3;
  int mercado_x = 75;

  mvwprintw(offscreen, 1, 1, "Pedidos:");
  mvwprintw(offscreen, bench_kitchen_y, bench_ingredients_x + 15, "Cozinhas:");
  mvwprintw(offscreen, bench_ingredients_y - 2, bench_ingredients_x - 5, "Ingredientes:");

  pthread_mutex_lock(&ingredient_mutex);
  //REGIAO CRITICA
  for(int i = 0; i < benches_n; i++)
  {
    if(benches_ingredient[i].status == AVAILABLE)
      display_bench(bench_ingredients_y, bench_ingredients_x, "AV", offscreen);
    else if(benches_ingredient[i].status == IN_USE)
      display_bench(bench_ingredients_y, bench_ingredients_x, "IP", offscreen);
    bench_ingredients_y += 6;
  }
  //REGIAO CRITICA
  pthread_mutex_unlock(&ingredient_mutex);

  pthread_mutex_lock(&kitchen_mutex);
  //REGIAO CRITICA
  for(int i = 0; i < benches_n; i++)
  {
    if(benches_kitchen[i].status == AVAILABLE)
      display_bench(bench_kitchen_y, bench_kitchen_x, "AV", offscreen);
    else if(benches_kitchen[i].status == IN_USE)
      display_bench(bench_kitchen_y, bench_kitchen_x, "IP", offscreen);
    bench_kitchen_x += 15;
  }
  //REGIAO CRITICA
  pthread_mutex_unlock(&kitchen_mutex);

  pthread_mutex_lock(&order_mutex);
  //REGIAO CRITICA
  Node_t *atual = orders_list->head;
  for(int i = 0; i < orders_list->n_orders && i < 4; i++) {
    display_orders(orders_y, orders_x, atual->order, offscreen);

    orders_x += 34;
    atual = atual->next;
  }
  //REGIAO CRITICA
  pthread_mutex_unlock(&order_mutex);

  //REGIAO CRITICA
  pthread_mutex_lock(&info_mutex);
  display_match_info(time_y, time_x, orders_list->time_left, offscreen);
  pthread_mutex_unlock(&info_mutex);
  //REGIAO CRITICA

  copywin(offscreen, stdscr, 0, 0, 0, 0, LINES - 1, COLS - 1, FALSE);
  refresh();
  delwin(offscreen);
}


/// Finaliza o uso do ncurses e restaura o terminal ao estado normal
void end_program() {
  endwin();
}
