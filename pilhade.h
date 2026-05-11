// Um grande artista assina a sua obra


#ifndef PILHADE_H_INCLUDED
#define PILHADE_H_INCLUDED
#include <stdlib.h>
#include <stdio.h>
#include "tipos.h"

typedef tp_pergunta tp_item_pilha;

typedef struct tp_no_pilha_aux {
  tp_item_pilha info;
  struct tp_no_pilha_aux *prox;
} tp_no_pilha;

typedef struct {
  tp_no_pilha *topo;
} tp_pilha;

tp_pilha *inicializa_pilha (){
   tp_pilha *pilha = (tp_pilha*) malloc(sizeof(tp_pilha));
   pilha->topo = NULL;
   return pilha;
}

int pilha_vazia (tp_pilha *pilha){
   return (pilha->topo == NULL);
}

tp_no_pilha *aloca_pilha() {
	return (tp_no_pilha*) malloc(sizeof(tp_no_pilha));
}

int push(tp_pilha *pilha, tp_item_pilha e) {
    tp_no_pilha *novo = aloca_pilha();
    if (novo == NULL) return 0;

    novo->info = e;
    novo->prox = pilha->topo;
    pilha->topo = novo;
    return 1;
}

int pop(tp_pilha *pilha, tp_item_pilha *e) {
    if (pilha_vazia(pilha)) return 0;

    tp_no_pilha *aux = pilha->topo;
    *e = aux->info;

    pilha->topo = aux->prox;
    free(aux);
    return 1;
}

int top (tp_pilha *pilha, tp_item_pilha *e){
  if (pilha_vazia(pilha)) return 0;
  *e = pilha->topo->info;
  return 1;
}

tp_pilha *destroi_pilha(tp_pilha *pilha) {
    tp_item_pilha e;
    while (!pilha_vazia(pilha)) {
        pop(pilha, &e);
    }
    free(pilha);
    return NULL;
}

void imprime_pilha(tp_pilha *pilha) {
     tp_pilha *pilha_aux = inicializa_pilha();
     tp_item_pilha e;

     printf("Topo -> [ ");
	 while (!pilha_vazia(pilha)) {
        pop(pilha, &e);
        printf("%c ", e.resposta);
        push(pilha_aux, e);
     }
     printf("] <- Fundo\n");

	 while (!pilha_vazia(pilha_aux)) {
        pop(pilha_aux, &e);
        push(pilha, e);
     }
     destroi_pilha(pilha_aux);
}

#endif // PILHADE_H_INCLUDED
