#include <iostream>
#include <fstream>
#include <string>

#define MAX_PESO 50000
#define MAX_VOLUME 100000
#define MAX_PACOTES 10000
#define MAX_VEICULOS 1000

using namespace std;

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
int n, m;

double*** dp;

void alocarMemoria() {
    veiculos = new Veiculo[MAX_VEICULOS];
    pacotes = new Pacote[MAX_PACOTES];
    dp = new double**[MAX_PACOTES + 1];
    for (int i = 0; i <= MAX_PACOTES; i++) {
        dp[i] = new double*[MAX_PESO + 1];
        for (int j = 0; j <= MAX_PESO; j++) {
            dp[i][j] = new double[MAX_VOLUME + 1];
        }
    }
}

void liberarMemoria() {
    for (int i = 0; i <= MAX_PACOTES; i++) {
        for (int j = 0; j <= MAX_PESO; j++) {
            delete[] dp[i][j];
        }
        delete[] dp[i];
    }
    delete[] dp;
    delete[] veiculos;
    delete[] pacotes;
}

void lerEntrada(const string& nomeArquivo) {
    ifstream entrada(nomeArquivo);
    entrada >> n;
    for (int i = 0; i < n; i++) {
        entrada >> veiculos[i].placa >> veiculos[i].peso_max >> veiculos[i].volume_max;
    }
    entrada >> m;
    for (int i = 0; i < m; i++) {
        entrada >> pacotes[i].codigo >> pacotes[i].valor >> pacotes[i].peso >> pacotes[i].volume;
    }
    entrada.close();
}

void mochila_2d(int peso_max, int volume_max, bool* escolhidos) {
    for (int i = 0; i <= m; i++) {
        for (int w = 0; w <= peso_max; w++) {
            for (int v = 0; v <= volume_max; v++) {
                dp[i][w][v] = 0;
            }
        }
    }

    for (int i = 1; i <= m; i++) {
        for (int w = peso_max; w >= pacotes[i-1].peso; w--) {
            for (int v = volume_max; v >= pacotes[i-1].volume; v--) {
                dp[i][w][v] = max(dp[i-1][w][v], dp[i-1][w - pacotes[i-1].peso][v - pacotes[i-1].volume] + pacotes[i-1].valor);
            }
        }
    }

    int w = peso_max, v = volume_max;
    for (int i = m; i > 0; i--) {
        if (dp[i][w][v] != dp[i-1][w][v]) {
            escolhidos[i-1] = true;
            w -= pacotes[i-1].peso;
            v -= pacotes[i-1].volume;
        }
    }
}

void gerarSaida(const string& nomeArquivo) {
    ofstream saida(nomeArquivo);
    bool* pacotesCarregados = new bool[MAX_PACOTES]();

    for (int i = 0; i < n; i++) {
        bool* escolhidos = new bool[MAX_PACOTES]();
        mochila_2d(veiculos[i].peso_max, veiculos[i].volume_max, escolhidos);

        double valorTotal = 0;
        int pesoTotal = 0, volumeTotal = 0;
        string pacotesEscolhidos = "";

        for (int j = 0; j < m; j++) {
            if (escolhidos[j]) {
                pacotesCarregados[j] = true;
                valorTotal += pacotes[j].valor;
                pesoTotal += pacotes[j].peso;
                volumeTotal += pacotes[j].volume;
                pacotesEscolhidos += (pacotesEscolhidos.empty() ? "" : ",") + pacotes[j].codigo;
            }
        }

        saida << "[" << veiculos[i].placa << "]R$" << valorTotal << "," << pesoTotal << "KG," << volumeTotal << "L -> " << pacotesEscolhidos << "\n";
        delete[] escolhidos;
    }

    for (int i = 0; i < m; i++) {
        if (!pacotesCarregados[i]) {
            saida << "PENDENTE:R$" << pacotes[i].valor << "," << pacotes[i].peso << "KG," << pacotes[i].volume << "L -> " << pacotes[i].codigo << "\n";
        }
    }

    delete[] pacotesCarregados;
    saida.close();
}

int main() {
    alocarMemoria();
    lerEntrada("input.txt");
    gerarSaida("output.txt");
    liberarMemoria();
    return 0;
}
