# include <stdio.h>
# include "pilha.h"
# include <stdlib.h>
# include <string.h>

typedef struct{
		char nome[10];
        int posicao = 0;
		tp_pilha perguntas[100];
	} jogador;
	
typedef struct{
		int Suca[2]= {1, 1}; // Primeira posição do vetor eh o se o personagem foi selecionado e o segundo para saber se ele ja foi printado no seleciona personagens
		int CB[2] = {1, 1};
		int Certinho[2] = {1, 1};
		int Sanvas[2] = {1, 1};
        int disponiveis = 4;
        int possi_disponiveis[4] = {1,1,1,1};
	} personagens;

int rola_dados(){
    int maior = 6; // define o maior valor que vai ser rolado no dado
	int menor = 1; // define o menor valor que será rolado no dado
	return rand() % (maior - menor + 1); // retorna um numero aleatorio entre 6 e 1 (valores dos dados)
}


void seleciona_personagens(personagens *p, jogador *j){
	int n;
	char nome[10];
    p->Suca[1] = 1;
    p->CB[1] = 1;
    p->Certinho[1] = 1;
    p->Sanvas[1] = 1;
    
	printf("Escolha o seu personagem:\n");
    for(int i = 0; i < p->disponiveis; i++){
	    if(p->Suca[0] == 1 && p->Suca[1] == 1){
            printf("%d Suça\n", i+1);
            p->Suca[1] == 0;
            break;
        }
	    if(p->CB[0] == 1 && p->CB[1] == 1){
            printf("%d CB\n", i+1);
            p->CB[1] == 0;
            break;
        }
	    if(p->Certinho[0] == 1 && p->Certinho[1] == 1){
            printf("%d Certinho\n", i+1);
            p->Certinho[1] == 0;
            break;
        }
	    if(p->Sanvas[0] == 1 && p->Sanvas[1] == 1){
            
            printf("%d Sanvas\n", i+1);
            p->Certinho[1] == 0;
            break;
        
        }
}
	scanf("%d", &n);
	while(n > p->disponiveis || n <= 0){
		printf("Numero invalido digite um numero valido\n");
		scanf("%d", &n);
	}
	
	
}
int main(){
	

char jogadores[10][4];




char p_facil[1000][10];

return 0;
}