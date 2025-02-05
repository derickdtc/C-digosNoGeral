#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define nMax 30

typedef struct{
    int items[nMax];
    int top;
}Pilha;

void inicializarPilha(Pilha *pil){
    pil->top = -1;
}
void push(Pilha *pil , int el){
    if (pil->top == nMax -1)
    {
        printf("A pilha está cheia");
    }
    pil->items[++pil->top] = el;
    printf("%d empilhado\n",el);
}

int pop(Pilha *pil){
    if (pil->top == -1)
    {
        printf("A pilha está vazia");
        return -1;
    }else{
        int varARemover = pil->items[pil->top--];
        printf("%d está fora da pilha", varARemover);
        return varARemover;
    }
    
}

void display(Pilha *pil ){
    if (pil->top == -1)
    {
        printf("A pilha está vazia");
        return;
    }
    for (int i = pil->top; i > -1 ; i--)
    {
        printf("%d ", pil->items[i]);

    }
    puts("\n");
}
void converter(int n , Pilha *pil){
    int valorAGuardar;
    int quociente = n;
    do
    {

        valorAGuardar = quociente%2;
        push(pil , valorAGuardar);
        quociente = quociente/2;
    } while (quociente >0);
    
     printf("Aqui está n convertido em binário\n");
     display(pil);
}
int main(){
    Pilha pil;
    inicializarPilha(&pil);
    /*
    push(&pil , 5);
     push(&pil , 4);
    push(&pil , 3);
    push(&pil , 2);
    push(&pil , 1);  
    display(&pil);  */ 
    
    
    converter(9 , &pil);
    return 0;
}