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

int tem_carta(int posi){
    if(eh_primo(posi)) return 1;
    if(eh_binario(posi)) return 1;
    if(eh_fibo(posi)) return 1;
    return 0;
}

int inicia_tabuleiro(tp_listade *tabuleiro, int tam){
    for(int i = 0; i < tam; i++){
        insere_listade_no_fim(tabuleiro, i+1, tem_carta(i+1));
    }
    return 1;
}

#endif
