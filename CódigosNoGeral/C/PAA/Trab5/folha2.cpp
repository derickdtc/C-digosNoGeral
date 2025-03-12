#include <iostream>
#include <fstream>
#include <cstring>
#include <limits> // Necessário para numeric_limits<streamsize>::max()

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
        // Ordena de forma decrescente pela compatibilidade
        if (L[i].compatibilidade >= R[j].compatibilidade)
            arr[k++] = L[i++];
        else
            arr[k++] = R[j++];
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

// --- Função principal ---
int main(int argc, char* argv[]) {
    // Para minimizar a sobrecarga, removemos as mensagens de debug.
    ifstream inputFile;
    ofstream outputFile;
    
    if (argc < 3) {
        inputFile.open("prof.in.txt");
        outputFile.open("output.txt");
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
        for (int j = 0; j < doencas[i].numGenes; j++) {
            inputFile >> doencas[i].genes[j];
        }
    }
    
    double threshold = 0.8; // Critério: 80% das subcadeias devem ser encontradas
    // Para cada doença, processa cada gene
    for (int i = 0; i < numDoencas; i++) {
        int genesDetectados = 0;
        for (int j = 0; j < doencas[i].numGenes; j++) {
            int geneLen = strlen(doencas[i].genes[j]);
            if (geneLen < tamSubcadeia)
                continue;
            
            int totalSubcadeias = geneLen - tamSubcadeia + 1;
            int subcadeiasEncontradas = 0;
            
            // Usa ponteiro para facilitar o acesso
            char* gene = doencas[i].genes[j];
            for (int k = 0; k < totalSubcadeias; k++) {
                // Em vez de copiar a subcadeia, temporariamente "corta" a string
                char backup = gene[k + tamSubcadeia]; // guarda o caractere sobrescrito
                gene[k + tamSubcadeia] = '\0';
                if (strstr(dna, gene + k) != NULL)
                    subcadeiasEncontradas++;
                gene[k + tamSubcadeia] = backup; // restaura o caractere
            }
            
            double frac = (double) subcadeiasEncontradas / totalSubcadeias;
            if (frac >= threshold)
                genesDetectados++;
        }
        doencas[i].compatibilidade = ((double) genesDetectados / doencas[i].numGenes) * 100.0;
    }
    
    // Ordena as doenças por compatibilidade (ordem decrescente)
    mergeSort(doencas, 0, numDoencas - 1);
    
    // Escreve a saída no arquivo, arredondando a compatibilidade
    for (int i = 0; i < numDoencas; i++) {
        int comp = (int)(doencas[i].compatibilidade + 0.5);
        outputFile << doencas[i].codigo << "->" << comp << "%" << endl;
    }
    
    delete [] doencas;
    inputFile.close();
    outputFile.close();
    
    return 0;
}
