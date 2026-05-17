#ifndef tabuleiro_h
#define tabuleiro_h
#include <stdio.h>
#include <stdlib.h>
#include "listade.h"
#include "pilhade.h"

int eh_primo(int posi){
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
    }
    return 1;
}

#endif
