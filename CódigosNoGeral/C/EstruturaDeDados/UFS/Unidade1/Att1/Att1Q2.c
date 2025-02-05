#include <stdio.h>
#include <string.h>
#include <time.h>
#include <stdlib.h>
#include <ctype.h>
struct Atleta{
 char nome[25];
 float peso;
 int idade;  
};
int main(){
    struct Atleta *vetorAtletas;
    int tamanho =0;
    int qtdMenores=0;
    float somaPesos=0;
    printf("Quantos atletas deseja cadastrar?");
    scanf("%d", &tamanho);
    // Alocando o vetor dinamicamente
    vetorAtletas= (struct Atleta*) malloc(tamanho * sizeof(struct Atleta));

    if (vetorAtletas == NULL) {
        printf("Erro ao alocar memoria.");
        return 1;
    }

    // Lendo os dados dos atletas
    for ( int i = 0; i < tamanho; i++) {
        printf("Digite o nome do atleta %d: ", i + 1);
        scanf("%s", &vetorAtletas[i].nome);
        printf("Digite a idade do atleta %d: ", i + 1);
        scanf("%d", &vetorAtletas[i].idade);
        if (vetorAtletas[i].idade<18)
        {
            qtdMenores++;
        }
        
        printf("Digite o peso do atleta %d: ", i + 1);
        scanf("%f", &vetorAtletas[i].peso);
        somaPesos+=vetorAtletas[i].peso;
    }

    // Exibindo os dados dos atletas
    printf("\nDados dos atletas cadastrados:\n");
    for (int i = 0; i < tamanho; i++) {
        printf("Atleta %d:\n", i + 1);
        printf("Nome: %s\n", vetorAtletas[i].nome);
        printf("Idade: %d\n", vetorAtletas[i].idade);        
        printf("Peso: %.2f\n", vetorAtletas[i].peso);
        printf("\n");
    }
    float mediaPesos = (float) somaPesos/tamanho;
    printf("A média dos pesos dos atletas é %f\n", mediaPesos);
    printf("A quantidade de atletas menores de idade é:  %d\n", qtdMenores);
    // Liberando a memória
    free(vetorAtletas);

    return 0;
}