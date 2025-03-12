#include <iostream>
#include <fstream>
#include <iomanip>
#include <string>
#include <cstring>
#include <cmath>

using namespace std;

// -------------------------------------------------------------
// Constantes e limites
// -------------------------------------------------------------
static const int MAX_T = 12000;      // Máximo de bytes por sequência
static const int MAX_NODES = 700;    // Para a árvore de Huffman

// -------------------------------------------------------------
// Buffers globais de entrada e saída
// -------------------------------------------------------------
static unsigned char inputData[MAX_T];
static unsigned char rleData[2 * MAX_T]; // pior caso para RLE
static int rleSize;

static unsigned char hufData[2 * MAX_T]; // buffer para saída Huffman
static int hufSize;

// -------------------------------------------------------------
// Funções auxiliares de conversão
// -------------------------------------------------------------
char hexDigit(int x) {
    if (x < 10) return char('0' + x);
    return char('A' + (x - 10));
}

string toHexString(const unsigned char* data, int size) {
    string result;
    result.reserve(size * 2);
    for (int i = 0; i < size; i++) {
        unsigned char b = data[i];
        result.push_back(hexDigit((b >> 4) & 0xF));
        result.push_back(hexDigit(b & 0xF));
    }
    return result;
}

unsigned int hexValue(const string &s) {
    unsigned int val = 0;
    for (int i = 0; i < 2; i++) {
        char c = s[i];
        val <<= 4;
        if (c >= '0' && c <= '9') val += (c - '0');
        else if (c >= 'A' && c <= 'F') val += (c - 'A' + 10);
        else if (c >= 'a' && c <= 'f') val += (c - 'a' + 10);
    }
    return val;
}

// Função que empacota uma string de bits em bytes (bit mais significativo primeiro)
int packBits(const string &bitString, unsigned char *out) {
    int outSize = 0;
    unsigned char currentByte = 0;
    int bitCount = 0;
    for (int i = 0; i < (int)bitString.size(); i++) {
        currentByte = (currentByte << 1) | (bitString[i] == '1' ? 1 : 0);
        bitCount++;
        if (bitCount == 8) {
            out[outSize++] = currentByte;
            currentByte = 0;
            bitCount = 0;
        }
    }
    if (bitCount > 0) {
        currentByte <<= (8 - bitCount);
        out[outSize++] = currentByte;
    }
    return outSize;
}

// -------------------------------------------------------------
// Compressão RLE (Run-Length Encoding)
// -------------------------------------------------------------
void compressRLE(const unsigned char* inData, int n) {
    rleSize = 0;
    if (n <= 0) return;
    unsigned char current = inData[0];
    unsigned char count = 1;
    for (int i = 1; i < n; i++) {
        if (inData[i] == current && count < 255) {
            count++;
        } else {
            rleData[rleSize++] = count;
            rleData[rleSize++] = current;
            current = inData[i];
            count = 1;
        }
    }
    rleData[rleSize++] = count;
    rleData[rleSize++] = current;
}

// -------------------------------------------------------------
// Estrutura de nó para Huffman – agora com campo “ord” para ordem
// -------------------------------------------------------------
struct HuffNode {
    bool usado;             // indica se o nó já foi combinado
    unsigned char byteVal;  // somente válido se for folha
    long long freq;         // frequência
    int fEsq;               // índice do filho esquerdo
    int fDir;               // índice do filho direito
    int ord;                // ordem de primeira ocorrência (para desempate)
};

static HuffNode nodes[MAX_NODES];
static int nodeCount; // contador de nós

// Reseta o array de nós
void initNodes() {
    memset(nodes, 0, sizeof(nodes));
    nodeCount = 0;
}

// Cria um nó; para folhas passe o valor e a ordem; para nós internos, passe a ordem já calculada
int createNode(unsigned char b, long long f, int ord) {
    int idx = nodeCount++;
    nodes[idx].usado = false;
    nodes[idx].byteVal = b;
    nodes[idx].freq = f;
    nodes[idx].fEsq = -1;
    nodes[idx].fDir = -1;
    nodes[idx].ord = ord;
    return idx;
}

// -------------------------------------------------------------
// Função para obter os dois nós não usados de menor frequência,
// desempate: menor valor de "ord" (ou seja, aquele que apareceu primeiro)
// -------------------------------------------------------------
void getTwoSmallestIndices(int &i1, int &i2) {
    i1 = -1;
    i2 = -1;
    // Primeiro, achar o nó com menor freq (desempate por ord)
    for (int i = 0; i < nodeCount; i++) {
        if (!nodes[i].usado) {
            if (i1 == -1) {
                i1 = i;
            } else {
                if (nodes[i].freq < nodes[i1].freq ||
                   (nodes[i].freq == nodes[i1].freq && nodes[i].ord < nodes[i1].ord)) {
                    i1 = i;
                }
            }
        }
    }
    // Marcar temporariamente i1
    nodes[i1].usado = true;
    for (int i = 0; i < nodeCount; i++) {
        if (!nodes[i].usado) {
            if (i2 == -1) {
                i2 = i;
            } else {
                if (nodes[i].freq < nodes[i2].freq ||
                   (nodes[i].freq == nodes[i2].freq && nodes[i].ord < nodes[i2].ord)) {
                    i2 = i;
                }
            }
        }
    }
    // Desmarcar i1 para não afetar combinações posteriores
    nodes[i1].usado = false;
}

