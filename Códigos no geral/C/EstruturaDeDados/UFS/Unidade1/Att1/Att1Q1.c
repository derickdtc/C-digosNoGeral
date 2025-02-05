#include <stdio.h>
#include <string.h>
#include <time.h>
#include <stdlib.h>
#include <ctype.h>

int main(){
    int *vetorN;
    int tamanho, soma =0;
    printf("Quantos números serão digitados?");
    scanf("%d", &tamanho);
    // Alocando o vetor dinamicamente
    vetorN= (int*) malloc(tamanho * sizeof(int));

    if (vetorN == NULL) {
        printf("Erro ao alocar memoria.");
        return 1;
    }

    printf("Digite os números\n");
    for (int i = 0; i < tamanho ; i++)
    {
        scanf("%d", &vetorN[i]);
        soma+=vetorN[i];
    }
    printf("Média do números digitados: %.2f\n", (float)soma / tamanho);
    
    printf("Os multiplos de 6:\n ");
    for (int i = 0; i < tamanho; i++)
    {
        if (vetorN[i]%6==0)
        {
            printf("%d\n", vetorN[i]);
        }
        
    }
    // Liberando a memória
    free(vetorN);

    return 0;
}