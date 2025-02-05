#include <iostream>
#include <cstring>
#include <fstream>
#include <cmath>
using namespace std;
// Variáveis para numes  
    int numTrocas = 0;
    int numChamadas = 0;

// LOMUTO ...
int partirComLomutoPadrao(int arr[] , int first , int last){
    int pivot = arr[last];
    int i = first-1;
    int j = first;
    for (j = first ; j < last; j++){
        if(arr[j] <= pivot) {
            swap(arr[++i], arr[j]);
            numTrocas++;
        }
       
    }
    swap(arr[++i], arr[last]);
    numTrocas++;
    return i;    
    
}
int partirComLomutoAleatorio(int arr[], int first, int last) {
    int n = last - first + 1;
    //int random = first + abs(arr[first]) % n; // Aplica a fórmula fornecida
    //swap(arr[random], arr[last]); // Coloca o pivô no final
    swap(arr[last], arr[first + abs(arr[first]) % n]);
    numTrocas++;
    return partirComLomutoPadrao(arr, first, last);
}
int partirComLomutoMediana(int arr[], int first, int last) {
    int n = last - first + 1;
    int idx1 = first + n / 4;
    int idx2 = first + n / 2;
    int idx3 = first + 3 * n / 4;
    // Calcula a mediana de três índices   
    if ((arr[idx1] <= arr[idx2] && arr[idx1] >= arr[idx3]) || (arr[idx1] >= arr[idx2] && arr[idx1] <= arr[idx3])   )
    {
        swap(arr[idx1], arr[last]); // Coloca o pivô no final
        numTrocas++;
        return partirComLomutoPadrao(arr, first, last);
    }else if((arr[idx2] <= arr[idx1] && arr[idx2] >= arr[idx3]) || (arr[idx2] >= arr[idx1] && arr[idx2] <= arr[idx3])){
        swap(arr[idx2], arr[last]); // Coloca o pivô no final
        numTrocas++;
        return partirComLomutoPadrao(arr, first, last);
    }else{
        swap(arr[idx3], arr[last]); // Coloca o pivô no final
        numTrocas++;
        return partirComLomutoPadrao(arr, first, last);
    }

    
}
//HOARE...

int partirComHoarePadrao(int arr[], int first, int last) {
    int pivot = arr[first]; // Escolhe o primeiro elemento como pivô
    int i = first - 1;
    int j = last + 1;
    while (true) {
        while(arr[--j] > pivot);
        while(arr[++i] < pivot);
        if(i < j) {
            swap(arr[i], arr[j]);
            numTrocas++;
        } else{
            return j;
        }    
    }
}
int partirComHoareAleatorio(int arr[], int first, int last) {
    int n = last - first + 1;
    //int random = first + abs(arr[first]) % n; // Aplica a fórmula fornecida
    //swap(arr[random], arr[first]); // Coloca o pivô no início
    swap(arr[first], arr[first + abs(arr[first]) % n] );
    numTrocas++;
    return partirComHoarePadrao(arr, first, last);
}
int partirComHoareMediana(int arr[], int first, int last) {
    int n = last - first + 1;
    int idx1 = first + n / 4;
    int idx2 = first + n / 2;
    int idx3 = first + 3 * n / 4; 

    if ((arr[idx1] <= arr[idx2] && arr[idx1] >= arr[idx3]) || (arr[idx1] >= arr[idx2] && arr[idx1] <= arr[idx3])   )
    {
        swap(arr[idx1], arr[first]); // Coloca o pivô no final
        numTrocas++;
        return partirComHoarePadrao(arr, first, last);
    }else if((arr[idx2] <= arr[idx1] && arr[idx2] >= arr[idx3]) || (arr[idx2] >= arr[idx1] && arr[idx2] <= arr[idx3])){
        swap(arr[idx2], arr[first]); // Coloca o pivô no final
        numTrocas++;
        return partirComHoarePadrao(arr, first, last);
    }else{
        swap(arr[idx3], arr[first]); // Coloca o pivô no final
        numTrocas++;
        return partirComHoarePadrao(arr, first, last);
    }
}
//Padrao ...
void quickSortLomutoPadrao(int arr[] , int first , int last){
    numChamadas++; // Incrementa o num de chamadas
    if (first < last)
    {
        int pivotIndexAfterPartir = partirComLomutoPadrao(arr , first , last);
        quickSortLomutoPadrao(arr , first , pivotIndexAfterPartir - 1);
        quickSortLomutoPadrao(arr , pivotIndexAfterPartir + 1 , last);

    }
    
}
void quickSortHoarePadrao(int arr[], int first, int last) {
    numChamadas++; // Incrementa o num de chamadas
    if (first < last) {
        int pi = partirComHoarePadrao(arr, first, last);
        quickSortHoarePadrao(arr, first, pi);    // Note que a recursão vai até 'pi', não 'pi - 1'
        quickSortHoarePadrao(arr, pi + 1, last);
 
    }
}

