#ifndef fila_h
#define fila_h
#include <stdio.h>
#define MAX 100

typedef int tp_item;

typedef struct{
	tp_item item[MAX];
	int ini, fim;
	// int tam;
} tp_fila;

void inicializaFila(tp_fila *f){
	f->ini = f->fim = MAX-1;
}

int filaVazia(tp_fila *f){
	if(f->ini == f->fim) return 1;
	return 0;
}
int proximo (int pos){
	if(pos == MAX-1) return 0;
	return ++pos;
}

int filaCheia (tp_fila *f){
	if(proximo(f->fim) == f->ini)
		return 1;
	return 0;
}
int insereFila(tp_fila *f, tp_item e){
	if(filaCheia(f))
		return 0;
	f->fim = proximo(f->fim);
	f->item[f->fim]=e;
	return 1;
}

int removeFila (tp_fila *f, tp_item *e){
	if(filaVazia(f)) return 0;
	f->ini = proximo(f->ini);
	*e = f->item[f->ini]; 
}

void imprimeFila(tp_fila f){
	tp_item e;
	while(!filaVazia(&f)){
		removeFila(&f, &e);
		printf("%d\n", e);
	}
}

int tamanhoFila(tp_fila *f){
	if (filaVazia(f)) return 0;
	if (f->ini < f->fim) return f->fim - f->ini;
	return MAX - 1 - f->ini + f->fim + 1;
}

#endif