#ifndef pergunta_h
#define pergunta_h
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include "pilha.h"
#include "tipos.h"

tp_pergunta questoes[10];

void baralhoQuestoes() {
    strcpy(questoes[0].enunciado, "As pilhas sao uma struct que retornam o primeiro dado inserido? V/F");
    strcpy(questoes[1].enunciado, "Para acessar o conteudo de uma variavel, deve-se igualar o ponteiro 'p' ao endereco da variavel V/F");
    strcpy(questoes[2].enunciado, "Qual o tipo de dado que consegue armazenar varios tipos de dados? int(1) float(2) char(3) struct(4)");
    strcpy(questoes[3].enunciado, "Qual o comando utilizado para adicionar um item na pilha? pop(1) top(2) push(3) endif(4)");
    strcpy(questoes[4].enunciado, "Em uma fila, qual o comando utilizado para que seja verificar se ela esta cheia? queue(1) proximo(2) remove(3) push(4)");
    strcpy(questoes[5].enunciado, "Em uma string, qual o termo utilizado para limitar a ultima casa? nulo(1) void(2) \\0(3) 0(4)");
    strcpy(questoes[6].enunciado, "Qual a principal caracteristica de uma fila? siso(1) fifo(2) lifo(3) filo(4)");
    strcpy(questoes[7].enunciado, "Considerando int x = y e p = x, qual das alternativas pode alterar o valor de x? p = 10(1) p = 10(2) &p = 10(3) nenhuma das anteriores(4)");
    strcpy(questoes[8].enunciado, "Em qual dos casos se faz necessario utilizar '->' ao inves de '*'? pilha(1) ponteiro(2) struct(3) string(4)");
    strcpy(questoes[9].enunciado, "Qual o especificador utilizado para imprimir uma fila contendo uma sequencia de caracteres? %%s(1) %%c(2) %%d(3) %%f(4)");
}

void gabaritoQuestoes() {
    questoes[0].correto = 'F';
    questoes[1].correto = 'V';
    questoes[2].correto = '4';
    questoes[3].correto = '3';
    questoes[4].correto = '2';
    questoes[5].correto = '3';
    questoes[6].correto = '2';
    questoes[7].correto = '4';
    questoes[8].correto = '3';
    questoes[9].correto = '1';

}


/*
void embaralhaQuestoes{
    
    for (int i=0; i < qtd; i++){


    }


}
*/

#endif
