#include <stdio.h>
#include <stdlib.h>


#define nMax 20

typedef struct 
{
    int valores[nMax];
    int inicio;
    int final;
}Queue;


void inicializarFila(Queue *fila){
    fila->inicio= -1;
    (*fila).final= -1;

}

int checarSeEncheu(Queue *fila){
    return fila->final == nMax -1;// Checa se a posição 19 está ocupada
}

int checarSeTaVazia(Queue *fila){
    return fila->inicio == -1 || fila->inicio == fila->final; //Checa se o início continua igual a como foi inicializado
}

void addNaFila(Queue *fila, int valor){
    if (checarSeEncheu(fila))
    {
         printf("A fila está cheia");
    }else{
        if (fila->inicio == -1)
            fila->inicio = 0;
        fila->valores[++fila->final] = valor;// Incrementa o valor do final(se a fila está vazia , -1 se torna 0 por exemplo) e acrescenta o valor naquela posição    

    }
    
}
int removerNafila(Queue *fila){
    if (checarSeTaVazia(fila))
    {
        printf("A fila está vazia");
        return -1;
    }
    int var = fila->valores[fila->inicio++];// O valor que será removido é sempre o que está na posição que marca o inicio da fila
    if (fila->inicio > fila->final)
        inicializarFila(fila);  // Reinicia a fila se todos os elementos foram removidos
    
    return var;
}

void imprimirFila(Queue *fila){
     if (checarSeTaVazia(fila))
    {
        printf("A fila está vazia");
        return ;
    }
    printf("Fila :\n");
    for (int i = fila->inicio; i <= fila->final ; i++)//Começa a imprimir do inicio até o final
    {
        printf("%d\n", fila->valores[i]);
    }
    
}
int main(int argc, char const *argv[])
{
    Queue fila;//Declarando a fila
    inicializarFila(&fila);


    addNaFila(&fila , 3);
    addNaFila(&fila , 5);
    addNaFila(&fila , 2);
    addNaFila(&fila , 8);
    addNaFila(&fila , 22);
    addNaFila(&fila , 17);
    addNaFila(&fila , 14);
    addNaFila(&fila , 44);
    addNaFila(&fila , 12222);

    imprimirFila(&fila);
    for (int i = 0; i < 4; i++)
    {
        removerNafila(&fila);

    }
    imprimirFila(&fila);

    return 0;
}
