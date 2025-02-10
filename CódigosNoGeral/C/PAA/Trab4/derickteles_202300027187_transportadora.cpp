#include <iostream>
#include <fstream>
#include <iomanip>
#include <string>
#include <cstdlib>

using namespace std;

#define MAX_PACOTES 10000
#define MAX_VEICULOS 1000

// Cada veículo possui placa, capacidade máxima de peso e volume.
struct Veiculo {
    string placa;
    int peso_max;
    int volume_max;
};

// Cada pacote possui um código, um valor (double) e requisitos de peso e volume.
struct Pacote {
    string codigo;
    double valor;
    int peso;
    int volume;
};

Veiculo* veiculos;    // array de veículos (alocado exatamente com n veículos)
Pacote* pacotes;      // array de pacotes (alocado exatamente com m pacotes)
bool* pacotesCarregados; // para cada pacote, indica se ele já foi carregado por algum veículo
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
    
    // Inicializa o array de pacotes carregados (inicialmente nenhum pacote foi carregado).
    pacotesCarregados = new bool[m];
    for (int i = 0; i < m; i++) {
        pacotesCarregados[i] = false;
    }
}

// Processa o veículo de índice "idx" – resolve o problema da mochila 2D para ele.
// O resultado é escrito no stream "saida".
void processVehicle(int idx, ofstream &saida) {
    int W = veiculos[idx].peso_max;   // capacidade de peso
    int V = veiculos[idx].volume_max;   // capacidade de volume

    // Aloca um array "dp" unidimensional que representará uma tabela 2D de dimensão (W+1) x (V+1).
    // Usamos armazenamento contíguo para ganhar em velocidade.
    int dpSize = (W + 1) * (V + 1);
    double* dp = new double[dpSize];
    int* choice = new int[dpSize]; // para cada estado, guarda qual pacote foi adicionado para alcançá-lo

    // Inicializa todos os estados como “inatingíveis” (usaremos -1) e choice com -1.
    for (int i = 0; i < dpSize; i++) {
        dp[i] = -1.0;
        choice[i] = -1;
    }
    dp[0] = 0.0; // o estado (0,0) é atingível (nenhum pacote) com valor 0

    // Para cada pacote que ainda não foi carregado, tenta “colocá-lo” (0/1 knapsack)
    for (int j = 0; j < m; j++) {
        if (pacotesCarregados[j])
            continue; // ignora os pacotes já carregados por veículos anteriores

        int w_req = pacotes[j].peso;
        int v_req = pacotes[j].volume;
        double val = pacotes[j].valor;

        // Percorre os estados em ordem decrescente para não usar o mesmo pacote duas vezes
        for (int w = W; w >= w_req; w--) {
            for (int v = V; v >= v_req; v--) {
                int idxCurr = w * (V + 1) + v;
                int idxPrev = (w - w_req) * (V + 1) + (v - v_req);
                if (dp[idxPrev] != -1.0) {
                    double newVal = dp[idxPrev] + val;
                    if (newVal > dp[idxCurr]) {
                        dp[idxCurr] = newVal;
                        choice[idxCurr] = j;
                    }
                }
            }
        }
    }

    // Procura o estado (w,v) que não ultrapassa as capacidades e que tenha o maior valor total
    double bestVal = -1.0;
    int best_w = 0, best_v = 0;
    for (int w = 0; w <= W; w++) {
        for (int v = 0; v <= V; v++) {
            int pos = w * (V + 1) + v;
            if (dp[pos] > bestVal) {
                bestVal = dp[pos];
                best_w = w;
                best_v = v;
            }
        }
    }

    // Backtracking: recupere os pacotes escolhidos.
    // Como o backtracking “retorna” os itens em ordem inversa, os armazenamos num array e depois invertemos a ordem.
    int* selectedIndices = new int[m]; // no pior caso, pode-se escolher todos os pacotes
    int count = 0;
    int cur_w = best_w, cur_v = best_v;
    while ((cur_w > 0 || cur_v > 0) && choice[cur_w * (V + 1) + cur_v] != -1) {
        int pkg = choice[cur_w * (V + 1) + cur_v];
        selectedIndices[count++] = pkg;
        cur_w -= pacotes[pkg].peso;
        cur_v -= pacotes[pkg].volume;
    }
    
    // Invertemos a ordem para que os pacotes apareçam na mesma ordem em que foram “selecionados”
    int* finalOrder = new int[count];
    for (int i = 0; i < count; i++) {
        finalOrder[i] = selectedIndices[count - 1 - i];
    }
    
    // Calcula o total de peso e volume efetivamente carregados neste veículo (e marca os pacotes como carregados)
    int totalWeight = 0, totalVolume = 0;
    for (int i = 0; i < count; i++) {
        int pkg = finalOrder[i];
        totalWeight += pacotes[pkg].peso;
        totalVolume += pacotes[pkg].volume;
        pacotesCarregados[pkg] = true;
    }
    
    // Calcula os percentuais (arredondados)
    int percentWeight = 0, percentVolume = 0;
    if (veiculos[idx].peso_max > 0)
        percentWeight = (totalWeight * 100 + veiculos[idx].peso_max / 2) / veiculos[idx].peso_max;
    if (veiculos[idx].volume_max > 0)
        percentVolume = (totalVolume * 100 + veiculos[idx].volume_max / 2) / veiculos[idx].volume_max;
    
    // Escreve a saída para este veículo no seguinte formato:
    // [PLACA]R$<valor_total com 2 decimais>,<peso_total>KG(<percent_weight>%),<volume_total>L(<percent_volume>%)->lista_de_codigos
    saida << "[" << veiculos[idx].placa << "]";
    saida << "R$" << fixed << setprecision(2) << bestVal << ",";
    saida << totalWeight << "KG(" << percentWeight << "%),";
    saida << totalVolume << "L(" << percentVolume << "%)->";
    for (int i = 0; i < count; i++) {
        saida << pacotes[finalOrder[i]].codigo;
        if (i < count - 1)
            saida << ",";
    }
    saida << "\n";
    
    delete[] dp;
    delete[] choice;
    delete[] selectedIndices;
    delete[] finalOrder;
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
        processVehicle(i, saida);
    }
    
    // Após processar os veículos, lista os pacotes que nunca foram carregados.
    // Para cada pacote pendente, a saída terá o formato:
    // PENDENTE:R$<valor com 2 decimais>,<peso>KG,<volume>L-><codigo>
    for (int j = 0; j < m; j++) {
        if (!pacotesCarregados[j]) {
            saida << "PENDENTE:R$" << fixed << setprecision(2) << pacotes[j].valor << ",";
            saida << pacotes[j].peso << "KG," << pacotes[j].volume << "L->" << pacotes[j].codigo << "\n";
        }
    }
    
    saida.close();
}

int main() {
    // Para acelerar a I/O
    std::ios_base::sync_with_stdio(false);
    
    lerEntrada("input.txt");
    gerarSaida("output.txt");
    
    delete[] veiculos;
    delete[] pacotes;
    delete[] pacotesCarregados;
    return 0;
}
