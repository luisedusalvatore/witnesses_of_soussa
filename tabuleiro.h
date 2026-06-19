#ifndef tabuleiro_h
#define tabuleiro_h
#include <stdio.h>
#include <stdlib.h>
#include "listade.h"


int eh_primo(int posi){
    // Se for 1 ou menor, não é primo
    if (posi <= 1) return 0;

    // A lógica correta: se for divisível (resto == 0) por qualquer número antes dele, não é primo!
    for (int i = 2; i < posi; i++){
        if (posi % i == 0) return 0; // O erro estava aqui!
    }
    return 1;
}


int eh_binario(int posi){
    // implementar lógica que verifica se o número da casa é um palindromo em binário
    char vetor[30];
    int tam = 0;
    tp_pilha *pilha = inicializa_pilha();
	tp_item_pilha e;
    // transforma o número em binário e armazena no vetor
    if (posi == 0) {
        vetor[tam++] = '0';
    } else {
        while (posi > 0) {

            vetor[tam++] = (posi % 2) + '0';
            posi = posi / 2;
        }
    }

    // armazena os elementos na pilha
    for (int i = 0; i < tam; i++) {
        tp_item_pilha n;
        n.resposta = vetor[i];
        push(pilha, n);
    }

   // compara com o vetor
    int eh_palindromo = 1;

    for (int i = 0; i < tam; i++) {

        pop(pilha, &e);

        if (vetor[i] != e.resposta) {
            eh_palindromo = 0; // Não é um palíndromo
            break;
        }
    }

    destroi_pilha(pilha);

    return eh_palindromo;
}

// retorna 0 (normal), 1 (pergunta) ou 2 (baú)
int tipo_casa(int posi) {
    if (posi == 1) return 0; // a casa inicial é sempre segura

    // 1. Calculamos as duas condições separadamente (incluindo as casas 6 e 8)
    int condicao_pergunta = eh_primo(posi) || (posi % 6 == 0) || (posi % 8 == 0);
    int condicao_bau = eh_binario(posi);

    // 2. O GRANDE SORTEIO (Se a casa atende às duas regras)
    if (condicao_pergunta && condicao_bau) {
        if (rand() % 2 == 0) {
            return 1; // 50% de chance de virar Pergunta
        } else {
            return 2; // 50% de chance de virar Baú
        }
    }

    // 3. Se atender apenas a uma das regras, aplica direto
    if (condicao_pergunta) return 1;
    if (condicao_bau) return 2;

    // 4. Se não bater em nada, é uma casa neutra (normal)
    return 0;
}

// Inicia o tabuleiro e já chama o sorteio para cada casa
int inicia_tabuleiro(tp_listade *tabuleiro, int tam){
    for(int i = 0; i < tam; i++){
        insere_listade_no_fim(tabuleiro, i+1, tipo_casa(i+1));
    }
    return 1;
}

void imprime_tabuleiro_visual(tp_listade *tabuleiro) {
    tp_no *atu = tabuleiro->ini;

    printf("\n==========================================================================================\n");
    printf("                                MAPA DO CAMINHO DO CONHECIMENTO                             \n");
    printf("==========================================================================================\n\n");

    while (atu != NULL) {
        // identifica o tipo de casa para o ícone
        char icone = ' ';
        if (atu->info.tira_carta == 1) icone = '?'; // pergunta
        if (atu->info.tira_carta == 2) icone = 'B'; // baú

        // imprime a base da casa com o número e o ícone
        printf("[ %02d %c | ", atu->info.posicao, icone);

        // verifica e imprime quais jogadores estão nesta casa usando as cores ansi
        int tem_jogador = 0;
        if (atu->info.cor[0] == 1) { printf("\033[31mP1 \033[0m"); tem_jogador = 1; }
        if (atu->info.cor[1] == 1) { printf("\033[32mP2 \033[0m"); tem_jogador = 1; }
        if (atu->info.cor[2] == 1) { printf("\033[34mP3 \033[0m"); tem_jogador = 1; }
        if (atu->info.cor[3] == 1) { printf("\033[33mP4 \033[0m"); tem_jogador = 1; }

        // se a casa estiver vazia, imprime espaços para manter o alinhamento perfeito
        if (!tem_jogador) {
            printf("   ");
        }

        printf("] ");

        // quebra de linha a cada 5 casas para formar o visual do tabuleiro
        if (atu->info.posicao % 5 == 0) {
            printf("\n\n");
        } else if (atu->prox != NULL) {
            // imprime uma setinha indicando o caminho se não for o fim da linha
            printf("-> ");
        }

        atu = atu->prox;
    }
    printf("==========================================================================================\n");
}

#endif
