#include <stdio.h>
#include <stdlib.h>

struct ponto
{
    float x;
    float y;
    struct ponto *prox;
};


typedef struct ponto Ponto;

Ponto *listaPontos;// Aponta para o início da lista


void addFirst(float x , float y){
    Ponto *p = (Ponto*) malloc((sizeof(Ponto)));
    p->x = x;
    p->y = y;
    p->prox = listaPontos;
    listaPontos = p;
    
}
void addLast(float x , float y){
     Ponto *p = (Ponto*) malloc((sizeof(Ponto)));
    p->x = x;
    p->y = y;
    p->prox=NULL;
    if (listaPontos==NULL)
    {
        listaPontos = p;
    }
    else
    {
        Ponto *listaAux = listaPontos;
        while (listaAux->prox != NULL)// Nesse while, ele verifica se há NULL na frente do último el , se n tiver,
                                        // ele passa pra frente até achar o NULL 
        {
            listaAux = listaAux -> prox;
        }
        listaAux->prox = p;
    }    
}
int length = 6;
void addAnyPosition(float x , float y , int index){
    Ponto *p = (Ponto*) malloc(sizeof(Ponto));
    p->x = x;
    p->y = y;
    if (index>= length)
    {
    printf("Posição inválida");
    }
    else
    {
        if (index==0)
        {
            p->prox = listaPontos;
            listaPontos = p;
        }else{
            Ponto *listaAux = listaPontos;
            int i = 0;
            while (i != index-1)
            {
                listaAux = listaAux ->prox;
                i++;
            }  
            p->prox = listaAux -> prox;
             listaAux->prox = p ;          
        }
        length++;
    }
    
}
void removeIndex(int index){
    if (index>length || length ==0 )
    {
        printf("Posição inválida");

    }
    else
    {
        if (index==0)
        {
            listaPontos = listaPontos->prox;
        }
        else
        {
            Ponto *listaAux = listaPontos;

            int i=0;
            while(i!= index-1)
            {
                listaAux = listaAux->prox;
                i++;
            }
            listaAux->prox = listaAux->prox->prox;
            length--;  
        }
        
    }
    


}
/*
void imprime(){
    Ponto *auxLista = listaPontos;
    while (auxLista!=NULL)
    {
        printf("\nPonto(%.1f , %.1f)", auxLista->x , auxLista->y);
        auxLista = auxLista->prox;
    }
    
}*/

void imprime(Ponto *p){
    if (p != NULL)
    {
        printf("\n Ponto(%.1f , %.1f)" , p->x , p->y);
        imprime(p->prox);
    }    
}


int main(){

    
    addAnyPosition(4,5,0);
    addAnyPosition(1,3,1);
    addAnyPosition(12,99,2);
    addAnyPosition(3,2,0);
    addAnyPosition(12,-4,3);

    
    //printf("%.0f\n", listaPontos->prox->x );
     Ponto *auxLista = listaPontos;
    imprime(auxLista);

    return 0;
}