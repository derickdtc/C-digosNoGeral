#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define nMax 20
#define tamNome 30
typedef struct 
{
    
    char nomePais[nMax][tamNome];
    int inicio;
    int final;
}fila;

void incializarFila(fila *fil){
    fil->inicio = -1;
    fil->final = -1;
}
    
void addNaFila(fila *fil , char nome[tamNome]){
    if (fil->final == nMax -1)
    {
        printf("A fila está cheia");
    }else{
        if (fil->inicio == -1)
            fil->inicio = 0;
        strcpy( fil->nomePais[++fil->final] , nome);    
       
        
    }

    
}

char* removerNaFila(fila *fil){
    if (fil->inicio == fil->final)
    {
        printf("A fila está vazia");
        return -1;
    }
    int varARemover = fil->nomePais[fil->inicio++];
    if (fil->inicio > fil->final)
        inicializarFila(fil);  // Reinicia a fila se todos os elementos foram removidos
    
    return varARemover;
    
}


void imprimirFila(fila *fil){
    if (fil->inicio == fil->final)
    {
     printf("A fila está vazia");

    }
    printf("\nFila :");
    for (int i = fil->inicio; i <= fil->final; i++)
    {
        printf("%s\n" , fil->nomePais[i]);
    }
    
}
int main(int argc, char const *argv[])
{
    fila novaFila;
    incializarFila(&novaFila);
    
    char nomesDePaises[nMax][tamNome] = {
         "Brasil",
        "Canadá",
        "Japão",
        "Alemanha",
        "Austrália",
        "África do Sul",
        "Coreia do Sul"
    };

    for (int i = 0; i < 7; i++)
    {
        addNaFila(&novaFila , nomesDePaises[i]);
    }
    
  
    imprimirFila(&novaFila);
    for (int i = 0; i < 4; i++)
    {
         removerNaFila(&novaFila);

    }
    

    imprimirFila(&novaFila);


    return 0;
}
