#include <stdio.h>
#include "pilha.h"
#include <stdlib.h>
#include <string.h>
#include <locale.h>
#include <ctype.h>
#include <time.h>

typedef struct{		// struct de jogadores

    char nome[25];
    int cor;
    int posicao;

}Player;
	

int rola_dados(){
    int valor; 
    valor = (rand()%(6))+1; // Gera um valor aleatorio
    return valor;
}

int seletor_de_questão(int qtd){
    int e; 
    e = rand() % (qtd); // Gera um valor aleatorio entre 0 e qtd de perguntas
    return e;
}

void move_posicao(Player *jogador, int posi){
    jogador->posicao += posi;
}


int quantidade(){
    int quant_players; // variavel local da função que irá retornar a quantidade de jogadores para a main

    printf("\t\nQuantos jogadores irão participar do jogo? (MAX: 4):\n");
    scanf("%d",&quant_players);

    while(quant_players < 1 || quant_players > 4){  // estrutura de repetição que certifica que usuario não irá prosseguir com valor inválido
        printf("Quantidade não permitida, escolha novamente (MAX: 4):\n");
        scanf("%d",&quant_players);
    }

    return quant_players;   // função retornando quantidade de jogadores
}

int verificarNome(char *nome){  // função que verifica se nome é valído

    if(strlen(nome) >= 20)  // string lenght usada para descobrir tamanho da string(nome) e verificar se é maior que o limite de caracteres ( 20 )
        return 0;   // inválido
    else
        return 1;   // válido
}

int verificarCor(Player *player, int corAux, int jogadorAtual){ // função para verificar se cor de futuros jogadores já não foi escolhida

    int i;

    if(corAux < 1 || corAux > 4){   // verificação prévia para ver se a cor inserida é uma opção
        return 0;
    }

    for(i=0; i < jogadorAtual; i++){ // laço que usa o numero do jogador atual como criterio de parada para evitar loop bruto

        if(player[i].cor == corAux) // se a cor de jogadores anteriores for igual ao do jogador atual, invalidar cor
            return 0;

        else
            return 1;   // se cor for diferente, válidar
    }
    return 1;
}

