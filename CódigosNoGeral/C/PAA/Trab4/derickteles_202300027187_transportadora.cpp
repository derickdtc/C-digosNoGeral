#include <iostream>
#include <fstream>
#include <iomanip>
#include <string>
#include <cstdlib>

using namespace std;

#define MAX_PACOTES 10000
#define MAX_VEICULOS 1000

struct Veiculo {
    string placa;
    int peso_max;
    int volume_max;
};

struct Pacote {
    string codigo;
    double valor;
    int peso;
    int volume;
};

Veiculo* veiculos;    
Pacote* pacotes;      
bool* pacotesCarregados; 
int n, m;

// Lê o arquivo de entrada e aloca os arrays com exatamente o necessário.
void lerEntrada(const string &nomeArquivo) {
    ifstream entrada(nomeArquivo.c_str());
    if (!entrada) {
        cerr << "Erro ao abrir o arquivo de entrada." << endl;
        exit(1);
    }
    
    entrada >> n;
    if(n > MAX_VEICULOS) {
        cerr << "Número de veículos excede o máximo permitido." << endl;
        exit(1);
    }
    veiculos = new Veiculo[n];
    for (int i = 0; i < n; i++) {
        entrada >> veiculos[i].placa >> veiculos[i].peso_max >> veiculos[i].volume_max;
    }
    
    entrada >> m;
    pacotes = new Pacote[m];
    for (int i = 0; i < m; i++) {
        entrada >> pacotes[i].codigo >> pacotes[i].valor >> pacotes[i].peso >> pacotes[i].volume;
    }
    entrada.close();    
    pacotesCarregados = new bool[m];
    for (int i = 0; i < m; i++) {
        pacotesCarregados[i] = false;
    }
}


void processarVeiculo(int index, ofstream &saida) {
    int W = veiculos[index].peso_max;   // capacidade de peso
    int V = veiculos[index].volume_max;   // capacidade de volume

   // Primeiro, reúna os índices dos pacotes que ainda não foram carregados (mantendo a ordem original)
int numPacotesDisponiveis = 0;
for (int j = 0; j < m; j++) {
    if (!pacotesCarregados[j])
        numPacotesDisponiveis++;
}
int* avail = new int[numPacotesDisponiveis];
int k = 0;
for (int j = 0; j < m; j++) {
    if (!pacotesCarregados[j])
        avail[k++] = j;
}
int N = numPacotesDisponiveis; 

// Cria a matriz 3D dp com dimensões (N+1) x (W+1) x (V+1)
int dim1 = N + 1;       
int dim2 = W + 1;       
int dim3 = V + 1;      
int totalCells = dim1 * dim2 * dim3;
double* dp = new double[totalCells];
// dec: 1 se o pacote foi incluído neste estado, 0 caso contrário.
char* dec = new char[totalCells];

// Macro para indexação: posição de (i, w, v)
#define index(i, w, v) ((i) * (dim2) * (dim3) + (w) * (dim3) + (v))

// Inicializa a camada i=0 (nenhum pacote considerado) com 0 e dec com 0.
for (int i = 0; i < totalCells; i++) {
    dp[i] = 0.0;
    dec[i] = 0;
}

// Preenche a tabela dp com a recorrência clássica 0/1
// Para i de 1 a N (cada pacote disponível na ordem original)
for (int i = 1; i <= N; i++) {
    int pkgIndex = avail[i - 1];  // índice do pacote no array original
    int pesoDoPacote = pacotes[pkgIndex].peso;
    int volumeDoPacote = pacotes[pkgIndex].volume;
    double valorDoPacote = pacotes[pkgIndex].valor;
    for (int w = 0; w <= W; w++) {
        for (int v = 0; v <= V; v++) {
            // Não incluir o pacote: valor do estado anterior
            double notTake = dp[index(i - 1, w, v)];
            double take = -1.0;
            if (w >= pesoDoPacote && v >= volumeDoPacote) {
                take = dp[index(i - 1, w - pesoDoPacote, v - volumeDoPacote)] + valorDoPacote;
            }
            // Modificação: se take for maior ou igual, forçamos a inclusão
            if (take >= notTake) {
                dp[index(i, w, v)] = take;
                dec[index(i, w, v)] = 1;  // indica que o pacote foi incluído
            } else {
                dp[index(i, w, v)] = notTake;
                dec[index(i, w, v)] = 0;
            }
        }
    }
}

// O melhor valor total obtido está em dp[N][W][V]
double bestVal = dp[index(N, W, V)];

// Reconstrói a solução a partir da tabela dec, de i = N até 1
int currentW = W, currentV = V;
int* chosen = new int[N]; // pode escolher no máximo N pacotes
int chosenCount = 0;
for (int i = N; i >= 1; i--) {
    if (dec[index(i, currentW, currentV)] == 1) {
        int origIndex = avail[i - 1];  
        chosen[chosenCount++] = origIndex;
        currentW -= pacotes[origIndex].peso;
        currentV -= pacotes[origIndex].volume;
    }
    
}
// Inverte a ordem dos itens para que fique na ordem de inclusão original
for (int i = 0; i < chosenCount / 2; i++) {
    int temp = chosen[i];
    chosen[i] = chosen[chosenCount - 1 - i];
    chosen[chosenCount - 1 - i] = temp;
}

// Calcula os totais de peso e volume, e marca os pacotes escolhidos como carregados
int totalPeso = 0, totalVolume = 0;
for (int i = 0; i < chosenCount; i++) {
    int pkg = chosen[i];
    totalPeso += pacotes[pkg].peso;
    totalVolume += pacotes[pkg].volume;
    pacotesCarregados[pkg] = true;
}

// Calcula os percentuais conforme a lógica original (com arredondamento)
int percentPeso = (veiculos[index].peso_max > 0 ? (totalPeso * 100 + veiculos[index].peso_max / 2) / veiculos[index].peso_max : 0);
int percentVolume = (veiculos[index].volume_max > 0 ? (totalVolume * 100 + veiculos[index].volume_max / 2) / veiculos[index].volume_max : 0);

saida << "[" << veiculos[index].placa << "]";
saida << "R$" << fixed << setprecision(2) << bestVal << ",";
saida << totalPeso << "KG(" << percentPeso << "%),";
saida << totalVolume << "L(" << percentVolume << "%)->";
for (int i = 0; i < chosenCount; i++) {
    saida << pacotes[chosen[i]].codigo;
    if (i < chosenCount - 1)
        saida << ",";
}
saida << "\n";

// Libera a memória utilizada
delete[] avail;
delete[] dp;
delete[] dec;
delete[] chosen;
#undef index


}

