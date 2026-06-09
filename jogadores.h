#ifndef jogadores_h
#define jogadores_h

#include <stdio.h>
#include <string.h>
#include "filade.h"
#include "pilhade.h"
#include "perguntas.h"
#include "randon.h"
#include "easteregg.h"
#include "listade.h"
#include "tipos.h"
#include "tabuleiro.h"

// declaração previa do prototipo da função de imprimir a opção dos itens ( estava dando erro de implicit sem ela)
void imprime_nome_item(int id);

// função de imprimir itens
void imprime_nome_item(int id) {
    switch(id) {
        case 1: printf("Rerol (Raro)"); break;
        case 2: printf("Dica de Soussa (Raro)"); break;
        case 3: printf("Tudo ou nada (Mitico)"); break;
        case 4: printf("NOSSO item (Mitico)"); break;
        case 5: printf("Buraco de Minhoca (Lendario)"); break;
        case 6: printf("Imunidade a resenha (Mitico)"); break;
        case 7: printf("Castigo (Raro)"); break;
        default: printf("Vazio"); break;
    }
}

void inicializa_posicao(Player *jogador, tp_listade *inicio){
    // define a posição de todos os jogadores no tabuleiro como o inicio
    jogador->posicao = inicio->ini;
}

int quantidade(){
    int quant_players = 0;
    int leu_certo; // verifica se o scanf funcionou

    printf("\t\nQuantos jogadores irao participar do jogo? (MAX: 4):\n");
    // o scanf retorna 1 se conseguiu ler 1 variável com sucesso
    leu_certo = scanf("%d", &quant_players);

    // o loop continua se o scanf não leu um número (leu_certo != 1)
    // ou se a quantidade estiver fora do limite
    while(leu_certo != 1 || quant_players < 2 || quant_players > 4){

        // função para ler e descartar caractere por caractere até achar enter
        while (getchar() != '\n' && !feof(stdin));

        printf("Quantidade nao permitida ou entrada invalida! Escolha um NUMERO (MIN: 2 | MAX: 4):\n");
        leu_certo = scanf("%d", &quant_players);
    }

    return quant_players; // se a entrada for valida e passou pelos processos de verificação, a função retorna a quantidade de jogadores
}

// valida o tamanho do nome e impede que dois jogadores usem o mesmo apelido
int verificarNome(char *nome, char nomes_usados[][100], int qtd_cadastrados){
    // verifica se estourou o limite de caracteres
    if(strlen(nome) >= 30)
        return 0;   // Erro 0: Inválido pelo tamanho

    // percorre a lista de nomes já cadastrados até o momento
    for(int i = 0; i < qtd_cadastrados; i++) {

        if(strcmp(nome, nomes_usados[i]) == 0) {
            return 2; // inválido por ser repetido
        }
    }

    return 1;   // Sucesso: Nome é válido e único
}

int verificarCor(int corAux, int cor_usada[]){
    if(corAux < 1 || corAux > 4){   // verifica se cor é válida de 1 a 4
        return 0;
    }
    // verifica no vetor se a cor já foi pega por outro jogador
    if(cor_usada[corAux-1] == 1) return 0;

    // marca a cor como em uso e retorna sucesso
    cor_usada[corAux-1] = 1;
    return 1;
}

void imprimirJogadores(Player *player, int quantidade) {
    int i;
    printf("\n=================================\n");
    printf("     LISTA FINAL DE JOGADORES    \n");
    printf("=================================\n");

    for(i = 0; i < quantidade; i++) {
        switch(player[i].cor) {
            case 1:
                printf("\033[31mJogador %d: %s \033[0m\n", i + 1, player[i].nome); // vermelho
                break;
            case 2:
                printf("\033[32mJogador %d: %s \033[0m\n", i + 1, player[i].nome); // verde
                break;
            case 3:
                printf("\033[34mJogador %d: %s \033[0m\n", i + 1, player[i].nome); // azul
                break;
            case 4:
                printf("\033[33mJogador %d: %s \033[0m\n", i + 1, player[i].nome); // amarelo
                break;
            default:
                printf("Jogador %d: %s \n", i + 1, player[i].nome);
                break;
        }
    }
    printf("\n");
}

