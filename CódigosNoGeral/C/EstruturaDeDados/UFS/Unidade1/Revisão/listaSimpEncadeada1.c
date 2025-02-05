#include <stdio.h>
#include <stdlib.h>

typedef struct no {
    int dado;
    struct no *proxNo;
}tipoNo;

typedef struct listaGerenciada{
    tipoNo *inicio;
    tipoNo *final;
    int quant;
}tipoLista;

void inicializarLista(tipoLista *lista){
    lista->inicio= NULL;
    lista->final= NULL;
    lista->quant= 0;
}
void inserirNoInicio(tipoLista *lista , int el){
    tipoNo *novoNo;

    novoNo = (tipoNo*) malloc(sizeof(tipoNo));
     novoNo->dado = el;
    novoNo->proxNo = NULL;
    if (novoNo == NULL)
    {
        printf("A memória não foi alocada");
    }
        

    if(lista->inicio == NULL)
    {
        lista->inicio = novoNo;
        lista->final= novoNo;
    }else{
        novoNo->proxNo = lista->inicio;
        lista->inicio = novoNo;
    }
    
    lista->quant++;

    
}
void inserirNoFinal(tipoLista *lista , int el){
    tipoNo *novoNo;
    tipoNo *atual , *antNo;//Nós que serão usados para percorrer a lista
    
    novoNo = (tipoNo*) malloc(sizeof(tipoNo));
    novoNo->dado= el;
    novoNo->proxNo = NULL;
    if (novoNo == NULL)
    {
        printf("A memória não foi alocada");
    }
    if (lista->inicio ==NULL)
    {
        lista->inicio = novoNo;
        lista->final= novoNo;  
    }
    else
    {
        lista->final->proxNo = novoNo;
        lista->final = novoNo; 
    }
    lista->quant++;
}
void removerElemento(tipoLista *lista , int el){
//Valide a existência do elemento na lista antes de removê-lo.
    tipoNo *atual, *antNo;
    
    antNo = NULL;
    atual = lista->inicio;
    while (atual != NULL && atual->dado != el) {
        antNo = atual;
        atual = atual->proxNo;
    }
     if (atual == NULL) {
        printf("Elemento %d não encontrado.\n", el);
        return;
    }
    if (antNo == NULL) {
        lista->inicio = atual->proxNo;
    } else {
        antNo->proxNo = atual->proxNo;
    }

    if (atual == lista->final) {
        lista->final = antNo;
    }

    free(atual);
    lista->quant--;
    printf("O valor %d foi removido da lista.\n", el);
    
}

void display(tipoLista *lista){
    tipoNo *atual;
    if (lista->quant == 0)
    {
        printf("A lista está vazia");
    }
    atual = lista->inicio;
    printf("A lista:");
    do
    {
        printf("%8d",atual->dado);
        atual = atual->proxNo;
    } while (atual!= NULL);
    puts("\n");
}
int main(int argc, char const *argv[])
{
    tipoLista lista;
    inicializarLista(&lista);

    inserirNoFinal(&lista , 7);
    inserirNoFinal(&lista , 8);
    inserirNoFinal(&lista , 2);
    inserirNoFinal(&lista , 1);
    inserirNoFinal(&lista , 23);
    inserirNoInicio(&lista , 5);
    inserirNoInicio(&lista , 2);
    inserirNoInicio(&lista , 1);
    inserirNoInicio(&lista , 4);
    inserirNoInicio(&lista , 99);

    display(&lista);

     removerElemento(&lista, 2);
     removerElemento(&lista,23);
    removerElemento(&lista,99);


    display(&lista);
  
    return 0;
}
