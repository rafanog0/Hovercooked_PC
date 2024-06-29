#include "../inc/display_infos.h"
#include "../inc/game.h"
#include <pthread.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>



int main() {

  initialize_screen();
  int choice = display_menu();
  pthread_t timer;
  pthread_t orders;
  pthread_t *chef;
  pthread_mutex_init(&order_mutex, NULL);
  pthread_mutex_init(&ingredient_mutex, NULL);
  pthread_mutex_init(&kitchen_mutex, NULL);
  pthread_mutex_init(&info_mutex, NULL);
  List_t *orders_list = (List_t*) malloc(sizeof(List_t));

  // TODO: Dificuldade ou Usuario definem o número de bancadas e cozinheiros
  benches_n = 2; 
  cooks_n = 3;
  // TODO: Dificuldade ou Usuario definem o número de bancadas e cozinheiros

  chef = (pthread_t*) malloc(cooks_n * sizeof(pthread_t));

  cooks = (cook*) malloc(cooks_n*sizeof(cook));
  for(int i = 0; i < cooks_n; i++)
  {
    strcpy(cooks[i].current_order, "NONE");
    cooks[i].order_status = NO_ORDERS;
  }


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

  pthread_create(&orders, NULL, create_orders, (void *)orders_list);

  //Essas instruções são papel do gerente (TODO: Gerente)
  int cook_id_f = 0;
  void **args_cooking_f = (void**) malloc(2*sizeof(void)); 
  args_cooking_f[0] = orders_list;
  args_cooking_f[1] = &cook_id_f;
  strcpy(cooks[0].current_order, "Pao de Queijo");

  int cook_id_s = 1;
  void **args_cooking_s = (void**) malloc(2*sizeof(void)); 
  args_cooking_s[0] = orders_list;
  args_cooking_s[1] = &cook_id_s;
  strcpy(cooks[1].current_order, "Bife Acebolado c Fritas");

  int cook_id_x = 2;
  void **args_cooking_x = (void**) malloc(2*sizeof(void)); 
  args_cooking_x[0] = orders_list;
  args_cooking_x[1] = &cook_id_x;
  strcpy(cooks[2].current_order, "Isca de Peixe");
  //Essas instruções são papel do gerente (TODO: Gerente)

  pthread_create(&chef[0], NULL, cooking, args_cooking_f);
  pthread_create(&chef[1], NULL, cooking, args_cooking_s);
  pthread_create(&chef[2], NULL, cooking, args_cooking_x);

  while(orders_list->time_left > 0)
    display_game(orders_list);

  pthread_mutex_destroy(&order_mutex);
  pthread_mutex_destroy(&ingredient_mutex);
  pthread_mutex_destroy(&kitchen_mutex);
  pthread_mutex_destroy(&info_mutex);
  end_program();
  return 0;
}
