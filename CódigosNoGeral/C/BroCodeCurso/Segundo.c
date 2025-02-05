#include <stdio.h>

void sort(int array[], int arraySize){
    for (int i = 0; i < arraySize-1 ; i++)
    {
        for (int j = 0; j < arraySize-1 ; j++)
    {
        if (array[j]<array[j+1])
        {
            int temp = array[j];
            array[j]= array[j+1];
            array[j+1]= temp;
        }
        
    }
    }
    for (int i = 0; i < arraySize; i++)
    {
        printf(" %d", array[i]);
        
    }
    
    
}
int main(){
    /*
    char operator;
    double n1;
    double n2;
    double result;
    printf("Qual a operação que deseja fazer? (+ - * /) ");
    scanf("%c", &operator);
    printf("Digite n1");
    scanf("%lf", &n1);
    printf("Digite n2 ");
    scanf("%lf", &n2);

    switch (operator)
    {
    case '+':
        result= n1+n2;
        printf("%.2lf", result);
        break;
    case '-':
        result= n1-n2;
        printf("%.2lf", result);
        break;   
    case '*':
         result= n1*n2;
        printf("%.2lf", result);
        break;
    case '/':    
         result= n1/n2;
        printf("%.2lf", result);
        break;   
    
    default:
        printf("Digite um operador válido");

        break;
    }
    int matriz[3][3]={
        {1,2,3},
        {4,5,6},
        {7,8,9}
    };
    for (int i = 0; i < sizeof(matriz)/sizeof(matriz[0]); i++)
    {
        for (int j = 0; j < sizeof(matriz[0])/sizeof(matriz[i][0]); j++)
        {
            printf(" %d", matriz[i][j]);
        }
        printf("\n");
    }*/
    int array[]={9,7,3,4,2,1,5,6,8};
     int arraySize= sizeof(array)/sizeof(array[0]);
    sort(array, arraySize);

    return 0;

}
