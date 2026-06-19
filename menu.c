#ifndef listade_h
#define listade_h
#include <stdio.h>
#include <stdlib.h>
#include "tipos.h"

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

tp_no *aloca_listade(){
    tp_no *pt;
    pt=(tp_no*) malloc(sizeof(tp_no));
    return pt;
}

int insere_listade_no_fim(tp_listade *lista, int posicao, int carta){
    tp_no *novo;
    novo = aloca_listade();
    if(!novo) return 0;

    novo->info.cor[0] = 0;
    novo->info.cor[1] = 0;
    novo->info.cor[2] = 0;
    novo->info.cor[3] = 0;
    novo->info.posicao = posicao;
    novo->info.tira_carta = carta;

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
            case 1:
                atu = lista->ini;
                while(atu != NULL){
                    printf("%d ", atu->info.posicao);
                    atu = atu->prox;
                }
                break;
            case 2:
                atu = lista->fim;
                while(atu != NULL){
                    printf("%d ", atu->info.posicao);
                    atu = atu->ant;
                }
                break;
        }
    }
}

int remove_listade(tp_listade *lista, int posicao_buscada){
    tp_no *atu;
    atu = lista->ini;

    while((atu != NULL) && (atu->info.posicao != posicao_buscada)){
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
        else if(lista->fim == atu){
            lista->fim = atu->ant;
            atu->ant->prox = NULL;
        }
        else{
            atu->prox->ant = atu->ant;
            atu->ant->prox = atu->prox;
        }
    }
    free(atu);
    return 1;
}

#endif
