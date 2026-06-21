#ifndef pergunta_h
#define pergunta_h
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <ctype.h>
#include <string.h>
#include "pilhade.h"
#include "tipos.h"
#include "historico.h"

// Preenche a pilha com perguntas faceis
void popula_perguntas(tp_pilha *pilha){
    int tam = 10;

    tp_pergunta perguntas_faceis[10] = {
        {"P001", 1, "Pilha",     "LIFO",         "Facil", "As pilhas são estruturas de dados que retornam sempre o primeiro dado inserido?\n\nV - Verdadeiro\nF - Falso", 'F'},
        {"P002", 2, "Ponteiros", "Endereco",      "Facil", "Para aceder ao conteúdo de uma variável usando um ponteiro 'p', deve-se igualá-lo ao endereço da variável?\n\nV - Verdadeiro\nF - Falso", 'V'},
        {"P003", 1, "Struct",    "Tipos",         "Facil", "Qual o tipo de dado em C que consegue armazenar internamente vários tipos de dados diferentes?\n\n1 - int\n2 - float\n3 - char\n4 - struct", '4'},
        {"P004", 1, "Pilha",     "Operacoes",     "Facil", "Qual é o comando clássico utilizado para adicionar um novo item no topo de uma pilha?\n\n1 - pop\n2 - top\n3 - push\n4 - insert", '3'},
        {"P005", 1, "Fila",      "Operacoes",     "Facil", "Em uma fila estática, qual seria a nomenclatura comum da função para verificar o próximo da fila sem o remover?\n\n1 - queue\n2 - peek (ou proximo)\n3 - pop\n4 - push", '2'},
        {"P006", 1, "String",    "Terminador",    "Facil", "Em uma string (vetor de char), qual o caractere utilizado para limitar e identificar o fim do texto?\n\n1 - nulo\n2 - void\n3 - \\0\n4 - 0", '3'},
        {"P007", 1, "Fila",      "FIFO",          "Facil", "Qual é a principal característica lógica e de comportamento de uma Fila?\n\n1 - SISO\n2 - FIFO\n3 - LIFO\n4 - FILO", '2'},
        {"P008", 2, "Ponteiros", "Derreferencia", "Facil", "Considerando 'int x = y' e um ponteiro 'p' apontando para 'x', qual das alternativas altera o valor de x?\n\n1 - p = 10\n2 - *p = 10\n3 - &p = 10\n4 - Nenhuma das anteriores", '2'},
        {"P009", 2, "Ponteiros", "Struct",        "Facil", "Em qual dos casos se faz estritamente necessário utilizar '->' ao invés do ponto '.'?\n\n1 - Pilha estática\n2 - Ponteiro de int\n3 - Ponteiro para struct\n4 - String", '3'},
        {"P010", 1, "Fila",      "Impressao",     "Facil", "Qual o especificador utilizado num printf para imprimir uma sequência completa de caracteres (string)?\n\n1 - %s\n2 - %c\n3 - %d\n4 - %f", '1'}
    };

    for(int i = 0; i < tam; i++){
        push(pilha, perguntas_faceis[i]);
    }
}

// Preenche a pilha com perguntas medias
void popula_perguntas_m(tp_pilha *pilha){
    int tam = 10;

    tp_pergunta perguntas_medias[10] = {
        {"P011", 2, "Lista",                    "Sentinela",   "Medio", "Qual é o elemento/valor de uma lista encadeada que é frequentemente utilizado para identificar o seu fim?\n\n1 - end\n2 - \\0\n3 - return 0\n4 - NULL", '4'},
        {"P012", 3, "Ordenacao",                "Complexidade","Medio", "No pior caso, o algoritmo Bubble Sort tem uma complexidade temporal maior que o Merge Sort.\n\nV - Verdadeiro\nF - Falso", 'V'},
        {"P013", 2, "Lista Encadeada",          "Estrutura",   "Medio", "Numa lista simplesmente encadeada, cada nó contém um ponteiro apontando de volta para o nó anterior.\n\nV - Verdadeiro\nF - Falso", 'F'},
        {"P014", 2, "Lista Duplamente Encadeada","Vantagens",  "Medio", "Qual a principal vantagem de uma lista duplamente encadeada em relação à lista simples?\n\n1 - Menor uso de memória\n2 - Percorrer em ambos os sentidos\n3 - Permite busca binária direta\n4 - Não precisa de ponteiros", '2'},
        {"P015", 3, "Complexidade",              "Espaco",     "Medio", "Qual a complexidade de espaço (memória auxiliar) para o algoritmo Bubble Sort tradicional?\n\n1 - O(1)\n2 - O(n)\n3 - O(log n)\n4 - O(n^2)", '1'},
        {"P016", 2, "Lista Encadeada",           "Remocao",    "Medio", "Para remover o último elemento de uma lista simples (sem ponteiro para o fim), é necessário percorrer toda a lista.\n\nV - Verdadeiro\nF - Falso", 'V'},
        {"P017", 3, "Complexidade",              "Notacao O",  "Medio", "Na notação Big-O, qual destas complexidades de tempo representa o algoritmo mais ineficiente (lento)?\n\n1 - O(n)\n2 - O(n log n)\n3 - O(n^2)\n4 - O(log n)", '3'},
        {"P018", 2, "Lista Duplamente Encadeada","Ponteiros",  "Medio", "Numa lista dupla padrão não-circular, qual o valor do ponteiro 'anterior' do 1º nó e do 'próximo' do último nó?\n\n1 - Zero e Um\n2 - NULL e NULL\n3 - NULL e Head\n4 - Tail e NULL", '2'},
        {"P019", 3, "Complexidade",              "Comparacao", "Medio", "Como regra geral em Ciência da Computação, a complexidade de tempo é sempre maior que a de espaço.\n\nV - Verdadeiro\nF - Falso", 'F'},
        {"P020", 2, "Lista Encadeada",           "Circular",   "Medio", "Em uma lista encadeada circular padrão, o ponteiro '*prox' do último nó aponta para onde?\n\n1 - NULL\n2 - Para si mesmo\n3 - O primeiro nó (Head)\n4 - O penúltimo nó", '3'}
    };

    for(int i = 0; i < tam; i++){
        push(pilha, perguntas_medias[i]);
    }
}

