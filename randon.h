#ifndef randon_h
#define randon_h
#include <stdlib.h>
#include <stdio.h>

int rola_dados(){
    int valor;
    valor = (rand()%(6))+1;
    printf("Numero rolado: %d\n", valor);
    return valor;
}

int seletor_de_questao(int qtd){
    int e;
    e = rand() % (qtd); // gera um valor aleatorio entre 0 e qtd-1
    return e;
}

int sortear_item() {
    int chance = rand() % 100; // gera de 0 a 99

    // 60% de chance de vir um raro (IDs 1, 2, 7)
    if (chance < 60) {
        int itens_raros[] = {1, 2, 7};
        return itens_raros[rand() % 3];
    }
    // 30% de chance de vir um mítico (IDs 3, 4, 6)
    else if (chance < 90) {
        int itens_miticos[] = {3, 4, 6};
        return itens_miticos[rand() % 3];
    }
    // 10% de chance de vir o lendário (ID 5 - Buraco de Minhoca)
    else {
        return 5;
    }
}

#endif
 
