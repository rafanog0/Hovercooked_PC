#include "../inc/display_infos.h"
#include "../inc/game.h"
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int main() {

  initialize_screen();
  Difficulty_t choice = display_menu();
  if (choice.difficulty_id == 4)
    return 0;

  pthread_t timer;
  pthread_t orders;
  pthread_t *chef;
  pthread_t manager;
  pthread_mutex_init(&order_mutex, NULL);
  pthread_mutex_init(&ingredient_mutex, NULL);
  pthread_mutex_init(&kitchen_mutex, NULL);
  pthread_mutex_init(&info_mutex, NULL);
  List_t *orders_list = (List_t *)malloc(sizeof(List_t));
  if (orders_list == NULL)
    return ERRO;

  benches_n = choice.benches;
  cooks_n = choice.cooks;

  chef = (pthread_t *)malloc(cooks_n * sizeof(pthread_t));
  cooks = (cook *)malloc(cooks_n * sizeof(cook));

  for (int i = 0; i < cooks_n; i++) {
    strcpy(cooks[i].current_order, "NONE");
    cooks[i].order_status = NO_ORDERS;
  }

  orders_list->time_left = choice.time;
  orders_list->create_order_time = choice.time / 3;
  benches_ingredient = (prep_bench *)malloc(benches_n * sizeof(prep_bench));
  benches_kitchen = (prep_bench *)malloc(benches_n * sizeof(prep_bench));
  if (benches_ingredient == NULL || benches_kitchen == NULL)
    return ERRO;

  pthread_create(&timer, NULL, match_clock, (void *)orders_list);
  pthread_create(&orders, NULL, create_orders, (void *)orders_list);

  // Essas instruções são papel do gerente (TODO: Gerente)
  int cook_id_f = 0;
  void **args_cooking_f = (void **)malloc(2 * sizeof(void));
  args_cooking_f[0] = orders_list;
  args_cooking_f[1] = &cook_id_f;
  strcpy(cooks[0].current_order, "Pao de Queijo");

  int cook_id_s = 1;
  void **args_cooking_s = (void **)malloc(2 * sizeof(void));
  args_cooking_s[0] = orders_list;
  args_cooking_s[1] = &cook_id_s;
  strcpy(cooks[1].current_order, "Bife Acebolado c Fritas");

  int cook_id_x = 2;
  void **args_cooking_x = (void **)malloc(2 * sizeof(void));
  args_cooking_x[0] = orders_list;
  args_cooking_x[1] = &cook_id_x;
  strcpy(cooks[2].current_order, "Isca de Peixe");
  // Essas instruções são papel do gerente (TODO: Gerente)

  // pthread_create(&chef[0], NULL, cooking, args_cooking_f);
  // pthread_create(&chef[1], NULL, cooking, args_cooking_s);
  // pthread_create(&chef[2], NULL, cooking, args_cooking_x);

  pthread_create(&manager, NULL, managing, (void *)orders_list);

  while (orders_list->time_left > 0)
    display_game(orders_list);

  pthread_mutex_destroy(&order_mutex);
  pthread_mutex_destroy(&ingredient_mutex);
  pthread_mutex_destroy(&kitchen_mutex);
  pthread_mutex_destroy(&info_mutex);
  free_list(orders_list);
  end_program();
  return 0;
}