void embaralhaQuestoes(tp_pilha *perguntas){
    srand(time(NULL));

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

    printf("\n[Tema: %s] [Subtema: %s]\n%s\nSua resposta: ", e.tema, e.subtema, e.enunciado);
    scanf(" %c", &alternativa_selecionada);
    while (getchar() != '\n' && !feof(stdin)); // limpa o buffer

    // descarta a pergunta usada na pilha de descartadas para nao repetir
    push(perguntas_descartadas, e);

    if (toupper(alternativa_selecionada) == e.respostaCorreta) {
        return 1; // acertou
    } else {
        return 0; // errou
    }
}

// Versão estendida usada pelo jogadores.h para registrar o historico.
int geraPerguntaComHistorico(tp_pilha *perguntas, tp_pilha *perguntas_descartadas,
                              tp_pergunta *pergunta_saida, char *resp_saida){
    tp_pergunta e;
    char alternativa_selecionada;

    if(pilha_vazia(perguntas)) transfere_perguntas(perguntas, perguntas_descartadas);

    pop(perguntas, &e);

    printf("\n[Tema: %s] [Subtema: %s]\n%s\nSua resposta: ", e.tema, e.subtema, e.enunciado);
    scanf(" %c", &alternativa_selecionada);
    while (getchar() != '\n' && !feof(stdin)); // limpa o buffer

    // devolve a pergunta e a resposta para quem chamou (jogadores.h)
    *pergunta_saida = e;
    *resp_saida     = toupper(alternativa_selecionada);

    // descarta a pergunta usada
    push(perguntas_descartadas, e);

    if (toupper(alternativa_selecionada) == e.respostaCorreta) {
        return 1; // acertou
    } else {
        return 0; // errou
    }
}

void popula_perguntas_d(tp_pilha *pilha){
    int tam = 10;

    tp_pergunta perguntas_dificeis[10] = {
        {"P021", 3, "Arvore BST", "Propriedade", "Dificil", "Numa BST, os nós à esquerda da raiz possuem valores menores e os da direita valores maiores.\n\nV - Verdadeiro\nF - Falso", 'V'},
        {"P022", 3, "Arvore BST", "Percurso", "Dificil", "No percurso in-ordem (infixo) numa BST, os elementos são visitados em qual ordem?\n\n1 - Aleatória\n2 - Crescente\n3 - Decrescente\n4 - De cima para baixo", '2'},
        {"P023", 3, "Arvores",    "Percurso", "Dificil", "Qual é o nome do percurso onde o nó raiz é visitado antes das suas subárvores?\n\n1 - Pré-ordem (Prefixo)\n2 - Decrescente\n3 - Pós-ordem (Sufixo)\n4 - Circular", '1'},
        {"P024", 3, "Arvore AVL", "Balanceamento", "Dificil", "Quais valores o Fator de Balanceamento pode assumir numa árvore AVL equilibrada?\n\n1 - Valores entre -2 e 2\n2 - Apenas -1, 0 ou 1\n3 - Sempre 0\n4 - Qualquer valor positivo", '2'},
        {"P025", 3, "Arvore BST", "Complexidade", "Dificil", "Qual é a pior complexidade de tempo possível na busca de uma Árvore de Busca Binária desbalanceada?\n\n1 - O(n)\n2 - O(n log n)\n3 - O(1)\n4 - O(log n)", '1'},
        {"P026", 3, "Arvore Trie", "Complexidade", "Dificil", "Qual é a complexidade algorítmica de busca numa árvore Trie de caracteres (onde 'm' é o tamanho da palavra)?\n\n1 - O(n)\n2 - O(m)\n3 - O(n log m)\n4 - O(1)", '2'},
        {"P027", 3, "Arvores",    "Conceitos", "Dificil", "Na teoria das árvores, o que significa o 'grau' de um determinado nó?\n\n1 - Distância até à raiz principal\n2 - Quantidade de filhos diretos\n3 - Tamanho ocupado em bytes\n4 - O número de nós pais", '2'},
        {"P028", 3, "Arvore Patricia", "Diferenca", "Dificil", "Qual a diferença estrutural principal que diferencia uma árvore Patricia de uma Trie tradicional?\n\n1 - A Trie apenas armazena números\n2 - Agrupamento de nós vazios\n3 - Compressão/agrupamento de caminhos (caracteres)\n4 - A Patricia é estritamente binária", '3'},
        {"P029", 3, "Arvore Patricia", "Vantagem", "Dificil", "Qual é a maior vantagem prática em utilizar uma árvore Patricia em vez de uma árvore Trie pura?\n\n1 - Melhoria na complexidade assintótica\n2 - Dispensa a utilização de ponteiros\n3 - Economiza e otimiza fortemente a memória\n4 - Não apresenta vantagens práticas reais", '3'},
        {"P030", 3, "Arvore B",   "Chaves", "Dificil", "Se uma árvore B for de 'ordem 4', qual o número máximo de chaves (valores) que um nó poderá guardar?\n\n1 - 2 chaves\n2 - 3 chaves\n3 - 4 chaves\n4 - 5 chaves", '2'}
    };

    for(int i = 0; i < tam; i++){
        push(pilha, perguntas_dificeis[i]);
    }
}

#endif
