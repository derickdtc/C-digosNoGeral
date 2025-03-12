#include <iostream>
#include <fstream>
#include <string>
#include <cmath>   
#include <limits>  // opcional, para ignore()

using namespace std;


#define MAX_DOENCAS 1000
#define MAX_GENES   100

struct Disease {
    string code;       // código da doença
    int numGenes;      // quantidade de genes
    string* genes;     // array dinâmico de genes
    double compatibility; // compatibilidade calculada
};

// --------------------- KMP ---------------------

/*
  computeLPS(pattern, m, lps):
  Preenche o array lps[] onde lps[i] é o comprimento do maior prefixo que também
  é sufixo para o padrão[0..i].
*/
void computeLPS(const string &pattern, int m, int *lps) {
    int len = 0; // tamanho do prefixo anterior
    lps[0] = 0;
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

/*
  kmpSearch(text, pattern):
  Retorna true se "pattern" for encontrado em "text" usando KMP, caso contrário false.
*/
bool kmpSearch(const string &text, const string &pattern) {
    int n = text.size();
    int m = pattern.size();
    if (m == 0) return true;  // string vazia é sempre encontrada

    // Cria array lps
    int *lps = new int[m];
    computeLPS(pattern, m, lps);

    int i = 0; // índice para text
    int j = 0; // índice para pattern

    while (i < n) {
        if (text[i] == pattern[j]) {
            i++;
            j++;
        }
        if (j == m) {
            delete[] lps;
            return true;
        } else if (i < n && text[i] != pattern[j]) {
            if (j != 0) {
                j = lps[j - 1];
            } else {
                i++;
            }
        }
    }

    delete[] lps;
    return false;
}


  
void merge(Disease arr[], int left, int mid, int right) {
    int n1 = mid - left + 1;
    int n2 = right - mid;

    // Arrays temporários
    Disease* L = new Disease[n1];
    Disease* R = new Disease[n2];

    // Copia dados para L[] e R[]
    for (int i = 0; i < n1; i++) {
        L[i] = arr[left + i];
    }
    for (int j = 0; j < n2; j++) {
        R[j] = arr[mid + 1 + j];
    }

    // Indices iniciais de L[], R[] e arr[]
    int i = 0, j = 0, k = left;

    // Une L[] e R[] de forma decrescente pela compatibilidade
    while (i < n1 && j < n2) {
        if (L[i].compatibility >= R[j].compatibility) {
            arr[k] = L[i];
            i++;
        } else {
            arr[k] = R[j];
            j++;
        }
        k++;
    }
    // Copia o restante de L[], se houver
    while (i < n1) {
        arr[k] = L[i];
        i++;
        k++;
    }
    // Copia o restante de R[], se houver
    while (j < n2) {
        arr[k] = R[j];
        j++;
        k++;
    }

    delete[] L;
    delete[] R;
}

/*
  mergeSort(arr, left, right):
  Ordena recursivamente o subarray arr[left..right] (decrescente).
*/
void mergeSort(Disease arr[], int left, int right) {
    if (left < right) {
        int mid = (left + right) / 2;
        mergeSort(arr, left, mid);
        mergeSort(arr, mid + 1, right);
        merge(arr, left, mid, right);
    }
}

// --------------------- MAIN ---------------------

int main(int argc, char* argv[]) {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    // Abertura de arquivos sem usar <vector> ou <algorithm>
    ifstream inFile;
    ofstream outFile;

    // Se não tiver argumentos, assume "prof.in.txt" e "output.txt"
    if (argc < 3) {
        inFile.open("prof.in.txt");
        outFile.open("output.txt");
    } else {
        inFile.open(argv[1]);
        outFile.open(argv[2]);
    }

    if (!inFile || !outFile) {
        cerr << "Erro ao abrir arquivo de entrada/saída!\n";
        return 1;
    }

    // Leitura do tamanho mínimo da subcadeia
    int tamSub;
    inFile >> tamSub;
    inFile.ignore(numeric_limits<streamsize>::max(), '\n');

    // Leitura do DNA
    string dna;
    getline(inFile, dna);

    // Leitura do número de doenças
    int numDoencas;
    inFile >> numDoencas;
    inFile.ignore(numeric_limits<streamsize>::max(), '\n');

    static Disease diseases[MAX_DOENCAS];

    for (int i = 0; i < numDoencas; i++) {
        string code;
        int nGenes;
        inFile >> code >> nGenes;

        // Preenche a estrutura
        diseases[i].code = code;
        diseases[i].numGenes = nGenes;
        diseases[i].compatibility = 0.0;

        // Aloca dinamicamente os genes desta doença
        diseases[i].genes = new string[nGenes];

        // Lê cada gene
        for (int g = 0; g < nGenes; g++) {
            inFile >> diseases[i].genes[g];
        }
    }

    double threshold = 0.8;  // 80% das subcadeias precisam ser encontradas

    // Para cada doença, calcula quantos genes são detectados
    for (int i = 0; i < numDoencas; i++) {
        int detectedGenes = 0;
        int totalGenes = diseases[i].numGenes;

        for (int g = 0; g < totalGenes; g++) {
            const string &gene = diseases[i].genes[g];
            int geneLen = (int)gene.size();
            // Se o gene for menor que o tamanho mínimo, não conta
            if (geneLen < tamSub) {
                continue;
            }
            int totalSub = geneLen - tamSub + 1;
            int foundSubs = 0;

            // Para cada posição do gene, extrai subcadeia de tamanho tamSub
            for (int pos = 0; pos < totalSub; pos++) {
                string sub = gene.substr(pos, tamSub);
                // Se a subcadeia for encontrada no DNA
                if (kmpSearch(dna, sub)) {
                    foundSubs++;
                }
            }

            double frac = (double)foundSubs / totalSub;
            if (frac >= threshold) {
                detectedGenes++;
            }
        }

        // Calcula compatibilidade
        diseases[i].compatibility = (double)detectedGenes / totalGenes * 100.0;
    }

    mergeSort(diseases, 0, numDoencas - 1);

    // Escreve a saída
    for (int i = 0; i < numDoencas; i++) {
        // Arredonda
        int comp = (int)(diseases[i].compatibility + 0.5);
        outFile << diseases[i].code << " ->" << comp << "%" << "\n";
    }

    // Libera os arrays de genes
    for (int i = 0; i < numDoencas; i++) {
        delete[] diseases[i].genes;
    }

    inFile.close();
    outFile.close();

    return 0;
}
