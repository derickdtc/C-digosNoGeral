#include <iostream>
#include <fstream>
using namespace std;

// Estrutura para armazenar um pacote
struct Pacote {
    int numero;
    int tamanho;
    char dados[512][4]; // Cada dado tem no máximo 3 caracteres + '\0'
};

// Função para ajustar o heap (Heapify)
void heapify(Pacote **heap, int n, int i) {
    int largest = i;
    int left = 2 * i + 1;
    int right = 2 * i + 2;

    if (left < n && heap[left]->numero > heap[largest]->numero) {
        largest = left;
    }

    if (right < n && heap[right]->numero > heap[largest]->numero) {
        largest = right;
    }

    if (largest != i) {
        swap(heap[i], heap[largest]);
        heapify(heap, n, largest);
    }
}

// Função para ordenar os pacotes com HeapSort
void heapSort(Pacote **heap, int n) {
    for (int i = n / 2 - 1; i >= 0; i--) {
        heapify(heap, n, i);
    }

    for (int i = n - 1; i > 0; i--) {
        swap(heap[0], heap[i]);
        heapify(heap, i, 0);
    }
}

void processarPacotes(Pacote **pacotes, int totalPacotes, int quantidadeAgrupamento, ofstream &arquivoSaida) {
    int index = 0;
    int proximoEsperado = 0;
    int heapSize = 0;
    Pacote **heap = new Pacote *[totalPacotes]; // Heap para armazenar pacotes fora de ordem

    bool primeiraLinha = true; // Para evitar quebra de linha na primeira saída

    while (index < totalPacotes || heapSize > 0) {
        string linhaSaida = "|"; // Inicia a linha de saída com uma barra
        bool pacoteAdicionado = false; // Controla se algum pacote foi adicionado à linha

        // Processar pacotes agrupados conforme `quantidadeAgrupamento`
        for (int i = 0; i < quantidadeAgrupamento && index < totalPacotes; i++) {
            if (pacotes[index]->numero == proximoEsperado) {
                // Adiciona o pacote corretamente à linha de saída
                for (int j = 0; j < pacotes[index]->tamanho; j++) {
                    linhaSaida += pacotes[index]->dados[j];
                    if (j < pacotes[index]->tamanho - 1) {
                        linhaSaida += ",";
                    }
                }
                linhaSaida += "|"; // Fecha o pacote com uma barra
                proximoEsperado++;
                index++;
                pacoteAdicionado = true;
            } else {
                // Se encontrar um pacote fora de ordem, adicionamos ao heap
                heap[heapSize++] = pacotes[index];
                index++;
            }
        }

        // Ordena o heap para processar pacotes fora de ordem
        heapSort(heap, heapSize);

        // Processa pacotes ordenados do heap
        while (heapSize > 0 && heap[0]->numero == proximoEsperado) {
            // Adiciona o pacote ordenado à linha de saída
            for (int j = 0; j < heap[0]->tamanho; j++) {
                linhaSaida += heap[0]->dados[j];
                if (j < heap[0]->tamanho - 1) {
                    linhaSaida += ",";
                }
            }
            linhaSaida += "|"; // Fecha o pacote com uma barra
            proximoEsperado++;

            // Remove o pacote do heap
            heap[0] = heap[heapSize - 1];
            heapSize--;
            heapify(heap, heapSize, 0);

            pacoteAdicionado = true;
        }

        // Escreve a linha de saída no arquivo, se houver pacotes processados
        if (pacoteAdicionado) {
            if (!primeiraLinha) {
                arquivoSaida << endl; // Quebra de linha para pacotes subsequentes
            }
            arquivoSaida << linhaSaida;
            primeiraLinha = false;
        }
    }

    // Libera a memória do heap
    delete[] heap;
}

int main(int argc, char* argv[]) {
    /*
    if (argc < 3) {
        cerr << "Uso: " << argv[0] << " <arquivo_entrada> <arquivo_saida>" << endl;
        return 1;
    }*/

    ifstream arquivoEntrada(argv[1]);
    ofstream arquivoSaida(argv[2]);
    //ifstream arquivoEntrada("input.txt");
    //ofstream arquivoSaida("output.txt");
    if (!arquivoEntrada || !arquivoSaida) {
        cerr << "Erro ao abrir o arquivo!" << endl;
        return 1;
    }

    int totalPacotes, quantidadeAgrupamento;
    arquivoEntrada >> totalPacotes >> quantidadeAgrupamento;

    // Aloca memória para todos os pacotes
    Pacote *pacotes = new Pacote[totalPacotes];

    // Lê os pacotes do arquivo
    for (int i = 0; i < totalPacotes; i++) {
        int numero, tamanho;
        arquivoEntrada >> numero >> tamanho;

        pacotes[i].numero = numero;
        pacotes[i].tamanho = tamanho;
        for (int j = 0; j < tamanho; j++) {
            arquivoEntrada >> pacotes[i].dados[j];
        }
    }

    // Cria um array de ponteiros para os pacotes
    Pacote **pacotesPtr = new Pacote *[totalPacotes];
    for (int i = 0; i < totalPacotes; i++) {
        pacotesPtr[i] = &pacotes[i];
    }

    // Processa os pacotes e gera a saída
    processarPacotes(pacotesPtr, totalPacotes, quantidadeAgrupamento, arquivoSaida);

    // Libera memória
    delete[] pacotes;
    delete[] pacotesPtr;

    arquivoEntrada.close();
    arquivoSaida.close();
    return 0;
}