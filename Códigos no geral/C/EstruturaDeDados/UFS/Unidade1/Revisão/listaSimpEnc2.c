#include <stdio.h>
#include <stdlib.h>

typedef struct num
{
    int n;
    struct num *proxN;
}typeNum;

typedef struct listaGerenciada
{
    struct num *inicio;
    struct num *fim;
    int quant;
} tipoLista;

void inicializarLista(tipoLista *lista){
    lista->inicio = NULL;
    lista->fim = NULL;
    lista->quant = 0;
}
void insercaoOrdenada(tipoLista *lista ,int valor){
    typeNum *novoN , *antN , *atual;
    novoN = (typeNum*) malloc(sizeof(typeNum));
    if (novoN == NULL)
    {
        printf("A memória não foi alocada");
    }
    novoN->n =valor;
    novoN->proxN = NULL;
     
        
    if (lista->inicio == NULL) {
        // Lista vazia: insere como o primeiro elemento
        lista->inicio = novoN;
        lista->fim = novoN;
    } else {
        atual = lista->inicio;
        antN = NULL;

        // Percorre a lista para encontrar a posição correta
        while (atual != NULL && atual->n < valor) {
            antN = atual;
            atual = atual->proxN;
        }

        if (antN == NULL) {
            // Inserção no início (valor menor que o primeiro elemento)
            novoN->proxN = lista->inicio;
            lista->inicio = novoN;
        } else if (atual == NULL) {
            // Inserção no final (valor maior que todos os elementos)
            antN->proxN = novoN;
            lista->fim = novoN;
        } else {
            // Inserção no meio da lista
            antN->proxN = novoN;
            novoN->proxN = atual;
        }
    }
    lista->quant++;
}
void pesquisarEl(tipoLista *lista ,int valor){
    typeNum *atual , *antN;
    antN = NULL;
    atual = lista->inicio;
    int i = 0;
    while (atual != NULL && atual->n != valor)
    {
        antN = atual;
        atual = atual->proxN;
        i++;
    }
    if (atual == NULL)
    {
        printf("O elemento não está na lista ");
    }else{
        printf("O elemento %d foi encontrado na posição %d", atual->n , i);
    }
    
    
}
void destruirLista(tipoLista *lista){
    typeNum *atual, *antN;
    atual = lista->inicio;
    do
    {
        antN = atual;
        atual = atual->proxN;
        free(antN);
    } while (atual != NULL);
    lista->inicio=NULL;
    lista->fim= NULL;
    lista->quant=0;
    printf("A lista está vazia\n");
    
}
void display(tipoLista *lista){
    typeNum *atual;
    if (lista->quant == 0)
    {
        printf("A lista está vazia");
    }
    atual = lista->inicio;
    printf("A lista:");
   
    do{
        printf("%8d" , atual->n);
        atual= atual->proxN;
    } while (atual != NULL); 
    puts("\n");

}
int main(int argc, char const *argv[])
{
    tipoLista lista;
    inicializarLista(&lista);
    insercaoOrdenada(&lista , 5);
    insercaoOrdenada(&lista , 4);
    insercaoOrdenada(&lista , 10);
    insercaoOrdenada(&lista , 33);
    insercaoOrdenada(&lista , 1);
    insercaoOrdenada(&lista , 0);
    insercaoOrdenada(&lista , 98);

    display(&lista);
    pesquisarEl(&lista, 11);
    return 0;
}