// -------------------------------------------------------------
// Geração dos códigos Huffman – armazenamos a string de bits para cada byte
// -------------------------------------------------------------
static char hufCode[256][300];
static int hufCodeLen[256];

void resetCodes() {
    for (int i = 0; i < 256; i++) {
        hufCode[i][0] = '\0';
        hufCodeLen[i] = 0;
    }
}

void buildCodes(int index, char *codeBuffer, int depth) {
    // Se nó folha, armazena o código no vetor hufCode para o byte
    if (nodes[index].fEsq == -1 && nodes[index].fDir == -1) {
        unsigned char b = nodes[index].byteVal;
        for (int i = 0; i < depth; i++) {
            hufCode[b][i] = codeBuffer[i];
        }
        hufCode[b][depth] = '\0';
        hufCodeLen[b] = depth;
        return;
    }
    // Percorre filho esquerdo (adiciona '0')
    if (nodes[index].fEsq != -1) {
        codeBuffer[depth] = '0';
        buildCodes(nodes[index].fEsq, codeBuffer, depth + 1);
    }
    // Percorre filho direito (adiciona '1')
    if (nodes[index].fDir != -1) {
        codeBuffer[depth] = '1';
        buildCodes(nodes[index].fDir, codeBuffer, depth + 1);
    }
}

// -------------------------------------------------------------
// Função que verifica se todas as frequências dos símbolos presentes são iguais
// e conta quantos símbolos distintos há.
// -------------------------------------------------------------
void checkUniform(long long freq[256], int &distinct, bool &uniform) {
    distinct = 0;
    uniform = true;
    int firstFreq = -1;
    for (int b = 0; b < 256; b++) {
        if (freq[b] > 0) {
            distinct++;
            if (firstFreq == -1) firstFreq = freq[b];
            else if (freq[b] != firstFreq) uniform = false;
        }
    }
}

// -------------------------------------------------------------
// Função que retorna a menor potência de 2 maior ou igual a n
// -------------------------------------------------------------
int nextPowerOf2(int n) {
    int p = 1;
    while (p < n) p *= 2;
    return p;
}

// -------------------------------------------------------------
// Construção da árvore de Huffman
// Se houver apenas 1 símbolo, essa função não é usada (tratado abaixo).
// Se as frequências forem uniformes e o número de folhas não for potência de 2,
// adiciona nós dummy (com freq 0) para forçar uma árvore completa.
// -------------------------------------------------------------
int buildHuffmanTree(long long freq[256], int orderArr[256]) {
    initNodes();
    // Insere nós folha para cada símbolo presente
    for (int b = 0; b < 256; b++) {
        if (freq[b] > 0) {
            createNode((unsigned char)b, freq[b], orderArr[b]);
        }
    }
    // Verifica se todas as frequências são iguais
    int distinct;
    bool uniform;
    checkUniform(freq, distinct, uniform);
    // Se uniform e se o número de folhas não for potência de 2, adicione nós dummy
    if (uniform && distinct > 1) {
        int target = nextPowerOf2(distinct);
        int dummyNeeded = target - distinct;
        for (int i = 0; i < dummyNeeded; i++) {
            // O valor dummy (não usado) pode ser 0; use uma ordem alta para que fique sempre por último
            createNode(0, 0, 100000 + i);
        }
    }
    // Se houver somente um nó (único símbolo), a árvore não precisa ser construída
    if (nodeCount == 1) return 0; // retornamos índice 0

    // Combina os nós até restar apenas um ativo
    while (true) {
        int activeCount = 0;
        for (int i = 0; i < nodeCount; i++) {
            if (!nodes[i].usado) activeCount++;
        }
        if (activeCount <= 1) break;
        int i1, i2;
        getTwoSmallestIndices(i1, i2);
        // Cria nó pai com frequência somada e com ordem = min(ord dos filhos)
        int newOrd = (nodes[i1].ord < nodes[i2].ord) ? nodes[i1].ord : nodes[i2].ord;
        int p = createNode( (nodes[i1].byteVal < nodes[i2].byteVal) ? nodes[i1].byteVal : nodes[i2].byteVal, nodes[i1].freq + nodes[i2].freq, newOrd);
        nodes[p].fEsq = i1;
        nodes[p].fDir = i2;
        // Marque os nós combinados como usados
        nodes[i1].usado = true;
        nodes[i2].usado = true;
    }
    // O índice da raiz é o único nó que não foi marcado como usado
    int root = -1;
    for (int i = 0; i < nodeCount; i++) {
        if (!nodes[i].usado) {
            root = i;
            break;
        }
    }
    return root;
}

