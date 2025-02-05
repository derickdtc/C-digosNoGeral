#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define nMax 40  // Define o tamanho máximo da pilha

typedef struct {
    char texto[nMax];  // Array para armazenar os caracteres
    int top;           // Índice do topo da pilha
} Pilha;

void inicializar(Pilha *p) {
    p->top = -1;  // Inicializa o topo como -1 (pilha vazia)
}

void push(Pilha *p, char c) {
    if (p->top == nMax - 1) {
        printf("A pilha está cheia\n");
        return;
    }
    p->texto[++p->top] = c;  // Adiciona o caractere no topo da pilha
}

char pop(Pilha *p) {
    if (p->top == -1) {
        printf("A pilha está vazia\n");
        return '\0';  // Retorna caractere nulo se a pilha estiver vazia
    }
    return p->texto[p->top--];  // Retorna o caractere do topo e decrementa o topo
}

void display(Pilha *p) {
    if (p->top == -1) {
        printf("A pilha está vazia\n");
        return;
    }
    printf("Pilha correta: \n");

    for (int i = p->top; i >= 0; i--) {
        printf("%c", p->texto[i]);  // Imprime cada caractere da pilha
    }
    puts("\n");
    printf("Pilha (da base para o topo): \n");
    for (int i = 0; i <= p->top; i++) {
        printf("%c", p->texto[i]);
    }
    printf("\n");
}
void verificarPalindromo(Pilha *p){
     char str1[100] = {0};  // Inicializa com zeros (null terminator)
    char str2[100] = {0};  // Inicializa com zeros (null terminator)

    // Preenche str1 com a string na ordem inversa (da pilha)
    for (int i = 0; i <= p->top; i++) {
        str1[i] = p->texto[p->top - i];
    }

    // Preenche str2 com a string na ordem normal
    for (int i = 0; i <= p->top; i++) {
        str2[i] = p->texto[i];
    }

    // Comparando as strings
    printf("str1: %s\nstr2: %s\n", str1, str2);
    if (strcmp(str1, str2) == 0) {
        printf("É palíndromo.\n");
    } else {
        printf("Não é palíndromo.\n");
    }
}
int main(int argc, char const *argv[]) {
    Pilha p;
    inicializar(&p);

    char frase[] = "ararabbs";

    // Empilha cada caractere da string, ignorando espaços
    for (int i = 0; i < strlen(frase); i++) {
        if (frase[i] != ' ') {
            push(&p, frase[i]);
        }
    }

    // Exibe o conteúdo da pilha
    display(&p);
    verificarPalindromo(&p);
    return 0;
}