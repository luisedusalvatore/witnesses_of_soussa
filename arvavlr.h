#ifndef ARVAVLR_H_INCLUDED
#define ARVAVLR_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <locale.h>
#include "tipos.h"

// 1. STRUCT CORRIGIDA (Renomeada e Descomentada)
struct tp_no_arvore {
   struct tp_no_arvore *esq;
   tp_hank info;    // <-- AGORA GUARDA O RANKING INTEIRO!
   int alt;
   int cont;
   struct tp_no_arvore *dir;
};

typedef struct tp_no_arvore* ArvAVL;

// PROTÓTIPOS
ArvAVL* criarAVL();
void preOrd(ArvAVL* raiz);
void emOrd(ArvAVL* raiz);
void posOrd(ArvAVL* raiz);
int inserir(ArvAVL* raiz, tp_hank novo_dado);
struct tp_no_arvore* buscarMenor(struct tp_no_arvore* atual);
int maior(int x, int y);
int alt_NO(struct tp_no_arvore* no);
int fb_NO(struct tp_no_arvore* no);
void RotacaoLL(ArvAVL* raiz);
void RotacaoRR(ArvAVL* raiz);
void RotacaoRL(ArvAVL* raiz);
void RotacaoLR(ArvAVL* raiz);

static int contPrint=0; // 3. STATIC ADICIONADO AQUI

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
      printf("%d", (*raiz)->info);
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
      printf("%d", (*raiz)->info);
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
      printf("%d", (*raiz)->info);
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

// Atualize também o protótipo lá em cima para: int inserir(ArvAVL* raiz, tp_hank novo_dado);

int inserir(ArvAVL* raiz, tp_hank novo_dado){
	int res;
	if(*raiz==NULL){
		struct tp_no_arvore* novo;
		novo = (struct tp_no_arvore*)malloc(sizeof(struct tp_no_arvore));
		if(novo==NULL) return 0;
		novo->info = novo_dado; // Copia a struct inteira com nome, erros, acertos e score!
		novo->cont = 0;
		novo->alt = 0;
		novo->dir = NULL;
		novo->esq = NULL;
		*raiz = novo;
		return 1;
	} else {
		struct tp_no_arvore* atual = *raiz;
        // COMPARA PELO SCORE DA STRUCT
		if(novo_dado.score < atual->info.score){
			if((res=inserir(&(atual->esq), novo_dado))==1){
				if(fb_NO(atual) >= 2){
					if(novo_dado.score < (*raiz)->esq->info.score){
						RotacaoLL(raiz);
					}else{
						RotacaoLR(raiz);
					}
				}
			}
		}else{
            // Se for MAIOR OU IGUAL (permite scores repetidos) vai para a direita
			if((res=inserir(&(atual->dir), novo_dado))==1){
				if(fb_NO(atual) >= 2){
                    // Usa >= para acompanhar a lógica de permitir empates
					if(novo_dado.score >= (*raiz)->dir->info.score){
						RotacaoRR(raiz);
					}else{
						RotacaoRL(raiz);
					}
				}
			}
		}
		atual->alt = maior(alt_NO(atual->esq), alt_NO(atual->dir)) + 1;
		return res;
	}
}

void imprimeRanking(ArvAVL* raiz){
   if(raiz==NULL) return;
   if(*raiz != NULL){
      imprimeRanking(&((*raiz)->dir)); // Visita os maiores scores (Direita)

      printf("Jogador: %-15s | Score: %4d | Acertos: %2d | Erros: %2d\n",
             (*raiz)->info.nome, (*raiz)->info.score, (*raiz)->info.acertos, (*raiz)->info.erros);

      imprimeRanking(&((*raiz)->esq)); // Visita os menores scores (Esquerda)
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
    // 4. LINHA CRÍTICA ADICIONADA: Passa o filho da direita de 'no' para a esquerda da 'raiz'
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
