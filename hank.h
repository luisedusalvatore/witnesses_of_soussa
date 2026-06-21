#ifndef hank_h
#define hank_h

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "arvavlr.h"
#include "tipos.h"

// Mantemos isso para inicializar o jogador
int inicializa_hank(Player *jogador){
    jogador->dados.acertos = 0;
    jogador->dados.erros = 0;
    jogador->dados.score = 0;

    // CORREÇÃO: Copia o nome do jogador para a estrutura de dados que será salva no txt!
    strcpy(jogador->dados.nome, jogador->nome);

    return 1;
}

// Função recursiva para percorrer a árvore AVL (In-Ordem) e imprimir no arquivo
void grava_no_arquivo(ArvAVL* raiz, FILE *arquivo){
    if(raiz == NULL || *raiz == NULL) return;

    grava_no_arquivo(&((*raiz)->esq), arquivo); // Vai para a esquerda

    fprintf(arquivo, "Casa: %02d | Visitas: %02d | Acertos: %02d | Erros: %02d\n",
            (*raiz)->info.numero_casa, (*raiz)->info.vezes_caiu,
            (*raiz)->info.acertos, (*raiz)->info.erros);

    grava_no_arquivo(&((*raiz)->dir), arquivo); // Vai para a direita
}

// Gera o arquivo de relatório final exigido pelo professor
void salva_relatorio_casas(ArvAVL* arvore){
    // Usamos "w" (Write) para criar um relatório fresco a cada partida jogada
    FILE *arquivo = fopen("relatorio_casas.txt", "w");

    if(arquivo != NULL) {
        fprintf(arquivo, "--- RELATORIO DE ESTATISTICAS DO TABULEIRO ---\n");
        fprintf(arquivo, "(Gerado atraves de percurso In-Ordem na Arvore AVL)\n\n");

        grava_no_arquivo(arvore, arquivo); // Manda a árvore se imprimir no arquivo

        fclose(arquivo);
    }
}

#endif