//ALeatorio
void quickSortLomutoAleatorio(int arr[] , int first , int last){
    numChamadas++; // Incrementa o num de chamadas
    if (first < last)
    {

        int pivotIndexAfterPartir = partirComLomutoAleatorio(arr , first , last);
        quickSortLomutoAleatorio(arr , first , pivotIndexAfterPartir - 1);
        quickSortLomutoAleatorio(arr , pivotIndexAfterPartir + 1 , last);

    }
    
}
void quickSortHoareAleatorio(int arr[], int first, int last) {
        numChamadas++; // Incrementa o num de chamadas    

    if (first < last) {

        int pi = partirComHoareAleatorio(arr, first, last);
        quickSortHoareAleatorio(arr, first, pi);    // Note que a recursão vai até 'pi', não 'pi - 1'
        quickSortHoareAleatorio(arr, pi + 1, last);
    }
}

//Mediana 

void quickSortLomutoMediana(int arr[], int first, int last) {
    numChamadas++; // Incrementa o num de chamadas

    if (first < last) {

        int pivotIndexAfterPartir = partirComLomutoMediana(arr, first, last);
        quickSortLomutoMediana(arr, first, pivotIndexAfterPartir - 1);
        quickSortLomutoMediana(arr, pivotIndexAfterPartir + 1, last);

    }
}
void quickSortHoareMediana(int arr[], int first, int last) {
     numChamadas++; // Incrementa o num de chamadas

    if (first < last) {
        int pi = partirComHoareMediana(arr, first, last);
        quickSortHoareMediana(arr, first, pi); // Subarray esquerdo
        quickSortHoareMediana(arr, pi + 1, last); // Subarray direito

    }
}

void mostrarVetoresPosOrdenacao(int** vetores, int* tamanhos, int numVetores) {
    cout << "\n " << endl;

    for (int i = 0; i < numVetores; i++) {
        cout << "Vetor " << i << ": ";
        for (int j = 0; j < tamanhos[i]; j++) {
            cout << vetores[i][j] << " ";
        }
        cout << endl;
    }
}
void restaurar(int** vetores , int* tamanhos , int numVetores , int** vetoresOriginais){
    for (int i = 0; i < numVetores; i++) {
        memcpy(vetores[i], vetoresOriginais[i], tamanhos[i] * sizeof(int));
    }
}

