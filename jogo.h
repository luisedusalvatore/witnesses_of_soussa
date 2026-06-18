#ifndef JOGO_H_INCLUDED
#define JOGO_H_INCLUDED

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <locale.h>
#include <ctype.h>
#include <time.h>
#include "tipos.h"
#include "pilhade.h"
#include "filade.h"
#include "randon.h"
#include "perguntas.h"
#include "easteregg.h"
#include "jogadores.h"
#include "listade.h"
#include "tabuleiro.h"
#include "hank.h"

int roda_jogo(){
    // ==========================================
    // NOVA INICIALIZAÇÃO DO RANKING (AVL)
    // ==========================================
    ArvAVL arvore = NULL;
    carrega_hank(&arvore);

    srand(time(NULL)); // gera a seed para os numeros aleatorios
    int quant;         // quantidade de jogadores

    // declaração da pilha, fila e lista dinamicas
    tp_pilha *perguntas_faceis, *perguntas_medias;
    tp_pilha *perguntas_faceis_descartadas, *perguntas_medias_descartadas;
    tp_fila *jogadores;
    tp_listade *tabuleiro;

    // inicialização das estruturas dinamicas
    perguntas_faceis = inicializa_pilha();
    perguntas_medias = inicializa_pilha();
    perguntas_faceis_descartadas = inicializa_pilha();
    perguntas_medias_descartadas = inicializa_pilha();
    jogadores = inicializa_fila();
    tabuleiro = inicializa_listade();

    // preenche o tabuleiro com a quantidade de casas que decidirmos por no jogo
    inicia_tabuleiro(tabuleiro, 20);

    // popula e embaralha as cartas
    popula_perguntas(perguntas_faceis);
    embaralhaQuestoes(perguntas_faceis);
    popula_perguntas_m(perguntas_medias);
    embaralhaQuestoes(perguntas_medias);

    setlocale(LC_ALL, "Portuguese"); // função responsável por adicionar caracteres do PT-BR

    quant = quantidade(); // atribuição da quantidade de jogadores

    // função para colocar os jogadores no tabuleiro
    lerDados(quant, jogadores, tabuleiro);

    while (getchar() != '\n' && !feof(stdin));

    //  Loop do jogo
    int fim_de_jogo = 0;
    while (!fim_de_jogo) {
        system("cls");

        fim_de_jogo = rodadaplayer(jogadores, tabuleiro, perguntas_faceis, perguntas_medias, perguntas_faceis_descartadas, perguntas_medias_descartadas);

        if (!fim_de_jogo) {
            printf("\nPressione ENTER para passar a vez...");
            getchar();
        }
    }

    // ==========================================
    // LIMPEZA DE MEMÓRIA E SALVAMENTO SEGURO
    // ==========================================
    Player e; // Struct normal alocada na stack (evita o crash!)

    // processo de limpar memoria
    destroi_pilha(perguntas_faceis);
    destroi_pilha(perguntas_faceis_descartadas);
    destroi_pilha(perguntas_medias);
    destroi_pilha(perguntas_medias_descartadas);

    while(!fila_vazia(jogadores)){
        remove_fila(jogadores, &e); // Passa o endereço da struct
        salva_hank(e.dados);        // Salva os dados do jogador atual no txt
    }
    destroi_fila(jogadores);

    while (!listade_vazia(tabuleiro)) {
        remove_listade(tabuleiro, tabuleiro->ini->info.posicao);
    }
    free(tabuleiro);

    // Obs: A árvore AVL será liberada pelo sistema operacional ao fim do programa,
    // mas num cenário ideal futuro você pode chamar um liberaABB(&arvore) aqui.

    return 0;
}

#endif // JOGO_H_INCLUDED
