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

int main(){
    srand(time(NULL)); // gera a seed para os numeros aleatorios
    int quant;         // quantidade de jogadores

    // declaração da pilha,  fila e lista dinamicas
    tp_pilha *perguntas;
    tp_pilha *perguntas_descartadas;
    tp_fila *jogadores;
    tp_listade *tabuleiro;

    // inicialização das estruturas dinamicas
    perguntas = inicializa_pilha();
    perguntas_descartadas = inicializa_pilha();
    jogadores = inicializa_fila();
    tabuleiro = inicializa_listade();

    // preenche o tabuleiro com a quantidade de casas que a unidade pede (10,20,30)
    inicia_tabuleiro(tabuleiro, 20);

    // popula e embaralha as cartas
    popula_perguntas(perguntas);
    embaralhaQuestoes(perguntas);

    setlocale(LC_ALL, "Portuguese"); // função responsável por adicionar caracteres do PT-BR

    quant = quantidade(); // atribuição da quantidade de jogadores

    // função para colocar os jogadores no tabuleiro
    lerDados(quant, jogadores, tabuleiro);

    rodadaplayer(jogadores, perguntas, perguntas_descartadas); // realiza uma rodada

    // limpar memória
    destroi_pilha(perguntas);
    destroi_pilha(perguntas_descartadas);
    destroi_fila(jogadores);
    while (!listade_vazia(tabuleiro)) {
        remove_listade(tabuleiro, tabuleiro->ini->info.posicao);
    }
    free(tabuleiro);

    return 0;
}
