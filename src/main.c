#include <ncurses.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define BUFFER 128
#define SUCESSO 1
#define ERRO -1

typedef struct pedido pedido;
typedef struct lista lista;
typedef struct knot knot;

struct pedido {
  char nome[BUFFER];
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

char report_erro[BUFFER];
lista *exemplo;
int gameover = 0;

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

void imprime_pedido_ncurses(int y, int x, pedido p) {
  mvprintw(y, x, "+------------------------------+");
  mvprintw(y + 1, x, "| Nome: %-23s |", p.nome);
  mvprintw(y + 2, x, "| Tempo: %-22d |", p.tempo);
  mvprintw(y + 3, x, "| Pontos: %-21d |", p.pontos);
  mvprintw(y + 4, x, "+------------------------------+");
}


void desenha_bancada(int y, int x, int num) {
    mvprintw(y, x,     "+----------+");
    mvprintw(y + 1, x, "|          |");
    mvprintw(y + 2, x, "|          |");
    mvprintw(y + 3, x, "|          |");
    mvprintw(y + 4, x, "+----------+");
    
    mvprintw(y + 2, x + 5, "%d", num);
}


void imprime_lista_ncurses(lista *head) {
    init_pair(5, COLOR_RED, COLOR_BLACK);
    clear();

    int pedidos_y = 1; 
    int pedidos_x = 1; 

    int bancada_y = 10;  
    int bancada_x = COLS - 13;  
    int bancada_num = 1; // Conteúdo escrito na bancada, também da para adptar ao tipo string

    knot *atual = head->first;
    while (atual != NULL) {
        imprime_pedido_ncurses(pedidos_y, pedidos_x, atual->info);
        pedidos_x += 34;
        atual = atual->prox;
    }

    desenha_bancada(bancada_y, bancada_x, 1);
    desenha_bancada(bancada_y + 6, bancada_x, 2);

    attron(COLOR_PAIR(5));
    mvprintw(LINES - 1, 0, "Encerrar -> 'q'; Remover -> 'r'");
    attroff(COLOR_PAIR(5));

    refresh();
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

int main() {
  pthread_t timer;
  exemplo = cria_lista();
  pthread_create(&timer, NULL, temporizador, NULL);

  if (exemplo == NULL) {
    strcpy(report_erro, "Erro ao criar a lista\n");
    return ERRO;
  }

  pedido pedidos[4];
  strcpy(pedidos[0].nome, "Bife");
  pedidos[0].tempo = 25;
  pedidos[0].pontos = 10;

  strcpy(pedidos[1].nome, "Macarrão");
  pedidos[1].tempo = 17;
  pedidos[1].pontos = 8;

  strcpy(pedidos[2].nome, "Arroz");
  pedidos[2].tempo = 34;
  pedidos[2].pontos = 5;

  strcpy(pedidos[3].nome, "Feijoada");
  pedidos[3].tempo = 21;
  pedidos[3].pontos = 15;

  for (int i = 0; i < 4; i++)
    insere_lista(exemplo, pedidos[i]);

  initscr();
  start_color();
  raw();
  noecho();
  keypad(stdscr, TRUE);

  refresh();

  imprime_lista_ncurses(exemplo);

  int ch;
  while ((ch = getch()) != 'q') {
    if (ch == 'r') {
      remove_primeiro(exemplo);
      imprime_lista_ncurses(exemplo);
    }
  }

  endwin();
  libera_lista(exemplo);
  return 0;
}
