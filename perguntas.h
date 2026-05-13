#ifndef pergunta_h
#define pergunta_h
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <ctype.h>
#include <string.h>
#include "pilhade.h"
#include "tipos.h"

void popula_perguntas(tp_pilha *pilha){
    int tam = 10;
    char perguntas[10][300] = {
        "As pilhas sao uma struct que retornam o primeiro dado inserido? V/F",
        "Para acessar o conteudo de uma variavel, deve-se igualar o ponteiro 'p' ao endereco da variavel V/F",
        "Qual o tipo de dado que consegue armazenar varios tipos de dados? int(1) float(2) char(3) struct(4)",
        "Qual o comando utilizado para adicionar um item na pilha? pop(1) top(2) push(3) endif(4)",
        "Em uma fila, qual o comando utilizado para que seja verificar se ela esta cheia? queue(1) proximo(2) remove(3) push(4)",
        "Em uma string, qual o termo utilizado para limitar a ultima casa? nulo(1) void(2) \\0(3) 0(4)",
        "Qual a principal caracteristica de uma fila? siso(1) fifo(2) lifo(3) filo(4)",
        "Considerando int x = y e p = x, qual das alternativas pode alterar o valor de x? p = 10(1) p = 10(2) &p = 10(3) nenhuma das anteriores(4)",
        "Em qual dos casos se faz necessario utilizar '->' ao inves de '*'? pilha(1) ponteiro(2) struct(3) string(4)",
        "Qual o especificador utilizado para imprimir uma fila contendo uma sequencia de caracteres? %%s(1) %%c(2) %%d(3) %%f(4)"
    };
    char respostas[10] = {
        'F',
        'V',
        '4',
        '3',
        '2',
        '3',
        '2',
        '4',
        '3',
        '1',
    };
void popula_perguntas_m(tp_pilha *pilha){
    int tam = 10;
    char perguntas[10][300] = {
        "qual o elemento de uma lista que é utilizado para identificar o fim dela? (end(1), /0(2), return 0(3), null(4))",
        "no pior caso, o algoritimo bubble tem uma complexidade maior que o merge sort (V/F)",
        "Em uma lista simplesmente encadeada, cada nó contém um ponteiro para o nó anterior. V/F",
        "Qual a principal vantagem de uma lista duplamente encadeada em relação à simples? (menor uso de memória (1)/ percorrer em ambos os sentidos (2)/ busca binária direta (3)/ não usa ponteiros(4))",
        "Qual a complexidade de espaço (O) para o algoritmo bubble sort no melhor caso? (O(1) (1)/ O(n) (2)/ O(log\ n) (3)/ O(n^2) (4))",
        "Para remover o último elemento de uma lista simplesmente encadeada, é necessário percorrer toda a lista até o penúltimo nó. V/F",
        "Na notação O, qual destas complexidades representa um algoritmo que ocupa mais espaço? (O(n) (1)/ O(n log\ n) (2)/ O(n^2) (3)/ O(log\ n) (4))",
        "Em uma lista duplamente encadeada, qual o valor do ponteiro (anterior) do primeiro nó (head) e do ponteiro (próximo) do último nó (tail)? (0 e 1 (1)/ nulo e nulo (2)/ nulo e head (3)/ tail e nulo(4))",
        "Normalmente, a complexidade de tempo é maior que a do espaço (V/F)",
        "Em uma lista encadeada circular, o ponteiro *prox do último nó aponta para: (nulo (1)/ (2)/ primeiro nó (3)/ penúltimo nó(4))"
    };
    char respostas[10] = {
        '4',
        'V',
        'F',
        '2',
        '2',
        'V',
        '3',
        '4',
        'F',
        '3',
    };
}
    tp_pergunta questoes;
    for(int i = 0; i < tam; i++){
        strcpy(questoes.enunciado, perguntas[i]);
        questoes.resposta = respostas[i];
        push(pilha, questoes);
    }
}

void embaralhaQuestoes(tp_pilha *perguntas){
    srand(time(NULL));

    // pilhas auxiliares alocadas dinamicamente
    tp_pilha *p1 = inicializa_pilha();
    tp_pilha *p2 = inicializa_pilha();
    tp_pilha *p3 = inicializa_pilha();
    tp_pilha *p4 = inicializa_pilha();
    tp_pilha *p5 = inicializa_pilha();

    while(!pilha_vazia(perguntas)){
        tp_pergunta e;
        int n = rand()%5;

        switch (n){
            case 0: pop(perguntas, &e); push(p1, e); break;
            case 1: pop(perguntas, &e); push(p2, e); break;
            case 2: pop(perguntas, &e); push(p3, e); break;
            case 3: pop(perguntas, &e); push(p4, e); break;
            case 4: pop(perguntas, &e); push(p5, e); break;
        }
    }

    while(!pilha_vazia(p1)){ tp_pergunta e; pop(p1, &e); push(perguntas, e); }
    while(!pilha_vazia(p2)){ tp_pergunta e; pop(p2, &e); push(perguntas, e); }
    while(!pilha_vazia(p3)){ tp_pergunta e; pop(p3, &e); push(perguntas, e); }
    while(!pilha_vazia(p4)){ tp_pergunta e; pop(p4, &e); push(perguntas, e); }
    while(!pilha_vazia(p5)){ tp_pergunta e; pop(p5, &e); push(perguntas, e); }

    // destruindo as auxiliares para evitar memory leak
    destroi_pilha(p1);
    destroi_pilha(p2);
    destroi_pilha(p3);
    destroi_pilha(p4);
    destroi_pilha(p5);
}

void transfere_perguntas(tp_pilha *perguntas, tp_pilha *descartadas){
    while(!pilha_vazia(descartadas)){
        tp_pergunta e;
        pop(descartadas, &e);
        push(perguntas, e);
    }
    embaralhaQuestoes(perguntas);
}

int geraPergunta(tp_pilha *perguntas, tp_pilha *perguntas_descartadas){
    tp_pergunta e;
    char alternativa_selecionada;
    if(pilha_vazia(perguntas)) transfere_perguntas(perguntas, perguntas_descartadas);

    pop(perguntas, &e);
    printf("\n%s\nSua resposta: ", e.enunciado);
    scanf(" %c", &alternativa_selecionada);

    if (e.resposta == toupper(alternativa_selecionada))
        return 1;
    else
        return 0;
}

#endif
