#include <ncurses.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define BUFFER 128
#define MAX_INGREDIENTES 8
#define BUFFER_INGREDIENTES 32
#define SUCESSO 1
#define ERRO -1
#define COLOR_BROWN 8
#define SELECIONADO 1
#define NAO_SELECIONADO 0

typedef struct pedido pedido;
typedef struct lista lista;
typedef struct knot knot;

struct pedido {
  char nome[BUFFER];
  char ingredientes[MAX_INGREDIENTES][BUFFER_INGREDIENTES];
  int numero_ingredientes;
  int tempo;
  int pontos;
};

struct knot {
  knot *prox;
  pedido info;
};

struct lista {
  knot *first;
  int size;
};

// variaveis globais
char report_erro[BUFFER];
lista *exemplo;
int gameover = 0;
pedido pedidos[8];
int mercado_selecionado = -1;
int numero_mercados = 3;


lista *cria_lista() {
  lista *head = (lista *)malloc(sizeof(lista));
  if (head == NULL) {
    strcpy(report_erro, "Erro ao criar a lista;\n");
    return NULL;
  }
  head->first = NULL;
  head->size = 0;
  return head;
}

int remove_lista(lista *head, char *nome) {
  knot *atual = head->first;
  knot *anterior = NULL;
  if (atual == NULL) {
    strcpy(report_erro, "Tentando remover de uma lista vazia!\n");
    return ERRO;
  }

  while (atual != NULL && strcmp(atual->info.nome, nome)) {
    anterior = atual;
    atual = atual->prox;
  }

  if (atual == NULL) {
    strcpy(report_erro, "Elemento a ser removido não foi encontrado!\n");
    return ERRO;
  }
  if (atual == head->first) // caso seja o primeiro elemento
    head->first = atual->prox;
  else
    anterior->prox = atual->prox;

  free(atual);
  head->size--;
  return SUCESSO;
}

void remove_primeiro(lista *head) {
  if (head->first != NULL)
    remove_lista(head, head->first->info.nome);
}

int insere_lista(lista *head, pedido info) {
  knot *novo = (knot *)malloc(sizeof(knot));
  if (novo == NULL) {
    strcpy(report_erro, "Erro ao inserir elemento na lista\n");
    return ERRO;
  }
  novo->info = info;
  novo->prox = NULL;

  if (head->first == NULL || head->first->info.tempo >= info.tempo) {
    novo->prox = head->first;
    head->first = novo;
  } else {
    knot *atual = head->first;
    while (atual->prox != NULL && atual->prox->info.tempo < info.tempo) {
      atual = atual->prox;
    }
    novo->prox = atual->prox;
    atual->prox = novo;
  }
  head->size++;
  return SUCESSO;
}

void imprime_pedido_ncurses(int y, int x, pedido p, WINDOW *offscreen) {

    const int largura_caixa = 32; // comprimento da linha horizontal
    int altura_caixa = 6 + p.numero_ingredientes; // altura inicial + número de ingredientes

    mvwprintw(offscreen, y, x, "+");
    for (int i = 0; i < largura_caixa - 2; i++) {
        mvwprintw(offscreen, y, x + 1 + i, "-");
    }
    mvwprintw(offscreen, y, x + largura_caixa - 1, "+");

    mvwprintw(offscreen, y + 1, x, "| Nome: %-23s|", p.nome);

    if (p.tempo <= 5) {
        mvwprintw(offscreen, y + 2, x, "|");
        wattron(offscreen, COLOR_PAIR(5));
        mvwprintw(offscreen, y + 2, x + 2, "Tempo: %-22d", p.tempo);
        wattroff(offscreen, COLOR_PAIR(5));
        mvwprintw(offscreen, y + 2, x + 30, " |");
    } else {
        mvwprintw(offscreen, y + 2, x, "| Tempo: %-22d|", p.tempo);
    }

    mvwprintw(offscreen, y + 3, x, "| Pontos: %-21d|", p.pontos);

    mvwprintw(offscreen, y + 4, x, "| Ingredientes:                |");

    for (int i = 0; i < p.numero_ingredientes; i++) {
        mvwprintw(offscreen, y + 5 + i, x, "|  %-28s|", p.ingredientes[i]);
    }

    mvwprintw(offscreen, y + altura_caixa - 1, x, "+");
    for (int i = 0; i < largura_caixa - 2; i++) {
        mvwprintw(offscreen, y + altura_caixa - 1, x + 1 + i, "-");
    }
    mvwprintw(offscreen, y + altura_caixa - 1, x + largura_caixa - 1, "+");
}



