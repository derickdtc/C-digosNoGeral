#include <stdio.h>
#include <stdlib.h>

typedef struct node {
    int dado;
    struct node *pai;
    struct node *filhoEsq;
    struct node *filhoDir;
}tipoNode ;

tipoNode* criarNo(int n , tipoNode* pai){
    tipoNode *novo;
    novo = (tipoNode*) malloc(sizeof(tipoNode));
    if (novo == NULL)
    {
        return NULL;
    }
    novo->dado = n;
    novo->filhoDir = NULL;
    novo->filhoEsq = NULL;
    novo->pai = pai;
    return novo;
}

void exibePreOrdem(tipoNode* atual){
    if (atual != NULL)
    {
        printf("%8d" , atual->dado);
        exibePreOrdem(atual->filhoEsq);
        exibePreOrdem(atual->filhoDir);
    }
    
}
int temFilhoDir(tipoNode *atual){
    if (atual -> filhoDir != NULL)
        return 1;
    else
        return 0;    
    
}
int temFilhoEsq(tipoNode *atual){
    if (!atual -> filhoEsq)
        return 1;
    else
        return 0;    
    
}

tipoNode* buscar(tipoNode* atual, int n){
    if (atual != NULL)
    {
        if (atual->dado == n )
            return atual;
        tipoNode *r1 = buscar(atual -> filhoEsq , n);
        if(r1) return r1;
        tipoNode *r2 = buscar(atual->filhoDir , n);
        if(r2) return r2;
    }
    return NULL;
}

int main(int argc, char const *argv[])
{
    tipoNode *raiz;
    raiz = criarNo(10 , NULL);
    int opcao , valor , valorPai;
    char filhoEouD;
    do
    {
        printf("\n1 - Inserir no na arvore");
        printf("\n2 - Exibir arvore pre-ordem");
        printf("\n0 - Sair");
        printf("\nDigite sua opcao:");
        scanf("%d",&opcao);
        switch (opcao)
        {
        case 1: printf("Digite o dado que quer incluir");
        scanf("%d" , &valor);
        if(raiz == NULL)
            raiz= criarNo(valor ,NULL);
        else{
            printf("\n Digite o valor do pai:");
            scanf("%d", &valorPai);
            tipoNode *procurarPai = buscar(raiz, valorPai);
            if (!procurarPai)
                printf("\n Pai não encontrado , inserção não feita");
            else
                 printf("\nDigite se o filho vai ser da esquerda <e> ou direita <d>:");
                scanf("%c",&filhoEouD);
        }        
            break;
        
        default:
            break;
        }
        
    } while (opcao != 0);
    
    return 0;
}
