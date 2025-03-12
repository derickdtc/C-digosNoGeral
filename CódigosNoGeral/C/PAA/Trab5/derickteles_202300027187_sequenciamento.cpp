#include <iostream>
#include <fstream>
#include <cstring>
#include <limits> // Para numeric_limits

using namespace std;

#define MAX_DOENCAS 1000
#define MAX_GENES 100
#define MAX_TAM_DNA 50000
#define MAX_TAM_GENE 12000

// Estrutura que armazena os dados de uma doença.
struct Doenca {
    char codigo[21];
    int numGenes;
    char genes[MAX_GENES][MAX_TAM_GENE];
    double compatibilidade;
    int idx;
};

// --- Funções de Merge Sort para ordenar as doenças por compatibilidade ---
void merge(Doenca arr[], int left, int mid, int right) {
    int n1 = mid - left + 1;
    int n2 = right - mid;
    
    Doenca* L = new Doenca[n1];
Doenca* R = new Doenca[n2];

for (int i = 0; i < n1; i++)
    L[i] = arr[left + i];
for (int j = 0; j < n2; j++)
    R[j] = arr[mid + 1 + j];

int i = 0, j = 0, k = left;
while (i < n1 && j < n2) {
    // Ordena de forma decrescente pela compatibilidade.
    // Em caso de empate, usa o índice de entrada para manter a ordem original.
    if (L[i].compatibilidade > R[j].compatibilidade) {
        arr[k++] = L[i++];
    } else if (L[i].compatibilidade < R[j].compatibilidade) {
        arr[k++] = R[j++];
    } else {  // empate na compatibilidade
        if (L[i].idx < R[j].idx)
            arr[k++] = L[i++];
        else
            arr[k++] = R[j++];
    }
    }
    while (i < n1)
        arr[k++] = L[i++];
    while (j < n2)
        arr[k++] = R[j++];

    delete [] L;
    delete [] R;

}

void mergeSort(Doenca arr[], int left, int right) {
    if (left < right) {
        int mid = left + (right - left) / 2;
        mergeSort(arr, left, mid);
        mergeSort(arr, mid + 1, right);
        merge(arr, left, mid, right);
    }
}

// Função de busca binária em um array de hashes (ordenado)
bool buscaBinaria(const unsigned long long* arr, int n, unsigned long long chave) {
    int ini = 0, fim = n - 1;
    while (ini <= fim) {
        int mid = (ini + fim) / 2;
        if (arr[mid] == chave)
            return true;
        else if (arr[mid] < chave)
            ini = mid + 1;
        else
            fim = mid - 1;
    }
    return false;
}

