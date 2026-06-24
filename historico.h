#ifndef historico_h
#define historico_h

#include <stdio.h>
#include <string.h>
#include "tipos.h"

// cria o arquivo csv com o cabecalho apenas se ele ainda nao existir.
// usar fopen com "r" primeiro evita apagar o historico de partidas anteriores.
void inicializa_historico() {
    FILE *f = fopen("historico_respostas.csv", "r");
    if (f != NULL) {
        fclose(f);
        return; // arquivo ja existe, nao sobrescreve
    }

    f = fopen("historico_respostas.csv", "w");
    if (f == NULL) {
        printf("[ERRO] Nao foi possivel criar o arquivo de historico.\n");
        return;
    }

    // cabecalho padrao definido pelo PDF do professor
    fprintf(f, "turma;nome_jogador;id_pergunta;unidade;tema;subtema;dificuldade;resposta_jogador;resposta_correta;resultado\n");
    fclose(f);
}

// salva uma linha no csv para cada resposta dada durante o jogo.
// Chamada pelo jogadores.h logo apos o jogador responder.
void salvarHistoricoResposta(char *nome_jogador, tp_pergunta p,
                              char resposta_jogador, char *resultado) {
    FILE *f = fopen("historico_respostas.csv", "a"); // "a" = append, nao apaga o que ja existe
    if (f == NULL) {
        printf("[ERRO] Nao foi possivel abrir o historico para escrita.\n");
        return;
    }

    fprintf(f, "AED-2026.1;%s;%s;%d;%s;%s;%s;%c;%c;%s\n",
        nome_jogador,       // nome do jogador que respondeu
        p.id,               // ex: P001
        p.unidade,          // 1, 2 ou 3
        p.tema,             // ex: Pilha
        p.subtema,          // ex: LIFO
        p.dificuldade,      // Facil ou Medio
        resposta_jogador,   // letra que o jogador digitou
        p.respostaCorreta,  // letra correta
        resultado           // "Acertou" ou "Errou"
    );

    fclose(f);
}

#endif
