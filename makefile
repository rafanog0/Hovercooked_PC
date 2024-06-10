# Makefile para compilar e executar programa em C com ncurses

# Nome do programa
PROGRAM = projeto

# Arquivo fonte
SRC = $(PROGRAM).c

# Compilador
CC = gcc

# Flags de compilação
CFLAGS = -lncurses

# Alvo padrão
all: $(PROGRAM)

# Regra para compilar o programa
$(PROGRAM): $(SRC)
	$(CC) $(SRC) -o $(PROGRAM) $(CFLAGS)

# Regra para executar o programa
run: $(PROGRAM)
	./$(PROGRAM)

# Regra para limpar arquivos gerados pela compilação
clean:
	rm -f $(PROGRAM)

# Alvo phony para evitar conflitos com arquivos
.PHONY: all run clean
