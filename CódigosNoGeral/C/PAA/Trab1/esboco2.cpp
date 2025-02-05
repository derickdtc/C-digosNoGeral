#include <iostream>
#include <cstring>
#include <fstream>
#include <math.h>
using namespace std;

struct  Container
{
    int ordemCadastramento = 0;
    char codigo[15];
    char cnpj[20];
    int peso;
};

struct ContainerErrado : public Container{
    int pesoCerto;
    char cnpjCerto[20];    
    float difPercent;
};
ContainerErrado* compararContainersMsmCodigo(Container* conjunto1, int qtdCont1, Container* conjunto2, int qtdCont2, int& qtdCont3) {
    ContainerErrado* conjunto3 = new ContainerErrado[qtdCont2]; // Alterado para ContainerErrado
    qtdCont3 = 0;

    for (int i = 0; i < qtdCont1; i++) {
        bool achou = false;

        for (int j = 0; j < qtdCont2; j++) {
            if (strcmp(conjunto1[i].codigo, conjunto2[j].codigo) == 0) {
                achou = true;
                float diffPercent = (abs(conjunto1[i].peso - conjunto2[j].peso) * 100.0) / conjunto1[i].peso;
                 // Preencher os dados no conjunto3
                strcpy(conjunto3[qtdCont3].codigo, conjunto2[j].codigo);
                strcpy(conjunto3[qtdCont3].cnpj, conjunto2[j].cnpj);
                strcpy(conjunto3[qtdCont3].cnpjCerto, conjunto1[i].cnpj);
                conjunto3[qtdCont3].peso = conjunto2[j].peso;
                conjunto3[i].ordemCadastramento = conjunto1[i].ordemCadastramento;
                conjunto3[qtdCont3].pesoCerto = conjunto1[i].peso;

                // Comparar os CNPJs
                if (strcmp(conjunto1[i].cnpj, conjunto2[j].cnpj) != 0) {                    
                    conjunto3[qtdCont3].difPercent = 0; // Sem diferença de peso
                    qtdCont3++;
                } else {
                    // Comparar os pesos
                    float diffPercent = (abs(conjunto1[i].peso - conjunto2[j].peso) * 100.0) / conjunto1[i].peso;
                    if (diffPercent > 10.0) {
                       
                        conjunto3[qtdCont3].difPercent = round(diffPercent); // Arredondado
                        qtdCont3++;
                    }
                }

                break; // Não precisa continuar procurando
            }
        }

        if (!achou) {
            cout << "Contêiner com código " << conjunto1[i].codigo << " não encontrado no conjunto2." << endl;
        }
    }

    return conjunto3;
}

// Função de comparação personalizada para os critérios dados
bool comparar(const ContainerErrado& a, const ContainerErrado& b) {
    // Critério 1: Priorizar containers com CNPJ errado
    if (strcmp(a.cnpj, a.cnpjCerto) != 0 && strcmp(b.cnpj, b.cnpjCerto) == 0) {
        return true;
    }
    if (strcmp(a.cnpj, a.cnpjCerto) == 0 && strcmp(b.cnpj, b.cnpjCerto) != 0) {
        return false;
    }

    // Se ambos ou nenhum têm CNPJ errado, passar para o próximo critério
    if (strcmp(a.cnpj, a.cnpjCerto) == 0 && strcmp(b.cnpj, b.cnpjCerto) == 0) {
        // Critério 2: Containers com diferença percentual de peso maior que 10%
        if (a.difPercent > 10.0 && b.difPercent <= 10.0) {
            return true;
        }
        if (a.difPercent <= 10.0 && b.difPercent > 10.0) {
            return false;
        }

        // Critério 3: Comparar pela diferença percentual em ordem decrescente
        if (a.difPercent != b.difPercent) {
            return a.difPercent > b.difPercent;
        }
    }

    // Critério 4: Comparar pela ordem de cadastramento em ordem crescente
    return a.ordemCadastramento < b.ordemCadastramento;
}

