#include <stdio.h>
#include <stdlib.h>
#include "tipos.h"
#include "tabuleiro.h"
#ifndef listade_h
#define listade_h

typedef struct tp_no_aux {
    struct tp_no_aux *ant;
    tp_casa info;
    struct tp_no_aux *prox;
} tp_no;
typedef struct {
    tp_no *ini;
    tp_no *fim;
} tp_listade;

tp_listade *inicializa_listade(){
    tp_listade *lista=(tp_listade*) malloc(sizeof(tp_listade));
    lista->ini = NULL;
    lista->fim = NULL;
    return lista;
}
int listade_vazia(tp_listade *lista){
    if((lista->ini == NULL) && (lista->fim == NULL)) return 1;
    return 0;
}
tp_no *aloca(){
    tp_no *pt;
    pt=(tp_no*) malloc(sizeof(tp_no));
}
int insere_listade_no_fim(tp_listade *lista, int posicao){
    tp_no *novo;
    novo = aloca();
    if(!novo) return 0;
    novo->info.cor = {0,0,0,0};
    novo->info.posicao = posicao;
    novo->info.tira_carta = tem_carta(posicao);
    if(listade_vazia(lista)){
        novo->prox = NULL;
        novo->ant = NULL;
        lista->ini = lista->fim = novo;
    }
    else{
        novo->prox = NULL;
        novo->ant = lista->fim;
        lista->fim->prox = novo;
        lista->fim = novo;
    }
    return 1;
}
void imprime_listade(tp_listade *lista, int ordem){
    if(lista==NULL)
    printf("Lista nao inicializada");
    else{
        tp_no *atu;
        switch(ordem){
            case 1: atu = lista->ini;
            while(atu != NULL){
                printf("%d", atu->info);
                atu = atu->prox;
            }
            break;
            case 2: atu = lista->fim;
            while(atu != NULL){
                printf("%d", atu->info);
                atu = atu->ant;
            }
            break;
        }
    }
}
int remove_listade(tp_listade *lista, tp_item e){
    tp_no *atu;
    atu = lista->ini;
    while((atu != NULL) && (atu->info != e)){
        atu=atu->prox;
    }
    if(atu == NULL) return 0;
    if(lista->ini == lista->fim){
        lista->ini = lista->fim = NULL;
    }
    else{
        if(lista->ini == atu){
            lista->ini = atu->prox;
            atu->prox->ant = NULL;
        }
        else if(lista->fim == NULL){
            lista->fim = atu->ant;
            atu->ant->prox = NULL;
        }
        else{
            atu->prox->ant = atu->ant;
            atu->ant->prox = atu->prox;
        }
    }
    free(atu);
}

#endif