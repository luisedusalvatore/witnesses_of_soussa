#include <stdio.h>
#include <stdlib.h>
#include "listade.h"

#ifndef tabuleiro_h
#define tabuleiro_h
int eh_primo(int posi){
    return 1;
}
int eh_binario(int posi){
    return 1;
}
int eh_fibo(int posi){
    return 1;
}
int inicia_tabuleiro(tp_listade *tabuleiro, int tam){
    for(int i = 0; i < tam; i++){
        insere_listade_no_fim(tabuleiro, i+1);
    }
}
int tem_carta(int posi){
    if(eh_primo(posi)) return 1;
    if(eh_binario(posi)) return 1;
    if(eh_fibo(posi)) return 1;
    return 0;
}
#endif