// cadastra os jogadores e prepara o estado inicial deles na partida
void lerDados(int quant, tp_fila *jogadores, tp_listade *tabuleiro){
    int i;
    int confirmNome, confirmCor;
    char nomeAux[99];
    int corAux;

    // Vetores de controle para evitar escolhas duplicadas
    int cores_usadas[4] = {0,0,0,0};
    char nomes_usados[4][100]; // <-- NOVO: Matriz para guardar os nomes

    for(i = 0; i < quant; i++) {
        Player jogador;

        // Coloca o ponteiro de posição do jogador na primeira casa do tabuleiro
        inicializa_posicao(&jogador, tabuleiro);

        printf("\n=============================\n");
        printf("   CADASTRANDO JOGADOR %d\n", i + 1);
        printf("=============================\n");

        // Loop para garantir um nome dentro do limite e único
        do{
            printf("\t\nQual o nome do jogador %d?\n", i+1);

            scanf(" %30[^\n]", nomeAux);
            getchar();  // limpa o '\n' que o scanf deixa no buffer

            // passamos o nomeAux, o vetor de bloqueio e o índice 'i'
            confirmNome = verificarNome(nomeAux, nomes_usados, i);

            if (confirmNome == 1){
                // copia para o jogador e salva no vetor de bloqueio
                strcpy(jogador.nome, nomeAux);
                strcpy(nomes_usados[i], nomeAux); // Registra que esse nome já é de alguém

                // sousaEasterEgg(jogador.nome); // em manutenção
            }
            else if (confirmNome == 2) {
                printf("[ERRO] Esse nome ja esta em uso por outro jogador! Escolha outro.\\n");
            }
            else {
                printf("[ERRO] Limite de 30 caracteres excedido. Tente novamente.\\n");
            }
        }while(confirmNome != 1);

        // loop para garantir que o jogador escolha uma cor válida e que não esteja em uso
        do{
            printf("\nQual cor deseja escolher?\n");
            printf("1 - Vermelho | 2 - Verde | 3 - Azul | 4 - Amarelo\n");
            printf("Sua escolha: ");
            scanf("%d", &corAux);

            // checa a disponibilidade da cor no vetor de controle
            confirmCor = verificarCor(corAux, cores_usadas);

            if(confirmCor == 1){    // caso o tabuleiro não tenha sido criado
                jogador.cor = corAux;   // salva a cor escolhida na struct do jogador
                if (tabuleiro->ini != NULL) { // caso o tabuleiro esteja vazio
                    tabuleiro->ini->info.cor[corAux - 1] = 1;
                }
                printf("Cor atribuida com sucesso ao jogador %s!\n", jogador.nome);
            }
            else
                printf("[ERRO] Cor ja escolhida por outro jogador ou invalida! Tente novamente.\n");
        }while(confirmCor!=1);

        // zera todos os status do jogador
        // faz com que ninguém comece a partida já com itens ou sofrendo punições
        jogador.qtd_itens = 0;
        jogador.status_imune = 0;
        jogador.status_castigo = 0;
        jogador.status_tudo_nada = 0;
        jogador.status_rerol = 0;

        // preenche o vetor do inventário com 0 (inventario vázio)
        for(int j = 0; j < 3; j++) {
            jogador.inventario[j] = 0;
        }

        insere_fila(jogadores, jogador);
        printf("\n>>> Jogador %d adicionado com sucesso! <<<\n", i + 1);
    }
}

// faz a locomoção do jogador na lista duplamente encadeada e atualiza o estado do tabuleiro visual
int move_posicao(Player *jogador, int posi){
    if (posi == 0) return 0; // se não for mover nada, sai da função

    // apaga a cor do jogador da casa atual
    if(jogador->posicao != NULL) {
        jogador->posicao->info.cor[jogador->cor-1] = 0;
    }

    // faz o movimento dos ponteiros na lista
    if(posi > 0){
        // avança usando o ponteiro prox
        for(int i = 0; i < posi; i++){
                if(jogador->posicao != NULL) jogador->posicao = jogador->posicao->prox;
                // se o ponteiro estourar o limite da lista null, retorna 1 sinalizando vitória
                if(jogador->posicao == NULL) return 1; // fim do tabuleiro
        }
    }
    else {
        // retrocede na lista usando o ponteiro ant
        // Como posi é negativo por ser penalidade, multiplica por -1 para iterar corretamente
        for(int i = 0; i < -1*posi; i++){
            // evita o jogo de crashar caso a penalidade por maior que a casa atual do jogador
            if(jogador->posicao != NULL && jogador->posicao->ant != NULL)
                jogador->posicao = jogador->posicao->ant;
        }
    }

    // pinta a cor do jogador na nova casa
    if(jogador->posicao != NULL) {
        jogador->posicao->info.cor[jogador->cor-1] = 1;
    }

    return 0;   // informa que o jogo deve continuar
}

