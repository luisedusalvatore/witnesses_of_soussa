#ifndef tabuleiro_h
#define tabuleiro_h
#include <stdio.h>
#include <stdlib.h>
#include "listade.h"

int eh_primo(int posi){
    // implementar lógica de número primo real
    return 1;
}

int eh_binario(int posi){
    // implementar lógica de potência de 2 real
    return 1;
}

int eh_fibo(int posi){
    // implementar lógica da sequência de Fibonacci real
    return 1;
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
