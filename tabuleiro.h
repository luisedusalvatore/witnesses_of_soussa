#ifndef tabuleiro_h
#define tabuleiro_h
#include <stdio.h>
#include <stdlib.h>
#include "listade.h"
#include "pilhade.h"

int eh_primo(int posi){
<<<<<<< HEAD
    // implementar lógica que verifica se o número da casa é primo
    for (int i = 2; i < posi; i++){
        if (posi % i != 0) return 0;
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


int eh_fibo(int posi){
    // implementar lógica que verifica se a casa tem número que faz parte da sequência de Fibonacci
    if (posi == 1) return 1;
    
    int a = 0, b = 1, c = a + b; 

    while (c < posi) {
        a = b;    
        b = c;    
        c = a + b; 
    }

    //fazendo a comparação
    if (c == posi) return 1;  
        
    return 0; 
    
}

int tem_carta(int posi){ // Define se a casa vai ter carta
    if(eh_primo(posi)) return 1; // Casa de numeros primos
    if(eh_binario(posi)) return 1; // Casas com números binários
    if(eh_fibo(posi)) return 1; // Casa de fibonacci
    return 0;
}

int inicia_tabuleiro(tp_listade *tabuleiro, int tam){ // Inicia o tabuleiro, marcando as casas que terão cartas
    for(int i = 0; i < tam; i++){
        insere_listade_no_fim(tabuleiro, i+1, tem_carta(i+1));
=======
    if (posi <= 1) return 0;
    for (int i = 2; i * i <= posi; i++) {
        if (posi % i == 0) return 0;
>>>>>>> e8ba0ef799e9e853021b36c00d8947cd6d620f1c
    }
    return 1;
}

int eh_binario(int posi){
    if (posi <= 0) return 0;
    return (posi & (posi - 1)) == 0;
}

int eh_fibo(int posi){
    if (posi == 0 || posi == 1) return 1;

    int a = 0, b = 1, c = a + b;
    while (c <= posi) {
        if (c == posi) return 1;
        a = b;
        b = c;
        c = a + b;
    }
    return 0;
}

// retorna 0 (normal), 1 (pergunta) ou 2 (baú)
int tipo_casa(int posi) {
    if (posi == 1) return 0; // a casa inicial é segura

    // damos prioridade para o baú na sobreposição
    if (eh_binario(posi)) return 2; // é uma potência de 2 -> BAÚ

    if (eh_primo(posi)) return 1;   // é número primo -> PERGUNTA

    return 0; // casa normal
}

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
