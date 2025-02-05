#include <stdio.h>
#include <string.h>
#include <time.h>
#include <stdlib.h>
#include <ctype.h>

struct Aluno{
 char nome[25];
 float media;
 int  matricula;  
};
// Função para ordenar os alunos pela média
int ordenaPorClass( const void *a, const void *b){
    struct Aluno *alunoA = (struct Aluno *)a;
    struct Aluno *alunoB = (struct Aluno *)b;
    if (alunoA->media < alunoB->media) {
        return 1;
    } else if (alunoA->media > alunoB->media) {
        return -1;
    } else {
        return 0;
    }
}

// Função para ordenar os alunos pela ordem alfabética

int ordenaPorAlfabetica ( const void *a, const void *b){
    struct Aluno *alunoA = (struct Aluno *)a;
    struct Aluno *alunoB = (struct Aluno *)b;
    return strcmp(alunoA->nome, alunoB->nome);

}

int main(){
    struct Aluno *vetorAlunos;
    int tamanho =0;
    int resposta;
    //Criando condição para encerrar o loop
    int ficarNoPrograma = 1;

    printf("Quantos alunos deseja cadastrar?");
    scanf("%d", &tamanho);
    // Alocando o vetor dinamicamente
    vetorAlunos= (struct Aluno*) malloc(tamanho * sizeof(struct Aluno));

    if (vetorAlunos == NULL) {
        printf("Erro ao alocar memoria.");
        return 1;
    }

    // Lendo os dados dos alunos
    for ( int i = 0; i < tamanho; i++) {
        printf("Digite o nome do aluno %d: ", i + 1);
        scanf("%s", &vetorAlunos[i].nome);
               
        printf("Digite a média do aluno %d: ", i + 1);
        scanf("%f", &vetorAlunos[i].media);
        printf("Digite a matrícula do aluno %d: ", i + 1);
        scanf("%d", &vetorAlunos[i].matricula);

    }

    // Perguntando ao usuário como deseja ver os dados
    
     while (ficarNoPrograma)
     {
    printf("Se deseja ver a tabela em ordem alfabética digite 1, caso queira ver em ordem de classificação, digite 2. Se quiser sair do programa, digite qualquer outro número ");
    scanf("%d",   &resposta);
    
     if (resposta==1)
    {
        qsort(vetorAlunos, tamanho, sizeof(struct Aluno), ordenaPorAlfabetica);
        printf("\nDados dos alunos cadastrados:\n");

        for (int i = 0; i < tamanho; i++) {
        printf("Nome: %s, Media: %.2f, Matricula: %d\n", vetorAlunos[i].nome, vetorAlunos[i].media, vetorAlunos[i].matricula);
    }
    }else if(resposta==2){
        qsort(vetorAlunos, tamanho, sizeof(struct Aluno), ordenaPorClass);
        printf("\nDados dos alunos cadastrados:\n");
        for (int i = 0; i < tamanho; i++) {
        printf("Nome: %s, Media: %.2f, Matricula: %d\n", vetorAlunos[i].nome, vetorAlunos[i].media, vetorAlunos[i].matricula);
    }
    }else{ ficarNoPrograma=0; }
    
     } 
    
    
   

    
    free(vetorAlunos);

    return 0;
}