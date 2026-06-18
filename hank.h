#ifndef hank_h
#define hank_h

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
// 1. Atualizado para incluir a nova biblioteca AVL
#include "arvavlr.h"
#include "tipos.h"

int inicializa_hank(Player *jogador){
    jogador->dados.acertos = 0;
    jogador->dados.erros = 0;
    jogador->dados.score = 0;
    // Removi o strcpy aqui pois o jogador->nome já é preenchido lá no lerDados!
    return 1;
}

// 2. Função agora é void. Ela apenas abre, lê e FECHA o arquivo.
void carrega_hank(ArvAVL *arvore){
    *arvore = NULL;

    // 3. MODO "r" (READ): Lê o arquivo sem apagá-lo
    FILE *arquivo = fopen("hank.txt", "r");

    // Proteção: Se for a 1ª vez rodando o jogo e o txt não existir, sai da função
    if (arquivo == NULL) {
        return;
    }

    tp_hank hank;

    // 4. O \n no final do fscanf é importante para ler quebras de linha corretamente
    while(fscanf(arquivo, "%d %d %d %[^\n]\n", &hank.acertos, &hank.erros, &hank.score, hank.nome) != EOF){

        // NOTA: Como nossa AVL atual só aceita INT, estou passando o score temporariamente.
        // Se formos guardar o 'tp_hank' inteiro na árvore, precisaremos adaptar o arvavlr.h
        inserir(arvore, hank);
    }

    // Fecha o arquivo para liberar a memória
    fclose(arquivo);
}

// 5. Função de salvar agora recebe apenas o jogador e ela mesma abre o arquivo
void salva_hank(tp_hank jogador){

    // MODO "a" (APPEND): Adiciona a nova pontuação na última linha do txt sem apagar o resto
    FILE *arquivo = fopen("hank.txt", "a");

    if(arquivo != NULL) {
        fprintf(arquivo, "%d %d %d %s\n", jogador.acertos, jogador.erros, jogador.score, jogador.nome);
        fclose(arquivo);
    }
}

#endif
