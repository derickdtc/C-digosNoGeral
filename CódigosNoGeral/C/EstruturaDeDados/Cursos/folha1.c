#include <stdio.h>
#include <stdlib.h>

struct ponto
{
    float x;
    float y;

};

typedef struct ponto Ponto;
int main(){
    /*
    int a = 10;
    int b, c;

    printf("&a = %p , a = %d \n" ,&a , a);
    printf("&b = %p , b = %d \n" ,&b , b);
    printf("&c = %p , c = %d \n\n" ,&c , c);
    b = 20 ; 
    c = a +b ;

    
    printf("&a = %p , a = %d \n" ,&a , a);
    printf("&b = %p , b = %d \n" ,&b , b);
    printf("&c = %p , c = %d \n" ,&c , c);*/
    
    /*
    char *p;
    char val = 'f';

    p = &val;
    printf("O valor apontado por p é %c", *p);*/

    Ponto *p = (Ponto*) malloc(sizeof(Ponto));
    p->x = 1 ; 
    p->y = 5;

    printf("Ponto = (%f,%f)", p->x , p->y);

    free(p);



    



    return 0;
}