#ifndef GAME_H
#define GAME_H

#include<pthread.h>

#define BUFFER 128

#define SUCESSO 1
#define ERRO -1

#define COLOR_BROWN 8

#define SELECIONADO 1
#define NAO_SELECIONADO 0

#define EXIT_SUCCESS 0
#define EXIT_FAILURE 1

#define MAX_ORDERS 15

#define AVAILABLE 0
#define IN_USE 1

#define NO_ORDERS 0

#define NOT_FOUND 404

/// Estrutura que representa um pedido
typedef struct order_t {
  char name[BUFFER];
  int ingredients_time;
  int cook_time;
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
  int create_order_time;
  int n_orders;
  int size;
} List_t;

typedef struct prep_bench{
  char order_in[BUFFER];
  int status;
}prep_bench;

typedef struct cook{
  char current_order[BUFFER];
  int order_status;
}cook;

extern char report_error[BUFFER];
extern int game_time;
extern int score;
extern int benches_n;
extern int cooks_n;
extern pthread_mutex_t order_mutex;
extern pthread_mutex_t ingredient_mutex;
extern pthread_mutex_t kitchen_mutex;
extern pthread_mutex_t info_mutex;
extern prep_bench* benches_ingredient;
extern prep_bench* benches_kitchen;
extern cook* cooks;


List_t *create_list();
void *create_orders(void *arg);
void *cooking(void **arg);
int insert_in_list(List_t *list, Order_t order);
int remove_by_name(List_t *list, char *name);
int remove_head(List_t *list);
int is_empty(List_t *list);
int size(List_t *list);
void print_list(List_t *list);
void free_list(List_t *list);
Order_t create_sample_order(const char *name, int time, int points);

#endif // GAME_H
