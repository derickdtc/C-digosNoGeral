#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>


int quant = 0;
typedef struct no{
    char nome[25];
    struct no *proxNo;
}tipoNo;




tipoNo* criarNo(char dados[]) {
    tipoNo* novoNo = (tipoNo*)malloc(sizeof(tipoNo));
    if (novoNo == NULL)
    {
        printf("Erro na alocação de memória\n");
        exit(1);
    }
    strcpy(novoNo->nome , dados);    
    novoNo->proxNo = NULL;
    quant++;
    return novoNo;
}


void inserirNoFim(tipoNo** cabeca, char dados[]){
    tipoNo* novoNo = criarNo(dados);
    if (*cabeca == NULL)
    {
        *cabeca = novoNo;
        novoNo->proxNo = *cabeca;//Aponta para si mesmo, formando um ciclo
    }else{
        tipoNo* temp = *cabeca;
        while (temp->proxNo != *cabeca)
        {
            temp = temp -> proxNo;
        }
        temp->proxNo = novoNo;
        novoNo->proxNo = *cabeca; // Formando o ciclo
    }
}


void imprimirLista(tipoNo* cabeca){
    if (cabeca == NULL) {
        printf("A lista está vazia.\n");
        return;
    }
    tipoNo* temp = cabeca;// Nó que será usado para percorrer a lista
    do
    {
        printf("\n%s", temp->nome);
        temp = temp->proxNo;
    }while (temp != cabeca); // Imprimire equanto os nós não forem iguais  
    printf("\n");
}


void imprimirIndice(tipoNo* cabeca, int indice){
    int cont = 0;
    tipoNo* temp = cabeca;
    while(cont != indice){
        temp = temp->proxNo;
        cont++;
    }
    printf("\n%s\n", temp->nome);
}


void removeNo(tipoNo** cabeca, int indice){
    tipoNo* temp1 = (*cabeca)->proxNo;//dois ponteiros para percorrer e alterar a lista.
    tipoNo* temp2 = *cabeca;
    if(indice == 0){
        while(temp1->proxNo != *cabeca){
            temp1 = temp1->proxNo;
        }
        temp1->proxNo = (*cabeca)->proxNo;
        free(temp2);
        *cabeca = (*cabeca)->proxNo;
    } else if(indice == quant - 1){
        do{
            temp1 = temp1->proxNo;//avança ate o ultimo no
            temp2 = temp2->proxNo;
        } while (temp1->proxNo != *cabeca);
        temp2->proxNo = temp1->proxNo;
        free(temp1);
    } else {
        int j;
        for(j = 1; j < indice; j++){
            temp1 = temp1->proxNo;
            temp2 = temp2->proxNo;
        }
        temp2->proxNo = temp1->proxNo;
        free(temp1);
    }
    quant--;
}


 int main(){
    tipoNo* cabeca = NULL;
    int sorteado;
    srand(time(NULL));


    char nomes[7][25] = {
        "Alice",
        "Bob",
        "Carol",
        "Dave",
        "Eve",
        "Frank",
        "Grace"
    };
    int i;
    for ( i = 0; i < 7; i++)
    {
       inserirNoFim(&cabeca , nomes[i]);
    }


    while(quant != 1){
        printf("\nParticipantes:");
        imprimirLista(cabeca);
        sorteado = rand();
        sorteado = sorteado % quant;
        printf("\nEliminado:");
        imprimirIndice(cabeca, sorteado);
        removeNo(&cabeca, sorteado);
    }
    printf("\nO vencedor foi %s", cabeca->nome);
    return 0;
}

