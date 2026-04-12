#include <stdio.h>
#include <string.h>
#include "tipos.h"
#include "pilha.h"
#include <stdlib.h>
#include <locale.h>
#include <ctype.h>
#include <time.h>
#include "fila.h"
#include "randon.h"
#include "perguntas.h"
#include "easteregg.h"
#include "jogadores.h"



int main(){
   

    srand(time(NULL)); // Usa o tempo para gerar os numeros aleatorios 
    int quant; // quantidade de jogadores
    tp_pilha perguntas, perguntas_descartadas;
    tp_fila jogadores;
    inicializa_pilha(&perguntas);
    inicializa_pilha(&perguntas_descartadas);
    inicializaFila(&jogadores);
    popula_perguntas(&perguntas);
    embaralhaQuestoes(&perguntas);
    setlocale(LC_ALL, "Portuguese"); // função responsável por adicionar caracteres do PT-BR
    quant = quantidade();   // atribuição da função que lê a quantidade de jogadores para a variavel de quantidade da main

   
	
    lerDados(quant, &jogadores); // Insere os dados dos Jogadores presentes
    rodadaplayer(&jogadores, &perguntas, &perguntas_descartadas); // realiza uma rodada 


    printf("Teste\n");
    

    return 0;
}