void executarEExibirResultados(int** vetores, int* tamanhos, int numVetores, int** vetoresOriginais) {
    // Para cada vetor
    for (int i = 0; i < numVetores; i++) {
        cout << i << ":N(" << tamanhos[i] << ")";

        // Array para armazenar os resultados das variantes
        const int numVariantes = 6;
        string nomes[numVariantes] = {"LP", "HP", "LM", "HM", "LA", "HA"};
        int resultados[numVariantes];

        // Lomuto Padrão
        numTrocas = 0;
        numChamadas = 0;
        quickSortLomutoPadrao(vetores[i], 0, tamanhos[i] - 1);
        resultados[0] = numTrocas + numChamadas;
        restaurar(vetores, tamanhos, numVetores, vetoresOriginais);

        // Hoare Padrão
        numTrocas = 0;
        numChamadas = 0;
        quickSortHoarePadrao(vetores[i], 0, tamanhos[i] - 1);
        resultados[1] = numTrocas + numChamadas;
        restaurar(vetores, tamanhos, numVetores, vetoresOriginais);

        // Lomuto Mediana
        numTrocas = 0;
        numChamadas = 0;
        quickSortLomutoMediana(vetores[i], 0, tamanhos[i] - 1);
        resultados[2] = numTrocas + numChamadas;
        restaurar(vetores, tamanhos, numVetores, vetoresOriginais);

        // Hoare Mediana
        numTrocas = 0;
        numChamadas = 0;
        quickSortHoareMediana(vetores[i], 0, tamanhos[i] - 1);
        resultados[3] = numTrocas + numChamadas;
        restaurar(vetores, tamanhos, numVetores, vetoresOriginais);

        // Lomuto Aleatório
        numTrocas = 0;
        numChamadas = 0;
        quickSortLomutoAleatorio(vetores[i], 0, tamanhos[i] - 1);
        resultados[4] = numTrocas + numChamadas;
        restaurar(vetores, tamanhos, numVetores, vetoresOriginais);

        // Hoare Aleatório
        numTrocas = 0;
        numChamadas = 0;
        quickSortHoareAleatorio(vetores[i], 0, tamanhos[i] - 1);
        resultados[5] = numTrocas + numChamadas;
        restaurar(vetores, tamanhos, numVetores, vetoresOriginais);

        // Ordenar manualmente os resultados em ordem crescente (Bubble Sort)
        for (int j = 0; j < numVariantes - 1; j++) {
            for (int k = 0; k < numVariantes - j - 1; k++) {
                if (resultados[k] > resultados[k + 1]) {
                    // Troca os valores dos resultados
                    int tempResultado = resultados[k];
                    resultados[k] = resultados[k + 1];
                    resultados[k + 1] = tempResultado;

                    // Troca os nomes correspondentes
                    string tempNome = nomes[k];
                    nomes[k] = nomes[k + 1];
                    nomes[k + 1] = tempNome;
                }
            }
        }

        // Exibir os resultados ordenados
        for (int j = 0; j < numVariantes; j++) {
            cout << "," << nomes[j] << "(" << resultados[j] << ")";
        }

        cout << endl; // Nova linha para o próximo vetor
    }
}

int main(int argc, char  *argv[])
{
    // Variáveis para numes     
    int numChamadas= 0;
    int numTrocas = 0;
   ifstream inputFile("input.txt");
    if (!inputFile.is_open()) {
        cerr << "Erro ao abrir o arquivo de entrada!" << endl;
        return 1;
    }
    // Lendo o número total de vetores
    int numVetores;
    inputFile >> numVetores;
    // Vetor de ponteiros para armazenar os vetores
    int** vetores = new int*[numVetores];
    int* tamanhos = new int[numVetores];
    // Processando cada vetor
    for (int i = 0; i < numVetores; i++) {
        // Lendo o tamanho do vetor atual
        int tamanho;
        inputFile >> tamanho;
        tamanhos[i] = tamanho;
        // Alocando memória para o vetor e lendo os elementos
        vetores[i] = new int[tamanho];
        for (int j = 0; j < tamanho; j++) {
            inputFile >> vetores[i][j];
        }
    }
    // Criando cópias dos vetores originais
    int** vetoresOriginais = new int*[numVetores];
    for (int i = 0; i < numVetores; i++) {
        vetoresOriginais[i] = new int[tamanhos[i]];
        memcpy(vetoresOriginais[i], vetores[i], tamanhos[i] * sizeof(int));
    }

    inputFile.close();   
    
    //Aplicando as ordenações Lomuto mediana
    /*
    for (int i = 0; i < numVetores; i++)
    {
        quickSortLomutoMediana(vetores[i], 0 , tamanhos[i] - 1);
    }   
    cout << "\n \n Vetores ordenados com Lomuto mediana" << endl;
    mostrarVetoresPosOrdenacao(vetores , tamanhos , numVetores);
    restaurar(vetores , tamanhos , numVetores, vetoresOriginais);
    mostrarVetoresPosOrdenacao(vetores , tamanhos , numVetores);
    //Aplicando as ordenações hoare mediana
    for (int i = 0; i < numVetores; i++)
    {
        quickSortHoareMediana(vetores[i], 0 , tamanhos[i] - 1);
    }   
    cout << "\n \n Vetores ordenados com Hoare mediana" << endl;
    mostrarVetoresPosOrdenacao(vetores , tamanhos , numVetores);
    restaurar(vetores , tamanhos , numVetores, vetoresOriginais);
    mostrarVetoresPosOrdenacao(vetores , tamanhos , numVetores);
    */
    executarEExibirResultados(vetores, tamanhos, numVetores, vetoresOriginais);

    // Liberando memória alocada
    for (int i = 0; i < numVetores; i++) {
        delete[] vetores[i];
        delete[] vetoresOriginais[i];

    }
    delete[] vetoresOriginais;
    delete[] vetores;
    delete[] tamanhos;

    return 0;
    
}
