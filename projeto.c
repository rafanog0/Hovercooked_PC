// gcc projeto.c -o projeto -lncurses
// ./projeto

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ncurses.h>
#include <pthread.h>

#define BUFFER 128
#define SUCESSO 1
#define ERRO -1

typedef struct pedido pedido;
typedef struct lista lista;
typedef struct knot knot;

struct pedido
{
    char nome[BUFFER];
    int tempo;
    int pontos;
};

struct knot
{
    knot *prox;
    pedido info;
};

struct lista
{
    knot *first;
    int size;
};

char report_erro[BUFFER];

lista *cria_lista()
{
    lista *head = (lista *)malloc(sizeof(lista));
    if (head == NULL)
    {
        strcpy(report_erro, "Erro ao criar a lista;\n");
        return NULL;
    }
    head->first = NULL;
    head->size = 0;
    return head;
}

int remove_lista(lista *head, int valor)
{
    knot *atual = head->first;
    knot *anterior = NULL;
    if (atual == NULL)
    {
        strcpy(report_erro, "Tentando remover de uma lista vazia!\n");
        return ERRO;
    }

    while (atual != NULL && atual->info.tempo != valor)
    {
        anterior = atual;
        atual = atual->prox;
    }

    if (atual == NULL)
    {
        strcpy(report_erro, "Elemento a ser removido não foi encontrado!\n");
        return ERRO;
    }
    if (atual == head->first) //caso seja o primeiro elemento
        head->first = atual->prox;
    else
        anterior->prox = atual->prox;

    free(atual);
    head->size--;
    return SUCESSO;
}

void remove_primeiro(lista *head)
{
    if (head->first != NULL)
        remove_lista(head, head->first->info.tempo);
}

int insere_lista(lista *head, pedido info)
{
    knot *novo = (knot *)malloc(sizeof(knot));
    if (novo == NULL)
    {
        strcpy(report_erro, "Erro ao inserir elemento na lista\n");
        return ERRO;
    }
    novo->info = info;
    novo->prox = NULL;

    if (head->first == NULL || head->first->info.tempo >= info.tempo)
    {
        novo->prox = head->first;
        head->first = novo;
    }
    else
    {
        knot *atual = head->first;
        while (atual->prox != NULL && atual->prox->info.tempo < info.tempo)
        {
            atual = atual->prox;
        }
        novo->prox = atual->prox;
        atual->prox = novo;
    }
    head->size++;
    return SUCESSO;
}

void imprime_pedido_ncurses(int y, int x, pedido p)
{
    mvprintw(y, x, "+------------------------------+");
    mvprintw(y + 1, x, "| Nome: %-23s |", p.nome);
    mvprintw(y + 2, x, "| Tempo: %-22d |", p.tempo);
    mvprintw(y + 3, x, "| Pontos: %-21d |", p.pontos);
    mvprintw(y + 4, x, "+------------------------------+");
}

void imprime_lista_ncurses(lista *head)
{
    init_pair(5, COLOR_RED, COLOR_BLACK);
    clear();
    int y = 1; // Linha fixa
    int x = 1; // Posição horizontal inicial
    knot *atual = head->first;
    while (atual != NULL)
    {
        imprime_pedido_ncurses(y, x, atual->info);
        x += 34; // Avança a posição horizontal para o próximo pedido
        atual = atual->prox;
    }
    attron(COLOR_PAIR(5));
    mvprintw(LINES - 1, 0, "Encerrar -> 'q'; Remover -> 'r'");
    attroff(COLOR_PAIR(5));

    refresh();
}

void libera_lista(lista *head)
{
    knot *atual = head->first;
    while (atual != NULL)
    {
        knot *temp = atual;
        atual = atual->prox;
        free(temp);
    }
    free(head);
}

int main()
{
    lista *exemplo = cria_lista();
    if (exemplo == NULL)
    {
        strcpy(report_erro, "Erro ao criar a lista\n");
        return ERRO;
    }

    pedido pedidos[4];
    strcpy(pedidos[0].nome, "Bife");
    pedidos[0].tempo = 5;
    pedidos[0].pontos = 10;

    strcpy(pedidos[1].nome, "Macarrão");
    pedidos[1].tempo = 4;
    pedidos[1].pontos = 8;

    strcpy(pedidos[2].nome, "Arroz");
    pedidos[2].tempo = 3;
    pedidos[2].pontos = 5;

    strcpy(pedidos[3].nome, "Feijoada");
    pedidos[3].tempo = 2;
    pedidos[3].pontos = 15;

    for (int i = 0; i < 4; i++)
        insere_lista(exemplo, pedidos[i]);

    initscr();
    start_color();
    raw();
    noecho();
    keypad(stdscr, TRUE);

    refresh();

    imprime_lista_ncurses(exemplo);

    int ch;
    while ((ch = getch()) != 'q')
    {
        if (ch == 'r')
        {
            remove_primeiro(exemplo);
            imprime_lista_ncurses(exemplo);
        }
    }

    endwin();
    libera_lista(exemplo);
    return 0;
}
