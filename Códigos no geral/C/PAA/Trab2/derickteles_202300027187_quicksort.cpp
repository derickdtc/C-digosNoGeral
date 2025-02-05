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
    int i = first-1 ,  j = first;
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
    int index1 = first + n / 4;
    int index2 = first + n / 2;
    int index3 = first + 3 * n / 4;
    // Calcula a mediana de três índices   
    if ((arr[index1] <= arr[index2] && arr[index1] >= arr[index3]) || (arr[index1] >= arr[index2] && arr[index1] <= arr[index3])   )
    {
        swap(arr[index1], arr[last]); // Coloca o pivô no final
        numTrocas++;
        return partirComLomutoPadrao(arr, first, last);
    }else if((arr[index2] <= arr[index1] && arr[index2] >= arr[index3]) || (arr[index2] >= arr[index1] && arr[index2] <= arr[index3])){
        swap(arr[index2], arr[last]); // Coloca o pivô no final
        numTrocas++;
        return partirComLomutoPadrao(arr, first, last);
    }else{
        swap(arr[index3], arr[last]); // Coloca o pivô no final
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
    int index1 = first + n / 4;
    int index2 = first + n / 2;
    int index3 = first + 3 * n / 4; 

    if ((arr[index1] <= arr[index2] && arr[index1] >= arr[index3]) || (arr[index1] >= arr[index2] && arr[index1] <= arr[index3])   )
    {
        swap(arr[index1], arr[first]); // Coloca o pivô no final
        numTrocas++;
        return partirComHoarePadrao(arr, first, last);
    }else if((arr[index2] <= arr[index1] && arr[index2] >= arr[index3]) || (arr[index2] >= arr[index1] && arr[index2] <= arr[index3])){
        swap(arr[index2], arr[first]); // Coloca o pivô no final
        numTrocas++;
        return partirComHoarePadrao(arr, first, last);
    }else{
        swap(arr[index3], arr[first]); // Coloca o pivô no final
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

// merge sort para ordenção dos resultados
void merge(string nomes[], int resultados[], int esq, int meio, int dir) {
    int n1 = meio - esq + 1;
    int n2 = dir - meio;
    // Arrays temporários para armazenar os valores
    int esqResultados[n1], dirResultados[n2];
    string esqNomes[n1], dirNomes[n2];
    // Copia os dados para os arrays temporários
    for (int i = 0; i < n1; i++) {
        esqResultados[i] = resultados[esq + i];
        esqNomes[i] = nomes[esq + i];
    }
    for (int i = 0; i < n2; i++) {
        dirResultados[i] = resultados[meio + 1 + i];
        dirNomes[i] = nomes[meio + 1 + i];
    }
    // Combina os dois arrays
    int i = 0, j = 0, k = esq;
    while (i < n1 && j < n2) {
        if (esqResultados[i] <= dirResultados[j]) {
            resultados[k] = esqResultados[i];
            nomes[k] = esqNomes[i];
            i++;
        } else {
            resultados[k] = dirResultados[j];
            nomes[k] = dirNomes[j];
            j++;
        }
        k++;
    }

    // Copia os elementos restantes de cada metade
    while (i < n1) {
        resultados[k] = esqResultados[i];
        nomes[k] = esqNomes[i];
        i++;
        k++;
    }
    while (j < n2) {
        resultados[k] = dirResultados[j];
        nomes[k] = dirNomes[j];
        j++;
        k++;
    }
}
void mergeSort(string nomes[], int resultados[], int esq, int dir) {
    if (esq < dir) {
        int meio = esq + (dir - esq) / 2;
        // Ordena as metades
        mergeSort(nomes, resultados, esq, meio);
        mergeSort(nomes, resultados, meio + 1, dir);
        // Combina as metades ordenadas
        merge(nomes, resultados, esq, meio, dir);
    }
}

void executarEExibirResultados(int** vetores, int* tamanhos, int numVetores, int** vetoresOriginais, ofstream& outputFile) {
    // Para cada vetor
    for (int i = 0; i < numVetores; i++) {
        //cout << i << ":N(" << tamanhos[i] << ")";
        outputFile << i << ":N(" << tamanhos[i] << ")";

        // Arrays para armazenar os resultados das variantes
        const int numVariantes = 6;
        string nomes[numVariantes] = {"LP", "HP", "LM", "HM", "LA", "HA"};
        int resultados[numVariantes];
        // Criação de cópias para cada variante
        int* copiaVetor = new int[tamanhos[i]];

        // Lomuto Padrão
        memcpy(copiaVetor, vetoresOriginais[i], tamanhos[i] * sizeof(int));
        numTrocas = 0;
        numChamadas = 0;
        quickSortLomutoPadrao(copiaVetor, 0, tamanhos[i] - 1);
        resultados[0] = numTrocas + numChamadas;

        // Hoare Padrão
        memcpy(copiaVetor, vetoresOriginais[i], tamanhos[i] * sizeof(int));
        numTrocas = 0;
        numChamadas = 0;
        quickSortHoarePadrao(copiaVetor, 0, tamanhos[i] - 1);
        resultados[1] = numTrocas + numChamadas;

        // Lomuto Mediana
        memcpy(copiaVetor, vetoresOriginais[i], tamanhos[i] * sizeof(int));
        numTrocas = 0;
        numChamadas = 0;
        quickSortLomutoMediana(copiaVetor, 0, tamanhos[i] - 1);
        resultados[2] = numTrocas + numChamadas;

        // Hoare Mediana
        memcpy(copiaVetor, vetoresOriginais[i], tamanhos[i] * sizeof(int));
        numTrocas = 0;
        numChamadas = 0;
        quickSortHoareMediana(copiaVetor, 0, tamanhos[i] - 1);
        resultados[3] = numTrocas + numChamadas;

        // Lomuto Aleatório
        memcpy(copiaVetor, vetoresOriginais[i], tamanhos[i] * sizeof(int));
        numTrocas = 0;
        numChamadas = 0;
        quickSortLomutoAleatorio(copiaVetor, 0, tamanhos[i] - 1);
        resultados[4] = numTrocas + numChamadas;

        // Hoare Aleatório
        memcpy(copiaVetor, vetoresOriginais[i], tamanhos[i] * sizeof(int));
        numTrocas = 0;
        numChamadas = 0;
        quickSortHoareAleatorio(copiaVetor, 0, tamanhos[i] - 1);
        resultados[5] = numTrocas + numChamadas;

        // Ordenar os resultados usando Merge Sort
        mergeSort(nomes, resultados, 0, numVariantes - 1);
        // Exibir os resultados ordenados
        for (int j = 0; j < numVariantes; j++) {
            //cout << "," << nomes[j] << "(" << resultados[j] << ")";
            outputFile << "," << nomes[j] << "(" << resultados[j] << ")";

        }

        outputFile << endl; // Nova linha para o próximo vetor

        // Libera a memória alocada para a cópia
        delete[] copiaVetor;
    }
}



int main(int argc, char *argv[])
{
    
   //ifstream inputFile("input.txt");
   // Verifica se os arquivos de entrada e saída foram fornecidos
   /*
    if (argc < 3) {
        cerr << "Uso: " << argv[0] << " <arquivo_entrada> <arquivo_saida>" << endl;
        return 1;
    }*/
    // Abrindo o arquivo de entrada
    ifstream inputFile(argv[1]);
    if (!inputFile.is_open()) {
        cerr << "Erro ao abrir o arquivo de entrada!" << endl;
        return 1;
    }
    // Abrindo o arquivo de saída
    ofstream outputFile(argv[2]);
    if (!outputFile.is_open()) {
        cerr << "Erro ao abrir o arquivo de saída!" << endl;
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
    
    executarEExibirResultados(vetores, tamanhos, numVetores, vetoresOriginais , outputFile);

    // Liberando memória alocada
    for (int i = 0; i < numVetores; i++) {
        delete[] vetores[i];
        delete[] vetoresOriginais[i];

    }
    inputFile.close();  
 
    delete[] vetoresOriginais;
    delete[] vetores;
    delete[] tamanhos;

    return 0;
    
}
