#include <stdio.h>

//Exemplo de Lista Encadeada sem usar nó cabeça

typedef struct no{
int dado;
struct no *proxNo;
}tipoNo;


int insereListaVazia(tipoNo **primeiro, tipoNo **ultimo, int valor){
tipoNo *novoNo = (tipoNo*) malloc(sizeof(tipoNo));
if(novoNo == NULL)
    return 0;
novoNo->dado = valor;
novoNo->proxNo = NULL;
*primeiro = novoNo;
*ultimo = novoNo;
//printf("Elemento dentro da funcao: %d",(*primeiro)->dado);
return 1;
}

int insereNaFrente(tipoNo **primeiro, tipoNo **ultimo, int valor){
tipoNo *novoNo;
if(*primeiro == NULL)
    insereListaVazia(&primeiro, &ultimo ,valor);
else{
    novoNo = (tipoNo*)malloc(sizeof(tipoNo));
    if(novoNo==NULL)
        return 0;
    novoNo->dado = valor;
    novoNo->proxNo = *primeiro;
    *primeiro = novoNo;
    return 1;
}
}

/*Função de exibição da lista simplesmente encadeada*/
int exibeLista(tipoNo **primeiro){
tipoNo *atual;  //Variável que será usada para percorrer a lista*/
if(*primeiro == NULL)
    return 0;
atual = *primeiro;
printf("\nLista encadeada: ");
while(atual !=NULL){
    printf("%8d",atual->dado);
    atual = atual->proxNo;
    }
return 1;
}

void main(){
tipoNo *inicio = NULL, *fim = NULL;
int resp, leitor;
printf("Digite um elemento: ");
scanf("%d",&leitor);
resp = insereListaVazia(&inicio,&fim, leitor);
//printf("\nEstou apos execucao da funcao");
if(!resp)
   printf("Memoria nao alocada!");
printf("\nPrimeiro elemento guardado: %d",inicio->dado);
printf("\nDigite outro elemento: ");
scanf("%d",&leitor);
resp = insereNaFrente(&inicio, &fim, leitor);
printf("\nPrimeiro elemento atualmente guardado: %d",inicio->dado);
printf("\nDigite outro elemento: ");
scanf("%d",&leitor);
resp = insereNaFrente(&inicio, &fim, leitor);
printf("\nPrimeiro elemento atualmente guardado: %d",inicio->dado);
printf("\n\nExibicao da lista encadeada\n");
resp = exibeLista(&inicio);
if(resp == 0)
    printf("\nLista vazia!");
}