void sousaEasterEgg(char *nomeAux){

    if (strcmp(nomeAux, "Sousa") == 0){    // string "i" compare para facilitar possiveis variantes de um mesmo resultado (isensitive)
        printf("\n\033[33m[SEGREDO DESBLOQUEADO] O dinossauro da computação entrou na partida!\033[0m\n");

        printf("\033[33m");

        printf("                      }}}}}}}}}}}}}}}{  \n");
        printf("                   Y}}}1({}}}}}}}}}}}}}{\n");
        printf("                   Y}}}}{{  }}}}}}}}}}}}}{\n");
        printf("                  Y}}}}}}}  }}}}}}}}}}}}{\n");
        printf("                   Y}}}}}}}}}}}}}}}}}}}{\n");
        printf("                   u}}}}}}}}}}{{{{{{{{{1\n");
        printf("                   u}}}}}}}}}{|||||/    \n");
        printf(")}_               _}(}}}}}}}}{{{{{{{)    \n");
        printf("1}|             |1)}}}}}}}}}}            <----- SOUSA KKKKKKKKKKKKKK\n");
        printf("{}){{        ___{}}}}}}}}}}}}]]]|        \n");
        printf("{}}}1        {}}}}}}}}}}}}}}}}}}}        \n");
        printf("{}}}}}1    1}}}}}}}}}}}}}}}} )}1        \n");
        printf("{}}}}}}}}}}}}}}}}}}}}}}}}}}}            \n");
        printf("1{}}}}}}}}}}}}}}}}}}}}}}}}}}            \n");
        printf("  1{}}}}}}}}}}}}}}}}}}}}}})1            \n");
        printf("   _{{{{}}}}}}}}}}}}}}}}{{              \n");
        printf("         {}}}}}}}}}}}}{{{                \n");
        printf("         {}}}}}}}}}}}}1                  \n");
        printf("          u[}}}}}{x}}}{                  \n");
        printf("          ]}}{})     {1                  \n");
        printf("          ]}}        {1                  \n");
        printf("          j[}}}{     {}}}{                \n");

        // reseta a cor de volta ao normal
        printf("\033[0m\n");
    }

    if (strcmp(nomeAux, "Sanval") == 0){    // string "i" compare para facilitar possiveis variantes de um mesmo resultado (isensitive)
        printf("\n\033[33m[SEGREDO DESBLOQUEADO] O cara do Breaking Bad? Ele também tá aqui!\033[0m\n");

        printf("\033[33m");

        printf("                                                                 \n");
        printf("                                                                     \n");
        printf("                                                                     \n");
        printf("                                Tffjrxxnnnnnnxjf                 \n");
        printf("                            FrrrjFjxxvcvvunxxnnnunnj             \n");
        printf("                          rxnrjjrxxnxuuvuuunnnxnnnuuuvun         \n");
        printf("                        jjxjjxuuuvuuvczczcvvvunrrnnunnnuxr       \n");
        printf("                      TjFjrvXYYUYYXXYUYYYXXzXcczzXYXzcurrxr      \n");
        printf("                     xFfFxXYUJJJJJLLLLQQmQLCCUCJUYYzzcccnjrr     \n");
        printf("                    TffFuXXUUYUUJJLQLQQQmQQQLCCCJUYXXXYXXurjr    \n");
        printf("                    ttFxvzYUUYUUJCLLQQQQmQQQLLLCJUYYXXzXXcxjff   \n");
        printf("                  TTfFnuzYYUUYJCCQQmwmmqwmmQLQLCJYXzzzzzcnrfft   \n");
        printf("                  !ttFxuzYYYUUJLLmqqppqqwqqmQQQCUYXXzvcvvxrfTt   \n");
        printf("                  l!!lfjuzYYYUUCLQwppqqqqwwwwmQmLJYzcvvvvvunTt!  \n");
        printf("                  !!!tFjucXXUJJJLQwpwqwqppwqqmQmQJUcvnnnczzvft!  \n");
        printf("                  ;I!fjxvzXXYJUJCQQmmmwwwqmmwmQmQCYzczvcUcczjt!  \n");
        printf("                  tIIfrucuncUULLQwwQmmmmmmQmQLJCCYYXXcuuXvvcrl!  \n");
        printf("                   IlfxnnnnvcYUCLQmQQmQmwwwmmQQCUXczcunzYvvvrt!  \n");
        printf("                   IIfjnvczYUJCJUYCwqmQmmmwmQLmQJUCXzcczzXcnrtt  \n");
        printf("               uYJUl!FrxrxnuvYJmQmQQQJCLQYzJLLLUXvnxjFfTFrvcnTTzXv\n");
        printf("               T:FJjTFFTtt!!tT!fFTtFTFrcUJYjtTtlIlIIIIIlltrccFnvnj\n");
        printf("               xTTfFtjjT!i!FjFfjjt;,:ttuYXvft.,ITrxxrjTltfrxxIuxzx\n");
        printf("               fzvrTrFffItl;:!I:lnItiltTfFTtilticl;IrI;tt!rFcjjTun\n");
        printf("                nFl!xnjjjFFjnnzYzvYUuFjXJUzjTxYYXXXcFrrTFFFruj!fjz\n");
        printf("                xvxrFrjvvcvjrucvzuzuxTuYCUccFxvvvXzurFjvunxrfutnnn\n");
        printf("                 znjtfTjcXXzzuvzcccvFruzCUzznjnvuvnnxuvccuTXXcrcz \n");
        printf("                 rnfIF!rruXYYUJJXcuFjxncCJcccFFxnvXXcccvjFfXXuvxn \n");
        printf("                  jFlfFt!FnvvvvuuvvnFrcXLLXcufrnxxrxnnnjtFnXzrvX  \n");
        printf("                  njTfFfjrxvvzYUCUYnvnvCQQCuvvxuczzvuunxrjxzcncY  \n");
        printf("                  !fftTffFrnuvzUJcffttfnXYcjFrjFuccvunxrxrnzxFr   \n");
        printf("                    FfTTFFrnucvrruF:,,:;::..,fnjjnxxrxrxruvv      \n");
        printf("                    jFFFfFFrxxrnxjFflIi,,:,;I!frxjjTjrjjxcnn      \n");
        printf("                     fFFFFfFjjjt!lllilI;;!IiiiIltTFxrxxjnur       \n");
        printf("                     fFfFjfFjflI;;ii;IlIiIliI;::;I!frxrjuxx       \n");
        printf("                     tTTfrfjti:,,::ii;;::;::,:::,,:IFnrrvjf       \n");
        printf("                      ;ltfjTl::!fFfFjrFTtttT!!fff;;!FrjnrF        \n");
        printf("                       .;!T!l;;ljrrFTI;:;iilTfrjTIItFfrTl         \n");
        printf("                       .,;!tI:ilfrxF!i;:;;;iljFtIII!TfI::         \n");
        printf("                 UzxTIi,,:ii::i;ltttI!llIIlIIiIi;;lI,,:IvJLCU     \n");
        printf("                 zYJYvTIi,,,,,::::illIIlIIi;;;;:;:,:,,,:linnzzvX  \n");
        printf("                vzXYJJcri:,:,,,,,,:;:;;;;:::::::,,,,,,,lt.!YwwXcUC \n");
        printf("              YYYYUUUUYcv;i::,,,.,,:::::::,,,.,,,.,,.,ltj!nLUcuuvc \n");
        printf("            zUYXzcUUJCUUzFlli;:,,,..,,,.,.,........,:ltfjFUUQYcccvU\n");
        printf("          uzzXYXzYLUcXXJJYt!T!I;:,,....     .....,:i!TTFfXUJUYzcnxx\n");
        printf("        uuzUUUUXJJUUJCLQXY!tTtli:,,.. .    ..,,,itTTFjfXzYYYntnczYXXYYYc \n");
        printf("    XXYUUUUYXvjruzYJJUXYJJUYtTTtli;::,......,,:;lTFjrjFXXQYnUuzXYXYYYXYYXYYYY  \n");
        printf("UUJUUJUUJJJUUUUctlfvUJLmmLJLnfrfI;;:::;:::::ilfFjxxrrYcLxvCXzYYJYYUYYYYXzXYUUJUYU\n");
        printf("JUJJUUUUYXUJJCCJJUvf;tzmbmUUJXfxFftI;;iIIIl!TfjxuxrnXvQnXmCcJJULJUJUYYXXYUUUUUUJU\n");
        printf("CJCLJJJJJzzYUCLLmmCYJF:;tucJmQYxnxrFT!ll!tTfFjruvrnccdcJqdvXLLJmCUJUYXczJCJJJJJCJ\n");
        printf("LCLmLLCCJUzzXULLQQCXJUcvFilxYqLCLnjrjfTtttfjxxnuncXcwzUwbLvCCLYpmJJUYzvzJCCCJUQmL\n");
        printf("UULwQLQLLCYzcXUCQQLYXCJYzvut;tYwwCffFFjjrrrrrxxnJLvpJzQbwnCCYLXqqCUYXvcYJCCCCQwwQ\n");
        printf("zzQpwmQLLLCYccYQwwmLXYmmLJvXXf;rqmQdkbpwLJYJQdbwwcQqYLpQvXLYJJYddmUXccXYJCLQmqmLJ\n");
        printf("XvmdwQwmQLLUcvYmdpqqJcCpdwmCvUzTcbbqkhaaooMaQdbwvXdUmbUcYLCJQYUbdwYccXXYCLQmwmCzU\n");
        printf("JvQbqmqwQLLJzvYmdddpmzXqmmCCmXvcFzmQwpbhhhahwhqJuqQCdUzJLJQmwXmbpCzzXYXYCLmwmCYYQ\n");
        printf("LvCdqwpqQLLCXzYQpbbddJcQbLQLYQwzurYJQbhahaMaQbwvCwXpmJmCJQqpQYdbQXYJUYYJLmwmCYXJQ\n");
        printf("QcJqqqpqQQQQUYUQpkkbbmzUpbCmwYUqpcumzwhahhoawdcXkXwbmqLUQqqpJQkmJJQQUYJLQwmCYXYJm\n");

        printf("\033[0m\n");
    }

}

