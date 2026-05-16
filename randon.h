#ifndef randon_h
#define randon_h
#include <stdlib.h>

int rola_dados(){
    int valor;
    valor = (rand()%(6))+1; // gera um valor aleatorio entre 1 e 6
    printf("Numero rolado: %d", valor)
    return valor;
}

int seletor_de_questao(int qtd){
    int e;
    e = rand() % (qtd); // gera um valor aleatorio entre 0 e qtd-1
    return e;
}

#endif