void desenha_bancada(int y, int x, int num, WINDOW *offscreen) {

  mvwprintw(offscreen, y, x,     "+----------+");
  mvwprintw(offscreen, y + 1, x, "|          |");
  mvwprintw(offscreen, y + 2, x, "|          |");
  mvwprintw(offscreen, y + 3, x, "|          |");
  mvwprintw(offscreen, y + 4, x, "+----------+");

  mvwprintw(offscreen, y + 2, x + 5, "%d", num);
}

void desenha_mercado(int y, int x, int num, WINDOW *offscreen, int selecionado) {

  init_color(COLOR_BROWN, 1000, 600, 0);
  init_pair(6, COLOR_BROWN, COLOR_BLACK);
  init_pair(7, COLOR_GREEN, COLOR_BLACK);

  if(selecionado == SELECIONADO)
  {
    wattron(offscreen, COLOR_PAIR(7));
    mvwprintw(offscreen, y, x,     "+----------+");
    mvwprintw(offscreen, y - 1, x, "|          |");
    mvwprintw(offscreen, y - 2, x, "|          |");
    mvwprintw(offscreen, y - 3, x, "|          |");
    mvwprintw(offscreen, y - 4, x, "+----------+");
    wattroff(offscreen, COLOR_PAIR(7));
  }
  else
  {
    wattron(offscreen, COLOR_PAIR(6));
    mvwprintw(offscreen, y, x,     "+----------+");
    mvwprintw(offscreen, y - 1, x, "|          |");
    mvwprintw(offscreen, y - 2, x, "|          |");
    mvwprintw(offscreen, y - 3, x, "|          |");
    mvwprintw(offscreen, y - 4, x, "+----------+");
    wattroff(offscreen, COLOR_PAIR(6));
  }

  mvwprintw(offscreen, y - 2, x + 5, "%d", num);
}

void imprime_lista_ncurses(lista *head) {

  WINDOW *offscreen = newwin(LINES, COLS, 0, 0); // Cria uma janela fora da tela

  if (offscreen == NULL) {
    endwin();
    strcpy(report_erro, "Erro ao criar janela offscreen.");
    exit(1);
  }

  init_pair(5, COLOR_RED, COLOR_BLACK);

  werase(offscreen); // Limpa a janela offscreen

  int pedidos_y = 2;
  int pedidos_x = 2;

  int bancada_y = 20;
  int bancada_x = COLS - 13;
  int bancada_num = 1; // Conteúdo escrito na bancada, também da para adptar ao tipo string

  int mercado_y = LINES - 3;
  int mercado_x = 75;

  knot *atual = head->first;
  while (atual != NULL) {
    imprime_pedido_ncurses(pedidos_y, pedidos_x, atual->info, offscreen);
    pedidos_x += 34;
    atual = atual->prox;
  }

  desenha_bancada(bancada_y, bancada_x, 1, offscreen);
  desenha_bancada(bancada_y + 6, bancada_x, 2, offscreen); // TODO dificuldade e quantidade de jogadores definem quantas bancadas de entrega
  // desenha_bancada(bancada_y + 12, bancada_x, 3, offscreen);
  // desenha_bancada(bancada_y + 18, bancada_x, 4, offscreen);


  for(int i = 0; i < numero_mercados; i++)
  {
    if(i == mercado_selecionado)
      desenha_mercado(mercado_y, mercado_x, i, offscreen, SELECIONADO);
    else
      desenha_mercado(mercado_y, mercado_x, i, offscreen, NAO_SELECIONADO);
    mercado_x += 15;

  }

  mvwprintw(offscreen, 1, 1, "Pedidos:");
  mvwprintw(offscreen, bancada_y - 1, bancada_x - 1, "Entregas:");
  wattron(offscreen, COLOR_PAIR(5));
  mvwprintw(offscreen, LINES - 1, 0, "Encerrar -> 'q'; Remover -> 'r'; Mercado -> 'm'");
  wattroff(offscreen, COLOR_PAIR(5));

  copywin(offscreen, stdscr, 0, 0, 0, 0, LINES - 1, COLS - 1, FALSE);
  refresh();
  delwin(offscreen);
}

