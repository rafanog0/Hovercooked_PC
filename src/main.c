#include "../inc/display_infos.h"
#include "../inc/game.h"
#include <pthread.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>



int main() {

  initialize_screen();
  int choice = display_menu();
  pthread_t timer;
  pthread_t pedidos;
  pthread_mutex_init(&order_mutex, NULL);
  pthread_mutex_init(&ingredient_mutex, NULL);
  pthread_mutex_init(&kitchen_mutex, NULL);
  pthread_mutex_init(&info_mutex, NULL);
  List_t *orders_list = (List_t*) malloc(sizeof(List_t));

  if(choice == 0)
  {
    orders_list->time_left = 100;
    orders_list->create_order_time = 7;
    benches_ingredient = (prep_bench*) malloc(2 * sizeof(prep_bench)); // TODO: número de bancadas relativo a dificuldade
    benches_kitchen = (prep_bench*) malloc(2 * sizeof(prep_bench)); // TODO: número de bancadas relativo a dificuldade
  }
  else if(choice == 1) // TODO: alterar para ser compativel com a estrtura choice_t
  {
    orders_list->time_left = 200;
    orders_list->create_order_time = 14;
    benches_ingredient = (prep_bench*) malloc(2 * sizeof(prep_bench));
    benches_kitchen = (prep_bench*) malloc(2 * sizeof(prep_bench));
  }
  else if(choice == 2)
  {
    orders_list->time_left = 300;
    orders_list->create_order_time = 21;
    benches_ingredient = (prep_bench*) malloc(2 * sizeof(prep_bench));
    benches_kitchen = (prep_bench*) malloc(2 * sizeof(prep_bench));
  }

  pthread_create(&timer, NULL, match_clock, (void *)orders_list);

  pthread_create(&pedidos, NULL, create_orders, (void *)orders_list);

  while(orders_list->time_left > 0)
    display_game(orders_list);

  pthread_mutex_destroy(&order_mutex);
  pthread_mutex_destroy(&ingredient_mutex);
  pthread_mutex_destroy(&kitchen_mutex);
  pthread_mutex_destroy(&info_mutex);
  end_program();
  return 0;
}
