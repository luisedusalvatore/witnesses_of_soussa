#include "raylib.h"
#include "jogo.h"
#include <math.h>
#include "soundsfx.h"
#include <time.h>
// baixem a versão 4.2 do raylib pfv
// --------------------------------------------------------------------------
// DEFINIÇÕES E ESTRUTURAS
// --------------------------------------------------------------------------
typedef enum {
    TELA_MENU,
    TELA_CADASTRO,
    TELA_JOGO,
    TELA_INFORMACOES,
    TELA_CREDITOS
} TelaAtual;

// ==========================================
// MÁQUINA DE ESTADOS DO JOGO (Substitui os scanfs)
typedef enum {
    ESTADO_ESPERANDO_DADO,
    ESTADO_ANIMANDO_DADO,
    ESTADO_REROL,
    ESTADO_MOVENDO,
    ESTADO_ACAO_CASA,
    ESTADO_PERGUNTA,
    ESTADO_FIM_TURNO
} EstadoJogo;
// ==========================================
typedef struct {
    Vector2 pos;
    Color cor;
    float velX;
    float velY;
    bool pulou;
    bool caiu;
} Corredor;

typedef struct {
    Vector2 pos;
    float raio;
    float alfa;
    float velPiscar;
} Estrela;

typedef struct {
    Vector2 pos;       // Posição na tela
    Vector2 vel;       // Velocidade de arremesso
    float tamanho;     // Tamanho do dado
    bool segurando;    // Verifica se o mouse está clicando nele
    int valor;         // O número atual da face (1 a 6)
} DadoInterativo;

#define NUM_CORREDORES 4
#define NUM_ESTRELAS 100

// --------------------------------------------------------------------------
// FUNÇÕES AUXILIARES
// --------------------------------------------------------------------------
void DesenharBotaoCentralizado(Rectangle rect, const char *texto, Font fonte, int tamanhoFonte) {
    DrawRectangleRounded(rect, 0.4f, 10, BLACK);
    Vector2 tamanhoTexto = MeasureTextEx(fonte, texto, tamanhoFonte, 1);
    Vector2 posTexto;
    posTexto.x = rect.x + (rect.width - tamanhoTexto.x) / 2.0f;
    posTexto.y = rect.y + (rect.height - tamanhoTexto.y) / 2.0f;
    DrawTextEx(fonte, texto, posTexto, tamanhoFonte, 1, WHITE);
}

void DesenharTextoComBorda(Font fonte, const char *texto, Vector2 pos, float tamanho, float espacamento, Color corTexto, Color corBorda, int grossura) {
    for (int x = -grossura; x <= grossura; x++) {
        for (int y = -grossura; y <= grossura; y++) {
            if (x != 0 || y != 0) {
                DrawTextEx(fonte, texto, (Vector2){pos.x + x, pos.y + y}, tamanho, espacamento, corBorda);
            }
        }
    }
    DrawTextEx(fonte, texto, pos, tamanho, espacamento, corTexto);
}

void ResetarCorredores(Corredor corredores[], float chaoY, float escala) {
    corredores[0] = (Corredor){ {-50 * escala, chaoY}, GREEN,  400 * escala, 0, false, false };
    corredores[1] = (Corredor){ {-180 * escala, chaoY}, BLUE,  360 * escala, 0, false, false };
    corredores[2] = (Corredor){ {-300 * escala, chaoY}, RED,   320 * escala, 0, false, false };
    corredores[3] = (Corredor){ {-420 * escala, chaoY}, YELLOW,280 * escala, 0, false, false };
}

void DesenharDadoFace(float x, float y, float size, int valor, float escala) {
    DrawRectangleRounded((Rectangle){x, y, size, size}, 0.2f, 5, WHITE);
    DrawRectangleRoundedLines((Rectangle){x, y, size, size}, 0.2f, 5, 2, BLACK);

    float p25 = size * 0.25f, p50 = size * 0.50f, p75 = size * 0.75f;
    float r = 4 * escala; // Raio das bolinhas

    if (valor == 1 || valor == 3 || valor == 5) DrawCircle(x + p50, y + p50, r, BLACK); // Centro
    if (valor == 2 || valor == 3 || valor == 4 || valor == 5 || valor == 6) {
        DrawCircle(x + p25, y + p25, r, BLACK); DrawCircle(x + p75, y + p75, r, BLACK); // Diagonais 1
    }
    if (valor == 4 || valor == 5 || valor == 6) {
        DrawCircle(x + p75, y + p25, r, BLACK); DrawCircle(x + p25, y + p75, r, BLACK); // Diagonais 2
    }
    if (valor == 6) {
        DrawCircle(x + p25, y + p50, r, BLACK); DrawCircle(x + p75, y + p50, r, BLACK); // Meios
    }
}

void DesenharCaminhoAED(tp_listade *tabuleiro, float escala, Font fonte) {
    if (tabuleiro == NULL || tabuleiro->ini == NULL) return;

    // Medidas do novo mundo horizontal
    float size = 120 * escala;           // Tamanho da ilha
    float gapX = 400 * escala;           // Distância horizontal entre as casas
    float larguraBioma = 10 * gapX;      // Cada bioma engloba exatas 10 casas
    float centroY = 0.0f;                // O eixo Y central do mundo

    // 1. DESENHAR O FUNDO DOS 3 BIOMAS (Faixas Infinitas com Degradê Horizontal)
    Color corOceano = (Color){ 70, 130, 180, 255 };
    Color corPantano = (Color){ 46, 139, 87, 255 };
    Color corVulcao = (Color){ 139, 0, 0, 255 };

    float fade = 800 * escala; // Largura do "borrão" da transição
    float transicao1 = larguraBioma;
    float transicao2 = larguraBioma * 2.0f;

    // Oceano Sólido (Vai do infinito negativo até começar a transição)
    DrawRectangle(-10000, -10000, 10000 + transicao1 - (fade/2.0f), 20000, corOceano);

    // Degradê: Oceano -> Pântano (Transição Suave)
    DrawRectangleGradientH(transicao1 - (fade/2.0f), -10000, fade, 20000, corOceano, corPantano);

    // Pântano Sólido (O meio do mapa)
    DrawRectangle(transicao1 + (fade/2.0f), -10000, larguraBioma - fade, 20000, corPantano);

    // Degradê: Pântano -> Vulcão (Transição Suave)
    DrawRectangleGradientH(transicao2 - (fade/2.0f), -10000, fade, 20000, corPantano, corVulcao);

    // Vulcão Sólido (Vai do fim da segunda transição até o infinito)
    DrawRectangle(transicao2 + (fade/2.0f), -10000, 20000, 20000, corVulcao);

    tp_no *atual = tabuleiro->ini;

    // 2. DESENHAR AS ILHAS
    while (atual != NULL) {
        int pos = atual->info.posicao - 1; // Índice 0 a 29
        int bioma = pos / 10;              // 0 (Oceano), 1 (Pântano), 2 (Vulcão)

        // Posição no Mundo: Eixo X contínuo, Eixo Y com uma leve onda seno para ficar charmoso!
        float px = (pos * gapX) + (gapX / 2.0f);
        float py = centroY + sinf(pos * 0.8f) * (150 * escala);

        // Cor base da ilha
        DrawCircle(px, py, size, DARKBROWN);
        DrawCircle(px, py, size * 0.8f, LIME);

        // === ÍCONES DOS EVENTOS ===
        if (atual->info.tira_carta == 2) {
            // BAÚ DE ITENS
            DrawRectangle(px - 25*escala, py - 15*escala, 50*escala, 30*escala, GOLD);
            DrawRectangle(px - 25*escala, py - 3*escala, 50*escala, 6*escala, BROWN);
            DrawRectangle(px - 6*escala, py - 6*escala, 12*escala, 12*escala, GRAY);
        }
        else if (atual->info.tira_carta == 1) {
            // EVENTOS DE PERGUNTA
            if (bioma == 0) {
                // REDEMOINHO
                DrawCircleLines(px, py, 25*escala, BLUE);
                DrawCircleLines(px, py, 40*escala, DARKBLUE);
            }
            else if (bioma == 1) {
                // JACARÉ
                DrawCircle(px - 15*escala, py, 12*escala, GREEN);
                DrawCircle(px + 15*escala, py, 12*escala, GREEN);
                DrawCircle(px - 15*escala, py, 5*escala, BLACK);
                DrawCircle(px + 15*escala, py, 5*escala, BLACK);
            }
            else if (bioma == 2) {
                // VULCÃO
                DrawTriangle((Vector2){px, py - 30*escala}, (Vector2){px - 30*escala, py + 20*escala}, (Vector2){px + 30*escala, py + 20*escala}, DARKGRAY);
                DrawCircle(px, py - 30*escala, 12*escala, ORANGE);
            }
        }

        // Número da casa
        const char* numCasa = TextFormat("%d", atual->info.posicao);
        DrawTextEx(fonte, numCasa, (Vector2){px - 15*escala, py + size*1.1f}, 40*escala, 1, WHITE);

        // 3. DESENHAR PEÕES
        float r = 15 * escala;
        if (atual->info.cor[0] == 1) DrawCircle(px - 25*escala, py - 25*escala, r, RED);
        if (atual->info.cor[1] == 1) DrawCircle(px + 25*escala, py - 25*escala, r, GREEN);
        if (atual->info.cor[2] == 1) DrawCircle(px - 25*escala, py + 25*escala, r, BLUE);
        if (atual->info.cor[3] == 1) DrawCircle(px + 25*escala, py + 25*escala, r, YELLOW);

        atual = atual->prox;
    }
}

