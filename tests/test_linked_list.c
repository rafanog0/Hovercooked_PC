#include "../inc/game.h"
#include <stdio.h>
#include <stdlib.h>

// Função principal para testar as funcionalidades da lista encadeada
int test_linked_list() {
  // Cria uma lista encadeada
  List_t *list = create_list();
  if (list == NULL) {
    fprintf(stderr, "Erro ao criar a lista: %s\n", report_error);
    return EXIT_FAILURE;
  }

  // Insere pedidos de exemplo na lista
  Order_t order1 = create_sample_order("Pedido 1", 5, 10);
  Order_t order2 = create_sample_order("Pedido 2", 3, 8);
  Order_t order3 = create_sample_order("Pedido 3", 10, 15);

  insert_in_list(list, order1);
  insert_in_list(list, order2);
  insert_in_list(list, order3);

  // Imprime a lista de pedidos
  printf("Lista de pedidos após inserção:\n");
  print_list(list);

  // Remove um pedido pelo nome
  remove_by_name(list, "Pedido 2");

  // Imprime a lista de pedidos após remoção
  printf("\nLista de pedidos após remover 'Pedido 2':\n");
  print_list(list);

  // Remove o primeiro pedido da lista
  remove_head(list);

  // Imprime a lista de pedidos após remover o primeiro pedido
  printf("\nLista de pedidos após remover o primeiro pedido:\n");
  print_list(list);

  // Verifica se a lista está vazia
  if (is_empty(list)) {
    printf("\nA lista está vazia.\n");
  } else {
    printf("\nA lista não está vazia.\n");
  }

  // Imprime o tamanho da lista
  printf("\nTamanho da lista: %d\n", size(list));

  // Limpa a memória alocada
  free_list(list);

  return EXIT_SUCCESS;
}
