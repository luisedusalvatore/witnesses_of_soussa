#ifndef ARVAVLR_H_INCLUDED
#define ARVAVLR_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <locale.h>
#include "tipos.h"

struct tp_no_arvore {
   struct tp_no_arvore *esq;
   tp_estatistica_casa info;
   int alt;
   struct tp_no_arvore *dir;
};

typedef struct tp_no_arvore* ArvAVL;

// prototipos
ArvAVL* criarAVL();
void preOrd(ArvAVL* raiz);
void emOrd(ArvAVL* raiz);
void posOrd(ArvAVL* raiz);
struct tp_no_arvore* buscarMenor(struct tp_no_arvore* atual);
int maior(int x, int y);
int alt_NO(struct tp_no_arvore* no);
int fb_NO(struct tp_no_arvore* no);
void RotacaoLL(ArvAVL* raiz);
void RotacaoRR(ArvAVL* raiz);
void RotacaoRL(ArvAVL* raiz);
void RotacaoLR(ArvAVL* raiz);
int inserir(ArvAVL* raiz, tp_estatistica_casa novo_dado);
void registra_passagem(ArvAVL* raiz, int num_casa, int acertou, int errou);

static int contPrint=0;

ArvAVL* criarAVL(){
    ArvAVL *raiz = (ArvAVL*)malloc(sizeof(ArvAVL));
    if(raiz != NULL){
        *raiz = NULL;
    }
    return raiz;
}

int estah_vaziaABB(ArvAVL* raiz){
	if(raiz==NULL) return 1;
	if(*raiz == NULL) return 1;
	return 0;
}

void preOrd(ArvAVL* raiz){
   if(raiz==NULL) return;
   if(*raiz != NULL){
   	  if(contPrint!=0){
	     printf(" ");
	  }
      printf("%d", (*raiz)->info.numero_casa);
      contPrint++;
      preOrd(&((*raiz)->esq));
      preOrd(&((*raiz)->dir));
   }
}

void emOrd(ArvAVL* raiz){
   if(raiz==NULL) return;
   if(*raiz != NULL){
      emOrd(&((*raiz)->esq));
   	  if(contPrint!=0){
	     printf(" ");
	  }
      printf("%d", (*raiz)->info.numero_casa);
      contPrint++;
      emOrd(&((*raiz)->dir));
   }
}

void posOrd(ArvAVL* raiz){
   if(raiz==NULL) return;
   if(*raiz != NULL){
      posOrd(&((*raiz)->esq));
      posOrd(&((*raiz)->dir));
   	  if(contPrint!=0){
	     printf(" ");
	  }
      printf("%d", (*raiz)->info.numero_casa);
      contPrint++;
   }
}

int alturaAVL(ArvAVL* raiz){
	if(raiz == NULL) return 0;
	if(*raiz == NULL) return 0;
	int alt_esq = alturaAVL(&((*raiz)->esq));
	int alt_dir = alturaAVL(&((*raiz)->dir));
	if(alt_esq > alt_dir){
		return alt_esq + 1;
	}else{
		return alt_dir + 1;
	}
}

int totalNOsABB(ArvAVL* raiz){
	if(raiz == NULL) return 0;
	if(*raiz == NULL) return 0;
	int tot_esq = totalNOsABB(&((*raiz)->esq));
	int tot_dir = totalNOsABB(&((*raiz)->dir));
	return (tot_esq + tot_dir + 1);
}


int inserir(ArvAVL* raiz, tp_estatistica_casa novo_dado){
	int res;
	if(*raiz==NULL){
		struct tp_no_arvore* novo = (struct tp_no_arvore*)malloc(sizeof(struct tp_no_arvore));
		if(novo==NULL) return 0;
		novo->info = novo_dado;
		novo->alt = 0;
		novo->dir = NULL;
		novo->esq = NULL;
		*raiz = novo;
		return 1;
	} else {
		struct tp_no_arvore* atual = *raiz;
		if(novo_dado.numero_casa < atual->info.numero_casa){
			if((res=inserir(&(atual->esq), novo_dado))==1){
				if(fb_NO(atual) >= 2){
					if(novo_dado.numero_casa < (*raiz)->esq->info.numero_casa) RotacaoLL(raiz);
					else RotacaoLR(raiz);
				}
			}
		} else if(novo_dado.numero_casa > atual->info.numero_casa){
			if((res=inserir(&(atual->dir), novo_dado))==1){
				if(fb_NO(atual) >= 2){
					if(novo_dado.numero_casa > (*raiz)->dir->info.numero_casa) RotacaoRR(raiz);
					else RotacaoRL(raiz);
				}
			}
		} else {
            return 0;
        }
		atual->alt = maior(alt_NO(atual->esq), alt_NO(atual->dir)) + 1;
		return res;
	}
}

// função para buscar a casa que caiu e atualizar os status
void registra_passagem(ArvAVL* raiz, int num_casa, int acertou, int errou) {
    if(raiz == NULL || *raiz == NULL) return;

    struct tp_no_arvore* atual = *raiz;

    while(atual != NULL){
        if(atual->info.numero_casa == num_casa){
            atual->info.vezes_caiu++;
            if(acertou) atual->info.acertos++;
            if(errou) atual->info.erros++;
            return;
        }

        if(num_casa < atual->info.numero_casa) atual = atual->esq;
        else atual = atual->dir;
    }
}

