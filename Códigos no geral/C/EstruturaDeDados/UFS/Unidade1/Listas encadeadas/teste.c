#include <stdio.h>
#include <stdlib.h>


int main(){
    /*
    int v[5] = {1,2,3,4,5};

    for (int i = 0; i < 5; i++)
    {
        printf("&v[%d] = %p , v[%d] = %d\n" , i , &v[i], i, v[i]);
    }
    printf("&v[6] = %p , v[6] = %d\n" , &v[6], v[6]);*/

    //Alocação de um vetor estático 

    int v[5] = {1,2,3,4,5};
    puts("##### VETOR ESTÁTICO");
    printf("&v = %p v= %p \n", &v , v);
     for (int i = 0; i < 5; i++)
    {
        printf("& v[%d] = %p , v[%d] = %d\n" , i , &v[i], i, v[i]);
    }
    puts("\n\n\n");

    //Alocação de vetor dinâmico
    puts("##### VETOR DINÃMICO COM MALLOC");

    int *vh_mal = (int*) malloc(5 * sizeof(int));
    printf("&vh_mal = %p vh_mal= %p \n", &vh_mal , vh_mal );
     for (int i = 0; i < 5; i++)
    {
        printf("& vh_mal[%d] = %p , vh_mal[%d] = %d\n" , i , &vh_mal[i], i, vh_mal[i]);
    }
    puts("\n");

    return 0;
}