void libera_lista(lista *head) {
  knot *atual = head->first;
  while (atual != NULL) {
    knot *temp = atual;
    atual = atual->prox;
    free(temp);
  }
  free(head);
}

void decrementa_tempo_pedidos() {
  knot *atual = exemplo->first;
  while (atual != NULL) {
    atual->info.tempo--;
    if (atual->info.tempo == 0)
      gameover = 1;
    atual = atual->prox;
  }
}

void *temporizador() {
  while (1) {
    if (gameover == 1)
      exit(1);
    sleep(1);
    decrementa_tempo_pedidos();
    imprime_lista_ncurses(exemplo);
  }
}

void gera_pedidos() {
  strcpy(pedidos[0].nome, "Bife Acebolado c Fritas");
  pedidos[0].tempo = 30;
  pedidos[0].pontos = 12;
  pedidos[0].numero_ingredientes = 4;
  strcpy(pedidos[0].ingredientes[0], "Carne");
  strcpy(pedidos[0].ingredientes[1], "Manteiga");
  strcpy(pedidos[0].ingredientes[2], "Batata");
  strcpy(pedidos[0].ingredientes[3], "Oléo");

  strcpy(pedidos[1].nome, "Pão de Queijo");
  pedidos[1].tempo = 30;
  pedidos[1].pontos = 12;
  pedidos[1].numero_ingredientes = 4;
  strcpy(pedidos[1].ingredientes[0], "Ovo");
  strcpy(pedidos[1].ingredientes[1], "Polvilho");
  strcpy(pedidos[1].ingredientes[2], "Leite");
  strcpy(pedidos[1].ingredientes[3], "Oléo");

  strcpy(pedidos[2].nome, "Isca de Peixe");
  pedidos[2].tempo = 30;
  pedidos[2].pontos = 9;
  pedidos[2].numero_ingredientes = 3;
  strcpy(pedidos[2].ingredientes[0], "Peixe");
  strcpy(pedidos[2].ingredientes[1], "Oléo");
  strcpy(pedidos[2].ingredientes[2], "Farinha");

  strcpy(pedidos[3].nome, "Misto Quente");
  pedidos[3].numero_ingredientes = 3;
  pedidos[3].tempo = 30;
  pedidos[3].pontos = 9;
  strcpy(pedidos[3].ingredientes[0], "Pão");
  strcpy(pedidos[3].ingredientes[1], "Presunto");
  strcpy(pedidos[3].ingredientes[2], "Queijo");
}

void pega_comandos()
{
  int ch;
  while ((ch = getch()) != 'q') {
    if (ch == 'r') {
      remove_primeiro(exemplo);
      imprime_lista_ncurses(exemplo);
    }
    else if(ch == 'm')
    {
      mercado_selecionado = 0;
      while((ch = getch()) != 'q')
      {
        if(ch == KEY_RIGHT && mercado_selecionado < numero_mercados - 1)
          mercado_selecionado += 1;
        else if(ch == KEY_LEFT && mercado_selecionado > 0 )
          mercado_selecionado -= 1;
      }
      ch = '.'; // Tecla qualquer para não finalizar o programa
      mercado_selecionado = -1; // Nenhum mercado selecionado
    }
  }
}

int s() {
  pthread_t timer;
  exemplo = cria_lista();
  pthread_create(&timer, NULL, temporizador, NULL);

  if (exemplo == NULL) {
    strcpy(report_erro, "Erro ao criar a lista\n");
    return ERRO;
  }

  gera_pedidos();

  for (int i = 0; i < 4; i++)
    insere_lista(exemplo, pedidos[i]);

  initscr();
  start_color();
  raw();
  noecho();
  keypad(stdscr, TRUE);

  refresh();

  imprime_lista_ncurses(exemplo);

  pega_comandos();


  endwin();
  libera_lista(exemplo);
  return 0;
}
