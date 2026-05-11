#ifndef FILADE_H_INCLUDED
#define FILADE_H_INCLUDED
#include <stdio.h>
#include <stdlib.h>
#include "tipos.h"

typedef Player tp_item_fila;

typedef struct tp_no_fila_aux {
  tp_item_fila info;
  struct tp_no_fila_aux *prox;
} tp_no_fila;

typedef struct {
  tp_no_fila *ini, *fim;
} tp_fila;

tp_fila *inicializa_fila (){
   tp_fila *fila = (tp_fila*) malloc(sizeof(tp_fila));
   fila->ini = fila->fim = NULL;
   return fila;
}

tp_no_fila *aloca_fila() {
	return (tp_no_fila*) malloc(sizeof(tp_no_fila));
}

int fila_vazia (tp_fila *fila){
    return (fila->ini == NULL);
}

int insere_fila (tp_fila *fila, tp_item_fila e){
    tp_no_fila *novo = aloca_fila();
    if(!novo) return 0;

    novo->info = e;
    novo->prox = NULL;

    if(fila_vazia(fila)){
        fila->ini = novo;
    } else {
        fila->fim->prox = novo;
    }
    fila->fim = novo;
    return 1;
}

int remove_fila (tp_fila *fila, tp_item_fila *e){
    if (fila_vazia(fila)) return 0;

    tp_no_fila *aux = fila->ini;
    *e = aux->info;

    fila->ini = aux->prox;

    if (fila->ini == NULL) {
        fila->fim = NULL;
    }

    free(aux);
    return 1;
}

// CORREÇÃO CRÍTICA: Resolvido o Use-After-Free
tp_fila *destroi_fila(tp_fila *fila) {
    tp_item_fila e;
    while (!fila_vazia(fila)) {
        remove_fila(fila, &e);
    }
    free(fila);
    return NULL;
}

void imprime_fila(tp_fila *fila) {
     tp_fila *fila_aux = inicializa_fila();
     tp_item_fila e;

     printf("Inicio -> [ ");
	 while (!fila_vazia(fila)) {
        remove_fila(fila, &e);
        printf("%s | ", e.nome);
        insere_fila(fila_aux, e);
     }
     printf("] <- Fim\n");

     fila->ini = fila_aux->ini;
     fila->fim = fila_aux->fim;

     fila_aux->ini = NULL;
     fila_aux->fim = NULL;
     free(fila_aux);
}

#endif // FILADE_H_INCLUDED