void atualiza_hank(Player *jogador, int pontos){
    if(pontos > 0){
        jogador->dados.acertos ++;
        jogador->dados.score += pontos;
    }
    else{
        jogador->dados.erros ++;
        if(jogador->dados.score + pontos >= 0) jogador->dados.score += pontos;
    }
}

int rodadaplayer(tp_fila *jogadores, tp_listade *tabuleiro ,tp_pilha *perguntas_faceis, tp_pilha *perguntas_medias, tp_pilha *perguntas_faceis_descartadas, tp_pilha *perguntas_medias_descartadas){
    Player jogador;
    int venceu = 0;
    int resposta;

    // retira o jogador da vez da fila
    remove_fila(jogadores, &jogador);

    imprime_tabuleiro_visual(tabuleiro);

    printf("\n=========================================\n");
    printf("VEZ DE: %s\n", jogador.nome);
    printf("=========================================\n");

    // verifica o status de castigo
    if (jogador.status_castigo == 1) {
        printf("\n[PUNICAO] %s esta sob o efeito do CASTIGO e perdeu a vez!\n", jogador.nome);
        jogador.status_castigo = 0; // zera para voltar ao normal no próximo turno
        insere_fila(jogadores, jogador); // volta pro fim da fila
        return 0; // encerra o turno
    }

    // se o jogador usou imunidade na rodada passada, ela acaba agora
    if (jogador.status_imune == 1) {
        jogador.status_imune = 0;
        printf("[INFO] A Imunidade de %s acabou.\n", jogador.nome);
    }

    // informações do inventário
    if (jogador.qtd_itens > 0) {
        int escolha_item;
        printf("\n--- SEU INVENTARIO ---\n");
        for (int i = 0; i < 3; i++) {
            if (jogador.inventario[i] != 0) {   // verifica se o jogador tem algo no inventário, se tem, é imprimido na tela (i<3 pois o limite de itens por jogador é 3)
                printf("Slot %d: [ID %d] - ", i + 1, jogador.inventario[i]);
                imprime_nome_item(jogador.inventario[i]);
                printf("\n");
            }
        }

        printf("\nDeseja usar algum item antes de rolar o dado? (Digite o ID ou 0 para pular): ");
        scanf("%d", &escolha_item);
        while (getchar() != '\n' && !feof(stdin)); // limpa o buffer do teclado

        if (escolha_item != 0) {
            // verifica se o jogador realmente tem o item escolhido
            int slot_usado = -1;
            for (int i = 0; i < 3; i++) {
                if (jogador.inventario[i] == escolha_item) {
                    slot_usado = i;
                    break;
                }
            }

            if (slot_usado != -1) {
                printf("\n>>> %s usou o item ", jogador.nome);
                imprime_nome_item(escolha_item);
                printf("! <<<\n");

                // remove o item antes do efeito
                jogador.inventario[slot_usado] = 0;
                jogador.qtd_itens--;

                // efeito dos itens

                if (escolha_item == 1) {
                    // ID 1: Rerol
                    jogador.status_rerol = 1;
                    printf("[EFEITO] Voce ativou o Rerol! Podera rolar o dado novamente se nao gostar do resultado.\n");
                }

                // ID 2: Dica de Soussa inativo pela ausência de perguntas dificeis ( estará presente na unidade 3)

                if (escolha_item == 3) {
                    // ID 3: Tudo ou nada
                    jogador.status_tudo_nada = 1;
                }

                else if (escolha_item == 4) {
                    // ID 4: NOSSO item
                    int tam_fila = 0;
                    tp_fila *fila_aux = inicializa_fila();
                    Player alvo;
                    int item_roubado = 0;
                    char nome_vitima[100];

                    while(!fila_vazia(jogadores)){
                        remove_fila(jogadores, &alvo);
                        insere_fila(fila_aux, alvo);
                        tam_fila++;
                    }

                    for(int i = 0; i < tam_fila; i++){
                        remove_fila(fila_aux, &alvo);

                        // encontra a primeira vítima que tenha itens e não esteja imune
                        if(item_roubado == 0 && alvo.qtd_itens > 0) {
                            if (alvo.status_imune == 1) {
                                printf("[DEFESA] O roubo tentou alvejar %s, mas ele(a) esta IMUNE!\n", alvo.nome);
                            } else {
                                // rouba o primeiro item que encontrar no inventário do alvo
                                for(int j = 0; j < 3; j++) {
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
                        insere_fila(jogadores, alvo);
                    }
                    destroi_fila(fila_aux);

                    if(item_roubado != 0) {
                        printf("[NOSSO ITEM] Sucesso! Voce roubou um(a) ");
                        imprime_nome_item(item_roubado);
                        printf(" de %s!\n", nome_vitima);

                        // guarda o item roubado no inventário do jogador atual
                        for(int k = 0; k < 3; k++) {
                            if(jogador.inventario[k] == 0) {
                                jogador.inventario[k] = item_roubado;
                                jogador.qtd_itens++;
                                break;
                            }
                        }
                    } else {
                        printf("[NOSSO ITEM] Falha! Ninguem tem itens para roubar.\n");
                    }
                }

                else if (escolha_item == 5) {
                    // ID 5: Buraco de Minhoca
                    int tam_fila = 0;
                    tp_fila *fila_aux = inicializa_fila();
                    Player alvo;

                    printf("\n[BURACO DE MINHOCA] Escolha o jogador com quem quer trocar de lugar:\n");
                    while(!fila_vazia(jogadores)){
                        remove_fila(jogadores, &alvo);
                        printf("%d - %s (Casa %d)\n", tam_fila + 1, alvo.nome, alvo.posicao->info.posicao);
                        insere_fila(fila_aux, alvo);
                        tam_fila++;
                    }

                    int escolha_troca;
                    printf("Digite o numero do jogador: ");
                    scanf("%d", &escolha_troca);
                    while (getchar() != '\n' && !feof(stdin));

                    int indice_atual = 1;
                    for(int i = 0; i < tam_fila; i++){
                        remove_fila(fila_aux, &alvo);
                        if(indice_atual == escolha_troca) {
                            if (alvo.status_imune == 1) {
                                printf("[DEFESA] %s esta IMUNE! A troca falhou!\n", alvo.nome);
                            } else {
                                // limpa as cores das posições atuais no tabuleiro
                                jogador.posicao->info.cor[jogador.cor - 1] = 0;
                                alvo.posicao->info.cor[alvo.cor - 1] = 0;

                                // troca os ponteiros da lista duplamente encadeada
                                tp_no *temp_pos = jogador.posicao;
                                jogador.posicao = alvo.posicao;
                                alvo.posicao = temp_pos;

                                // aplica as cores nas novas posições
                                jogador.posicao->info.cor[jogador.cor - 1] = 1;
                                alvo.posicao->info.cor[alvo.cor - 1] = 1;

                                printf("[BURACO DE MINHOCA] ZUUM! Voce trocou de lugar com %s!\n", alvo.nome);
                            }
                        }
                        insere_fila(jogadores, alvo);
                        indice_atual++;
                    }
                    destroi_fila(fila_aux);
                }

                else if (escolha_item == 6) {
                    // ID 6: Imunidade a resenha
                    jogador.status_imune = 1;
                }
                else if (escolha_item == 7) {
                    // ID 7: Castigo
                    // lógica para sortear alguém da fila e aplicar o debuff
                    int tam_fila = 0;
                    tp_fila *fila_aux = inicializa_fila();
                    Player alvo;

                    // conta quantos jogadores tem na fila
                    while(!fila_vazia(jogadores)){
                        remove_fila(jogadores, &alvo);
                        insere_fila(fila_aux, alvo);
                        tam_fila++;
                    }

                    // sorteia um índice
                    int alvo_idx = rand() % tam_fila;

                    // devolve os jogadores para a fila principal, aplicando o castigo no sorteado
                    for(int i = 0; i < tam_fila; i++){
                        remove_fila(fila_aux, &alvo);

                        if(i == alvo_idx) {
                            if (alvo.status_imune == 0) {
                                alvo.status_castigo = 1;
                                printf("[EFEITO] %s foi atingido pelo Castigo e perdera a proxima vez!\n", alvo.nome);
                            } else {
                                printf("[DEFESA] O Castigo atingiu %s, mas ele(a) estava IMUNE!\n", alvo.nome);
                            }
                        }
                        insere_fila(jogadores, alvo);
                    }
                    destroi_fila(fila_aux);
                }

            } else {
                printf("[ERRO] Voce nao possui este item. Seguindo para a rolagem...\n");
            }
        }
    }


    // sistema para o item de rolar os dados novamente
   int valor_dado = rola_dados();

        if (jogador.status_rerol == 1) {    // se o jogador apertou para usar o item, a função acontece
            char opcao_rerol;
            printf("\n[REROL ATIVO] Voce tirou um %d. Deseja rolar novamente? (S/N): ", valor_dado);
            scanf(" %c", &opcao_rerol);
            while (getchar() != '\n' && !feof(stdin));

            if (toupper(opcao_rerol) == 'S') {
                printf("Rolando novamente...\n");
                valor_dado = rola_dados();
            } else {
                printf("Voce escolheu manter o valor %d.\n", valor_dado);
            }
            jogador.status_rerol = 0; // desativa o efeito para o próximo turno
        }

    // movimentar o jogador pelo dado
    venceu = move_posicao(&jogador, valor_dado);

    system("cls"); // limpa a tela para dar a ilusão de animação
    imprime_tabuleiro_visual(tabuleiro);

    printf("\n>>> %s tirou %d no dado e avancou para a Casa %d! <<<\n", jogador.nome, valor_dado, jogador.posicao->info.posicao);

    if (venceu == 1) {
        printf("\n>>> PARABENS! %s ALCANCOU A CHEGADA E VENCEU O JOGO! <<<\n", jogador.nome);
        return 1;
    }

    printf("%s avancou para a Casa %d.\n", jogador.nome, jogador.posicao->info.posicao);

    // verifica se a casa atual tem pergunta ou baú
    if (jogador.posicao->info.tira_carta == 1) {
        printf("\n[ATENCAO] Voce caiu em uma casa de pergunta!\n");

        if (jogador.posicao->info.posicao <= 10) {
            printf("[Nivel: FACIL]\n");
            resposta = geraPergunta(perguntas_faceis, perguntas_faceis_descartadas);
            if (resposta == 1) {
                printf("Voce acertou! Bonus: Avance 1 casa extra.\n");
                atualiza_hank(&jogador, 10);
                venceu = move_posicao(&jogador, 1);
            } else {
                printf("Voce errou! Penalidade: Volte 1 casa.\n");
                atualiza_hank(&jogador, -10);
                move_posicao(&jogador, -1);
            }
        }
        else {
            printf("[Nivel: MEDIO]\n");
            resposta = geraPergunta(perguntas_medias, perguntas_medias_descartadas);
            if (resposta == 1) {
                printf("Voce acertou! Bonus: Avance 2 casas extras.\n");
                atualiza_hank(&jogador, 20);
                venceu = move_posicao(&jogador, 2);
            } else {
                printf("Voce errou! Penalidade: Volte 2 casas.\n");
                atualiza_hank(&jogador, -20);
                move_posicao(&jogador, -2);
            }
        }

        if (venceu == 1) {
            printf("\n>>> PARABENS! %s ALCANCOU A CHEGADA E VENCEU O JOGO! <<<\n", jogador.nome);
            return 1;
        }
    }

    // lógica de cair numa casa especial de baú
    else if (jogador.posicao->info.tira_carta == 2) {
        printf("\n[SURPRESA] %s caiu numa CASA DE BAU!\n", jogador.nome);

        // verifica se o inventário não está cheio (máximo 3 itens)
        if (jogador.qtd_itens < 3) {
            int novo_item = sortear_item();

            // procura o primeiro slot vazio (que tem valor 0) e guarda o item
            for (int i = 0; i < 3; i++) {
                if (jogador.inventario[i] == 0) {
                    jogador.inventario[i] = novo_item;
                    jogador.qtd_itens++;

                    printf(">>> Encontrou um item: ");
                    imprime_nome_item(novo_item);
                    printf("! Guardado no Slot %d. <<<\n", i + 1);
                    break; // sai do ciclo for após guardar o item
                }
            }
        } else {
            printf("[INVENTARIO CHEIO] Abriu o bau, mas o seu inventario ja tem 3 itens. Nao pode carregar mais!\n");
        }
    }
    // ==========================================
    else {
        printf("\nEsta e uma casa comum. Nada acontece.\n");
    }

    // devolve o jogador para o fim da fila
    insere_fila(jogadores, jogador);
    return 0; // o jogo continua
}

#endif