// --------------------------------------------------------------------------
// FUNÇÃO PRINCIPAL
// --------------------------------------------------------------------------
int main(void) {
    const int screenWidth = 1920;
    const int screenHeight = 1080;

    InitWindow(screenWidth, screenHeight, "Caminho do Conhecimento - Navegação");
    Font fonteOffbit = LoadFontEx("offbit.ttf", 80, 0, 250);
    SetTargetFPS(60);

    InitAudioSystem();
    PlayMusicTrack(0);
    float volumeMaster = 0.3f;
    SetMasterVolume(volumeMaster);

    TelaAtual tela = TELA_MENU;
    Corredor corredores[NUM_CORREDORES];
    Estrela estrelas[NUM_ESTRELAS];
    bool primeiraVez = true;
    srand(time(NULL));

    // ========================================================
    // INTEGRAÇÃO COM O BACKEND: Criando o Tabuleiro na Memória
    tp_listade *tabuleiroFisico = inicializa_listade();
    inicia_tabuleiro(tabuleiroFisico, 30);

    // FILA OFICIAL DE JOGADORES (Substitui os testes antigos)
    tp_fila *filaJogadores = inicializa_fila();

    // VARIÁVEIS DE CONTROLE DA TELA DE CADASTRO
    int qtdJogadoresConfig = 0;
    int jogadorAtualCadastro = 1;
    int estadoCadastro = 0; // 0 = Qtd, 1 = Nome, 2 = Cor
    char nomeDigitado[30] = "\0";
    int numLetras = 0;
    bool coresUsadas[4] = {false, false, false, false}; // R, G, B, Y
    char nomesUsados[4][30];
    bool erroNome = false;

    // ========================================================
    // VARIÁVEIS DA MÁQUINA DE ESTADOS E SISTEMA DE PERGUNTAS
    Player jogadorDaVez;
    bool turnoIniciado = false;
    EstadoJogo estadoJogoAtual = ESTADO_ESPERANDO_DADO;

    DadoInterativo dadoJogo = { {0,0}, {0,0}, 80.0f, false, 6 };
    char mensagemSistema[200] = "O jogo vai comecar!";

    // PILHAS DE PERGUNTAS DO BACKEND
    tp_pilha *perguntas_faceis = inicializa_pilha();
    tp_pilha *perguntas_medias = inicializa_pilha();
    tp_pilha *perguntas_faceis_descartadas = inicializa_pilha();
    tp_pilha *perguntas_medias_descartadas = inicializa_pilha();
    popula_perguntas(perguntas_faceis);
    embaralhaQuestoes(perguntas_faceis);
    popula_perguntas_m(perguntas_medias);
    embaralhaQuestoes(perguntas_medias);

    // VARIÁVEIS DA HUD DE PERGUNTA
    tp_pergunta perguntaAtual;
    int subEstadoPergunta = 0;   // 0 = Lendo, 1 = Resultado
    bool acertouPergunta = false;
    int casasModificadas = 0;    // Quantas casas avança ou volta
    int itemSorteado = 0;        // NOVO: Guarda o ID do item pego no bau
    // ========================================================


    // VARIÁVEIS DO MENU PRINCIPAL (Que tinham sido apagadas)
    float posNuvem1 = screenWidth * 0.25f;
    float posNuvem2 = screenWidth * 0.76f;
    float creditosY = screenHeight;



    // --- FRASES DO SPLASH (Estilo Minecraft) ---
    const char* frasesSplash[] = {
        "Vai ficar abstrato!!!",
        "O jogo é uma pilha",
        "Feito no SENAI CIMATEC!",
        "Soussa deixa eu passar em AED?",
        "Talvez crashe",
        "Cuidado com o buraco, vermelho!",
        "C > Python",
        "O merge sort"
    };
    int totalFrasesSplash = 7;
    const char* splashEscolhido = ""; // Variável que guardará a frase sorteada

    // Procure pela linha: const char* splashEscolhido = "";
    // Adicione isto logo abaixo:
    DadoInterativo dadosInfo[2];
    bool dadosIniciados = false;

    const char* linhasCreditos[] = {
        "CAMINHO DO CONHECIMENTO",
        "Trabalho semestral de A.E.D",
        "By: Testemunhas de Soussa",
        "",
        "Desenvolvedores:",
        "Luis Eduardo de Jesus Peres de Oliveira Salvatore",
        "Eduardo Costa Bloisi",
        "Lucas Monteiro de Lima",
        "Carlos Monteiro Neto",
        "André Victor da Silva Oliveira",
        "",
        "Menções honrosas:",
        "Márcio Renê Brandão Soussa",
        "",
        "Agradecimentos:",
        "Formigão (cachorro de Eduardo)",
        "Pretinho (outro cachorro de Eduardo)",
        "Betha (cadela de Luis)",
        "Hulk (cachorro de Carlos)",
        "Groot (cachorro de André)",
        "Gefferson Francisco (Etios de Luis)",
        "",
        "Instituição:",
        "SENAI CIMATEC - SALVADOR - BAHIA"
    };
    int totalLinhasCreditos = 24;

    // ========================================================
    // CÂMERA 2D DO JOGO (Para navegação estilo mapa)
    Camera2D camera = { 0 };
    camera.target = (Vector2){ 0.0f, 0.0f };
    camera.offset = (Vector2){ screenWidth / 2.0f, screenHeight / 2.0f }; // Centraliza o foco
    camera.rotation = 0.0f;
    camera.zoom = 1.0f;
    // ========================================================

    while (!WindowShouldClose()) {

        int larguraAtual = GetScreenWidth();
        int alturaAtual = GetScreenHeight();
        float escala = (float)alturaAtual / 600.0f;
        float dt = GetFrameTime();

        float chaoY = alturaAtual * 0.80f;
        float tamanhoQuadrado = 50 * escala;
        float gapInicio = larguraAtual * 0.40f;
        float gapFim = larguraAtual * 0.60f;

        float mesaW = larguraAtual * 0.7f;
        float mesaH = alturaAtual * 0.4f;
        float mesaX = (larguraAtual - mesaW) / 2.0f;
        float mesaY = alturaAtual * 0.55f;

        if (primeiraVez) {
            ResetarCorredores(corredores, chaoY - tamanhoQuadrado, escala);
            for (int i = 0; i < NUM_ESTRELAS; i++) {
                estrelas[i].pos.x = GetRandomValue(0, 3000);
                estrelas[i].pos.y = GetRandomValue(0, 3000);
                estrelas[i].raio = GetRandomValue(1, 3);
                estrelas[i].alfa = GetRandomValue(10, 100) / 100.0f;
                estrelas[i].velPiscar = GetRandomValue(5, 15) * 0.1f;
            }
            splashEscolhido = frasesSplash[GetRandomValue(0, totalFrasesSplash - 1)];
            primeiraVez = false;
        }

        float btnLargo = 200.0f * escala;
        float btnAlto = 60.0f * escala;
        Rectangle btnIniciar  = { larguraAtual / 2.0f - btnLargo / 2.0f, alturaAtual * 0.45f, btnLargo, btnAlto };
        Rectangle btnInfo     = { larguraAtual / 2.0f - btnLargo / 2.0f, alturaAtual * 0.58f, btnLargo, btnAlto };
        Rectangle btnCreditos = { larguraAtual / 2.0f - btnLargo / 2.0f, alturaAtual * 0.71f, btnLargo, btnAlto };
        Rectangle btnSair     = { larguraAtual - (120.0f * escala), 20.0f * escala, 100.0f * escala, 50.0f * escala };
        Rectangle btnVoltar   = { 20.0f * escala, 20.0f * escala, 120.0f * escala, 50.0f * escala };
        Rectangle btnVolMenos = { 20.0f * escala, alturaAtual - (70.0f * escala), 50.0f * escala, 50.0f * escala };
        Rectangle btnVolMais  = { 180.0f * escala, alturaAtual - (70.0f * escala), 50.0f * escala, 50.0f * escala };

        int tamFonteTitulo = 50 * escala;
        int tamFonteBotoes = 30 * escala;
        int tamFonteTexto  = 20 * escala;

        Vector2 mousePoint = GetMousePosition();
        bool clique = IsMouseButtonPressed(MOUSE_BUTTON_LEFT);

        // ==========================================
        // SWITCH 1: LÓGICA (UPDATE)
        // ==========================================
        switch (tela) {
            case TELA_MENU:
                if (clique) {
                    if (CheckCollisionPointRec(mousePoint, btnIniciar)) {
                        tela = TELA_CADASTRO;
                        estadoCadastro = 0;
                        jogadorAtualCadastro = 1;
                        numLetras = 0;
                        nomeDigitado[0] = '\0';
                        for(int c = 0; c < 4; c++) coresUsadas[c] = false;

                        // ========================================================
                        // CORREÇÃO DO BUG DOS FANTASMAS (Limpeza Total de Memória)
                        // ========================================================
                        // 1. Esvazia a fila de jogadores caso alguém tenha desistido no meio do cadastro
                        Player lixo;
                        while(!fila_vazia(filaJogadores)){
                            remove_fila(filaJogadores, &lixo);
                        }

                        // 2. Limpa as cores de todas as casas do tabuleiro
                        tp_no *atu = tabuleiroFisico->ini;
                        while(atu != NULL) {
                            atu->info.cor[0] = 0;
                            atu->info.cor[1] = 0;
                            atu->info.cor[2] = 0;
                            atu->info.cor[3] = 0;
                            atu = atu->prox;
                        }

                        // 3. Reseta a Máquina de Estados para um novo jogo limpo
                        turnoIniciado = false;
                        estadoJogoAtual = ESTADO_ESPERANDO_DADO;
                        strcpy(mensagemSistema, "O jogo vai comecar!");
                        // ========================================================
                    }
                    else if (CheckCollisionPointRec(mousePoint, btnInfo)) tela = TELA_INFORMACOES;
                    else if (CheckCollisionPointRec(mousePoint, btnCreditos)) tela = TELA_CREDITOS;
                    else if (CheckCollisionPointRec(mousePoint, btnSair)) return 0;
                    else if (CheckCollisionPointRec(mousePoint, btnVolMenos)) {
                        volumeMaster -= 0.1f;
                        if (volumeMaster < 0.0f) volumeMaster = 0.0f;
                        SetMasterVolume(volumeMaster);
                    }
                    else if (CheckCollisionPointRec(mousePoint, btnVolMais)) {
                        volumeMaster += 0.1f;
                        if (volumeMaster > 1.0f) volumeMaster = 1.0f;
                        SetMasterVolume(volumeMaster);
                    }
                }
                posNuvem1 -= 30.0f * escala * dt;
                posNuvem2 -= 50.0f * escala * dt;
                if (posNuvem1 < -150 * escala) posNuvem1 = larguraAtual + 150 * escala;
                if (posNuvem2 < -150 * escala) posNuvem2 = larguraAtual + 150 * escala;

                bool todosTerminaram = true;
                float gravidade = 1500.0f * escala;
                float forcaPulo = -800.0f * escala;

                for (int i = 0; i < NUM_CORREDORES; i++) {
                    corredores[i].pos.x += corredores[i].velX * dt;
                    if (corredores[i].pos.x + (tamanhoQuadrado / 2.0f) > gapInicio && corredores[i].pos.x < gapFim) {
                        if (corredores[i].cor.r == RED.r && corredores[i].cor.g == RED.g && !corredores[i].caiu) {
                            corredores[i].caiu = true;
                        } else if (!corredores[i].pulou && !corredores[i].caiu) {
                            corredores[i].velY = forcaPulo;
                            corredores[i].pulou = true;
                        }
                    }
                    if (corredores[i].pulou || corredores[i].caiu) {
                        corredores[i].velY += gravidade * dt;
                        corredores[i].pos.y += corredores[i].velY * dt;
                        if (corredores[i].pulou && corredores[i].pos.y >= chaoY - tamanhoQuadrado) {
                            if (corredores[i].pos.x + (tamanhoQuadrado / 2.0f) > gapFim) {
                                corredores[i].pos.y = chaoY - tamanhoQuadrado;
                                corredores[i].velY = 0;
                                corredores[i].pulou = false;
                            }
                        }
                    }
                    if (corredores[i].pos.x < larguraAtual + 100 && corredores[i].pos.y < alturaAtual + 100) {
                        todosTerminaram = false;
                    }
                }
                if (todosTerminaram) ResetarCorredores(corredores, chaoY - tamanhoQuadrado, escala);
                break;

            case TELA_CADASTRO:
                if (clique && CheckCollisionPointRec(mousePoint, btnVoltar)) tela = TELA_MENU;
                if (estadoCadastro == 0) {
                    Rectangle btn2 = {larguraAtual/2.0f - 150*escala, alturaAtual/2.0f, 80*escala, 80*escala};
                    Rectangle btn3 = {larguraAtual/2.0f - 40*escala, alturaAtual/2.0f, 80*escala, 80*escala};
                    Rectangle btn4 = {larguraAtual/2.0f + 70*escala, alturaAtual/2.0f, 80*escala, 80*escala};
                    if (clique) {
                        if (CheckCollisionPointRec(mousePoint, btn2)) { qtdJogadoresConfig = 2; estadoCadastro = 1; }
                        else if (CheckCollisionPointRec(mousePoint, btn3)) { qtdJogadoresConfig = 3; estadoCadastro = 1; }
                        else if (CheckCollisionPointRec(mousePoint, btn4)) { qtdJogadoresConfig = 4; estadoCadastro = 1; }
                    }
                } else if (estadoCadastro == 1) {
                    int key = GetCharPressed();
                    while (key > 0) {
                        if ((key >= 32) && (key <= 125) && (numLetras < 20)) {
                            nomeDigitado[numLetras] = (char)key;
                            nomeDigitado[numLetras+1] = '\0';
                            numLetras++;
                        }
                        key = GetCharPressed();
                    }
                    if (IsKeyPressed(KEY_BACKSPACE)) {
                        numLetras--;
                        if (numLetras < 0) numLetras = 0;
                        nomeDigitado[numLetras] = '\0';
                        erroNome = false;
                    }
                    if (key > 0) erroNome = false;

                    Rectangle btnAvancar = {larguraAtual/2.0f - 100*escala, alturaAtual*0.65f, 200*escala, 60*escala};
                    if (clique && CheckCollisionPointRec(mousePoint, btnAvancar) && numLetras > 0) {
                        bool nomeRepetido = false;
                        for (int i = 0; i < jogadorAtualCadastro - 1; i++) {
                            if (strcmp(nomeDigitado, nomesUsados[i]) == 0) {
                                nomeRepetido = true;
                                break;
                            }
                        }
                        if (nomeRepetido) {
                            erroNome = true;
                        } else {
                            estadoCadastro = 2;
                            erroNome = false;
                        }
                    }
                } else if (estadoCadastro == 2) {
                    Rectangle btnCores[4] = {
                        {larguraAtual/2.0f - 220*escala, alturaAtual/2.0f, 100*escala, 100*escala},
                        {larguraAtual/2.0f - 110*escala, alturaAtual/2.0f, 100*escala, 100*escala},
                        {larguraAtual/2.0f +   0*escala, alturaAtual/2.0f, 100*escala, 100*escala},
                        {larguraAtual/2.0f + 110*escala, alturaAtual/2.0f, 100*escala, 100*escala}
                    };
                    if (clique) {
                        int corEscolhida = 0;
                        for(int c = 0; c < 4; c++) {
                            if (!coresUsadas[c] && CheckCollisionPointRec(mousePoint, btnCores[c])) {
                                corEscolhida = c + 1;
                                coresUsadas[c] = true;
                                break;
                            }
                        }
                        if (corEscolhida > 0) {
                            Player novoJogador;
                            strcpy(novoJogador.nome, nomeDigitado);
                            strcpy(nomesUsados[jogadorAtualCadastro - 1], nomeDigitado);
                            novoJogador.cor = corEscolhida;

                            inicializa_posicao(&novoJogador, tabuleiroFisico);
                            tabuleiroFisico->ini->info.cor[corEscolhida - 1] = 1;

                            novoJogador.qtd_itens = 0;
                            novoJogador.status_imune = 0;
                            novoJogador.status_castigo = 0;
                            novoJogador.status_tudo_nada = 0;
                            novoJogador.status_rerol = 0;
                            for(int j = 0; j < 3; j++) novoJogador.inventario[j] = 0;
                            inicializa_hank(&novoJogador);

                            insere_fila(filaJogadores, novoJogador);

                            jogadorAtualCadastro++;
                            if (jogadorAtualCadastro > qtdJogadoresConfig) {
                                tela = TELA_JOGO;
                            } else {
                                estadoCadastro = 1;
                                nomeDigitado[0] = '\0';
                                numLetras = 0;
                            }
                        }
                    }
                }
                break;

            case TELA_CREDITOS:
                if (clique && CheckCollisionPointRec(mousePoint, btnVoltar)) tela = TELA_MENU;
                creditosY -= 60.0f * escala * dt;
                float alturaTotalTexto = 0;
                for (int i = 0; i < totalLinhasCreditos; i++) {
                    int tamanhoDaVez = (i == 0 || i == 4 || i == 11 || i == 14 || i == 18) ? 40 * escala : 25 * escala;
                    Vector2 tamTexto = MeasureTextEx(fonteOffbit, linhasCreditos[i], tamanhoDaVez, 1);
                    alturaTotalTexto += tamTexto.y + (45.0f * escala);
                }
                if (creditosY + alturaTotalTexto < 0) creditosY = alturaAtual;
                for (int i = 0; i < NUM_ESTRELAS; i++) {
                    estrelas[i].alfa += estrelas[i].velPiscar * dt;
                    if (estrelas[i].alfa >= 1.0f) { estrelas[i].alfa = 1.0f; estrelas[i].velPiscar *= -1.0f; }
                    else if (estrelas[i].alfa <= 0.1f) { estrelas[i].alfa = 0.1f; estrelas[i].velPiscar *= -1.0f; }
                }
                break;

            case TELA_JOGO:
                if (clique && CheckCollisionPointRec(mousePoint, btnVoltar)) tela = TELA_MENU;

                if (IsMouseButtonDown(MOUSE_BUTTON_RIGHT)) {
                    Vector2 delta = GetMouseDelta();
                    camera.target.x -= delta.x / camera.zoom;
                    camera.target.y -= delta.y / camera.zoom;
                }

                {
                    float wheel = GetMouseWheelMove();
                    if (wheel != 0) {
                        Vector2 mouseWorldPos = GetScreenToWorld2D(mousePoint, camera);
                        Vector2 mouseWorldPosNew;
                        camera.zoom += (wheel * 0.1f);
                        if (camera.zoom < 0.2f) camera.zoom = 0.2f;
                        if (camera.zoom > 3.0f) camera.zoom = 3.0f;
                        mouseWorldPosNew = GetScreenToWorld2D(mousePoint, camera);
                        camera.target.x -= (mouseWorldPosNew.x - mouseWorldPos.x);
                        camera.target.y -= (mouseWorldPosNew.y - mouseWorldPos.y);
                    }
                }

                {
                    float gapX = 400 * escala;
                    float limiteMinX = -200 * escala;
                    float limiteMaxX = (30 * gapX) + (200 * escala);
                    float limiteMinY = -600 * escala;
                    float limiteMaxY = 600 * escala;
                    if (camera.target.x < limiteMinX) camera.target.x = limiteMinX;
                    if (camera.target.x > limiteMaxX) camera.target.x = limiteMaxX;
                    if (camera.target.y < limiteMinY) camera.target.y = limiteMinY;
                    if (camera.target.y > limiteMaxY) camera.target.y = limiteMaxY;
                }

                if (!turnoIniciado && !fila_vazia(filaJogadores)) {
                    remove_fila(filaJogadores, &jogadorDaVez);
                    if (jogadorDaVez.status_castigo == 1) {
                        jogadorDaVez.status_castigo = 0;
                        sprintf(mensagemSistema, "[CASTIGO] %s perdeu a vez!", jogadorDaVez.nome);
                        estadoJogoAtual = ESTADO_FIM_TURNO;
                    } else {
                        dadoJogo.tamanho = 80 * escala;
                        dadoJogo.pos = (Vector2){larguraAtual - 150*escala, alturaAtual - 150*escala};
                        dadoJogo.vel = (Vector2){0,0};
                        dadoJogo.segurando = false;
                        if (jogadorDaVez.status_imune == 1) jogadorDaVez.status_imune = 0;
                        sprintf(mensagemSistema, "Vez de %s! Role o dado.", jogadorDaVez.nome);
                        estadoJogoAtual = ESTADO_ESPERANDO_DADO;
                    }
                    turnoIniciado = true;
                }

                {
                    float bandejaX = larguraAtual - 300*escala;
                    float bandejaY = alturaAtual - 300*escala;

                    if (estadoJogoAtual == ESTADO_ESPERANDO_DADO) {
                        int i;
                        for (i=0; i<3; i++) {
                            Rectangle slot = { 40*escala + i*(50*escala), 135*escala, 40*escala, 40*escala };
                            if (clique && CheckCollisionPointRec(mousePoint, slot) && jogadorDaVez.inventario[i] != 0) {
                                int idItem = jogadorDaVez.inventario[i];
                                jogadorDaVez.inventario[i] = 0;
                                jogadorDaVez.qtd_itens--;

                                if (idItem == 1) {
                                    jogadorDaVez.status_rerol = 1;
                                    strcpy(mensagemSistema, "[ITEM] Rerol ativado! Poderar rolar de novo.");
                                } else if (idItem == 3) {
                                    jogadorDaVez.status_tudo_nada = 1;
                                    strcpy(mensagemSistema, "[ITEM] Tudo ou Nada ativado! Risco dobrado!");
                                } else if (idItem == 4) {
                                    int item_roubado = 0;
                                    char nome_vitima[30] = "";
                                    tp_fila *fila_aux = inicializa_fila();
                                    Player alvo;
                                    int j, k;

                                    while(!fila_vazia(filaJogadores)){
                                        remove_fila(filaJogadores, &alvo);
                                        if(item_roubado == 0 && alvo.qtd_itens > 0) {
                                            if (alvo.status_imune == 1) {
                                                alvo.status_imune = 0;
                                            } else {
                                                for(j=0; j<3; j++) {
                                                    if(alvo.inventario[j] != 0) {
                                                        item_roubado = alvo.inventario[j];
                                                        alvo.inventario[j] = 0;
                                                        alvo.qtd_itens--;
                                                        strcpy(nome_vitima, alvo.nome);
                                                        break;
                                                    }
                                                }
                                            }
                                        }
                                        insere_fila(fila_aux, alvo);
                                    }
                                    while(!fila_vazia(fila_aux)){
                                        remove_fila(fila_aux, &alvo);
                                        insere_fila(filaJogadores, alvo);
                                    }
                                    destroi_fila(fila_aux);

                                    if(item_roubado != 0) {
                                        for(k=0; k<3; k++) {
                                            if(jogadorDaVez.inventario[k] == 0) {
                                                jogadorDaVez.inventario[k] = item_roubado;
                                                jogadorDaVez.qtd_itens++;
                                                break;
                                            }
                                        }
                                        sprintf(mensagemSistema, "[ITEM] Roubou um item de %s!", nome_vitima);
                                    } else {
                                        strcpy(mensagemSistema, "[ITEM] Ninguem tem itens pra roubar!");
                                    }

                                } else if (idItem == 5) {
                                    tp_fila *fila_aux = inicializa_fila();
                                    Player alvo;
                                    bool trocou = false;
                                    tp_no *temp;

                                    while(!fila_vazia(filaJogadores)){
                                        remove_fila(filaJogadores, &alvo);
                                        if(!trocou) {
                                            if (alvo.status_imune == 1) {
                                                alvo.status_imune = 0;
                                                sprintf(mensagemSistema, "[ITEM] %s defendeu (Imune)!", alvo.nome);
                                            } else {
                                                jogadorDaVez.posicao->info.cor[jogadorDaVez.cor - 1] = 0;
                                                alvo.posicao->info.cor[alvo.cor - 1] = 0;
                                                temp = jogadorDaVez.posicao;
                                                jogadorDaVez.posicao = alvo.posicao;
                                                alvo.posicao = temp;
                                                jogadorDaVez.posicao->info.cor[jogadorDaVez.cor - 1] = 1;
                                                alvo.posicao->info.cor[alvo.cor - 1] = 1;
                                                sprintf(mensagemSistema, "[ITEM] ZUUM! Trocou com %s!", alvo.nome);
                                                trocou = true;
                                            }
                                        }
                                        insere_fila(fila_aux, alvo);
                                    }
                                    while(!fila_vazia(fila_aux)){
                                        remove_fila(fila_aux, &alvo);
                                        insere_fila(filaJogadores, alvo);
                                    }
                                    destroi_fila(fila_aux);

                                } else if (idItem == 6) {
                                    jogadorDaVez.status_imune = 1;
                                    strcpy(mensagemSistema, "[ITEM] Escudo de Imunidade ativado!");
                                } else if (idItem == 7) {
                                    tp_fila *fila_aux = inicializa_fila();
                                    Player alvo;
                                    bool castigou = false;

                                    while(!fila_vazia(filaJogadores)){
                                        remove_fila(filaJogadores, &alvo);
                                        if(!castigou) {
                                            if (alvo.status_imune == 1) {
                                                alvo.status_imune = 0;
                                                sprintf(mensagemSistema, "[ITEM] %s defendeu o Castigo!", alvo.nome);
                                            } else {
                                                alvo.status_castigo = 1;
                                                sprintf(mensagemSistema, "[ITEM] %s castigado (pula vez)!", alvo.nome);
                                            }
                                            castigou = true;
                                        }
                                        insere_fila(fila_aux, alvo);
                                    }
                                    while(!fila_vazia(fila_aux)){
                                        remove_fila(fila_aux, &alvo);
                                        insere_fila(filaJogadores, alvo);
                                    }
                                    destroi_fila(fila_aux);
                                }
                            }
                        }

                        {
                            Rectangle recDado = { dadoJogo.pos.x, dadoJogo.pos.y, dadoJogo.tamanho, dadoJogo.tamanho };
                            if (clique && CheckCollisionPointRec(mousePoint, recDado)) dadoJogo.segurando = true;
                            if (dadoJogo.segurando) {
                                Vector2 delta = GetMouseDelta();
                                dadoJogo.pos.x += delta.x;
                                dadoJogo.pos.y += delta.y;

                                if (dadoJogo.pos.x < bandejaX) dadoJogo.pos.x = bandejaX;
                                if (dadoJogo.pos.x + dadoJogo.tamanho > larguraAtual) dadoJogo.pos.x = larguraAtual - dadoJogo.tamanho;
                                if (dadoJogo.pos.y < bandejaY) dadoJogo.pos.y = bandejaY;
                                if (dadoJogo.pos.y + dadoJogo.tamanho > alturaAtual) dadoJogo.pos.y = alturaAtual - dadoJogo.tamanho;

                                dadoJogo.vel.x = delta.x * 15.0f;
                                dadoJogo.vel.y = delta.y * 15.0f;

                                if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT)) {
                                    dadoJogo.segurando = false;
                                    if (fabs(dadoJogo.vel.x) > 50 || fabs(dadoJogo.vel.y) > 50) {
                                        estadoJogoAtual = ESTADO_ANIMANDO_DADO;
                                        strcpy(mensagemSistema, "Rolando...");
                                    }
                                }
                            }
                        }

                    } else if (estadoJogoAtual == ESTADO_ANIMANDO_DADO) {
                        dadoJogo.pos.x += dadoJogo.vel.x * dt;
                        dadoJogo.pos.y += dadoJogo.vel.y * dt;
                        dadoJogo.vel.x *= 0.95f;
                        dadoJogo.vel.y *= 0.95f;

                        if (dadoJogo.pos.x < bandejaX) { dadoJogo.pos.x = bandejaX; dadoJogo.vel.x *= -0.8f; }
                        if (dadoJogo.pos.x + dadoJogo.tamanho > larguraAtual) { dadoJogo.pos.x = larguraAtual - dadoJogo.tamanho; dadoJogo.vel.x *= -0.8f; }
                        if (dadoJogo.pos.y < bandejaY) { dadoJogo.pos.y = bandejaY; dadoJogo.vel.y *= -0.8f; }
                        if (dadoJogo.pos.y + dadoJogo.tamanho > alturaAtual) { dadoJogo.pos.y = alturaAtual - dadoJogo.tamanho; dadoJogo.vel.y *= -0.8f; }

                        if ((fabs(dadoJogo.vel.x) > 40.0f || fabs(dadoJogo.vel.y) > 40.0f) && GetRandomValue(0,10) > 7) {
                            dadoJogo.valor = GetRandomValue(1, 6);
                        }

                        if (fabs(dadoJogo.vel.x) < 5.0f && fabs(dadoJogo.vel.y) < 5.0f) {
                            dadoJogo.vel = (Vector2){0,0};
                            sprintf(mensagemSistema, "%s tirou %d!", jogadorDaVez.nome, dadoJogo.valor);

                            if (jogadorDaVez.status_rerol == 1) {
                                estadoJogoAtual = ESTADO_REROL;
                            } else {
                                estadoJogoAtual = ESTADO_MOVENDO;
                            }
                        }
                    }
                }

                if (estadoJogoAtual == ESTADO_REROL) {
                    Rectangle btnSim = {larguraAtual/2.0f - 150*escala, alturaAtual/2.0f + 50*escala, 120*escala, 60*escala};
                    Rectangle btnNao = {larguraAtual/2.0f + 30*escala, alturaAtual/2.0f + 50*escala, 120*escala, 60*escala};

                    if (clique && CheckCollisionPointRec(mousePoint, btnSim)) {
                        jogadorDaVez.status_rerol = 0;
                        dadoJogo.vel = (Vector2){800*escala, -800*escala};
                        estadoJogoAtual = ESTADO_ANIMANDO_DADO;
                        strcpy(mensagemSistema, "Rolando novamente...");
                    }
                    if (clique && CheckCollisionPointRec(mousePoint, btnNao)) {
                        jogadorDaVez.status_rerol = 0;
                        estadoJogoAtual = ESTADO_MOVENDO;
                    }
                }

                if (estadoJogoAtual == ESTADO_MOVENDO) {
                    int venceu = move_posicao(&jogadorDaVez, dadoJogo.valor);

                    if (venceu) {
                        sprintf(mensagemSistema, "PARABENS! %s VENCEU O JOGO!", jogadorDaVez.nome);
                        estadoJogoAtual = ESTADO_FIM_TURNO;
                    } else {
                        int tipo = jogadorDaVez.posicao->info.tira_carta;

                        if (tipo == 1) {
                            int posAtual = jogadorDaVez.posicao->info.posicao;
                            tp_pilha *pilhaAtiva = NULL;
                            tp_pilha *pilhaDescarte = NULL;
                            char formatado[600] = "";
                            char temp[300];
                            char *token;
                            char linhaCorrente[300] = "";

                            estadoJogoAtual = ESTADO_PERGUNTA;
                            subEstadoPergunta = 0;

                            if (posAtual <= 9) {
                                pilhaAtiva = perguntas_faceis;
                                pilhaDescarte = perguntas_faceis_descartadas;
                                casasModificadas = 1;
                            } else if (posAtual <= 20) {
                                pilhaAtiva = perguntas_medias;
                                pilhaDescarte = perguntas_medias_descartadas;
                                casasModificadas = 2;
                            } else {
                                pilhaAtiva = perguntas_medias;
                                pilhaDescarte = perguntas_medias_descartadas;
                                casasModificadas = 3;
                            }

                            if (pilha_vazia(pilhaAtiva)) transfere_perguntas(pilhaAtiva, pilhaDescarte);

                            pop(pilhaAtiva, &perguntaAtual);

                            strcpy(temp, perguntaAtual.enunciado);
                            token = strtok(temp, " ");

                            while (token != NULL) {
                                char teste[300];
                                strcpy(teste, linhaCorrente);
                                strcat(teste, token);
                                strcat(teste, " ");

                                if (strlen(teste) > 40) {
                                    strcat(formatado, linhaCorrente);
                                    strcat(formatado, "\n");
                                    strcpy(linhaCorrente, token);
                                    strcat(linhaCorrente, " ");
                                } else {
                                    strcpy(linhaCorrente, teste);
                                }
                                token = strtok(NULL, " ");
                            }
                            strcat(formatado, linhaCorrente);
                            strcpy(perguntaAtual.enunciado, formatado);

                        } else if (tipo == 2) {
                            if (jogadorDaVez.qtd_itens < 3) {
                                int i;
                                itemSorteado = sortear_item();
                                for (i = 0; i < 3; i++) {
                                    if (jogadorDaVez.inventario[i] == 0) {
                                        jogadorDaVez.inventario[i] = itemSorteado;
                                        jogadorDaVez.qtd_itens++;
                                        break;
                                    }
                                }
                                estadoJogoAtual = ESTADO_ACAO_CASA;
                            } else {
                                strcat(mensagemSistema, " (Bau! Mas inventario cheio)");
                                estadoJogoAtual = ESTADO_FIM_TURNO;
                            }
                        } else {
                            estadoJogoAtual = ESTADO_FIM_TURNO;
                        }
                    }
                }
                else if (estadoJogoAtual == ESTADO_ACAO_CASA) {
                    Rectangle btnCont = {larguraAtual/2.0f - 100*escala, alturaAtual/2.0f + 70*escala, 200*escala, 60*escala};
                    if (clique && CheckCollisionPointRec(mousePoint, btnCont)) {
                        estadoJogoAtual = ESTADO_FIM_TURNO;
                    }
                }
                else if (estadoJogoAtual == ESTADO_PERGUNTA) {
                    if (subEstadoPergunta == 0) {
                        int key = GetKeyPressed();
                        char respostaEscolhida = '\0';

                        if (key == KEY_ONE || key == KEY_KP_1) respostaEscolhida = '1';
                        else if (key == KEY_TWO || key == KEY_KP_2) respostaEscolhida = '2';
                        else if (key == KEY_THREE || key == KEY_KP_3) respostaEscolhida = '3';
                        else if (key == KEY_FOUR || key == KEY_KP_4) respostaEscolhida = '4';
                        else if (key == KEY_V) respostaEscolhida = 'V';
                        else if (key == KEY_F) respostaEscolhida = 'F';

                        if (respostaEscolhida != '\0') {
                            int casasReais = casasModificadas;
                            if (jogadorDaVez.status_tudo_nada == 1) casasReais *= 2;

                            if (toupper(respostaEscolhida) == perguntaAtual.resposta) {
                                acertouPergunta = true;
                                atualiza_hank(&jogadorDaVez, casasReais * 10);
                                move_posicao(&jogadorDaVez, casasReais);
                            } else {
                                acertouPergunta = false;
                                atualiza_hank(&jogadorDaVez, -casasReais * 10);
                                move_posicao(&jogadorDaVez, -casasReais);
                            }

                            jogadorDaVez.status_tudo_nada = 0;

                            if (casasModificadas == 1) push(perguntas_faceis_descartadas, perguntaAtual);
                            else push(perguntas_medias_descartadas, perguntaAtual);

                            subEstadoPergunta = 1;
                        }
                    } else if (subEstadoPergunta == 1) {
                        Rectangle btnCont = {larguraAtual/2.0f - 100*escala, alturaAtual/2.0f + 150*escala, 200*escala, 60*escala};
                        if (clique && CheckCollisionPointRec(mousePoint, btnCont)) {
                            estadoJogoAtual = ESTADO_FIM_TURNO;
                        }
                    }
                }
                else if (estadoJogoAtual == ESTADO_FIM_TURNO) {
                    Rectangle btnPassar = {larguraAtual/2.0f - 150*escala, alturaAtual*0.85f, 300*escala, 60*escala};
                    if (clique && CheckCollisionPointRec(mousePoint, btnPassar)) {
                        insere_fila(filaJogadores, jogadorDaVez);
                        turnoIniciado = false;
                    }
                }
                break;

            case TELA_INFORMACOES:
                if (clique && CheckCollisionPointRec(mousePoint, btnVoltar)) tela = TELA_MENU;
                if (!dadosIniciados) {
                    dadosInfo[0] = (DadoInterativo){ {mesaX + 60*escala, mesaY + mesaH/2 - 40*escala}, {0,0}, 40*escala, false, 5 };
                    dadosInfo[1] = (DadoInterativo){ {dadosInfo[0].pos.x + 50*escala, dadosInfo[0].pos.y + 30*escala}, {0,0}, 40*escala, false, 3 };
                    dadosIniciados = true;
                }
                for (int i = 0; i < 2; i++) {
                    Rectangle recDado = { dadosInfo[i].pos.x, dadosInfo[i].pos.y, dadosInfo[i].tamanho, dadosInfo[i].tamanho };
                    if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT) && CheckCollisionPointRec(mousePoint, recDado)) dadosInfo[i].segurando = true;
                    if (dadosInfo[i].segurando) {
                        Vector2 delta = GetMouseDelta();
                        dadosInfo[i].pos.x += delta.x;
                        dadosInfo[i].pos.y += delta.y;
                        dadosInfo[i].vel.x = delta.x * 15.0f;
                        dadosInfo[i].vel.y = delta.y * 15.0f;
                        if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT)) dadosInfo[i].segurando = false;
                    } else {
                        dadosInfo[i].pos.x += dadosInfo[i].vel.x * dt;
                        dadosInfo[i].pos.y += dadosInfo[i].vel.y * dt;
                        dadosInfo[i].vel.x *= 0.95f;
                        dadosInfo[i].vel.y *= 0.95f;
                        if ((fabs(dadosInfo[i].vel.x) > 40.0f || fabs(dadosInfo[i].vel.y) > 40.0f) && GetRandomValue(0,10) > 7) {
                            dadosInfo[i].valor = GetRandomValue(1, 6);
                        }
                    }
                    if (dadosInfo[i].pos.x < mesaX) { dadosInfo[i].pos.x = mesaX; dadosInfo[i].vel.x *= -0.8f; }
                    if (dadosInfo[i].pos.x + dadosInfo[i].tamanho > mesaX + mesaW) { dadosInfo[i].pos.x = mesaX + mesaW - dadosInfo[i].tamanho; dadosInfo[i].vel.x *= -0.8f; }
                    if (dadosInfo[i].pos.y < mesaY) { dadosInfo[i].pos.y = mesaY; dadosInfo[i].vel.y *= -0.8f; }
                    if (dadosInfo[i].pos.y + dadosInfo[i].tamanho > mesaY + mesaH) { dadosInfo[i].pos.y = mesaY + mesaH - dadosInfo[i].tamanho; dadosInfo[i].vel.y *= -0.8f; }
                }
                break;
        }

        if (tela != TELA_CREDITOS) creditosY = alturaAtual;

        // MUSICAS
        static TelaAtual telaAnterior = (TelaAtual)-1;
        if (tela != telaAnterior) {
            if (tela == TELA_MENU && telaAnterior == TELA_CREDITOS) PlayMusicTrack(0);
            else if (tela == TELA_CREDITOS) PlayMusicTrack(1);
            telaAnterior = tela;
        }
        UpdateAudioSystem();

        // ==========================================
        // SWITCH 2: RENDERIZAÇÃO (DRAW)
        // ==========================================
        BeginDrawing();
        if (tela == TELA_MENU) ClearBackground(SKYBLUE);
        else if (tela == TELA_CREDITOS) ClearBackground(BLACK);
        else ClearBackground(GRAY);

        switch (tela) {
            case TELA_MENU:
                DrawRectangle(0, chaoY, gapInicio, alturaAtual - chaoY, DARKBROWN);
                DrawRectangle(gapInicio - 10, chaoY - 20, 10, 40, DARKBROWN);
                DrawRectangle(gapFim, chaoY, larguraAtual - gapFim, alturaAtual - chaoY, DARKBROWN);
                DrawRectangle(gapFim, chaoY - 20, 10, 40, DARKBROWN);

                DrawCircle(posNuvem1 - 50 * escala, alturaAtual * 0.15f, 40 * escala, WHITE);
                DrawCircle(posNuvem1,               alturaAtual * 0.15f, 60 * escala, WHITE);
                DrawCircle(posNuvem1 + 50 * escala, alturaAtual * 0.15f, 40 * escala, WHITE);

                DrawCircle(posNuvem2 - 60 * escala, alturaAtual * 0.20f, 50 * escala, WHITE);
                DrawCircle(posNuvem2,               alturaAtual * 0.20f, 70 * escala, WHITE);
                DrawCircle(posNuvem2 + 60 * escala, alturaAtual * 0.20f, 50 * escala, WHITE);

                for (int i = 0; i < NUM_CORREDORES; i++) DrawRectangle(corredores[i].pos.x, corredores[i].pos.y, tamanhoQuadrado, tamanhoQuadrado, corredores[i].cor);

                Rectangle rectTitulo = { larguraAtual / 2.0f - (130.0f * escala), alturaAtual * 0.05f, 260.0f * escala, 80.0f * escala };
                const char* t1 = "Caminho do";
                const char* t2 = "conhecimento";
                Vector2 tam1 = MeasureTextEx(fonteOffbit, t1, tamFonteTitulo, 1);
                Vector2 tam2 = MeasureTextEx(fonteOffbit, t2, tamFonteTitulo, 1);
                Vector2 posT1 = { (larguraAtual - tam1.x) / 2.0f, alturaAtual * 0.10f };
                Vector2 posT2 = { (larguraAtual - tam2.x) / 2.0f, posT1.y + tam1.y + (10 * escala) };

                int grossuraBorda = 4 * escala;
                DesenharTextoComBorda(fonteOffbit, t1, posT1, tamFonteTitulo, 1, WHITE, BLACK, grossuraBorda);
                DesenharTextoComBorda(fonteOffbit, t2, posT2, tamFonteTitulo, 1, WHITE, BLACK, grossuraBorda);

                float tempoCorrido = GetTime();
                float oscilacao = 1.0f + sinf(tempoCorrido * 8.0f) * 0.1f;
                float tamanhoSplash = 25.0f * escala * oscilacao;
                Vector2 tamSplash = MeasureTextEx(fonteOffbit, splashEscolhido, tamanhoSplash, 1);
                Vector2 posSplash = { posT2.x + tam2.x + (10 * escala), posT2.y + tam2.y };
                Vector2 origemSplash = { tamSplash.x / 2.0f, tamSplash.y / 2.0f };

                DrawTextPro(fonteOffbit, splashEscolhido, (Vector2){posSplash.x + 3, posSplash.y + 3}, origemSplash, -15.0f, tamanhoSplash, 1, Fade(BLACK, 0.5f));
                DrawTextPro(fonteOffbit, splashEscolhido, posSplash, origemSplash, -15.0f, tamanhoSplash, 1, YELLOW);

                DesenharBotaoCentralizado(btnIniciar, "Iniciar", fonteOffbit, tamFonteBotoes);
                if (CheckCollisionPointRec(mousePoint, btnIniciar)) DrawRectangleRoundedLines(btnIniciar, 0.4f, 10, 3, WHITE);

                DesenharBotaoCentralizado(btnInfo, "Informações", fonteOffbit, tamFonteBotoes);
                if (CheckCollisionPointRec(mousePoint, btnInfo)) DrawRectangleRoundedLines(btnInfo, 0.4f, 10, 3, WHITE);

                DesenharBotaoCentralizado(btnCreditos, "Créditos", fonteOffbit, tamFonteBotoes);
                if (CheckCollisionPointRec(mousePoint, btnCreditos)) DrawRectangleRoundedLines(btnCreditos, 0.4f, 10, 3, WHITE);

                DesenharBotaoCentralizado(btnSair, "Sair", fonteOffbit, tamFonteBotoes);
                if (CheckCollisionPointRec(mousePoint, btnSair)) DrawRectangleRoundedLines(btnSair, 0.4f, 10, 3, WHITE);

                DesenharBotaoCentralizado(btnVolMenos, "-", fonteOffbit, tamFonteBotoes);
                if (CheckCollisionPointRec(mousePoint, btnVolMenos)) DrawRectangleRoundedLines(btnVolMenos, 0.4f, 10, 3, WHITE);

                DesenharBotaoCentralizado(btnVolMais, "+", fonteOffbit, tamFonteBotoes);
                if (CheckCollisionPointRec(mousePoint, btnVolMais)) DrawRectangleRoundedLines(btnVolMais, 0.4f, 10, 3, WHITE);

                const char* textoVol = TextFormat("VOL: %d%%", (int)(volumeMaster * 100.0f + 0.5f));
                Vector2 tamTextoVol = MeasureTextEx(fonteOffbit, textoVol, tamFonteTexto, 1);
                DrawTextEx(fonteOffbit, textoVol, (Vector2){ 125.0f * escala - (tamTextoVol.x / 2.0f), alturaAtual - (55.0f * escala) }, tamFonteTexto, 1, WHITE);
                break;

            case TELA_CADASTRO:
                DrawTextEx(fonteOffbit, "CADASTRO DE JOGADORES", (Vector2){larguraAtual/2.0f - MeasureTextEx(fonteOffbit, "CADASTRO DE JOGADORES", 40*escala, 1).x/2.0f, alturaAtual*0.1f}, 40*escala, 1, WHITE);
                if (estadoCadastro == 0) {
                    DrawTextEx(fonteOffbit, "Quantos jogadores irao participar?", (Vector2){larguraAtual/2.0f - MeasureTextEx(fonteOffbit, "Quantos jogadores irao participar?", 30*escala, 1).x/2.0f, alturaAtual*0.3f}, 30*escala, 1, LIGHTGRAY);
                    Rectangle btn2 = {larguraAtual/2.0f - 150*escala, alturaAtual/2.0f, 80*escala, 80*escala};
                    Rectangle btn3 = {larguraAtual/2.0f - 40*escala, alturaAtual/2.0f, 80*escala, 80*escala};
                    Rectangle btn4 = {larguraAtual/2.0f + 70*escala, alturaAtual/2.0f, 80*escala, 80*escala};
                    DesenharBotaoCentralizado(btn2, "2", fonteOffbit, 40*escala);
                    DesenharBotaoCentralizado(btn3, "3", fonteOffbit, 40*escala);
                    DesenharBotaoCentralizado(btn4, "4", fonteOffbit, 40*escala);
                    if (CheckCollisionPointRec(mousePoint, btn2)) DrawRectangleRoundedLines(btn2, 0.4f, 10, 3, WHITE);
                    if (CheckCollisionPointRec(mousePoint, btn3)) DrawRectangleRoundedLines(btn3, 0.4f, 10, 3, WHITE);
                    if (CheckCollisionPointRec(mousePoint, btn4)) DrawRectangleRoundedLines(btn4, 0.4f, 10, 3, WHITE);
                } else if (estadoCadastro == 1) {
                    const char* tituloJogador = TextFormat("JOGADOR %d", jogadorAtualCadastro);
                    DrawTextEx(fonteOffbit, tituloJogador, (Vector2){larguraAtual/2.0f - MeasureTextEx(fonteOffbit, tituloJogador, 40*escala, 1).x/2.0f, alturaAtual*0.25f}, 40*escala, 1, YELLOW);
                    DrawTextEx(fonteOffbit, "Digite seu nome (teclado):", (Vector2){larguraAtual/2.0f - MeasureTextEx(fonteOffbit, "Digite seu nome (teclado):", 30*escala, 1).x/2.0f, alturaAtual*0.4f}, 30*escala, 1, LIGHTGRAY);
                    Rectangle caixaTexto = {larguraAtual/2.0f - 200*escala, alturaAtual*0.5f, 400*escala, 60*escala};
                    DrawRectangleRounded(caixaTexto, 0.2f, 10, RAYWHITE);
                    DrawRectangleRoundedLines(caixaTexto, 0.2f, 10, 3, BLACK);
                    DrawTextEx(fonteOffbit, nomeDigitado, (Vector2){caixaTexto.x + 15*escala, caixaTexto.y + 15*escala}, 30*escala, 1, DARKBLUE);
                    if (((int)(GetTime() * 2)) % 2 == 0 && numLetras < 20) {
                        DrawTextEx(fonteOffbit, "_", (Vector2){caixaTexto.x + 15*escala + MeasureTextEx(fonteOffbit, nomeDigitado, 30*escala, 1).x, caixaTexto.y + 15*escala}, 30*escala, 1, DARKBLUE);
                    }
                    if (erroNome) {
                        const char* txtErro = "Este nome ja esta em uso!";
                        DrawTextEx(fonteOffbit, txtErro, (Vector2){larguraAtual/2.0f - MeasureTextEx(fonteOffbit, txtErro, 25*escala, 1).x/2.0f, caixaTexto.y + 70*escala}, 25*escala, 1, RED);
                    }
                    Rectangle btnAvancar = {larguraAtual/2.0f - 100*escala, alturaAtual*0.65f, 200*escala, 60*escala};
                    DesenharBotaoCentralizado(btnAvancar, "Avançar", fonteOffbit, 30*escala);
                    if (CheckCollisionPointRec(mousePoint, btnAvancar) && numLetras > 0) DrawRectangleRoundedLines(btnAvancar, 0.4f, 10, 3, WHITE);
                } else if (estadoCadastro == 2) {
                    const char* tituloJogador = TextFormat("JOGADOR %d: %s", jogadorAtualCadastro, nomeDigitado);
                    DrawTextEx(fonteOffbit, tituloJogador, (Vector2){larguraAtual/2.0f - MeasureTextEx(fonteOffbit, tituloJogador, 40*escala, 1).x/2.0f, alturaAtual*0.25f}, 40*escala, 1, YELLOW);
                    DrawTextEx(fonteOffbit, "Escolha sua cor:", (Vector2){larguraAtual/2.0f - MeasureTextEx(fonteOffbit, "Escolha sua cor:", 30*escala, 1).x/2.0f, alturaAtual*0.4f}, 30*escala, 1, LIGHTGRAY);
                    Rectangle btnCores[4] = {
                        {larguraAtual/2.0f - 220*escala, alturaAtual/2.0f, 100*escala, 100*escala},
                        {larguraAtual/2.0f - 110*escala, alturaAtual/2.0f, 100*escala, 100*escala},
                        {larguraAtual/2.0f +   0*escala, alturaAtual/2.0f, 100*escala, 100*escala},
                        {larguraAtual/2.0f + 110*escala, alturaAtual/2.0f, 100*escala, 100*escala}
                    };
                    Color coresArray[4] = {RED, GREEN, BLUE, YELLOW};
                    for(int c = 0; c < 4; c++) {
                        if (coresUsadas[c]) DrawRectangleRounded(btnCores[c], 0.2f, 10, DARKGRAY);
                        else {
                            DrawRectangleRounded(btnCores[c], 0.2f, 10, coresArray[c]);
                            if (CheckCollisionPointRec(mousePoint, btnCores[c])) DrawRectangleRoundedLines(btnCores[c], 0.2f, 10, 5, WHITE);
                        }
                        DrawRectangleRoundedLines(btnCores[c], 0.2f, 10, 3, BLACK);
                    }
                }
                break;

            case TELA_CREDITOS:
                for (int i = 0; i < NUM_ESTRELAS; i++) {
                    float x = fmod(estrelas[i].pos.x, larguraAtual);
                    float y = fmod(estrelas[i].pos.y, alturaAtual);
                    DrawCircle(x, y, estrelas[i].raio * escala, Fade(WHITE, estrelas[i].alfa));
                }
                float yAtual = creditosY;
                for (int i = 0; i < totalLinhasCreditos; i++) {
                    int tamanhoDaVez = (i == 0 || i == 4 || i == 11 || i == 14 || i == 18) ? 40 * escala : 25 * escala;
                    Color corDaVez = (i == 0 || i == 4 || i == 11 || i == 14 || i == 18) ? GOLD : YELLOW;
                    Vector2 tamTexto = MeasureTextEx(fonteOffbit, linhasCreditos[i], tamanhoDaVez, 1);
                    DrawTextEx(fonteOffbit, linhasCreditos[i], (Vector2){(larguraAtual - tamTexto.x) / 2.0f, yAtual}, tamanhoDaVez, 1, corDaVez);
                    yAtual += tamTexto.y + 45.0f * escala;
                }
                break;

            case TELA_INFORMACOES:
                DrawTextEx(fonteOffbit, "COMO JOGAR", (Vector2){ (larguraAtual - MeasureTextEx(fonteOffbit, "COMO JOGAR", 40 * escala, 1).x) / 2.0f, alturaAtual * 0.08f }, 40 * escala, 1, BLACK);
                float textoX = larguraAtual * 0.25f;
                DrawTextEx(fonteOffbit, "- Jogue o dado para avancar casas.", (Vector2){textoX, alturaAtual * 0.20f}, tamFonteTexto, 1, DARKGRAY);
                DrawTextEx(fonteOffbit, "- Casas 'B' dao bonus ao jogador.", (Vector2){textoX, alturaAtual * 0.27f}, tamFonteTexto, 1, DARKGRAY);
                DrawTextEx(fonteOffbit, "- Casas '?' farao perguntas.", (Vector2){textoX, alturaAtual * 0.34f}, tamFonteTexto, 1, DARKGRAY);
                DrawTextEx(fonteOffbit, "- Chegue ao fim do caminho para vencer!", (Vector2){textoX, alturaAtual * 0.41f}, tamFonteTexto, 1, DARKGRAY);

                DrawRectangleRounded((Rectangle){mesaX, mesaY, mesaW, mesaH}, 0.1f, 10, DARKBROWN);
                float tabW = mesaW * 0.6f; float tabH = mesaH * 0.8f;
                float tabX = mesaX + (mesaW - tabW) / 2.0f + 60 * escala;
                float tabY = mesaY + (mesaH - tabH) / 2.0f;
                DrawRectangleRounded((Rectangle){tabX, tabY, tabW, tabH}, 0.05f, 10, RAYWHITE);
                DrawRectangleRoundedLines((Rectangle){tabX, tabY, tabW, tabH}, 0.05f, 10, 3, BLACK);

                float casaSize = 25 * escala; float cx = tabX + 20 * escala; float cy = tabY + 20 * escala;
                Color coresCasas[] = {RED, BLUE, GREEN, YELLOW, ORANGE, PURPLE, PINK, LIME};
                for(int i = 0; i < 5; i++) DrawRectangle(cx + i*(casaSize + 5*escala), cy, casaSize, casaSize, coresCasas[i%8]);
                for(int i = 0; i < 3; i++) DrawRectangle(cx + 4*(casaSize + 5*escala), cy + (i+1)*(casaSize + 5*escala), casaSize, casaSize, coresCasas[(5+i)%8]);
                for(int i = 0; i < 4; i++) DrawRectangle(cx + (3-i)*(casaSize + 5*escala), cy + 3*(casaSize + 5*escala), casaSize, casaSize, coresCasas[(8+i)%8]);

                DesenharDadoFace(dadosInfo[0].pos.x, dadosInfo[0].pos.y, dadosInfo[0].tamanho, dadosInfo[0].valor, escala);
                DesenharDadoFace(dadosInfo[1].pos.x, dadosInfo[1].pos.y, dadosInfo[1].tamanho, dadosInfo[1].valor, escala);
                break;

            case TELA_JOGO:
                BeginMode2D(camera);
                DesenharCaminhoAED(tabuleiroFisico, escala, fonteOffbit);
                EndMode2D();

                if (turnoIniciado) {
                    Rectangle painelHUD = { 20*escala, 20*escala, 350*escala, 180*escala };
                    DrawRectangleRounded(painelHUD, 0.1f, 10, Fade(BLACK, 0.8f));
                    DrawRectangleRoundedLines(painelHUD, 0.1f, 10, 3, WHITE);

                    const char* txtVez = TextFormat("TURNO: %s", jogadorDaVez.nome);
                    Color corTxt = WHITE;
                    if (jogadorDaVez.cor == 1) corTxt = RED;
                    if (jogadorDaVez.cor == 2) corTxt = GREEN;
                    if (jogadorDaVez.cor == 3) corTxt = BLUE;
                    if (jogadorDaVez.cor == 4) corTxt = YELLOW;

                    DrawTextEx(fonteOffbit, txtVez, (Vector2){painelHUD.x + 20*escala, painelHUD.y + 15*escala}, 30*escala, 1, corTxt);
                    DrawTextEx(fonteOffbit, TextFormat("Score: %d", jogadorDaVez.dados.score), (Vector2){painelHUD.x + 20*escala, painelHUD.y + 55*escala}, 25*escala, 1, LIGHTGRAY);
                    DrawTextEx(fonteOffbit, "INVENTARIO:", (Vector2){painelHUD.x + 20*escala, painelHUD.y + 90*escala}, 20*escala, 1, GRAY);

                    for (int i=0; i<3; i++) {
                        Rectangle slot = {painelHUD.x + 20*escala + i*(50*escala), painelHUD.y + 115*escala, 40*escala, 40*escala};
                        DrawRectangleRounded(slot, 0.1f, 5, Fade(DARKGRAY, 0.5f));
                        DrawRectangleRoundedLines(slot, 0.1f, 5, 2, GRAY);
                        if (jogadorDaVez.inventario[i] != 0) {
                            DrawCircle(slot.x + slot.width/2, slot.y + slot.height/2, 10*escala, GOLD);

                            // Efeito Hover para indicar que é clicável!
                            if (estadoJogoAtual == ESTADO_ESPERANDO_DADO && CheckCollisionPointRec(mousePoint, slot)) {
                                DrawRectangleRoundedLines(slot, 0.1f, 5, 3, WHITE);
                                DrawTextEx(fonteOffbit, "USAR", (Vector2){slot.x + 2*escala, slot.y - 15*escala}, 15*escala, 1, YELLOW);
                            }
                        }
                    }
                }

                Rectangle recMsg = {larguraAtual/2.0f - 400*escala, 20*escala, 800*escala, 60*escala};
                DrawRectangleRounded(recMsg, 0.5f, 10, Fade(DARKBLUE, 0.9f));
                DrawRectangleRoundedLines(recMsg, 0.5f, 10, 3, SKYBLUE);
                DrawTextEx(fonteOffbit, mensagemSistema, (Vector2){recMsg.x + recMsg.width/2 - MeasureTextEx(fonteOffbit, mensagemSistema, 25*escala, 1).x/2, recMsg.y + 15*escala}, 25*escala, 1, WHITE);

                if (estadoJogoAtual == ESTADO_ESPERANDO_DADO || estadoJogoAtual == ESTADO_ANIMANDO_DADO) {
                    float bandejaX = larguraAtual - 300*escala;
                    float bandejaY = alturaAtual - 300*escala;
                    Rectangle recBandeja = {bandejaX, bandejaY, 300*escala, 300*escala};

                    DrawRectangleRounded(recBandeja, 0.1f, 10, Fade(DARKBROWN, 0.9f));
                    DrawRectangleRoundedLines(recBandeja, 0.1f, 10, 5, BLACK);
                    DrawTextEx(fonteOffbit, "JOGUE O DADO", (Vector2){bandejaX + 50*escala, bandejaY + 20*escala}, 25*escala, 1, Fade(WHITE, 0.5f));
                    DesenharDadoFace(dadoJogo.pos.x, dadoJogo.pos.y, dadoJogo.tamanho, dadoJogo.valor, escala);
                }

                if (estadoJogoAtual == ESTADO_FIM_TURNO) {
                    Rectangle btnPassar = {larguraAtual/2.0f - 150*escala, alturaAtual*0.85f, 300*escala, 60*escala};
                    DesenharBotaoCentralizado(btnPassar, "Passar Turno", fonteOffbit, 30*escala);
                    if (CheckCollisionPointRec(mousePoint, btnPassar)) DrawRectangleRoundedLines(btnPassar, 0.4f, 10, 3, GREEN);
                }

                // 5. Interface da Pergunta (Cobre a tela inteira quando ativada)
                if (estadoJogoAtual == ESTADO_PERGUNTA) {
                    Rectangle modal = {larguraAtual/2.0f - 400*escala, alturaAtual/2.0f - 250*escala, 800*escala, 500*escala};

                    // Fundo escuro
                    DrawRectangle(0, 0, larguraAtual, alturaAtual, Fade(BLACK, 0.8f));

                    // Modal Branco
                    DrawRectangleRounded(modal, 0.05f, 10, RAYWHITE);
                    DrawRectangleRoundedLines(modal, 0.05f, 10, 5, BLACK);

                    if (subEstadoPergunta == 0) {
                        const char* tituloUni = TextFormat("PERGUNTA - UNIDADE %d", casasModificadas);
                        DrawTextEx(fonteOffbit, tituloUni, (Vector2){modal.x + 20*escala, modal.y + 20*escala}, 40*escala, 1, DARKBLUE);
                        DrawTextEx(fonteOffbit, "(Use os numeros 1, 2, 3, 4 ou as letras V e F no teclado)", (Vector2){modal.x + 20*escala, modal.y + 60*escala}, 20*escala, 1, GRAY);

                        // Desenha o texto que foi formatado no backend
                        DrawTextEx(fonteOffbit, perguntaAtual.enunciado, (Vector2){modal.x + 40*escala, modal.y + 120*escala}, 30*escala, 1, BLACK);

                    } else if (subEstadoPergunta == 1) {
                        const char *tituloRes = acertouPergunta ? "RESPOSTA CORRETA!" : "RESPOSTA ERRADA!";
                        Color corRes = acertouPergunta ? GREEN : RED;
                        const char *txtAcao = acertouPergunta ? TextFormat("Voce avanca %d casa(s)!", casasModificadas) : TextFormat("A penalidade te volta %d casa(s)!", casasModificadas);
                        Rectangle btnCont = {modal.x + modal.width/2 - 100*escala, modal.y + modal.height - 100*escala, 200*escala, 60*escala};

                        DrawTextEx(fonteOffbit, tituloRes, (Vector2){modal.x + modal.width/2 - MeasureTextEx(fonteOffbit, tituloRes, 50*escala, 1).x/2, modal.y + 80*escala}, 50*escala, 1, corRes);
                        DrawTextEx(fonteOffbit, txtAcao, (Vector2){modal.x + modal.width/2 - MeasureTextEx(fonteOffbit, txtAcao, 30*escala, 1).x/2, modal.y + 150*escala}, 30*escala, 1, DARKGRAY);

                        DesenharBotaoCentralizado(btnCont, "Continuar", fonteOffbit, 30*escala);
                        if (CheckCollisionPointRec(mousePoint, btnCont)) DrawRectangleRoundedLines(btnCont, 0.4f, 10, 3, BLACK);
                    }
                }

                // 6. Interface do Baú (Modal)
                if (estadoJogoAtual == ESTADO_ACAO_CASA) {
                    Rectangle modal = {larguraAtual/2.0f - 350*escala, alturaAtual/2.0f - 150*escala, 700*escala, 300*escala};
                    const char *nomeItem = "";
                    const char *txtItem;
                    Rectangle btnCont = {larguraAtual/2.0f - 100*escala, alturaAtual/2.0f + 70*escala, 200*escala, 60*escala};

                    DrawRectangle(0, 0, larguraAtual, alturaAtual, Fade(BLACK, 0.8f));
                    DrawRectangleRounded(modal, 0.05f, 10, RAYWHITE);
                    DrawRectangleRoundedLines(modal, 0.05f, 10, 5, GOLD);

                    DrawTextEx(fonteOffbit, "VOCE ENCONTROU UM BAU!", (Vector2){modal.x + modal.width/2 - MeasureTextEx(fonteOffbit, "VOCE ENCONTROU UM BAU!", 40*escala, 1).x/2, modal.y + 30*escala}, 40*escala, 1, ORANGE);

                    switch(itemSorteado) {
                        case 1: nomeItem = "Rerol (Raro)"; break;
                        case 2: nomeItem = "Dica de Soussa (Raro)"; break;
                        case 3: nomeItem = "Tudo ou nada (Mitico)"; break;
                        case 4: nomeItem = "NOSSO item (Mitico)"; break;
                        case 5: nomeItem = "Buraco de Minhoca (Lendario)"; break;
                        case 6: nomeItem = "Imunidade a resenha (Mitico)"; break;
                        case 7: nomeItem = "Castigo (Raro)"; break;
                        default: nomeItem = "Vazio"; break;
                    }

                    txtItem = TextFormat("Item: %s", nomeItem);
                    DrawTextEx(fonteOffbit, txtItem, (Vector2){modal.x + modal.width/2 - MeasureTextEx(fonteOffbit, txtItem, 30*escala, 1).x/2, modal.y + 110*escala}, 30*escala, 1, DARKGRAY);

                    DesenharBotaoCentralizado(btnCont, "Guardar", fonteOffbit, 30*escala);
                    if (CheckCollisionPointRec(mousePoint, btnCont)) DrawRectangleRoundedLines(btnCont, 0.4f, 10, 3, BLACK);
                }

                // 7. Interface do Rerol (Modal)
                if (estadoJogoAtual == ESTADO_REROL) {
                    Rectangle modal = {larguraAtual/2.0f - 250*escala, alturaAtual/2.0f - 100*escala, 500*escala, 250*escala};
                    Rectangle btnSim = {larguraAtual/2.0f - 150*escala, alturaAtual/2.0f + 50*escala, 120*escala, 60*escala};
                    Rectangle btnNao = {larguraAtual/2.0f + 30*escala, alturaAtual/2.0f + 50*escala, 120*escala, 60*escala};

                    DrawRectangle(0, 0, larguraAtual, alturaAtual, Fade(BLACK, 0.7f));
                    DrawRectangleRounded(modal, 0.05f, 10, RAYWHITE);
                    DrawRectangleRoundedLines(modal, 0.05f, 10, 5, BLUE);

                    DrawTextEx(fonteOffbit, "REROL ATIVO!", (Vector2){modal.x + modal.width/2 - MeasureTextEx(fonteOffbit, "REROL ATIVO!", 40*escala, 1).x/2, modal.y + 20*escala}, 40*escala, 1, DARKBLUE);
                    DrawTextEx(fonteOffbit, TextFormat("Voce tirou %d. Deseja rolar de novo?", dadoJogo.valor), (Vector2){modal.x + 20*escala, modal.y + 80*escala}, 25*escala, 1, DARKGRAY);

                    DesenharBotaoCentralizado(btnSim, "SIM", fonteOffbit, 30*escala);
                    if (CheckCollisionPointRec(mousePoint, btnSim)) DrawRectangleRoundedLines(btnSim, 0.4f, 10, 3, BLACK);

                    DesenharBotaoCentralizado(btnNao, "NAO", fonteOffbit, 30*escala);
                    if (CheckCollisionPointRec(mousePoint, btnNao)) DrawRectangleRoundedLines(btnNao, 0.4f, 10, 3, BLACK);
                }

                DrawTextEx(fonteOffbit, "Dica: Mova com Botao Direito | Zoom na Rodinha", (Vector2){20 * escala, alturaAtual - 40 * escala}, 20 * escala, 1, Fade(LIGHTGRAY, 0.6f));
                break;
        }

        if (tela != TELA_MENU) {
            DesenharBotaoCentralizado(btnVoltar, "<- Voltar", fonteOffbit, 20 * escala);
            if (CheckCollisionPointRec(mousePoint, btnVoltar)) DrawRectangleRoundedLines(btnVoltar, 0.4f, 10, 3, WHITE);
        }

        EndDrawing();
    }

    CloseAudioSystem();
    UnloadFont(fonteOffbit);
    CloseWindow();
    return 0;
}
