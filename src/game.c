#include "../inc/game.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>



// VARIAVEIS GLOBAIS
char report_error[BUFFER];
int game_time;

// VARIAVEIS GLOBAIS

/// Cria uma lista encadeada
List_t *create_list() {
  List_t *list = (List_t *)malloc(sizeof(List_t));

  if (list == NULL) {
    strcpy(report_error, "Erro ao criar a lista;\n");
    return NULL;
  }

  list->head = NULL;
  list->size = 0;

  return list;
}

/// Insere um pedido na lista encadeada
int insert_in_list(List_t *list, Order_t order) {
  Node_t *new_node = (Node_t *)malloc(sizeof(Node_t));

  if (new_node == NULL) {
    strcpy(report_error, "Erro ao alocar memória para o novo nó;\n");
    return ERRO;
  }

  new_node->order = order;
  new_node->next = NULL;

  if (list->head == NULL || list->head->order.time >= order.time) {
    new_node->next = list->head;
    list->head = new_node;
  } else {
    Node_t *current = list->head;

    while (current->next != NULL && current->next->order.time < order.time) {
      current = current->next;
    }

    new_node->next = current->next;
    current->next = new_node;
  }

  list->size++;

  return SUCESSO;
}

/// Remove um pedido da lista encadeada com base no nome
int remove_by_name(List_t *list, char *name) {
  Node_t *current = list->head;
  Node_t *previous = NULL;

  if (current == NULL) {
    strcpy(report_error, "Tentando remover de uma lista vazia!\n");
    return ERRO;
  }

  while (current != NULL && strcmp(current->order.name, name)) {
    previous = current;
    current = current->next;
  }

  if (current == NULL) {
    strcpy(report_error, "Elemento a ser removido não foi encontrado!\n");
    return ERRO;
  }

  if (current == list->head) {
    list->head = current->next;
  } else {
    previous->next = current->next;
  }

  free(current);

  list->size--;

  return SUCESSO;
}

/// Remove o primeiro pedido da lista encadeada
int remove_head(List_t *list) {
  if (list->head != NULL) {
    remove_by_name(list, list->head->order.name);
  }

  return SUCESSO;
}

/// Verifica se a lista encadeada está vazia
int is_empty(List_t *list) { return list->head == NULL; }

/// Tamanho da lista
int size(List_t *list) { return list->size; }

/// Imprime a lista encadeada
void print_list(List_t *list) {
  Node_t *current = list->head;

  if (current == NULL) {
    printf("Lista vazia.\n");
    return;
  }

  while (current != NULL) {
    printf("Nome: %s\n", current->order.name);
    printf("\n");
    printf("Tempo: %d\n", current->order.time);
    printf("Pontos: %d\n", current->order.points);
    printf("\n");
    current = current->next;
  }
}

/// Libera a memória alocada para a lista encadeada
void free_list(List_t *list) {
  Node_t *current = list->head;
  Node_t *next;

  while (current != NULL) {
    next = current->next;

    free(current);

    current = next;
  }

  free(list);
}

/// Cria um pedido de exemplo
Order_t create_sample_order(const char *name, int time, int points) {
  Order_t order;
  strncpy(order.name, name, BUFFER);
  order.time = time;
  order.points = points;
  return order;
}

void create_orders(List_t *orders_list) {

  Order_t orders[15];

  strcpy(orders[0].name, "Bife Acebolado c Fritas");
  orders[0].time = 20;
  orders[0].points = 15;

  strcpy(orders[1].name, "Pão de Queijo");
  orders[1].time = 25;
  orders[1].points = 10;

  strcpy(orders[2].name, "Isca de Peixe");
  orders[2].time = 15;
  orders[2].points = 12;

  strcpy(orders[3].name, "Misto Quente");
  orders[3].time = 10;
  orders[3].points = 7;

  strcpy(orders[4].name, "Feijoada");
  orders[4].time = 40;
  orders[4].points = 25;

  strcpy(orders[5].name, "Moqueca de Camarão");
  orders[5].time = 35;
  orders[5].points = 22;

  strcpy(orders[6].name, "Brigadeiro");
  orders[6].time = 20;
  orders[6].points = 10;

  strcpy(orders[7].name, "Escondidinho de Carne");
  orders[7].time = 30;
  orders[7].points = 18;

  strcpy(orders[8].name, "Tapioca");
  orders[8].time = 15;
  orders[8].points = 9;

  strcpy(orders[9].name, "Vatapá");
  orders[9].time = 25;
  orders[9].points = 20;

  strcpy(orders[10].name, "Coxinha");
  orders[10].time = 20;
  orders[10].points = 14;

  strcpy(orders[11].name, "Farofa");
  orders[11].time = 10;
  orders[11].points = 8;

  strcpy(orders[12].name, "Acarajé");
  orders[12].time = 30;
  orders[12].points = 18;

  strcpy(orders[13].name, "Bobó de Camarão");
  orders[13].time = 35;
  orders[13].points = 22;

  strcpy(orders[14].name, "Quindim");
  orders[14].time = 20;
  orders[14].points = 15;

  for(int i = 0; i < MAX_ORDERS; i++)
    insert_in_list(orders_list, orders[i]);

  
}
