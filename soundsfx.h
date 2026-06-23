#ifndef SOUNDSFX_H
#define SOUNDSFX_H

#include "raylib.h"

// Variáveis estáticas internas para Música (Background)
static Music musica_menu_principal;
static Music musica_creditos;
static Music musica_fim_jogo;
static Music *musica_atual = NULL; // Ponteiro para a música ativa no momento

// Variáveis estáticas internas para Efeitos Sonoros (SFX)
static Sound som_dados_rolando;
static Sound som_salto_player;
static Sound som_breaking_bad;
static Sound som_merge_sort;

// Inicializa o sistema com as musicas e efeitos sonoros
static void InitAudioSystem() {
    InitAudioDevice();

    // Carregando Músicas (Streams longas)
    musica_menu_principal = LoadMusicStream("assets/musica_menu_principal.mp3");
    musica_creditos = LoadMusicStream("assets/musica_creditos.mp3");
    musica_fim_jogo = LoadMusicStream("assets/musica_fim_jogo.mp3");
  
    som_dados_rolando = LoadSound("assets/som_dados_rolando.wav");
    som_salto_player = LoadSound("assets/som_salto_player.wav");
    som_breaking_bad = LoadSound("assets/som_breaking_bad.wav");
    som_merge_sort = LoadSound("assets/som_merge_sort.wav");
}

// Função para trocar de música de fundo
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
        case 2:
            musica_atual = &musica_fim_jogo;    
    }
    
    // Inicia a reprodução da nova música
    if (musica_atual != NULL) {
        PlayMusicStream(*musica_atual);
    }
}

// Função para tocar um efeito sonoro (SFX)
static void PlaySoundEffect(int sfxIndex) {
    // Toca o som instantaneamente baseado no índice
    switch(sfxIndex){
        case 0:
            PlaySound(som_dados_rolando);
            break;
        case 1:
            PlaySound(som_salto_player);
            break;
        case 2:
            PlaySound(som_breaking_bad);
            break; 
        case 3:
            PlaySound(som_merge_sort);    
    }
}

// Atualiza a música ativa (deve ser chamado no loop principal do jogo)
static void UpdateAudioSystem() {
    if (musica_atual != NULL) {
        UpdateMusicStream(*musica_atual);
    }
}

// Libera recursos da memória
static void CloseAudioSystem() {
    // Descarrega as Músicas
    UnloadMusicStream(musica_menu_principal);
    UnloadMusicStream(musica_creditos);
    UnloadMusicStream(musica_fim_jogo);

    // Descarrega os Efeitos Sonoros
    UnloadSound(som_dados_rolando);
    UnloadSound(som_salto_player);
    UnloadSound(som_breaking_bad);
    UnloadSound(som_merge_sort);

    // Fecha o dispositivo de áudio
    CloseAudioDevice();
}

#endif