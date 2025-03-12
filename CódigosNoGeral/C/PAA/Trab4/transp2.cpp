#include <iostream>
#include <sstream>
#include <vector>
#include <string>
#include <fstream>
#include <iomanip>
#include <algorithm>
using namespace std;

struct Veiculo {
    string placa;
    int maxPeso;
    int maxVolume;
    int idx;
};

struct Pacote {
    string codigo;
    double valor;
    int peso;
    int volume;
    bool usado = false;
};

struct Cell {
    double value;
    int pacoteIdx, prevW, prevV;
    Cell() : value(-1), pacoteIdx(-1), prevW(-1), prevV(-1) {}
};

struct Res {
    string placa;
    double totalValor;
    int totalPeso;
    int totalVolume;
    vector<string> codigos;
};

int main(int argc , char* argv[]) {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    ifstream inputFile;
    ofstream outputFile;
    
    if(argc < 3){
        inputFile.open("inputDoServidor.txt");
        outputFile.open("transportadoraMeuOutput.txt");
    } else {
        inputFile.open(argv[1]);
        outputFile.open(argv[2]);
    }
    int n;
    cin >> n;
    vector<Veiculo> veiculos(n);
    for (int i = 0; i < n; i++){
        cin >> veiculos[i].placa >> veiculos[i].maxPeso >> veiculos[i].maxVolume;
        veiculos[i].idx = i;
    }
    
    int m;
    cin >> m;
    vector<Pacote> pacotes(m);
    for (int i = 0; i < m; i++){
        cin >> pacotes[i].codigo >> pacotes[i].valor >> pacotes[i].peso >> pacotes[i].volume;
    }
    
    vector<Res> resultados(n);
    
    // Para cada veículo, resolvemos o problema da mochila 0/1 com duas restrições.
    // Nesta versão, dp[w][vol] representa o valor máximo atingido usando exatamente w KG e vol L.
    // Iniciamos dp[0][0] = 0 e os demais com -1 (inacessíveis).
    for (int v = 0; v < n; v++){
        int capPeso = veiculos[v].maxPeso;
        int capVolume = veiculos[v].maxVolume;
        vector<vector<Cell>> dp(capPeso + 1, vector<Cell>(capVolume + 1));
        dp[0][0].value = 0; // estado inicial
        
        // Itera sobre todos os pacotes disponíveis para este veículo
        for (int i = 0; i < m; i++){
            if (pacotes[i].usado) continue;
            if (pacotes[i].peso > capPeso || pacotes[i].volume > capVolume) continue;
            // Percorre os estados atuais (representados pela soma de peso e volume usados)
            // em ordem decrescente para evitar reutilização do mesmo pacote.
            for (int w = capPeso - pacotes[i].peso; w >= 0; w--){
                for (int vol = capVolume - pacotes[i].volume; vol >= 0; vol--){
                    if (dp[w][vol].value != -1) {
                        int newW = w + pacotes[i].peso;
                        int newVol = vol + pacotes[i].volume;
                        double newVal = dp[w][vol].value + pacotes[i].valor;
                        if (newVal > dp[newW][newVol].value) {
                            dp[newW][newVol].value = newVal;
                            dp[newW][newVol].pacoteIdx = i;
                            dp[newW][newVol].prevW = w;
                            dp[newW][newVol].prevV = vol;
                        }
                    }
                }
            }
        }
        
        // Procura o estado que maximize o valor (pode ser que o ótimo não ocupe toda a capacidade)
        double melhorVal = 0;
        int melhorW = 0, melhorV = 0;
        for (int w = 0; w <= capPeso; w++){
            for (int vol = 0; vol <= capVolume; vol++){
                if (dp[w][vol].value > melhorVal) {
                    melhorVal = dp[w][vol].value;
                    melhorW = w;
                    melhorV = vol;
                }
            }
        }
        
        // Reconstrói a solução a partir da tabela dp
        vector<int> selecionados;
        int curW = melhorW, curV = melhorV;
        while(curW != 0 || curV != 0){
            int idxPac = dp[curW][curV].pacoteIdx;
            if (idxPac == -1) break;
            selecionados.push_back(idxPac);
            int antW = dp[curW][curV].prevW;
            int antV = dp[curW][curV].prevV;
            curW = antW;
            curV = antV;
        }
        reverse(selecionados.begin(), selecionados.end());
        
        // Marca os pacotes alocados e acumula os totais para o veículo
        double somaValor = 0.0;
        int somaPeso = 0, somaVolume = 0;
        vector<string> codigos;
        // (Como cada pacote é usado no máximo uma vez, não há risco de duplicação.)
        for (int idx : selecionados){
            pacotes[idx].usado = true;
            somaValor += pacotes[idx].valor;
            somaPeso += pacotes[idx].peso;
            somaVolume += pacotes[idx].volume;
            codigos.push_back(pacotes[idx].codigo);
        }
        
        resultados[v] = { veiculos[v].placa, somaValor, somaPeso, somaVolume, codigos };
    }
    
    // Imprime os resultados para cada veículo na ordem de entrada.
    for (int i = 0; i < n; i++){
        const Res &r = resultados[i];
        cout << "[" << r.placa << "]";
        cout << "R$" << fixed << setprecision(2) << r.totalValor << ",";
        cout << r.totalPeso << "KG(";
        int percPeso = (r.totalPeso * 100) / veiculos[i].maxPeso;
        cout << percPeso << "%),";
        cout << r.totalVolume << "L(";
        int percVolume = (r.totalVolume * 100) / veiculos[i].maxVolume;
        cout << percVolume << "%)->";
        for (size_t j = 0; j < r.codigos.size(); j++){
            cout << r.codigos[j];
            if(j+1 < r.codigos.size())
                cout << ",";
        }
        cout << "\n";
    }
    
    // Calcula e imprime os pacotes pendentes (não alocados em nenhum veículo)
    double pendValor = 0.0;
    int pendPeso = 0, pendVolume = 0;
    vector<string> pendCodigos;
    for (int i = 0; i < m; i++){
        if (!pacotes[i].usado) {
            pendValor += pacotes[i].valor;
            pendPeso += pacotes[i].peso;
            pendVolume += pacotes[i].volume;
            pendCodigos.push_back(pacotes[i].codigo);
        }
    }
    if (!pendCodigos.empty()){
        cout << "PENDENTE:";
        cout << "R$" << fixed << setprecision(2) << pendValor << ",";
        cout << pendPeso << "KG,";
        cout << pendVolume << "L->";
        for (size_t i = 0; i < pendCodigos.size(); i++){
            cout << pendCodigos[i];
            if(i+1 < pendCodigos.size())
                cout << ",";
        }
        cout << "\n";
    }
    
    return 0;
}
