#include <stdio.h>
#include <string.h>
#include <math.h>
#include <ctype.h>

int main() {
    /*
    char name[25];//bytes
    int age;
    printf(" What's your name?\n ");
    fgets(name, 25, stdin);
    name[strlen(name)-1]= '\0';

    printf("\nHow old are you?");    
    scanf("%d", &age);

    printf("Hello %s, how are you?\n", name);
    printf("You are %d years old", age);

    return 0;*/
    char unit;
    float temp;
    float newTemp;
    printf("A temperatura é em celsius(C) ou farenheit(F)");
    scanf("%c", &unit);

    unit= toupper(unit);
    if (unit== 'C')
    {
        printf("Coloque a temperatura em celsius");
        scanf("%f", &temp);
        newTemp= (9/5*temp)+32;
        printf("A temperatura convertida é %f", newTemp);
    }else if(unit=='F')
    {
         printf("Coloque a temperatura em farenheit");
        scanf("%f", &temp);
        newTemp= 5*(temp-32)/9;
        printf("A temperatura convertida é %f", newTemp);
    }
    else
    {
        printf("Digite uma letra válida"); 
    }
    
}

