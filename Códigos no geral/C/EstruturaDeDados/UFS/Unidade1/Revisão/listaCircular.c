#include <stdio.h>
#include <stdlib.h>
#include <string.h>
typedef struct nomes
{
    char nome[25];
    struct nomes *proxNome;
}tipoNome;

typedef struct gerenciarLista
{
    tipoNome *fim;
    int quant;
    
}tipoLista;

void inicializar(tipoLista *listaCirc){
    listaCirc->fim = NULL;
    listaCirc->quant = 0;
}
void inserirInicio(tipoLista *listaCirc, char el[]){
    tipoNome *novoNome ;
    novoNome = (tipoNome*) malloc(sizeof(tipoNome));    
    if (novoNome == NULL)
    {
        printf("Memória não alocada");
    }
    strcpy(novoNome->nome , el);
    if (listaCirc->quant==0)
    {
        listaCirc->fim = novoNome;
        listaCirc->fim->proxNome = novoNome;
    }
    else
    {

        novoNome->proxNome = listaCirc->fim->proxNome;
        listaCirc->fim->proxNome = novoNome;
        
    }




    listaCirc->quant++;
}
void inserirFinal(tipoLista *listaCirc, char el[]){
    tipoNome *novoNome , *atual;
    novoNome = (tipoNome*) malloc(sizeof(tipoNome));    
    if (novoNome == NULL)
    {
        printf("Memória não alocada");
    }
    strcpy(novoNome->nome , el);
    if (listaCirc->quant==0)
    {
        listaCirc->fim = novoNome;
        listaCirc->fim->proxNome = novoNome;
    }
    else
    {

        novoNome->proxNome = listaCirc->fim->proxNome ;
        listaCirc->fim->proxNome = novoNome;
        listaCirc->fim = novoNome;
    }

    listaCirc->quant++;
}
void removerElemento(tipoLista *listaCirc , char el[]){
    tipoNome  *atual, *antNome;
    if (listaCirc->quant == 0)
    {
        printf("A lista está vazia");
    }
    antNome = listaCirc->fim;
    atual = listaCirc->fim->proxNome;
    do
    {
        if (strcmp(atual->nome ,el) == 0)
        {
            if (atual == listaCirc->fim->proxNome)  // Remover primeiro nó
            {
                listaCirc->fim->proxNome = atual->proxNome;
            }
            else
            {
                antNome->proxNome = atual->proxNome;
            }
            if (atual == listaCirc->fim)//Remover último nói
            {
                listaCirc->fim = antNome;
            }
            free(atual);
            listaCirc->quant--;
            printf("O valor %s foi removido da lista.\n", el);
            return;
        }
        antNome = atual;
        atual = atual->proxNome;
    } while (atual!= listaCirc->fim->proxNome );
    
    printf("Elemento %s não existe.\n", el);

    
}
void buscarElemento(tipoLista *listaCirc , char el[]){
    tipoNome  *atual, *antNome;
    if (listaCirc->quant == 0)
    {
        printf("A lista está vazia");
    }
    atual = listaCirc->fim;    
    antNome = NULL;
    do
    {
        antNome = atual;
        atual = atual->proxNome;
    } while (atual != listaCirc->fim || atual->nome != el );

     if (atual == listaCirc->fim && atual->nome !=el ) {
        printf("Elemento %d não existe.\n", el);
        return;
    }else{
        printf("O elemento %s foi encontrado na posição ", atual->nome );
    }

}
void mostrarLista(tipoLista *listaCirc){
    tipoNome *auxNome;
     if (listaCirc->quant == 0)
    {
        printf("A lista está vazia");
        return;
    }
    auxNome = listaCirc->fim->proxNome;
    printf("A lista:");
    do
    {
        printf("   %s",auxNome->nome);
        auxNome = auxNome->proxNome;
    } while (auxNome!= listaCirc->fim->proxNome);
    puts("\n");
}
int main(int argc, char const *argv[])
{
    tipoLista listaCirc;
    inicializar(&listaCirc);
    char nomes[9][25] = {
        "Alice",
        "Bob",
        "Carol",
        "Dave",
        "Eve",
        "Frank",
        "Grace",
        "Donatelo",
        "Rafael"
    };

    for (int i = 0; i < 4; i++)
    {
        inserirFinal(&listaCirc, nomes[i]);
    }
    for (int i = 4; i < 9; i++)
    {
        inserirInicio(&listaCirc, nomes[i]);
    }
    mostrarLista(&listaCirc);
    return 0;
}
