#ifndef GAME_H
#define GAME_H

#define BUFFER 128
#define SUCESSO 1
#define ERRO -1
#define COLOR_BROWN 8
#define SELECIONADO 1
#define NAO_SELECIONADO 0
#define EXIT_SUCCESS 0
#define EXIT_FAILURE 1
#define MAX_ORDERS 15

/// Estrutura que representa um pedido
typedef struct order_t {
  char name[BUFFER];
  int time;
  int points;
} Order_t;

/// Estrutura que representa um nó da lista encadeada
typedef struct node_t {
  Order_t order;
  struct node_t *next;
} Node_t;

/// Estrutura que representa uma lista encadeada
typedef struct list_t {
  Node_t *head;
  int time_left;
  int size;
} List_t;

extern char report_error[BUFFER];

List_t *create_list();
int insert_in_list(List_t *list, Order_t order);
int remove_by_name(List_t *list, char *name);
int remove_head(List_t *list);
int is_empty(List_t *list);
int size(List_t *list);
void print_list(List_t *list);
void free_list(List_t *list);
Order_t create_sample_order(const char *name, int time, int points);

#endif // GAME_H
