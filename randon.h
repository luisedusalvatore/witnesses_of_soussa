#ifndef randon_h
#define randon_h
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
#endif