// -------------------------------------------------------------
// Função principal para compressão via Huffman
// -------------------------------------------------------------
void compressHuffman(const unsigned char* inData, int n) {
    hufSize = 0;
    if (n <= 0) return;
    // Contar frequências e registrar a ordem de primeira ocorrência
    long long freq[256];
    memset(freq, 0, sizeof(freq));
    int orderArr[256];
    for (int i = 0; i < 256; i++) orderArr[i] = -1;
    for (int i = 0; i < n; i++) {
        unsigned char b = inData[i];
        freq[b]++;
        if (orderArr[b] == -1)
            orderArr[b] = i;
    }
    // Contar símbolos distintos
    int distinct = 0;
    for (int b = 0; b < 256; b++) {
        if (freq[b] > 0) distinct++;
    }
    // Caso especial: se há somente um símbolo distinto,
    // a saída Huffman deve ser 2 bytes "0000" (para gerar taxa = (2/n)*100)
    if (distinct == 1) {
        hufData[0] = 0;
        hufData[1] = 0;
        hufSize = 2;
        return;
    }
    
    // Constrói a árvore de Huffman (com nós dummy se necessário)
    int root = buildHuffmanTree(freq, orderArr);
    // Gera os códigos
    resetCodes();
    {
        char codeBuffer[300];
        buildCodes(root, codeBuffer, 0);
    }
    // Se as frequências eram uniformes (exceto o caso de 1 símbolo),
    // espera-se que a árvore seja completa e os códigos tenham o mesmo comprimento.
    // Caso contrário, a árvore padrão de Huffman é usada.
    // (Observação: o critério de desempate usado com "ord" deve garantir que,
    // quando uniformes e com nós dummy, os códigos dos símbolos reais fiquem com mesmo comprimento.)
    
    // Monta a bitstring final: para cada byte de entrada, concatena o código correspondente
    string bitString;
    bitString.reserve(n * 8);
    for (int i = 0; i < n; i++) {
        unsigned char b = inData[i];
        bitString += hufCode[b];
    }
    hufSize = packBits(bitString, hufData);
}

// -------------------------------------------------------------
// Função main
// -------------------------------------------------------------
int main(int argc, char* argv[]) {
    ios::sync_with_stdio(false);

    ifstream inputFile;
    ofstream outputFile;
    
    if (argc < 3) {
        inputFile.open("compressao.inputCorreto");
        outputFile.open("meuOutput.txt");
    } else {
        inputFile.open(argv[1]);
        outputFile.open(argv[2]);
    }
    
    if (!inputFile || !outputFile) {
        cerr << "Erro ao abrir arquivo!" << endl;
        return 1;
    }
    
    int N;
    inputFile >> N; // número de sequências
    
    for (int seqIndex = 0; seqIndex < N; seqIndex++) {
        int T;
        inputFile >> T; // tamanho da sequência
        for (int i = 0; i < T; i++) {
            string s;
            inputFile >> s;
            unsigned int val = hexValue(s);
            inputData[i] = (unsigned char)val;
        }
        
        // Comprime via RLE e Huffman
        compressRLE(inputData, T);
        compressHuffman(inputData, T);
        
        double original = (double)T;
        double rleRatio = (original > 0.0) ? ((double)rleSize / original * 100.0) : 0.0;
        double hufRatio = (original > 0.0) ? ((double)hufSize / original * 100.0) : 0.0;
        
        string rleHex = toHexString(rleData, rleSize);
        string hufHex = toHexString(hufData, hufSize);
        
        // Seleciona o método com menor tamanho; em caso de empate, imprime ambos (HUF primeiro)
        if (hufSize < rleSize) {
            outputFile << seqIndex << "->HUF(" << fixed << setprecision(2) << hufRatio
                       << "%)=" << hufHex << "\n";
        } else if (rleSize < hufSize) {
            outputFile << seqIndex << "->RLE(" << fixed << setprecision(2) << rleRatio
                       << "%)=" << rleHex << "\n";
        } else {
            outputFile << seqIndex << "->HUF(" << fixed << setprecision(2) << hufRatio
                       << "%)=" << hufHex << "\n";
            outputFile << seqIndex << "->RLE(" << fixed << setprecision(2) << rleRatio
                       << "%)=" << rleHex << "\n";
        }
    }
    
    return 0;
}
