#include <stdio.h>
#include <stdlib.h>

int potencia(int n , int p){
    if (p == 1 )
    {
        return n;
    }else{
        return n*potencia(n , p-1);
    }
    

}


int main(){
    int v = potencia(2,4);
    printf("%d", v );
    return 0;
}