// Processa todos os veículos e depois escreve os pacotes pendentes.
void gerarSaida(const string &nomeArquivo) {
    ofstream saida(nomeArquivo.c_str());
    if (!saida) {
        cerr << "Erro ao abrir o arquivo de saída." << endl;
        exit(1);
    }
    
    // Processa cada veículo (na ordem de entrada)
    for (int i = 0; i < n; i++) {
        processarVeiculo(i, saida);
    }
    
    // Após processar os veículos, lista os pacotes que nunca foram carregados.    
double totalValorPendente = 0.0;
int totalPesoPendente = 0;
int totalVolumePendente = 0;
int countPendente = 0;
for (int j = 0; j < m; j++) {
    if (!pacotesCarregados[j]) {
        totalValorPendente += pacotes[j].valor;
        totalPesoPendente += pacotes[j].peso;
        totalVolumePendente += pacotes[j].volume;
        countPendente++;
    }
}
if (countPendente > 0) {
    saida << "PENDENTE:";
    saida << "R$" << fixed << setprecision(2) << totalValorPendente << ",";
    saida << totalPesoPendente << "KG," << totalVolumePendente << "L->";
    int printed = 0;
    for (int j = 0; j < m; j++) {
        if (!pacotesCarregados[j]) {
            printed++;
            saida << pacotes[j].codigo;
            if (printed < countPendente)
                saida << ",";
        }
    }
    saida << "\n";
}
    
    saida.close();
}

int main(int argc, char* argv[]) {
    // Para acelerar a I/O
    std::ios_base::sync_with_stdio(false);
    
    ifstream inputFile;
    ofstream outputFile;
    
    if (argc < 3) {
        lerEntrada("inputDoServidor.txt");
        gerarSaida("transportadoraMeuOutput.txt");
    } else {
        lerEntrada(argv[1]);
        gerarSaida(argv[2]);
    } 
    
    
    delete[] veiculos;
    delete[] pacotes;
    delete[] pacotesCarregados;
    return 0;
}