// Função para mesclar dois subarrays ordenados
void merge(ContainerErrado* arr, int left, int mid, int right) {
    int n1 = mid - left + 1; // Tamanho do subarray esquerdo
    int n2 = right - mid;    // Tamanho do subarray direito

    // Arrays temporários para os subarrays
    ContainerErrado* leftArr = new ContainerErrado[n1];
    ContainerErrado* rightArr = new ContainerErrado[n2];

    // Copiar os dados para os arrays temporários
    for (int i = 0; i < n1; i++) {
        leftArr[i] = arr[left + i];
    }
    for (int i = 0; i < n2; i++) {
        rightArr[i] = arr[mid + 1 + i];
    }

    // Mesclar os dois subarrays de volta ao array original
    int i = 0, j = 0, k = left;
    while (i < n1 && j < n2) {
        if (comparar(leftArr[i], rightArr[j])) {
            arr[k++] = leftArr[i++];
        } else {
            arr[k++] = rightArr[j++];
        }
    }

    // Copiar os elementos restantes do subarray esquerdo (se houver)
    while (i < n1) {
        arr[k++] = leftArr[i++];
    }

    // Copiar os elementos restantes do subarray direito (se houver)
    while (j < n2) {
        arr[k++] = rightArr[j++];
    }

    // Liberar memória dos arrays temporários
    delete[] leftArr;
    delete[] rightArr;
}

// Função recursiva do MergeSort
void mergeSort(ContainerErrado* arr, int left, int right) {
    if (left < right) {
        int mid = left + (right - left) / 2;

        // Dividir o array em duas partes
        mergeSort(arr, left, mid);
        mergeSort(arr, mid + 1, right);

        // Mesclar as duas partes ordenadas
        merge(arr, left, mid, right);
    }
}


int main(int argc, char* argv[])
{
    int qtdCont1 , qtdCont2, qtdCont3;

    ifstream inputFile(argv[1]); // Abrir o arquivo de entrada
    if (!inputFile) {
        cerr << "Erro ao abrir o arquivo input.txt" << endl;
        return 1;
    }

    inputFile>> qtdCont1;
    Container* conjunto1 = new Container[qtdCont1];

     // Ler os dados do primeiro conjunto
    for (int i = 0; i < qtdCont1; i++) {
        inputFile >> conjunto1[i].codigo >> conjunto1[i].cnpj >> conjunto1[i].peso;
        conjunto1[i].ordemCadastramento = i +1;
    }

    // Ler o número de contêineres do segundo conjunto
    inputFile >> qtdCont2;
    Container* conjunto2 = new Container[qtdCont2]; // Alocar dinamicamente

    // Ler os dados do segundo conjunto
    for (int i = 0; i < qtdCont2; i++) {
        inputFile >> conjunto2[i].codigo >> conjunto2[i].cnpj >> conjunto2[i].peso;
    }

    // Exemplo de saída para verificar os dados
    /*
    cout << "Conjunto 1:" << endl;
    for (int i = 0; i < qtdCont1; i++) {
        cout << conjunto1[i].codigo << " " << conjunto1[i].cnpj << " " << conjunto1[i].peso << endl;
    }

    cout << "Conjunto 2:" << endl;
    for (int i = 0; i < qtdCont2; i++) {
        cout << conjunto2[i].codigo << " " << conjunto2[i].cnpj << " " << conjunto2[i].peso << endl;
    }*/

     // Comparando os dois conjuntos
    ContainerErrado* conjunto3 = compararContainersMsmCodigo(conjunto1, qtdCont1, conjunto2, qtdCont2, qtdCont3);

    cout << "Contêineres divergentes:" << endl;
    for (int i = 0; i < qtdCont3; i++) {
        
        cout << conjunto3[i].codigo << ":" << conjunto3[i].cnpj << "<->" << conjunto3[i].cnpjCerto<< " " << conjunto3[i].peso << " " << conjunto3[i].difPercent << " " << conjunto3[i].ordemCadastramento << endl; 
        
        
    }
     mergeSort(conjunto3 , 0 , qtdCont3-1 );
    // Exibir o conjunto4 (ordenado)
    cout << "Conjunto4 (ordenado):" << endl;
    for (int i = 0; i < qtdCont3; i++) {
        // Verificar se o CNPJ está errado
    if (strcmp(conjunto3[i].cnpj, conjunto3[i].cnpjCerto) != 0) {
        // Saída para CNPJ errado
        cout << conjunto3[i].codigo << ":"
             << conjunto3[i].cnpjCerto << "<->" << conjunto3[i].cnpj << endl;
    } else if (conjunto3[i].difPercent > 10.0) {
        // Maior diferença de peso saindo primeiro(com a ordem de entrada)
        int difPeso = abs(conjunto3[i].peso - conjunto3[i].pesoCerto);
        cout << conjunto3[i].codigo << ":"
             << difPeso << "kg(" << static_cast<int>(round(conjunto3[i].difPercent)) << "%)" << endl;
    }
    }

    ofstream outputFile(argv[2]);
    // Liberar a memória alocada dinamicamente
    delete[] conjunto1;
    delete[] conjunto2;
    delete[] conjunto3;
    inputFile.close();
    return 0;
}
