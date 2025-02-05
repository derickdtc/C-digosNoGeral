#include <iostream>
#include <fstream>
using namespace std;

// Estrutura para armazenar um pacote
struct Pacote {
    int nDoPacote;
    int tamDoPacote;
    char dados[512][4]; // Cada dado tem no máximo 3 caracteres + '\0'
};
// Ajusta o heap ao adicionar um novo elemento (heapify-up)
void heapify_up(Pacote **heap, int index) {
    while (index > 0) {
        int pai = (index - 1) / 2;
        if (heap[pai]->nDoPacote <= heap[index]->nDoPacote) break; // Ordem já correta
        swap(heap[pai], heap[index]);
        index = pai;
    }
}

// Remove o menor elemento do heap (heapify-down)
void heapify_down(Pacote **heap, int heapSize) {
    int i = 0;
    while (true) {
        int fEsq = 2 * i + 1 , fDir = 2 * i + 2, menorElemento = i;

        if (fEsq < heapSize && heap[fEsq]->nDoPacote < heap[menorElemento]->nDoPacote) {
            menorElemento = fEsq;
        }
        if (fDir < heapSize && heap[fDir]->nDoPacote < heap[menorElemento]->nDoPacote) {
            menorElemento = fDir;
        }
        if (menorElemento == i) break;

        swap(heap[i], heap[menorElemento]);
        i = menorElemento;
    }
}

void processarPacotes(Pacote **pacotes, int totalPacotes, int quantidadeAgrupamento, ofstream &outputFile) {
    int index = 0;
    int nPacoteCorreto = 0;
    int heapSize = 0;
    Pacote **heap = new Pacote *[totalPacotes]; // Heap para armazenar pacotes fora de ordem
    static string bufferSaida;
    while (index < totalPacotes || heapSize > 0) {
        string linhaSaida = "|";
        bool pacoteAdicionado = false;
        for (int i = 0; i < quantidadeAgrupamento && index < totalPacotes; i++) {
            if (pacotes[index]->nDoPacote == nPacoteCorreto) {
                for (int j = 0; j < pacotes[index]->tamDoPacote; j++) {
                    linhaSaida += pacotes[index]->dados[j];
                    if (j < pacotes[index]->tamDoPacote - 1) linhaSaida += ",";
                }
                linhaSaida += "|";
                nPacoteCorreto++;
                index++;
                pacoteAdicionado = true;
            } else {
                heap[heapSize] = pacotes[index];
                heapify_up(heap, heapSize);
                heapSize++;
                index++;
            }
        }

        while (heapSize > 0 && heap[0]->nDoPacote == nPacoteCorreto) {
            for (int j = 0; j < heap[0]->tamDoPacote; j++) {
                linhaSaida += heap[0]->dados[j];
                if (j < heap[0]->tamDoPacote - 1) linhaSaida += ",";
            }
            linhaSaida += "|";
            nPacoteCorreto++;

            heap[0] = heap[heapSize - 1];
            heapSize--;
            heapify_down(heap, heapSize);
            pacoteAdicionado = true;
        }

        if (pacoteAdicionado) {
            bufferSaida += linhaSaida + "\n";
            if (bufferSaida.size() > 100000) {
                outputFile << bufferSaida;
                bufferSaida.clear();
            }
        }
    }
    outputFile << bufferSaida;
    delete[] heap;
}

int main(int argc, char* argv[]) {
    //ifstream inputFile("input.txt");
    //ofstream outputFile("output.txt");
    ifstream inputFile( argv[1] );
    ofstream outputFile(argv[2]); 
    if (!inputFile || !outputFile) {
        cerr << "Erro ao abrir o arquivo!" << endl;
        return 1;
    }
    
    int totalPacotes, quantidadeAgrupamento;
    inputFile >> totalPacotes >> quantidadeAgrupamento;

    Pacote **pacotes = new Pacote *[totalPacotes];
    for (int i = 0; i < totalPacotes; i++) {
        pacotes[i] = new Pacote;
        inputFile >> pacotes[i]->nDoPacote >> pacotes[i]->tamDoPacote;
        for (int j = 0; j < pacotes[i]->tamDoPacote; j++) {
            inputFile >> pacotes[i]->dados[j];
        }
    }
    processarPacotes(pacotes, totalPacotes, quantidadeAgrupamento, outputFile);
    for (int i = 0; i < totalPacotes; i++) {
        delete pacotes[i];
    }
    delete[] pacotes;

    inputFile.close();
    outputFile.close();
    return 0;
}
