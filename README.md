## PROJETO DE PROGRAMAÇÃO CONCORRENTE IDP-DF

### Objetivo
O objetivo desse projeto é aprender e aprofundar em conceitos de gerenciamento e execução de tarefas que envolvam a utilização de recursos que serão utilizadas por multiplas threads


### Execução

#### Usando makefile:
```bash
make
```
#### Diretamente no terminal:
```bash
gcc -c -w -I./inc -g src/display_infos.c -o obj/display_infos.o
gcc -c -w -I./inc -g src/game.c -o obj/game.o
gcc -c -w -I./inc -g src/main.c -o obj/main.o
gcc  ./obj/*.o -o main -lncurses
```

```bash
./main
```
