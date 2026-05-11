#ifndef jogadores_h
#define jogadores_h
#include <stdio.h>
#include <string.h>
#include "filade.h"
#include "pilhade.h"
#include "perguntas.h"
#include "randon.h"
#include "easteregg.h"
#include "listade.h"
#include "tipos.h"

void inicializa_posicao(Player *jogador, tp_listade *inicio){
    // define a posição de todos os jogadores no tabuleiro como o início
    jogador->posicao = inicio->ini;
}

int quantidade(){
    int quant_players;

    printf("\t\nQuantos jogadores irao participar do jogo? (MAX: 4):\n");
    scanf("%d",&quant_players);

    while(quant_players < 2 || quant_players > 4){
        printf("Quantidade nao permitida, escolha novamente (MAX: 4):\n");
        scanf("%d",&quant_players);
    }

    return quant_players;
}

int verificarNome(char *nome){
    if(strlen(nome) >= 20)
        return 0;   // inválido
    else
        return 1;   // válido
}

int verificarCor(int corAux, int cor_usada[]){
    if(corAux < 1 || corAux > 4){
        return 0;
    }
    if(cor_usada[corAux-1] == 1) return 0;

    cor_usada[corAux-1] = 1;
    return 1;
}

void imprimirJogadores(Player *player, int quantidade) {
    int i;
    printf("\n=================================\n");
    printf("     LISTA FINAL DE JOGADORES    \n");
    printf("=================================\n");

    for(i = 0; i < quantidade; i++) {
        switch(player[i].cor) {
            case 1:
                printf("\033[31mJogador %d: %s \033[0m\n", i + 1, player[i].nome); // vermelho
                break;
            case 2:
                printf("\033[32mJogador %d: %s \033[0m\n", i + 1, player[i].nome); // verde
                break;
            case 3:
                printf("\033[34mJogador %d: %s \033[0m\n", i + 1, player[i].nome); // azul
                break;
            case 4:
                printf("\033[33mJogador %d: %s \033[0m\n", i + 1, player[i].nome); // amarelo
                break;
            default:
                printf("Jogador %d: %s \n", i + 1, player[i].nome);
                break;
        }
    }
    printf("\n");
}

void lerDados(int quant, tp_fila *jogadores, tp_listade *tabuleiro){
    int i;
    int confirmNome, confirmCor;
    char nomeAux[99];
    int corAux;
    int cores_usadas[4] = {0,0,0,0};

    for(i = 0; i < quant; i++) {
        Player jogador;
        inicializa_posicao(&jogador, tabuleiro);

        printf("\n=============================\n");
        printf("   CADASTRANDO JOGADOR %d\n", i + 1);
        printf("=============================\n");

        do{
            printf("\t\nQual o nome do jogador %d?\n", i+1);
            scanf(" %99[^\n]", nomeAux);
            getchar();

            confirmNome = verificarNome(nomeAux);
            if (confirmNome == 1){
                strcpy(jogador.nome, nomeAux);
                // sousaEasterEgg(jogador.nome); // Descomente quando o easteregg.h estiver pronto
            }
            else
                printf("[ERRO] Limite de 19 caracteres excedido. Tente novamente.\n");
        }while(confirmNome!=1);

        do{
            printf("\nQual cor deseja escolher?\n");
            printf("1 - Vermelho | 2 - Verde | 3 - Azul | 4 - Amarelo\n");
            printf("Sua escolha: ");
            scanf("%d", &corAux);

            confirmCor = verificarCor(corAux, cores_usadas);

            if(confirmCor == 1){
                jogador.cor = corAux;
                if (tabuleiro->ini != NULL) { // Proteção extra caso o tabuleiro esteja vazio
                    tabuleiro->ini->info.cor[confirmCor - 1] = 1;
                }
                printf("Cor atribuida com sucesso ao jogador %s!\n", jogador.nome);
            }
            else
                printf("[ERRO] Cor ja escolhida por outro jogador ou invalida! Tente novamente.\n");
        }while(confirmCor!=1);

        insere_fila(jogadores, jogador);
        printf("\n>>> Jogador %d adicionado com sucesso! <<<\n", i + 1);
    }
}

int move_posicao(Player *jogador, int posi){
    if(posi > 0){
        if(jogador->posicao != NULL) jogador->posicao->info.cor[jogador->cor-1] = 0;
        for(int i = 0; i < posi; i++){
                if(jogador->posicao != NULL) jogador->posicao = jogador->posicao->prox;
                if(jogador->posicao == NULL) return 1; // fim do tabuleiro
        }
    }
    else{
        for(int i = 0; i < -1*posi; i++){
            if(jogador->posicao != NULL && jogador->posicao->ant != NULL)
                jogador->posicao = jogador->posicao->ant;
        }
    }
    if(jogador->posicao != NULL) jogador->posicao->info.cor[jogador->cor-1] = 1;
    return 0;
}

void rodadaplayer(tp_fila *jogadores, tp_pilha *perguntas, tp_pilha *perguntas_descartadas){
    int resposta;
    Player jogador;

    remove_fila(jogadores, &jogador);
    printf("Vez de %s\n", jogador.nome);

    resposta = geraPergunta(perguntas, perguntas_descartadas);
    if(resposta == 1) move_posicao(&jogador, rola_dados());
    else{
        move_posicao(&jogador, -3);
    }

    insere_fila(jogadores, jogador);
}

#endif
