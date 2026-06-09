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

typedef struct {
    char nome[100];
    int cor;
    tp_no *posicao;
    tp_hank dados;

    // sistema de inventario
    int inventario[3]; // guarda os IDs dos itens
    int qtd_itens;

    // status do jogador
    int status_rerol;
    int status_imune;        // 1 se usou "Imunidade à resenha", 0 normal
    int status_castigo;      // 1 se foi alvo do "Castigo", 0 normal
    int status_tudo_nada;    // 1 se ativou o "Tudo ou nada", 0 normal
} Player;

typedef struct{
    int id;
    char enunciado[300];
    char resposta;
} tp_pergunta;

typedef struct {
    int acertos;
    int erros;
    int score;
    char nome[50];
} tp_hank;
# endif
