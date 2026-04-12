#include <stdio.h>
#include "fila.h"
#include "pilha.h"
#include "perguntas.h"
#include "randon.h"
#include "easteregg.h"
#ifndef jogadores_h
#define jogadores_h

void inicializa_posicao(Player *jogador){ // Define a posição de todos os jogadores no tabileiro como 0
    jogador->posicao = 0;
}

int quantidade(){
    int quant_players; // variavel local da função que irá retornar a quantidade de jogadores para a main

    printf("\t\nQuantos jogadores irão participar do jogo? (MAX: 4):\n");
    scanf("%d",&quant_players);

    while(quant_players < 2 || quant_players > 4){  // estrutura de repetição que certifica que usuario não irá prosseguir com valor inválido
        printf("Quantidade não permitida, escolha novamente (MAX: 4):\n");
        scanf("%d",&quant_players);
    }

    return quant_players;   // função retornando quantidade de jogadores
}

int verificarNome(char *nome){  // função que verifica se nome é valído

    if(strlen(nome) >= 20)  // string lenght usada para descobrir tamanho da string(nome) e verificar se é maior que o limite de caracteres ( 20 )
        return 0;   // inválido
    else
        return 1;   // válido
}

int verificarCor(int corAux, int cor_usada[]){ // função para verificar se cor de futuros jogadores já não foi escolhida


    if(corAux < 1 || corAux > 4){   // verificação prévia para ver se a cor inserida é uma opção
        return 0;
    }

    if(cor_usada[corAux-1] == 1) return 0;
    
    cor_usada[corAux-1] = 1;
    return 1;
}



void imprimirJogadores(Player *player, int quantidade) { // função para imprimir todos os jogadores após a função de ler os dados
    int i;

    printf("\n=================================\n");
    printf("     LISTA FINAL DE JOGADORES    \n");
    printf("=================================\n");

    for(i = 0; i < quantidade; i++) {
        // Analisa a cor do jogador atual para escolher a cor do printf
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
                printf("Jogador %d: %s \n", i + 1, player[i].nome); // caso de segurança
                break;
        }
    }
    printf("\n");
}

void lerDados(int quant, tp_fila *jogadores){
    int i;
    int confirmNome, confirmCor; // variáveis usadas como adaptação para variável boolean
    char nomeAux[99];   // variavel local para armazenar nome original inserido pelo usuario
    int corAux; // variavel local para armazenar cor original inserida pelo usuario
    int cores_usadas[4] = {0,0,0,0};
    for(i = 0; i < quant; i++) {    // estrutura de repetição para cadastrar jogadores com condição de parada a quantidade de jogadores
        Player jogador;
        inicializa_posicao(&jogador);

        printf("\n=============================\n");
        printf("   CADASTRANDO JOGADOR %d\n", i + 1);
        printf("=============================\n");

        do{
            printf("\t\nQual o nome do jogador %d?\n", i+1);
            scanf(" %99[^\n]", nomeAux);
            getchar();  // limpar buffer do teclado por conta do enter inserido

            confirmNome = verificarNome(nomeAux); // variável "boolean"  recebendo o retorno da função que verifica se o nome inserido é permitido
                                                                                                            // 0 = inválido     1 = válido
            if (confirmNome == 1){
                strcpy(jogador.nome, nomeAux);    // se o nome for válido, ocorre a atribuíção da variavel local auxiliar para o vetor struct
                                                                                                        // que armazena o nome com a função de string copy
                sousaEasterEgg(jogador.nome);
            }
            else
                printf("[ERRO] Limite de 19 caracteres excedido. Tente novamente.\n"); // caso inválido, estrutura de repetição while obriga inserir um válido
        }while(confirmNome!=1);

            do{
            printf("\nQual cor deseja escolher?\n");
            printf("1 - Vermelho | 2 - Verde | 3 - Azul | 4 - Amarelo\n");
            printf("Sua escolha: ");
            scanf("%d", &corAux);

            confirmCor = verificarCor(corAux, cores_usadas);  // chamada da função que irá verificar se a cor inserida pelo usuario não foi inserida anteriormente

            if(confirmCor == 1){    // se a cor não foi repetida, ela é validada e atribuída ao jogador atual
                jogador.cor = corAux;
                printf("Cor atribuida com sucesso ao jogador %s!\n", jogador.nome);
            }

            else
                printf("[ERRO] Cor ja escolhida por outro jogador ou invalida! Tente novamente.\n");
                // se cor inválida, é impresso erro e obriga o usuario a inserir uma nova cor
            }while(confirmCor!=1);

            insereFila(jogadores, jogador);
            printf("\n>>> Jogador %d adicionado com sucesso! <<<\n", i + 1);
    }
}

void move_posicao(Player *jogador, int posi){ // Move a posição dos jogadores se eles tiverem acertado ou errado a resposta (Código n operacional)
    if(jogador->posicao >= 0){
        jogador->posicao += posi;
    }
    
}



void rodadaplayer(tp_fila *jogadores, tp_pilha *perguntas, tp_pilha *perguntas_descartadas){ // Realiza as rodadas de cada jogador, definindo e mudando paras o proximo jogador
    int resposta;
    Player jogador;
    removeFila(jogadores, &jogador);
    printf("Vez de %s\n", jogador.nome);
    resposta = geraPergunta(perguntas, perguntas_descartadas);
    if(resposta == 1)move_posicao(&jogador, rola_dados());
    else{
        move_posicao(&jogador, -3);
    }
    insereFila(jogadores, jogador);
    }
    

#endif