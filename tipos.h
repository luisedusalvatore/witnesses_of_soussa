#include "listade.h";
# ifndef tipos_h
# define tipos_h
typedef struct{		// struct de jogadores

    char nome[25];
    int cor;
    tp_no *posicao;

}Player;

typedef struct{

    char enunciado[300];
    char resposta;
} tp_pergunta;

typedef struct {
    int cor[4];
    int tira_carta;
    int posicao;
} tp_casa;
	
# endif
