#include <stdio.h>
#include <string.h>
#include <time.h>
#include <stdlib.h>
#include <ctype.h>

typedef struct{
 char name[25];
 char password[12];
 int id;  
}user;
enum day{Sun=1, Mon= 2, Tue=3, Wed=4, Thu=5, Fri=6, Sat=7};

int main(){
    /*
    user user1= {"Derick", "senha123", 2543};
    user user2= {"Joana", "ssefsefasf33", 2235};
    printf("%s\n", user1.name);
    printf("%s\n", user1.password);
    printf("%d\n", user1.id);
    printf("\n" );
    printf("%s\n", user2.name);
    printf("%s\n", user2.password);
    printf("%d\n", user2.id);



    enum day today= Sun;
    if (today == Sun || today== Sat)
    {
        printf("Dia da folga");
    }
    else
    {
        printf("Trabalhar");
  
    }
    srand(time(0));
    int n1= (rand() %6) +1;
    int n2= (rand() %6) +1;
    int n3= (rand() %6) +1;
    printf("%d\n", n1);
    printf("%d\n", n2);
    printf("%d\n", n3);



    */
    const int min=1;
    const int max=100;
    int guess;
    int guesses=0;
    int answer;
    srand(time(0));
    answer = (rand()% max) +min;

    do
    {
        printf("Adivinhe o número entre 1 e 100:");
        scanf("%d", &guess);
        
        if (answer<guess)
        {
            printf("Muito alto\n");
        }
        else
        {
            printf("Muito baixo\n");

        }
        guesses++;
    } while (answer != guess);
    
    printf("Parabéns, você advinhou o número %d\n", answer);
    printf("Foram necessárias %d tentativas", guesses);
    
     /* FILE *file;
    
    file = fopen("C:\\DTC\\arquivoTeste.txt", "r");    
    if (file == NULL) {
        printf("Erro ao abrir o arquivo!\n");
        return 1;
    }
    char buffer[255];
    // Verifica se o arquivo foi aberto corretamente
    
    fgets(buffer, 255 ,file);
    while (fgets(buffer, 255 ,file) != NULL)
    {
           printf("%s", buffer);

    }   

    fclose(file);
    
    //printf("Arquivo criado com sucesso!\n");*/


    return 0;
}