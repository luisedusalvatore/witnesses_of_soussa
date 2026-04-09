# ifndef tipos_h
# define tipos_h
typedef struct{		// struct de jogadores

    char nome[25];
    int cor;
    int posicao;

}Player;

typedef struct{

    char pergunta[200];
    char resposta[100];
    char correto;
} tp_pergunta;
	
# endif