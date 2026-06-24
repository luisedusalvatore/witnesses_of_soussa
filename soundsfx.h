#ifndef SOUNDSFX_H
#define SOUNDSFX_H

#include "raylib.h"

// variáveis estáticas internas para musica
static Music musica_menu_principal;
static Music musica_creditos;
static Music musica_fim_jogo;
static Music *musica_atual = NULL; // ponteiro para a música ativa no momento

// variáveis estáticas internas para os efeitos sonoros
static Sound som_dados_rolando;
static Sound som_salto_player;
static Sound som_breaking_bad;
static Sound som_merge_sort;

// inicializa o sistema com as musicas e efeitos sonoros
static void InitAudioSystem() {
    InitAudioDevice();

    // load musica
    musica_menu_principal = LoadMusicStream("assets/musica_menu_principal.mp3");
    musica_creditos = LoadMusicStream("assets/musica_creditos.mp3");
    musica_fim_jogo = LoadMusicStream("assets/musica_fim_jogo.mp3");

    som_dados_rolando = LoadSound("assets/som_dados_rolando.wav");
    som_salto_player = LoadSound("assets/som_salto_player.wav");
    som_breaking_bad = LoadSound("assets/som_breaking_bad.wav");
    som_merge_sort = LoadSound("assets/som_merge_sort.wav");
}

// função para trocar de musica de fundo
static void PlayMusicTrack(int trackIndex) {
    // para a musica atual se houver alguma tocando
    if (musica_atual != NULL) StopMusicStream(*musica_atual);

    // define qual sera a nova de acordo com o case
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

    // inicia a reprodução da nova musica
    if (musica_atual != NULL) {
        PlayMusicStream(*musica_atual);
    }
}

// Função para tocar um efeito sonoro
static void PlaySoundEffect(int sfxIndex) {
    // toca o som instantaneamente baseado no case
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

// atualiza a musica ativa (deve ser chamado no loop principal do jogo)
static void UpdateAudioSystem() {
    if (musica_atual != NULL) {
        UpdateMusicStream(*musica_atual);
    }
}

// libera memoria
static void CloseAudioSystem() {
    // descarrega as musicas
    UnloadMusicStream(musica_menu_principal);
    UnloadMusicStream(musica_creditos);
    UnloadMusicStream(musica_fim_jogo);

    // descarrega os efeitos sonoros
    UnloadSound(som_dados_rolando);
    UnloadSound(som_salto_player);
    UnloadSound(som_breaking_bad);
    UnloadSound(som_merge_sort);

    // fecha o dispositivo de áudio
    CloseAudioDevice();
}

#endif
