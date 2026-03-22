# include <stdio.h>
# include "pilha.h"
# include <stdlib.h>
# include <string.h>

typedef struct{
		char nome[51];
        int posicao;
		char cor[10];
		tp_pilha perguntas[100];
	} jogador;
	

int rola_dados(){
    int maior = 6; // define o maior valor que vai ser rolado no dado
	int menor = 1; // define o menor valor que será rolado no dado
	return rand() % (maior - menor + 1); // retorna um numero aleatorio entre 6 e 1 (valores dos dados)
}


void inicializa_personagens(jogador *j){ // Inicializa o personagem, definido a sua podeição como 0 e defindo seu nome e sua cor (a cor não foi feita ainda)
    j->posicao = 0;
	char afirma = 's';
	
	
	while(afirma != 'n'){
	printf("Defina um nome para seu personagem (máximo de 50 caracteres):\n");
    scanf(" %[^\n]", j->nome);
	printf("O seu nome eh: %s\nDeseja alerar?(s/n)\n: ", j->nome);
	scanf(" %c", &afirma);
	
	}
}

int main(){
	jogador j1;
	inicializa_personagens(&j1);


	 return 0;
}