void imprimirJogadores(Player *player, int quantidade) { // função para imprimir todos os jogadores após a função de ler os dados
    int i;

    printf("\n=================================\n");
    printf("     LISTA FINAL DE JOGADORES    \n");
    printf("=================================\n");

    for(i = 0; i < quantidade; i++) {
        // Analisa a cor do jogador atual para escolher a cor do printf
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
                printf("Jogador %d: %s \n", i + 1, player[i].nome); // caso de segurança
                break;
        }
    }
    printf("\n");
}

void lerDados(Player *player, int quant){
    int i;
    int confirmNome, confirmCor; // variáveis usadas como adaptação para variável boolean
    char nomeAux[99];   // variavel local para armazenar nome original inserido pelo usuario
    int corAux; // variavel local para armazenar cor original inserida pelo usuario

    for(i = 0; i < quant; i++) {    // estrutura de repetição para cadastrar jogadores com condição de parada a quantidade de jogadores
        printf("\n=============================\n");
        printf("   CADASTRANDO JOGADOR %d\n", i + 1);
        printf("=============================\n");

        do{
            printf("\t\nQual o nome do jogador %d?\n", i+1);
            scanf(" %99[^\n]", nomeAux);
            getchar();  // limpar buffer do teclado por conta do enter inserido

            confirmNome = verificarNome(nomeAux); // variável "boolean"  recebendo o retorno da função que verifica se o nome inserido é permitido
                                                                                                            // 0 = inválido     1 = válido
            if (confirmNome == 1){
                strcpy(player[i].nome, nomeAux);    // se o nome for válido, ocorre a atribuíção da variavel local auxiliar para o vetor struct
                                                                                                        // que armazena o nome com a função de string copy
                sousaEasterEgg(player[i].nome);
            }
            else
                printf("[ERRO] Limite de 19 caracteres excedido. Tente novamente.\n"); // caso inválido, estrutura de repetição while obriga inserir um válido
        }while(confirmNome!=1);

            do{
            printf("\nQual cor deseja escolher?\n");
            printf("1 - Vermelho | 2 - Verde | 3 - Azul | 4 - Amarelo\n");
            printf("Sua escolha: ");
            scanf("%d", &corAux);

            confirmCor = verificarCor(player, corAux, i);  // chamada da função que irá verificar se a cor inserida pelo usuario não foi inserida anteriormente

            if(confirmCor == 1){    // se a cor não foi repetida, ela é validada e atribuída ao jogador atual
                player[i].cor = corAux;
                printf("Cor atribuida com sucesso ao jogador %s!\n", player[i].nome);
            }

            else
                printf("[ERRO] Cor ja escolhida por outro jogador ou invalida! Tente novamente.\n");
                // se cor inválida, é impresso erro e obriga o usuario a inserir uma nova cor
            }while(confirmCor!=1);


            printf("\n>>> Jogador %d adicionado com sucesso! <<<\n", i + 1);
    }
}

