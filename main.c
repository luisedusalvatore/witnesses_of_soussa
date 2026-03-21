# include <stdio.h>
# include "pilha.h"
# include <stdlib.h>
# include <string.h>

typedef struct{
		char nome[51];
        int posicao = 0;
		tp_pilha perguntas[100];
	} jogador;
	

int rola_dados(){
    int maior = 6; // define o maior valor que vai ser rolado no dado
	int menor = 1; // define o menor valor que será rolado no dado
	return rand() % (maior - menor + 1); // retorna um numero aleatorio entre 6 e 1 (valores dos dados)
}


void seleciona_personagens(jogador *j){
    
	printf("Escolha o seu personagem, o nome deve ter até 50 caracteres:\n");
    scanf(" %[^\n]", j->nome);
	}
	
	
int main(){


return 0;
}