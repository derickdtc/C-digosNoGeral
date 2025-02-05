#include <iostream>
#include <cstring>
#include <fstream>
#include <cmath>
using namespace std;

// Estrutura Container
struct Container {
    int ordemCadastramento = 0;
    char codigo[15];
    char cnpj[20];
    int peso;
};

// Estrutura ContainerErrado
struct ContainerErrado : public Container {
    char cnpjCerto[20];
    float difPercent;
    int pesoCerto;
};
// Estrutura de nó para armazenar pares chave-valor
struct HashNode {
    char key[15];       // Código do container (chave)
    Container value;    // Objeto Container (valor)
    HashNode* next;     // Próximo nó na lista encadeada

    HashNode(const char* k, const Container& v) {
        strcpy(key, k);
        value = v;
        next = nullptr;
    }
};

// Estrutura da Tabela Hash
struct HashMap {
    int size;           // Tamanho da tabela
    HashNode** table;   // Array de listas encadeadas

    // Construtor
    HashMap(int s) {
        size = s;
        table = new HashNode*[size];
        for (int i = 0; i < size; i++) {
            table[i] = nullptr;
        }
    }

    // Destrutor
    ~HashMap() {
        for (int i = 0; i < size; i++) {
            HashNode* current = table[i];
            while (current) {
                HashNode* toDelete = current;
                current = current->next;
                delete toDelete;
            }
        }
        delete[] table;
    }

    // Função hash (baseada no valor ASCII dos caracteres)
    int hashFunction(const char* key) {
        int hash = 0;
        for (int i = 0; key[i] != '\0'; i++) {
            hash = (hash * 31 + key[i]) % size; // 31 é uma constante arbitrária
        }
        return hash;
    }

    // Inserir um par chave-valor
    void insert(const char* key, const Container& value) {
        int index = hashFunction(key);
        HashNode* newNode = new HashNode(key, value);

        // Inserir no início da lista encadeada
        newNode->next = table[index];
        table[index] = newNode;
    }

    // Buscar um valor pela chave
    Container* search(const char* key) {
        int index = hashFunction(key);
        HashNode* current = table[index];
        while (current) {
            if (strcmp(current->key, key) == 0) {
                return &current->value;
            }
            current = current->next;
        }
        return nullptr; // Não encontrado
    }
};

// Função para comparar containers no MergeSort
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
void merge(ContainerErrado* arr, int inicio, int meio, int fim) {
    int n1 = meio - inicio + 1; // Tamanho do subarray esquerdo
    int n2 = fim - meio;    // Tamanho do subarray direito

    // Arrays temporários para os subarrays
    ContainerErrado* inicioArr = new ContainerErrado[n1];
    ContainerErrado* fimArr = new ContainerErrado[n2];

    // Copiar os dados para os arrays temporários
    for (int i = 0; i < n1; i++) {
        inicioArr[i] = arr[inicio + i];
    }
    for (int i = 0; i < n2; i++) {
        fimArr[i] = arr[meio + 1 + i];
    }

    // Mesclar os dois subarrays de volta ao array original
    int i = 0, j = 0, k = inicio;
    while (i < n1 && j < n2) {
    if (comparar(inicioArr[i], fimArr[j])) {
        arr[k++] = inicioArr[i++];
    } else {
        arr[k++] = fimArr[j++];
    }
}

    // Copiar os elementos restantes do subarray esquerdo (se houver)
    while (i < n1) {
        arr[k++] = inicioArr[i++];
    }

    // Copiar os elementos restantes do subarray direito (se houver)
    while (j < n2) {
        arr[k++] = fimArr[j++];
    }

    // Liberar memória dos arrays temporários
    delete[] inicioArr;
    delete[] fimArr;
}

void mergeSort(ContainerErrado* arr, int inicio, int fim) {
    if (inicio < fim) {
        int meio = inicio + (fim - inicio) / 2;

        // Dividir o array em duas partes
        mergeSort(arr, inicio, meio);
        mergeSort(arr, meio + 1, fim);

        // Mesclar as duas partes ordenadas
        merge(arr, inicio, meio, fim);
    }
}

// Função para comparar containers e gerar o conjunto de containers errados
ContainerErrado* compararContainersMsmCodigo(Container* conjunto1, int qtdCont1, Container* conjunto2, int qtdCont2, int& qtdCont3) {
    // Criar tabela hash personalizada
    HashMap mapaCerto(1000000); // Tamanho da tabela hash (ajustar conforme necessário)
    for (int i = 0; i < qtdCont1; i++) {
        mapaCerto.insert(conjunto1[i].codigo, conjunto1[i]);
    }

    // Criar conjuntoErrado
    ContainerErrado* conjuntoErrado = new ContainerErrado[qtdCont2];
    qtdCont3 = 0;

    for (int i = 0; i < qtdCont2; i++) {
        Container* contCerto = mapaCerto.search(conjunto2[i].codigo);
        if (contCerto) {
            bool cnpjErrado = strcmp(conjunto2[i].cnpj, contCerto->cnpj) != 0;
            float difPercent = round((abs(contCerto->peso - conjunto2[i].peso) * 100.0) / contCerto->peso);

            if (cnpjErrado || difPercent > 10.0) {
                ContainerErrado errado;
                errado.ordemCadastramento = contCerto->ordemCadastramento;
                strcpy(errado.codigo, conjunto2[i].codigo);
                strcpy(errado.cnpj, conjunto2[i].cnpj);
                strcpy(errado.cnpjCerto, contCerto->cnpj);
                errado.peso = conjunto2[i].peso;
                errado.pesoCerto = contCerto->peso;
                errado.difPercent = difPercent;

                conjuntoErrado[qtdCont3++] = errado;
            }
        }
    }

    return conjuntoErrado;
}


// Programa principal
int main(int argc, char* argv[])
{
    int qtdCont1 , qtdCont2, qtdCont3;

    ifstream inputFile(argv[1]); // Abrir o arquivo de entrada
    //ifstream inputFile("input.txt");
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

     // Gerar o conjuntoErrado
    ContainerErrado* conjuntoErrado = compararContainersMsmCodigo(conjunto1, qtdCont1, conjunto2, qtdCont2, qtdCont3);

    // Ordenar com MergeSort
    mergeSort(conjuntoErrado, 0, qtdCont3 - 1);

    // Exibir o conjunto ordenado
    for (int i = 0; i < qtdCont3; i++) {
        if (strcmp(conjuntoErrado[i].cnpj, conjuntoErrado[i].cnpjCerto) != 0) {
            cout << conjuntoErrado[i].codigo << ":"
                 << conjuntoErrado[i].cnpjCerto << "<->" << conjuntoErrado[i].cnpj << " "<< endl;
        } else {
            int difPeso = abs(conjuntoErrado[i].peso - conjuntoErrado[i].pesoCerto);
            cout << conjuntoErrado[i].codigo << ":"
                 << difPeso << "kg(" << static_cast<int>(round(conjuntoErrado[i].difPercent)) << "%)" << endl;
        }
    }
    ofstream outputFile(argv[2]);

    // Liberar memória
    delete[] conjuntoErrado;

    return 0;
}