#include "../inc/display_infos.h"
#include "../inc/game.h"
#include <pthread.h>
#include <unistd.h>



int main() {

  initialize_screen();
  int choice = display_menu();
  pthread_t timer;

  int match_time;
  if(choice == 0)
    match_time = 5;
  else if(choice == 1) // Não está multiplicando por 60 por motivos de teste
    match_time = 10;
  else if(choice == 2)
    match_time = 15;


  List_t *orders_list = (List_t*) malloc(sizeof(List_t));

  orders_list->time_left = match_time;


  pthread_create(&timer, NULL, match_clock, (void *)orders_list);

  create_orders(orders_list);

  while(orders_list->time_left > 0)
    display_game(orders_list);


  end_program();
  return 0;
}
