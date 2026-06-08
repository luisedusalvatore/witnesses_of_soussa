#include "raylib.h"
#include "jogo.h"
#include <math.h>
#include "soundsfx.h"
// baixem a versão 4.2 do raylib pfv
// --------------------------------------------------------------------------
// DEFINIÇÕES E ESTRUTURAS
// --------------------------------------------------------------------------
typedef enum {
    TELA_MENU,
    TELA_JOGO,
    TELA_INFORMACOES,
    TELA_CREDITOS
} TelaAtual;

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

    TelaAtual tela = TELA_MENU;
    Corredor corredores[NUM_CORREDORES];
    Estrela estrelas[NUM_ESTRELAS];
    bool primeiraVez = true;

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

    while (!WindowShouldClose()) {   

        int larguraAtual = GetScreenWidth();
        int alturaAtual = GetScreenHeight();
        float escala = (float)alturaAtual / 600.0f;
        float dt = GetFrameTime();

        float chaoY = alturaAtual * 0.80f;
        float tamanhoQuadrado = 50 * escala;
        float gapInicio = larguraAtual * 0.40f;
        float gapFim = larguraAtual * 0.60f;

        if (primeiraVez) {
            ResetarCorredores(corredores, chaoY - tamanhoQuadrado, escala);

            for (int i = 0; i < NUM_ESTRELAS; i++) {
                estrelas[i].pos.x = GetRandomValue(0, 3000);
                estrelas[i].pos.y = GetRandomValue(0, 3000);
                estrelas[i].raio = GetRandomValue(1, 3);
                estrelas[i].alfa = GetRandomValue(10, 100) / 100.0f;
                estrelas[i].velPiscar = GetRandomValue(5, 15) * 0.1f;
            }

            // SORTEIA A FRASE DO SPLASH QUANDO O JOGO ABRE
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

        int tamFonteTitulo = 50 * escala;
        int tamFonteBotoes = 30 * escala;
        int tamFonteTexto  = 20 * escala;

        Vector2 mousePoint = GetMousePosition();
        bool clique = IsMouseButtonPressed(MOUSE_BUTTON_LEFT);

        switch (tela) {
            case TELA_MENU:
                if (clique) {
                    if (CheckCollisionPointRec(mousePoint, btnIniciar)) tela = TELA_JOGO;
                    else if (CheckCollisionPointRec(mousePoint, btnInfo)) tela = TELA_INFORMACOES;
                    else if (CheckCollisionPointRec(mousePoint, btnCreditos)) tela = TELA_CREDITOS;
                    else if (CheckCollisionPointRec(mousePoint, btnSair)) return 0;
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
                        }
                        else if (!corredores[i].pulou && !corredores[i].caiu) {
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

                if (todosTerminaram) {
                    ResetarCorredores(corredores, chaoY - tamanhoQuadrado, escala);
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

                if (creditosY + alturaTotalTexto < 0) {
                    creditosY = alturaAtual;
                }

                for (int i = 0; i < NUM_ESTRELAS; i++) {
                    estrelas[i].alfa += estrelas[i].velPiscar * dt;
                    if (estrelas[i].alfa >= 1.0f) {
                        estrelas[i].alfa = 1.0f;
                        estrelas[i].velPiscar *= -1.0f;
                    } else if (estrelas[i].alfa <= 0.1f) {
                        estrelas[i].alfa = 0.1f;
                        estrelas[i].velPiscar *= -1.0f;
                    }
                }
                break;

            case TELA_JOGO:
            case TELA_INFORMACOES:
                if (clique && CheckCollisionPointRec(mousePoint, btnVoltar)) tela = TELA_MENU;
                break;
        }

        if (tela != TELA_CREDITOS) {
            creditosY = alturaAtual;
        }

        // --------------------------------------------------------------------------
        // LÓGICA DE MUSICAS
        // --------------------------------------------------------------------------
        
        static TelaAtual telaAnterior = (TelaAtual)-1; // identifica qual era a tela anterior para mudar as musicas do menu

            if (tela != telaAnterior) {
                if (tela == TELA_MENU && telaAnterior == TELA_CREDITOS) {
                    PlayMusicTrack(0); // Toca música do Menu
                } 
                else if (tela == TELA_CREDITOS) {
                    PlayMusicTrack(1); // Toca música dos Créditos
                }
                
                telaAnterior = tela; // Atualiza o estado da tela
            }

            UpdateAudioSystem(); // Continua atualizando o stream
 

        // ----------------------------------------------------------------------
        // LÓGICA DE RENDERIZAÇÃO (Draw)
        // ----------------------------------------------------------------------
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

                for (int i = 0; i < NUM_CORREDORES; i++) {
                    DrawRectangle(corredores[i].pos.x, corredores[i].pos.y, tamanhoQuadrado, tamanhoQuadrado, corredores[i].cor);
                }

                // TÍTULO DO JOGO
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

                // --- EFEITO SPLASH MINECRAFT ---
                float tempoCorrido = GetTime();
                // A função seno cria uma onda perfeita que vai de -1 a 1. Ajustamos para oscilar a escala do texto.
                float oscilacao = 1.0f + sinf(tempoCorrido * 8.0f) * 0.1f;
                float tamanhoSplash = 25.0f * escala * oscilacao;

                Vector2 tamSplash = MeasureTextEx(fonteOffbit, splashEscolhido, tamanhoSplash, 1);

                // Define a posição para o canto inferior direito da palavra "conhecimento"
                Vector2 posSplash = { posT2.x + tam2.x + (10 * escala), posT2.y + tam2.y };

                // Define a origem no centro do texto para ele pulsar a partir do meio
                Vector2 origemSplash = { tamSplash.x / 2.0f, tamSplash.y / 2.0f };

                // Usa DrawTextPro para rotacionar (-15 graus) e desenhar uma sombra primeiro, depois o texto
                DrawTextPro(fonteOffbit, splashEscolhido, (Vector2){posSplash.x + 3, posSplash.y + 3}, origemSplash, -15.0f, tamanhoSplash, 1, Fade(BLACK, 0.5f));
                DrawTextPro(fonteOffbit, splashEscolhido, posSplash, origemSplash, -15.0f, tamanhoSplash, 1, YELLOW);
                // -------------------------------

                DesenharBotaoCentralizado(btnIniciar, "Iniciar", fonteOffbit, tamFonteBotoes);
                if (CheckCollisionPointRec(mousePoint, btnIniciar)) DrawRectangleRoundedLines(btnIniciar, 0.4f, 10, 3, WHITE);

                DesenharBotaoCentralizado(btnInfo, "Informações", fonteOffbit, tamFonteBotoes);
                if (CheckCollisionPointRec(mousePoint, btnInfo)) DrawRectangleRoundedLines(btnInfo, 0.4f, 10, 3, WHITE);

                DesenharBotaoCentralizado(btnCreditos, "Créditos", fonteOffbit, tamFonteBotoes);
                if (CheckCollisionPointRec(mousePoint, btnCreditos)) DrawRectangleRoundedLines(btnCreditos, 0.4f, 10, 3, WHITE);

                DesenharBotaoCentralizado(btnSair, "Sair", fonteOffbit, tamFonteBotoes);
                if (CheckCollisionPointRec(mousePoint, btnSair)) DrawRectangleRoundedLines(btnSair, 0.4f, 10, 3, WHITE);
                break;

            case TELA_CREDITOS:
                for (int i = 0; i < NUM_ESTRELAS; i++) {
                    float x = fmod(estrelas[i].pos.x, larguraAtual);
                    float y = fmod(estrelas[i].pos.y, alturaAtual);
                    Color corEstrela = Fade(WHITE, estrelas[i].alfa);
                    DrawCircle(x, y, estrelas[i].raio * escala, corEstrela);
                }

                float yAtual = creditosY;
                float espacamento = 45.0f * escala;

                for (int i = 0; i < totalLinhasCreditos; i++) {
                    int tamanhoDaVez = (i == 0 || i == 4 || i == 11 || i == 14 || i == 18) ? 40 * escala : 25 * escala;
                    Color corDaVez = (i == 0 || i == 4 || i == 11 || i == 14 || i == 18) ? GOLD : YELLOW;

                    Vector2 tamTexto = MeasureTextEx(fonteOffbit, linhasCreditos[i], tamanhoDaVez, 1);
                    Vector2 posTexto = { (larguraAtual - tamTexto.x) / 2.0f, yAtual };

                    DrawTextEx(fonteOffbit, linhasCreditos[i], posTexto, tamanhoDaVez, 1, corDaVez);
                    yAtual += tamTexto.y + espacamento;
                }
                break;

            case TELA_INFORMACOES: {
                const char* tituloInfo = "COMO JOGAR";
                Vector2 tamTituloInfo = MeasureTextEx(fonteOffbit, tituloInfo, 40 * escala, 1);
                DrawTextEx(fonteOffbit, tituloInfo, (Vector2){ (larguraAtual - tamTituloInfo.x) / 2.0f, alturaAtual * 0.08f }, 40 * escala, 1, BLACK);

                float textoX = larguraAtual * 0.25f;
                DrawTextEx(fonteOffbit, "- Jogue o dado para avancar casas.", (Vector2){textoX, alturaAtual * 0.20f}, tamFonteTexto, 1, DARKGRAY);
                DrawTextEx(fonteOffbit, "- Casas 'B' dao bonus ao jogador.", (Vector2){textoX, alturaAtual * 0.27f}, tamFonteTexto, 1, DARKGRAY);
                DrawTextEx(fonteOffbit, "- Casas '?' farao perguntas.", (Vector2){textoX, alturaAtual * 0.34f}, tamFonteTexto, 1, DARKGRAY);
                DrawTextEx(fonteOffbit, "- Chegue ao fim do caminho para vencer!", (Vector2){textoX, alturaAtual * 0.41f}, tamFonteTexto, 1, DARKGRAY);

                float mesaW = larguraAtual * 0.7f;
                float mesaH = alturaAtual * 0.4f;
                float mesaX = (larguraAtual - mesaW) / 2.0f;
                float mesaY = alturaAtual * 0.55f;
                DrawRectangleRounded((Rectangle){mesaX, mesaY, mesaW, mesaH}, 0.1f, 10, DARKBROWN);

                float tabW = mesaW * 0.6f;
                float tabH = mesaH * 0.8f;
                float tabX = mesaX + (mesaW - tabW) / 2.0f + 60 * escala;
                float tabY = mesaY + (mesaH - tabH) / 2.0f;
                DrawRectangleRounded((Rectangle){tabX, tabY, tabW, tabH}, 0.05f, 10, RAYWHITE);
                DrawRectangleRoundedLines((Rectangle){tabX, tabY, tabW, tabH}, 0.05f, 10, 3, BLACK);

                float casaSize = 25 * escala;
                float cx = tabX + 20 * escala;
                float cy = tabY + 20 * escala;
                Color coresCasas[] = {RED, BLUE, GREEN, YELLOW, ORANGE, PURPLE, PINK, LIME};

                for(int i = 0; i < 5; i++) {
                    DrawRectangle(cx + i*(casaSize + 5*escala), cy, casaSize, casaSize, coresCasas[i%8]);
                }
                for(int i = 0; i < 3; i++) {
                    DrawRectangle(cx + 4*(casaSize + 5*escala), cy + (i+1)*(casaSize + 5*escala), casaSize, casaSize, coresCasas[(5+i)%8]);
                }
                for(int i = 0; i < 4; i++) {
                    DrawRectangle(cx + (3-i)*(casaSize + 5*escala), cy + 3*(casaSize + 5*escala), casaSize, casaSize, coresCasas[(8+i)%8]);
                }

                float dadoSize = 40 * escala;

                float dado1X = mesaX + 60 * escala;
                float dado1Y = mesaY + mesaH / 2.0f - dadoSize;
                DrawRectangleRounded((Rectangle){dado1X, dado1Y, dadoSize, dadoSize}, 0.2f, 5, WHITE);
                DrawRectangleRoundedLines((Rectangle){dado1X, dado1Y, dadoSize, dadoSize}, 0.2f, 5, 2, BLACK);
                DrawCircle(dado1X + dadoSize*0.25f, dado1Y + dadoSize*0.25f, 4*escala, BLACK);
                DrawCircle(dado1X + dadoSize*0.75f, dado1Y + dadoSize*0.25f, 4*escala, BLACK);
                DrawCircle(dado1X + dadoSize*0.50f, dado1Y + dadoSize*0.50f, 4*escala, BLACK);
                DrawCircle(dado1X + dadoSize*0.25f, dado1Y + dadoSize*0.75f, 4*escala, BLACK);
                DrawCircle(dado1X + dadoSize*0.75f, dado1Y + dadoSize*0.75f, 4*escala, BLACK);

                float dado2X = dado1X + 50 * escala;
                float dado2Y = dado1Y + 30 * escala;
                DrawRectangleRounded((Rectangle){dado2X, dado2Y, dadoSize, dadoSize}, 0.2f, 5, WHITE);
                DrawRectangleRoundedLines((Rectangle){dado2X, dado2Y, dadoSize, dadoSize}, 0.2f, 5, 2, BLACK);
                DrawCircle(dado2X + dadoSize*0.25f, dado2Y + dadoSize*0.75f, 4*escala, BLACK);
                DrawCircle(dado2X + dadoSize*0.50f, dado2Y + dadoSize*0.50f, 4*escala, BLACK);
                DrawCircle(dado2X + dadoSize*0.75f, dado2Y + dadoSize*0.25f, 4*escala, BLACK);
                break;
            }

            case TELA_JOGO:
                DrawTextEx(fonteOffbit, "INTERFACE DO JOGO (EM DESENVOLVIMENTO)", (Vector2){larguraAtual * 0.15f, alturaAtual * 0.41f}, 30 * escala, 1, DARKBLUE);
                DrawTextEx(fonteOffbit, "Seu tabuleiro de 20 casas entrara aqui futuramente.", (Vector2){larguraAtual * 0.20f, alturaAtual * 0.53f}, tamFonteTexto, 1, DARKGRAY);
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
