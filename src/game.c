#include "../inc/game.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char report_error[BUFFER];

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
int is_empty(List_t *list) {
  return list->head == NULL;
}

/// Tamanho da lista
int size(List_t *list) {
  return list->size;
}

/// Imprime a lista encadeada
void print_list(List_t *list) {
  Node_t *current = list->head;

  if (current == NULL) {
    printf("Lista vazia.\n");
    return;
  }

  while (current != NULL) {
    printf("Nome: %s\n", current->order.name);
    printf("Ingredientes: ");
    for (int i = 0; i < current->order.ingredients_quantity; i++) {
      printf("%s ", current->order.ingredients[i]);
    }
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
  order.ingredients_quantity = 2;
  strncpy(order.ingredients[0], "Ingredient1", BUFFER_INGREDIENTES);
  strncpy(order.ingredients[1], "Ingredient2", BUFFER_INGREDIENTES);
  order.time = time;
  order.points = points;
  return order;
}
