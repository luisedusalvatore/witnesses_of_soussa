#ifndef SOUNDSFX_H
#define SOUNDSFX_H

#include "raylib.h"

// Variáveis esticas internas
static Music musica_menu_principal;
static Music musica_creditos;
static Music *musica_atual = NULL; // Ponteiro para a música ativa no momento

// Inicializa o sistema com as musicas (pra colocar uma musica insira no aquivo "assets" e coloque o nome do arquivo com a expensão dele)
static void InitAudioSystem() {
    InitAudioDevice();
    musica_menu_principal = LoadMusicStream("assets/musica_menu_principal.mp3");
    musica_creditos = LoadMusicStream("assets/musica_creditos.mp3");
}

// Função para trocar de música, segue o modelo
static void PlayMusicTrack(int trackIndex) {
    // Para a música atual se houver alguma tocando
    if (musica_atual != NULL) StopMusicStream(*musica_atual);

    // Define qual será a nova baseando-se no índice
    
    switch(trackIndex){
        case 0:
            musica_atual = &musica_menu_principal;
            break;
        case 1:
            musica_atual = &musica_creditos;
            break;

    }    
    // Inicia a reprodução da nova música
    PlayMusicStream(*musica_atual);
}

// Atualiza a música ativa (deve ser chamado no loop principal)
static void UpdateAudioSystem() {
    if (musica_atual != NULL) UpdateMusicStream(*musica_atual);
}

// Libera recursos
static void CloseAudioSystem() {
    UnloadMusicStream(musica_menu_principal);
    UnloadMusicStream(musica_creditos);
    CloseAudioDevice();
}

#endif

