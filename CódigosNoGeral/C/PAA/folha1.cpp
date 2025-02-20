#include <iostream>
#include <fstream>
#include <cstring>

using namespace std;

// Limites ajustáveis conforme a necessidade
#define MAX_DOENCAS 1000
#define MAX_GENES 50
#define MAX_TAM_DNA 10000
#define MAX_TAM_GENE 101

// Estrutura que armazena os dados de uma doença.
struct Doenca {
    char codigo[21];
    int numGenes;
    char genes[MAX_GENES][MAX_TAM_GENE];
    double compatibilidade;
};

// --- Funções KMP ---
// Calcula o vetor "lps" (longest prefix which is also suffix) para o padrão.
void computeLPS(const char *pattern, int m, int *lps) {
    lps[0] = 0;
    int len = 0;
    int i = 1;
    while (i < m) {
        if (pattern[i] == pattern[len]) {
            len++;
            lps[i] = len;
            i++;
        } else {
            if (len != 0) {
                len = lps[len - 1];
            } else {
                lps[i] = 0;
                i++;
            }
        }
    }
}

// Retorna true se o padrão for encontrado no texto usando KMP.
bool kmpSearch(const char* text, const char* pattern) {
    int n = strlen(text);
    int m = strlen(pattern);
    if (m == 0)
        return true; // padrão vazio sempre encontrado
    int* lps = new int[m];
    computeLPS(pattern, m, lps);
    
    int i = 0, j = 0;
    while (i < n) {
        if (text[i] == pattern[j]) {
            i++; j++;
        }
        if (j == m) {
            delete [] lps;
            return true;
        } else if (i < n && text[i] != pattern[j]) {
            if (j != 0)
                j = lps[j - 1];
            else
                i++;
        }
    }
    delete [] lps;
    return false;
}

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
        if (L[i].compatibilidade >= R[j].compatibilidade) {
            arr[k++] = L[i++];
        } else {
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

// --- Função principal ---
int main(int argc, char* argv[]) {
   // ifstream inputFile("input.txt");
    //ofstream outputFile("output.txt");
    ifstream inputFile(argv[1]);
    ofstream outputFile(argv[2]);
    if (!inputFile || !outputFile) {
        cout << "Erro ao abrir arquivo!" << endl;
        return 1;
    }
    
    int tamSubcadeia, numDoencas;
    inputFile >> tamSubcadeia;
    inputFile.ignore(); // ignora o '\n'
    
    char dna[MAX_TAM_DNA];
    inputFile.getline(dna, MAX_TAM_DNA);
    
    inputFile >> numDoencas;
    inputFile.ignore();
    
    // Aloca dinamicamente o array de doenças
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
            // Se o gene é menor que o tamanho mínimo, ignora-o
            if (geneLen < tamSubcadeia)
                continue;
            
            int totalSubcadeias = geneLen - tamSubcadeia + 1;
            int subcadeiasEncontradas = 0;
            
            // Para cada posição do gene, extrai uma subcadeia de tamanho 'tamSubcadeia'
            for (int k = 0; k < totalSubcadeias; k++) {
                char subcadeia[MAX_TAM_GENE];
                strncpy(subcadeia, doencas[i].genes[j] + k, tamSubcadeia);
                subcadeia[tamSubcadeia] = '\0';
                if (kmpSearch(dna, subcadeia))
                    subcadeiasEncontradas++;
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
        outputFile << doencas[i].codigo << " ->" << comp << "%" << endl;
    }
    
    // Libera a memória alocada e fecha os arquivos
    delete [] doencas;
    inputFile.close();
    outputFile.close();
    
    return 0;
}
