// Um grande artista assina a sua obra


#ifndef pilha_h
#define pilha_h
#include <stdio.h>
#include "tipos.h"
#define MAX 100


typedef tp_pergunta
	tp_item;
	
typedef struct {
	int topo;
	tp_item item[MAX];
} tp_pilha;

void inicializa_pilha( tp_pilha *p){
	p->topo = -1;
}

int pilha_vazia(tp_pilha *p){
	if (p->topo == -1) return 1;
	return 0;
}

int pilha_cheia(tp_pilha *p){
	if (p->topo == MAX -1) return 1;
	return 0;
}

int push(tp_pilha *p, tp_item e){
	if(pilha_cheia(p) == 0){
		p->topo += 1;
		strcpy(p->item[p->topo].enunciado, e.enunciado);
		p->item[p->topo].resposta = e.resposta;
		return 1;
	}
	return 0;
		
	
}

int pop(tp_pilha *p, tp_item *e){
	if (pilha_vazia(p) == 1){
		return 0;
	}
	strcpy(p->item[p->topo].enunciado, e->enunciado);
	e->resposta = p->item[p->topo].resposta;
	p->topo--;
	return 1;
}
int top(tp_pilha *p, tp_item *e){
		if (pilha_vazia(p) == 1){
		return 0;
	}
	*e = p->item[p->topo];
	return 1;
}

void imprime_pilha(tp_pilha p){
	tp_item e;
	printf("\n");
	while(!pilha_vazia(&p)){
		pop(&p, &e);
		printf("%d\n", e);
	}
	printf("\n");
}

int altura_pilha(tp_pilha *p){
	return p->topo+1;
}

# endif
