#include <stdint.h>
#include <stdio.h>

void trocar(uint32_t* a , uint32_t* b){
    uint32_t temp = *a;
    *a = *b;
    *b = temp;
}
void selection_sort(uint32_t* V, uint32_t n ){
    for (uint32_t i = 0; i < n -1; i++)
    {
        uint32_t min = i;
        for (uint32_t j = i + 1; j < n; j++)
        {
            if (V[j] < V[min]) min = j;            
        }
        if(i != min) trocar(&V[i], &V[min]);

    }
    
}

int main(int argc, char const *argv[])
{
    uint32_t V[] = {64 ,25, 12, 22, 11};
    uint32_t n = sizeof(V) / sizeof(V[0]);
    printf("Vetor original: ");
     for (uint32_t i = 0; i < n; i++) {
        printf("%u ", V[i]);
    }
    printf("\n");

    // Ordena o vetor
    selection_sort(V, n);

    printf("Vetor ordenado: ");
    for (uint32_t i = 0; i < n; i++) {
        printf("%u ", V[i]);
    }
    printf("\n");

    return 0;
}
