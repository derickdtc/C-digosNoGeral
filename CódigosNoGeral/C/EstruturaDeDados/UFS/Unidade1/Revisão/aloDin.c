#include <stdio.h>
#include <stdlib.h>
#include <string.h>


int main(int argc, char const *argv[])
{
    int *vetorN = NULL;
    int tamanhoV , soma =0;
    float media = 0;
    printf("Qual a qtd de número que você deseja adicionar?");
    scanf("%d", &tamanhoV);

    vetorN = (int*) malloc(tamanhoV * sizeof(int));

    if (vetorN == NULL)
    {
        printf("Erro ao alocar memória");
        return 1;
    }
    printf("Digite os número que deseja alocar\n");
    for (int i = 0; i < tamanhoV   ; i++)
    {
        scanf("%d", &vetorN[i]);
        soma+=vetorN[i];
    }
    media = (float) soma/tamanhoV;
    printf("%f", media);
    free(vetorN);
    return 0;
}
