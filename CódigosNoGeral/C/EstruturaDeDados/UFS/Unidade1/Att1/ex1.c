#include <stdio.h>
#include <stdlib.h>
#include <string.h>


void swap(int *a , int *b){
    int temp = *a;
    *a = *b;
    *b = temp; 

}
void reverseString(char *str){
    int length = strlen(str);
    char *start = str; 
    char *end = str + length -1;
    char temp;
    while (start < end)
    {
        temp = *start;
        *start = *end;
        *end = temp;

        start++;
        end--;

    }
    
}

int main(){
    /*
    //EX1
    int x = 14;
    int y = 31;

    printf("Antes da troca: x = %d, y = %d\n", x, y);

    swap(&x , &y);
    printf("Depois da troca da troca: x = %d, y = %d\n", x, y);

    //EX2
    int *vetorN;

    int tamanho =5;
    vetorN = (int*) malloc(5 * sizeof(int));

    printf("Digite os numeros");
    for (int i = 0; i < 5; i++)
    {
        scanf("%d", &vetorN[i]);
    }

    for (int i = 0; i < 5; i++)
    {
        printf("%d\n" , vetorN[i]);
    }*/

   /*Exercício 3 (Médio)
    Implemente um programa que utilize uma função para inverter uma 
    string usando ponteiros. A função deve receber um ponteiro para a string como 
    argumento e inverter a string no local (in-place).*/
    
    char str[100];
    
    printf("Digite uma string\n");   
    fgets(str , sizeof(str) , stdin);

    str[strcspn(str , "\n")] = "\0";
    printf("String original: %s\n" ,str);

    reverseString(str);
    printf("String invertida: %s\n" ,str);

    



    return 0;
}