int geraPergunta(){
    char resposta;
    int e = seletor_de_questão(10);
    switch(e){
        case 0: 
            printf("As pilhas sao uma struct que retornam o primeiro dado inserido? V/F\n");
            scanf(" %c", &resposta); 
            if (toupper(resposta) == 'F')
                return 1;
            else 
                return 0;
        case 1:
            printf("Para acessar o conteudo de uma variavel, deve-se igualar o ponteiro 'p' ao endereco da variavel V/F\n");
            scanf(" %c", &resposta);
            if (toupper(resposta) == 'V')
                return 1;
            else 
                return 0;        
        case 2:
            printf("Qual o tipo de dado que consegue armazenar varios tipos de dados? int(1) float(2) char(3) struct(4)\n");
            scanf(" %c", &resposta);
            if (toupper(resposta) == '4')
                return 1;
            else 
                return 0;
        case 3:
            printf("Qual o comando utilizado para adicionar um item na pilha? pop(1) top(2) push(3) endif(4)\n");
            scanf(" %c", &resposta);
            if (toupper(resposta) == '3')
                return 1;
            else 
                return 0;
        case 4:
            printf("Em uma fila, qual o comando utilizado para que seja verificar se ela esta cheia? queue(1) proximo(2) remove(3) push(4)\n");
            scanf(" %c", &resposta);
            if (toupper(resposta) == '2')
                return 1;
            else 
                return 0;    
        case 5:
            printf("Em uma string, qual o termo utilizado para limitar a ultima casa?  nulo(1) void(2) \\0(3) 0(4)\n");
            scanf(" %c", &resposta);
            if (toupper(resposta) == '3')
                return 1;
            else 
                return 0;    
        case 6:
            printf("Qual a principal caracteristica de uma fila? siso(1) fifo(2) lifo(3) filo(4)\n");
            scanf(" %c", &resposta);
            if (toupper(resposta) == '2')
                return 1;
            else 
                return 0;
        case 7:
            printf("Considerando int x = y e *p = x, qual das alternativas pode alterar o valor de x? *p = 10(1) p = 10(2) &p = 10(3) nenhuma das anteriores(4)\n");
            scanf(" %c", &resposta);
            if (toupper(resposta) == '4')
                return 1;
            else 
                return 0;    
        case 8:
            printf("Em qual dos casos se faz necessario utilizar '->' ao inves de '*'? pilha(1) ponteiro(2) struct(3) string(4)\n");
            scanf(" %c", &resposta);
            if (toupper(resposta) == '3')
                return 1;
            else 
                return 0;    
        case 9:
            printf("Qual o especificador utilizado para imprimir uma fila contendo uma sequencia de caracteres? %%s(1) %%c(2) %%d(3) %%f(4)\n");
            scanf(" %c", &resposta);
            if (toupper(resposta) == '1')
                return 1;
            else 
                return 0;    
    }

    return 0;
}