// --- Função principal ---
int main(int argc, char* argv[]) {
    // Removemos mensagens de debug para minimizar a sobrecarga de I/O.
    ifstream inputFile;
    ofstream outputFile;
    
    if (argc < 3) {
        inputFile.open("inputDoServidor.txt");
        outputFile.open("minhaSaida.txt");
    } else {
        inputFile.open(argv[1]);
        outputFile.open(argv[2]);
    }
    
    if (!inputFile || !outputFile) {
        cerr << "Erro ao abrir arquivo!" << endl;
        return 1;
    }
    
    int tamSubcadeia;
    if (!(inputFile >> tamSubcadeia)) {
        cerr << "Erro ao ler tamSubcadeia." << endl;
        return 1;
    }
    inputFile.ignore(numeric_limits<streamsize>::max(), '\n');
    
    char dna[MAX_TAM_DNA];
    if (!inputFile.getline(dna, sizeof(dna))) {
        cerr << "Erro ao ler a sequência de DNA." << endl;
        return 1;
    }
    
    int numDoencas;
    if (!(inputFile >> numDoencas)) {
        cerr << "Erro ao ler numDoencas." << endl;
        return 1;
    }
    inputFile.ignore(numeric_limits<streamsize>::max(), '\n');
    
    if (numDoencas <= 0) {
        cerr << "Valor inválido para numDoencas: " << numDoencas << endl;
        return 1;
    }
    
    Doenca* doencas = new Doenca[numDoencas];
    
    for (int i = 0; i < numDoencas; i++) {
        inputFile >> doencas[i].codigo >> doencas[i].numGenes;
        doencas[i].idx = i;
        for (int j = 0; j < doencas[i].numGenes; j++) {
            inputFile >> doencas[i].genes[j];
        }
    }
    
    // Pré-processamento do DNA usando rolling hash.
    // Usamos base 131 e o tipo unsigned long long para minimizar colisões.
    const unsigned long long base = 131ULL;
    int lenDNA = strlen(dna);
    unsigned long long* hashDNA = new unsigned long long[lenDNA + 1];
    unsigned long long* potDNA = new unsigned long long[lenDNA + 1];
    hashDNA[0] = 0;
    potDNA[0] = 1;
    for (int i = 0; i < lenDNA; i++) {
        hashDNA[i + 1] = hashDNA[i] * base + (unsigned long long)(dna[i]);
        potDNA[i + 1] = potDNA[i] * base;
    }
    
    // Armazena os hashes de todas as subcadeias do DNA de tamanho tamSubcadeia.
    int totalSubDNA = lenDNA - tamSubcadeia + 1;
    unsigned long long* dnaHashes = new unsigned long long[totalSubDNA];
    for (int i = 0; i < totalSubDNA; i++) {
        dnaHashes[i] = hashDNA[i + tamSubcadeia] - hashDNA[i] * potDNA[tamSubcadeia];
    }
    
    // Ordena os hashes do DNA para permitir busca binária.
    // Implementa uma simples ordenação usando qsort do C.
    auto compareULL = [](const void* a, const void* b) -> int {
        unsigned long long arg1 = *(const unsigned long long*)a;
        unsigned long long arg2 = *(const unsigned long long*)b;
        if (arg1 < arg2) return -1;
        if (arg1 > arg2) return 1;
        return 0;
    };
    qsort(dnaHashes, totalSubDNA, sizeof(unsigned long long), compareULL);
    
    double threshold = 0.8; // Critério: 80% das subcadeias devem ser encontradas
    // Para cada doença, processa cada gene
    for (int i = 0; i < numDoencas; i++) {
        int genesDetectados = 0;
        for (int j = 0; j < doencas[i].numGenes; j++) {
            char* gene = doencas[i].genes[j];
            int lenGene = strlen(gene);
            if (lenGene < tamSubcadeia)
                continue;
            
            int totalSubGene = lenGene - tamSubcadeia + 1;
            int subcadeiasEncontradas = 0;
            
            // Pré-processa o gene para rolling hash.
            unsigned long long* hashGene = new unsigned long long[lenGene + 1];
            unsigned long long* potGene = new unsigned long long[lenGene + 1];
            hashGene[0] = 0;
            potGene[0] = 1;
            for (int k = 0; k < lenGene; k++) {
                hashGene[k + 1] = hashGene[k] * base + (unsigned long long)(gene[k]);
                potGene[k + 1] = potGene[k] * base;
            }
            
            for (int k = 0; k < totalSubGene; k++) {
                unsigned long long hashSub = hashGene[k + tamSubcadeia] - hashGene[k] * potGene[tamSubcadeia];
                // Busca binária no array de hashes do DNA.
                if (buscaBinaria(dnaHashes, totalSubDNA, hashSub))
                    subcadeiasEncontradas++;
            }
            
            delete [] hashGene;
            delete [] potGene;
            
            double frac = (double) subcadeiasEncontradas / totalSubGene;
            if (frac >= threshold)
                genesDetectados++;
        }
        doencas[i].compatibilidade = ((double) genesDetectados / doencas[i].numGenes) * 100.0;
        int comp = (int)(doencas[i].compatibilidade + 0.5);
        doencas[i].compatibilidade = comp;

    }
    
    // Ordena as doenças por compatibilidade (ordem decrescente)
    mergeSort(doencas, 0, numDoencas - 1);
    
    // Escreve a saída no arquivo, arredondando a compatibilidade
    for (int i = 0; i < numDoencas; i++) {
        outputFile << doencas[i].codigo << "->" <<  doencas[i].compatibilidade << "%" << endl;
    }
    
    delete [] doencas;
    delete [] hashDNA;
    delete [] potDNA;
    delete [] dnaHashes;
    
    inputFile.close();
    outputFile.close();
    
    return 0;
}
