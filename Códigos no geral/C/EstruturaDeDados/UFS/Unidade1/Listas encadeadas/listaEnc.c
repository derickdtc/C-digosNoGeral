#include <stdio.h>
#include <stdlib.h>
//As funções pedidadas estão entre as linhas 56 e 196


//Exemplo de Lista Encadeada usando nó cabeça

typedef struct no{
int dado;
struct no *proxNo;
}tipoNo;

//Abaixo está o nó cabeça 

typedef struct listaGerenciada{
  tipoNo *inicio;
  tipoNo *fim;
  int quant;
  }tipoLista;

void inicializa(tipoLista *lista){
    lista->inicio = NULL;
    lista->fim = NULL;
    lista->quant = 0;
}

int insereListaVazia(tipoLista *lista, int valor){
tipoNo *novoNo = (tipoNo*) malloc(sizeof(tipoNo));
if(novoNo == NULL)
    return 0;
novoNo->dado = valor;
novoNo->proxNo = NULL;
lista->inicio = novoNo;
lista->fim = novoNo;
lista->quant++;
}


int insereNaFrente(tipoLista *lista, int valor){
tipoNo *novoNo;
if(lista->inicio == NULL)
    insereListaVazia(&lista ,valor);
else{
    novoNo = (tipoNo*)malloc(sizeof(tipoNo));
    if(novoNo==NULL)
        return 0;
    novoNo->dado = valor;
    novoNo->proxNo = lista->inicio;
    lista->inicio = novoNo;
    return 1;
}
}
//INÍCIO DA FUNÇÕES PEDIDAS 


void removerNoFimLista(tipoLista *lista){
    tipoNo *atual;//Variável que será usada para percorrer a lista
    tipoNo *antNo;// Para pegar o nó anterior 
    atual = (*lista).inicio;
    while((*atual).proxNo != NULL){
        antNo = atual;
        atual = (*atual).proxNo;        
    }
    (*antNo).proxNo = NULL;
    (*lista).fim= antNo;
    (*lista).quant--;
    free(atual);
}
void removerNoInicioLista(tipoLista *lista){
    tipoNo *atual;//Variável que será usada para percorrer a lista
    tipoNo *antNo;// Para pegar o nó anterior 
    if (lista->inicio == NULL) {
        
        return;
    }
    
    atual = lista->inicio;
    lista->inicio = atual->proxNo;
    
    if (lista->inicio == NULL) {
        // A lista ficou vazia após a remoção
        lista->fim = NULL;
    }
    
    lista->quant--;
    free(atual);
    

}
int length = 5;
void addNoIndex(tipoLista *lista , int valor , int index){
    
  if (index< 0 || index> lista->quant)
    {
    printf("Posição inválida");
    }  
    tipoNo *novoNo = (tipoNo*) malloc(sizeof(tipoNo)); 
    novoNo->dado = valor;
    novoNo->proxNo = NULL;
    if (index == 0)
    {
        novoNo->proxNo = lista->inicio;
        lista->inicio = novoNo;
        if (lista->quant == 0)
        {
            lista->fim = novoNo;
        }
    }else{
        tipoNo *atual = lista->inicio;
        tipoNo *antNo = NULL;
        for (int i = 0; i < index; ++i)
        {
            antNo = atual;
            atual = atual->proxNo; // Percorrendo a lista até a posição desejada
        }
        antNo->proxNo = novoNo;// Colocando o novo nó onde estava o atual(posição desejada), e jogando o atual pra frente
        novoNo->proxNo = atual;
        if ((*novoNo).proxNo == NULL)
        {
            lista->fim = novoNo;
        }
    }


    lista->quant++;
}

void removerNoIndex(tipoLista *lista , int index){
    tipoNo *antNo;
    tipoNo *atual;
    if (index< 0 || index>= lista->quant)
    {
    printf("Posição inválida");
    }
    atual = lista->inicio;
    antNo = NULL;
    if (index == 0)
    {
        lista->inicio = atual->proxNo;// Início da lista recebe o próximo nó e para de apontar para o primeiro
        if (lista->inicio == NULL)
        {
            lista->fim = NULL;
        }
        free(atual);// Liberando o espaço na memória do primeiro nó
    }else{
        for (int i = 0; i < index; ++i)// Usando o for para chegar até o espaço antes do nó que quero excluir
        {
            antNo = atual;
            atual = atual->proxNo;
        }

        antNo->proxNo = atual->proxNo; // Parando de apontar para o nó que quero excluir e apontando para o que está na frente dele
        if (antNo->proxNo = NULL)
        {
            lista->fim = antNo;
        }

        free(atual);

    }

    lista->quant--;
}

void retornaIndexPesquisado(tipoLista *lista , int valor){    
    tipoNo *atual = lista->inicio;// Atual recebe o primeiro item da lista
    int i = 0;

    while(atual!=NULL){
        if (atual->dado == valor)
        {
            return printf("O valor %d foi achado na Posição %d\n", valor , i);
        }
        atual = atual->proxNo; //Percorrendo a lista
        i++;
    }
     return printf("Valor não encontrado");   // Caso o valor não seja encontrado

}



void destruirLista(tipoLista *lista){
    tipoNo *atual = lista->inicio;

    while(atual != NULL){
        tipoNo *noQueSai = atual;
        atual = atual->proxNo;
        free(noQueSai);
    }
    lista->inicio=NULL;
    lista->fim= NULL;
    lista->quant=0;
    printf("A lista está vazia");
}
//FIM DAS FUNÇÕES PEDIDAS
/*Função de exibição da lista simplesmente encadeada*/
int exibeLista(tipoLista *lista){
tipoNo *atual;  //Variável que será usada para percorrer a lista*/
if(lista->inicio == NULL)
    return 0;
atual = lista->inicio;
printf("\nLista encadeada: ");
while(atual !=NULL){
    printf("%8d",atual->dado);
    atual = atual->proxNo;
    }
    puts("\n\n");
return 1;
}


void main(){
tipoLista lista;

inicializa(&lista);

int leitor;
/*
printf("Digite um elemento: ");
scanf("%d",&leitor);
int resp = insereListaVazia(&lista, leitor);
if(!resp)
   printf("Memoria nao alocada!");
printf("Elemento guardado: %d",lista.inicio->dado);
printf("\nDigite outro elemento: ");
scanf("%d",&leitor);
resp = insereNaFrente(&lista,leitor);*/

addNoIndex(&lista, 10, 0);
addNoIndex(&lista, 20, 0);
addNoIndex(&lista, 30, 2);
addNoIndex(&lista, 40, 1);
addNoIndex(&lista, 50, 0);
exibeLista(&lista);


retornaIndexPesquisado(&lista , 40);





}