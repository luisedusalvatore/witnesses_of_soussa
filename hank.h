#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "arvore.h"
#include "tipos.h"
#ifndef hank_h
#define hank_h

int inicializa_hank(Player *jogador){
    jogador->dados.acertos = 0;
    jogador->dados.erros = 0;
    jogador->dados.score = 0;
    strcpy(jogador->nome, jogador->dados.nome);
    return 1;
}

FILE carrega_hank(tp_arvore *arvore){
    *arvore = inicializa_arvore();
    FILE *arquivo;
    arquivo = fopen("hank.txt", "w");
    tp_hank hank;
    while(fscanf(arquivo, "%d %d %d %[^\n]", &hank.acertos, &hank.erros, &hank.score, hank.nome) != EOF){
        insere_no(arvore, hank);
    }
    return *arquivo;
}
void salva_hank(FILE *arquivo, tp_hank jogador){
    fprintf(arquivo,"%d %d %d %[^\n]", jogador.acertos, jogador.erros, jogador.score, jogador.nome);
}
#endif