void liberaNO(struct tp_no_arvore* no){
   if(no==NULL) return;
   liberaNO(no->esq);
   liberaNO(no->dir);
   free(no);
   no = NULL;
}

void liberaABB(ArvAVL* raiz){
   if(raiz==NULL) return;
   liberaNO(*raiz);
   free(raiz);
}
/*
int consultarValorAVL(ArvAVL* raiz, int valor){
	if(raiz == NULL) return 0;
	if(*raiz == NULL) return 0;
	struct tp_no_arvore* atual = *raiz;
	while(atual != NULL){
		if(atual->info == valor){
			return 1;
		}else{
			if(atual->info > valor){
				atual = atual->esq;
			}else{
				atual = atual->dir;
			}
		}
	}
	return 0;
}
*/
int alt_NO(struct tp_no_arvore* no){
   if(no == NULL) return -1;
   else return no->alt;
}

int fb_NO(struct tp_no_arvore* no){
	int esq = alt_NO(no->esq);
	int dir = alt_NO(no->dir);
	return labs(esq - dir);
}

int maior(int x, int y){
	return (x>y)?x:y;
}

void RotacaoLL(ArvAVL* raiz){
	struct tp_no_arvore *no;
	no = (*raiz)->esq;
    (*raiz)->esq = no->dir;
	no->dir = *raiz;
	(*raiz)->alt = maior(alt_NO((*raiz)->esq), alt_NO((*raiz)->dir)) +1;
	no->alt = maior(alt_NO(no->esq), (*raiz)->alt) + 1;
	*raiz = no;
}

void RotacaoRR(ArvAVL* raiz){
   struct tp_no_arvore* no;
   no = (*raiz)->dir;
   (*raiz)->dir = no->esq;
   no->esq = *raiz;
   (*raiz)->alt = maior(alt_NO((*raiz)->esq), alt_NO((*raiz)->dir)) + 1;
   no->alt = maior(alt_NO(no->dir), (*raiz)->alt) + 1;
   *raiz = no;
}

void RotacaoLR(ArvAVL* raiz){
	RotacaoRR(&(*raiz)->esq);
	RotacaoLL(raiz);
}

void RotacaoRL(ArvAVL* raiz){
	RotacaoLL(&(*raiz)->dir);
	RotacaoRR(raiz);
}

/*int remover(ArvAVL* raiz, int valor){
	int res;
	if(*raiz==NULL){ // Valor não encontrado!
		printf("Valor %d nao encontrado na arvore!", valor);
		return 0;
	}
	if(valor < (*raiz)->info){
		if((res=remover(&(*raiz)->esq, valor))==1){
			if(fb_NO(*raiz) >= 2){
				if(alt_NO((*raiz)->dir->esq) <= alt_NO((*raiz)->dir->dir)){
					RotacaoRR(raiz);
				}else{
					RotacaoRL(raiz);
				}
			}
		}
	}else if(valor > (*raiz)->info){
		if((res=remover(&(*raiz)->dir, valor))==1){
			if(fb_NO(*raiz) >= 2){
				if(alt_NO((*raiz)->esq->dir) <= alt_NO((*raiz)->esq->esq)){
					RotacaoLL(raiz);
				}else{
					RotacaoLR(raiz);
				}
			}
		}
	}else{ // raiz->info == valor...
		if(((*raiz)->esq == NULL) || ((*raiz)->dir == NULL)){ // O Pai tem 1 ou nenhum filho
			struct tp_no_arvore* noVelho = (*raiz);
			if((*raiz)->esq != NULL) *raiz = (*raiz)->esq;
			else *raiz = (*raiz)->dir;
			free(noVelho);
			return 1;
		}else{ // o nó tem 2 filhos: precisamos substituir pelo nó mais a esquerda da subárvore da direita
			struct tp_no_arvore* temp = buscarMenor((*raiz)->dir);
			(*raiz)->info = temp->info;
			remover(&(*raiz)->dir, (*raiz)->info);
			if(fb_NO(*raiz) >= 2){
				if(alt_NO((*raiz)->esq->dir) <= alt_NO((*raiz)->esq->esq)){
					RotacaoLL(raiz);
				}else{
					RotacaoLR(raiz);
				}
			}
			return 1;
		}
		return res;
	}
}
*/
struct tp_no_arvore* buscarMenor(struct tp_no_arvore* atual){
	struct tp_no_arvore* no1 = atual;
	struct tp_no_arvore* no2 = atual->esq;
	while(no2 != NULL){
		no1 = no2;
		no2 = no1->esq;
	}
	return no1;
}

void salva_hank(tp_hank jogador){
    FILE *arquivo = fopen("hank.txt", "a");
    if(arquivo != NULL) {
        fprintf(arquivo, "%d %d %d %s\n", jogador.acertos, jogador.erros, jogador.score, jogador.nome);
        fclose(arquivo);
    }
}

/*
int CaiuNaCasa(ArvAVL* raiz, int id_casa) {
    if (raiz == NULL || *raiz == NULL) return 0;

    struct tp_no_arvore* atual = *raiz;
    while (atual != NULL) {
        if (atual->info == id_casa) {
            atual->cont++;
            return 1;
        } else if (id_casa < atual->info) {
            atual = atual->esq;
        } else {
            atual = atual->dir;
        }
    }
    return 0;
}
*/
#endif // ARVAVLR_H_INCLUDED
