#include <stdio.h>
#include <stdlib.h>

# ifndef tipos_h
# define tipos_h


typedef struct {
    int cor[4];
    int tira_carta;
    int posicao;
} tp_casa;

typedef struct tp_no_aux {
    struct tp_no_aux *ant;
    tp_casa info;
    struct tp_no_aux *prox;
} tp_no;
typedef struct {
    tp_no *ini;
    tp_no *fim;
} tp_listade;

typedef struct{		// struct de jogadores

    char nome[25];
    int cor;
    tp_no *posicao;

}Player;

typedef struct{

    char enunciado[300];
    char resposta;
} tp_pergunta;
	
# endif