void jogadorfila(tp_fila *f, char p[],int tam){
    for(int i=0, i < tam, i++){
        inserefila(f,p[i]);
    }
}

void rodadaplayer(tp_fila *f){
    Player pi;
    if(!filavazia(f)){
        removefila(f,&pi);
        int resultado = rola_dados();
        p.posicao += resultado;
        if(p.posicao >= 30){
        printf("o jogador %s venceu o jogo", p.nome);
        }else{
        inserefila(f,p);
    }
    
}
}
int main(){
    int e;
    srand(time(NULL)); // Usa o tempo para gerar os numeros aleatorios 
	tp_fila f;
    inicializafila(&f);
    int tam;

    Player player[4]; // definição da struct na main como vetor com quantidade máxima de player
    int quant; // quantidade de jogadores

    setlocale(LC_ALL, "Portuguese"); // função responsável por adicionar caracteres do PT-BR
    quant = quantidade();   // atribuição da função que lê a quantidade de jogadores para a variavel de quantidade da main

	jogadorfila(&f,player,tam);

    rodadaplayer(&f);
	
    lerDados(player, quant); // função para ler todos os dados dos jogadores, com o vetor de struct e a quantidade de jogadores como parâmetro

    printf("%d\n", geraPergunta());


    printf("Teste\n");
    

    return 0;
}
