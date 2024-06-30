#include "../inc/display_infos.h"
#include "../inc/game.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>



// VARIAVEIS GLOBAIS
char report_error[BUFFER];
int game_time;
int score = 0;
int benches_n;
int cooks_n;
int cook_choice;
Order_t order_choice;


pthread_mutex_t order_mutex;
pthread_mutex_t ingredient_mutex;
pthread_mutex_t kitchen_mutex;
pthread_mutex_t info_mutex;
prep_bench* benches_ingredient;
prep_bench* benches_kitchen;
cook* cooks;
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

    if (list->head == NULL) {
        // Lista vazia, insere como primeiro elemento
        list->head = new_node;
    } else {
        // Percorre a lista até o último nó
        Node_t *current = list->head;
        while (current->next != NULL) {
            current = current->next;
        }
        // Insere no final da lista
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
    printf("Tempo: %d\n", current->order.cook_time);
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


/// Busca por um pedido especifico na lista de pedidos
Node_t *search_order_list(List_t *orders, char *name)
{
  Node_t *current = orders->head;
  while(current != NULL)
  {
    if(!strcmp(current->order.name, name))
      break;
    current = current->next;
  }
  return current;
}

/// Cria um pedido de exemplo
Order_t create_sample_order(const char *name, int time, int points) {
  Order_t order;
  strncpy(order.name, name, BUFFER);
  order.cook_time = time;
  order.points = points;
  return order;
}

/// Rotina que cria os pedidos e insere periodicamente, conforme a dificuldade
void *create_orders(void *arg) {

  List_t* orders_list = (List_t *) arg;
  Order_t orders[15];

  strcpy(orders[0].name, "Bife Acebolado c Fritas");
  orders[0].ingredients_time = 5;
  orders[0].cook_time = 5;
  orders[0].points = 15;

  strcpy(orders[1].name, "Pao de Queijo");
  orders[1].ingredients_time = 7;
  orders[1].cook_time = 7;
  orders[1].points = 10;

  strcpy(orders[2].name, "Isca de Peixe");
  orders[2].ingredients_time = 10;
  orders[2].cook_time = 10;
  orders[2].points = 120;

  strcpy(orders[3].name, "Misto Quente");
  orders[3].ingredients_time = 10;
  orders[3].cook_time = 10;
  orders[3].points = 7;

  strcpy(orders[4].name, "Feijoada");
  orders[4].ingredients_time = 40;
  orders[4].cook_time = 40;
  orders[4].points = 25;

  strcpy(orders[5].name, "Moqueca de Camarao");
  orders[5].ingredients_time = 35;
  orders[5].cook_time = 35;
  orders[5].points = 22;

  strcpy(orders[6].name, "Brigadeiro");
  orders[6].ingredients_time = 20;
  orders[6].cook_time = 20;
  orders[6].points = 10;

  strcpy(orders[7].name, "Escondidinho de Carne");
  orders[7].ingredients_time = 30;
  orders[7].cook_time = 30;
  orders[7].points = 18;

  strcpy(orders[8].name, "Tapioca");
  orders[8].ingredients_time = 15;
  orders[8].cook_time = 15;
  orders[8].points = 9;

  strcpy(orders[9].name, "Vatapa");
  orders[9].ingredients_time = 25;
  orders[9].cook_time = 25;
  orders[9].points = 20;

  strcpy(orders[10].name, "Coxinha");
  orders[10].ingredients_time = 20;
  orders[10].cook_time = 20;
  orders[10].points = 14;

  strcpy(orders[11].name, "Farofa");
  orders[11].ingredients_time = 10;
  orders[11].cook_time = 10;
  orders[11].points = 8;

  strcpy(orders[12].name, "Acarajé");
  orders[12].ingredients_time = 30;
  orders[12].cook_time = 30;
  orders[12].points = 18;

  strcpy(orders[13].name, "Bobo de Camarao");
  orders[13].ingredients_time = 35;
  orders[13].cook_time = 35;
  orders[13].points = 22;

  strcpy(orders[14].name, "Quindim");
  orders[14].ingredients_time = 20;
  orders[14].cook_time = 20;
  orders[14].points = 15;
  pthread_mutex_lock(&order_mutex);
  insert_in_list(orders_list, orders[0]);
  insert_in_list(orders_list, orders[1]);
  insert_in_list(orders_list, orders[2]);
  orders_list->size = 3;
  pthread_mutex_unlock(&order_mutex);

  for(int i = 3; i < MAX_ORDERS; i++)
  {
    sleep(orders_list->create_order_time);
    pthread_mutex_lock(&order_mutex);
    //REGIAO CRITICA
    orders_list->size++;
    insert_in_list(orders_list, orders[i]);
    //REGIAO CRITICA
    pthread_mutex_unlock(&order_mutex);
  }

  
}

// Busca por uma bancada de ingredientes disponivel para começar o pedido
int search_available_ingredient_bench()
{
  pthread_mutex_lock(&ingredient_mutex);
  for(int i = 0; i<benches_n; i++)
  {
    if(benches_ingredient[i].status == AVAILABLE)
    {
      benches_ingredient[i].status = IN_USE;
      pthread_mutex_unlock(&ingredient_mutex);
      return i;
    }
  }
  pthread_mutex_unlock(&ingredient_mutex);
  return NOT_FOUND;
}

// Busca por uma bancada de cozinhas disponivel para começar o pedido
int search_available_kitchen_bench()
{
  pthread_mutex_lock(&kitchen_mutex);
  for(int i = 0; i<benches_n; i++)
  {
    if(benches_kitchen[i].status == AVAILABLE)
    {
      benches_kitchen[i].status = IN_USE;
      pthread_mutex_unlock(&kitchen_mutex);
      return i;
    }
  }
  pthread_mutex_unlock(&kitchen_mutex);
  return NOT_FOUND;
}


/// Rotina que permite um cozinheiro cozinhar
void *cooking(void** args)
{
  int ingredient_bench = NOT_FOUND;
  int kitchen_bench = NOT_FOUND;
  List_t* orders_list = (List_t *) args[0];
  int cook_id = *(int*)args[1];
  
  Node_t *node_to_be_cooked = search_order_list(orders_list, cooks[cook_id].current_order);
  if(node_to_be_cooked == NULL)
    strcpy(report_error, "Order doesnt exist\n");
  
  Order_t to_be_cooked = node_to_be_cooked->order;

  while(1)
  {
    ingredient_bench = search_available_ingredient_bench(); // Se não tiver bancadas disponiveis, aguarda e tenta novamente
    if(ingredient_bench != NOT_FOUND) break;
    sleep(5);
  }

  sleep(to_be_cooked.ingredients_time); // Tempo para preparar os ingredientes

  pthread_mutex_lock(&ingredient_mutex);
  benches_ingredient[ingredient_bench].status = AVAILABLE;
  pthread_mutex_unlock(&ingredient_mutex);
  
  while(1)
  {
    kitchen_bench = search_available_kitchen_bench(); // Se não tiver bancadas disponiveis, aguarda e tenta novamente
    if(kitchen_bench != NOT_FOUND) break;
    sleep(5);
  }

  sleep(to_be_cooked.cook_time); // Tempo para cozinhar

  pthread_mutex_lock(&kitchen_mutex);
  benches_kitchen[kitchen_bench].status = AVAILABLE;
  pthread_mutex_unlock(&kitchen_mutex);

  pthread_mutex_lock(&info_mutex);
  score += to_be_cooked.points;
  pthread_mutex_unlock(&info_mutex);
  
  pthread_mutex_lock(&order_mutex);
  int ret = remove_by_name(orders_list, to_be_cooked.name);
  pthread_mutex_unlock(&order_mutex);

}


void print_menu(WINDOW *menu_win, int highlight, char *choices[], int n_choices) {
    int x, y, i;
    
    x = 2;
    y = 2;
    box(menu_win, 0, 0);
    for(i = 0; i < n_choices; ++i) {
        if(highlight == i + 1) {
            wattron(menu_win, A_REVERSE);
            mvwprintw(menu_win, y, x, "%s", choices[i]);
            wattroff(menu_win, A_REVERSE);
        } else
            mvwprintw(menu_win, y, x, "%s", choices[i]);
        ++y;
    }
    wrefresh(menu_win);
}

void *managing(void *arg) {
    // Inicializa a janela e outras variáveis

    char c;
    
    while(1) {
        c = getch();
        if(menu_n == ORDERS_MENU)
        {
          switch(c) {
              case '1':
                  highlight_manager = 1;
                  break;
              case '2':
                  highlight_manager = 2;
                  break;
              case '3':
                  highlight_manager = 3;
                  break;
              case '4':
                  highlight_manager = 3;
                  break;
              case '5':
                  highlight_manager = 3;
                  break;
              case 10: // Enter key
                  choice_manager = highlight_manager;
                  menu_n = COOKS_MENU;
                  break;
              default:
                  // refresh();
                  break;
          }
        }
        else
          switch(c) {
              case '1':
                  highlight_manager = 1;
                  break;
              case '2':
                  highlight_manager = 2;
                  break;
              case '3':
                  highlight_manager = 3;
                  break;
              case 10: // Enter key
                  choice_manager = highlight_manager;
                  cook_choice = highlight_manager - 1;
                  menu_n = ORDERS_MENU;

                  break;
              default:
                  // refresh();
                  break;
          }
        {

        }
    }
}
