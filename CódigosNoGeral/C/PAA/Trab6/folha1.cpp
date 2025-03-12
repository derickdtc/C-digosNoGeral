#include <iostream>
#include <fstream>
#include <iomanip>
#include <string>
#include <cstring> // para memset etc.

using namespace std;

// ============================================================
//                    CONSTANTES / LIMITES
// ============================================================

// Máximo de bytes na entrada de uma sequência
static const int MAX_T = 10000;

// Para Huffman, no máximo 256 nós folha + nós internos (até 511)
static const int MAX_NODES = 600; // margem de segurança

// ============================================================
//                    VARIÁVEIS GLOBAIS AUXILIARES
// ============================================================

// Vetor estático para ler os bytes (até 10k)
static unsigned char inputData[MAX_T];

// Resultado comprimido por RLE
static unsigned char rleData[2 * MAX_T]; // no pior caso, 2*T
static int rleSize;                      // tamanho efetivo do RLE

// Resultado comprimido por Huffman
static unsigned char hufData[2 * MAX_T]; // reserva (em geral menor)
static int hufSize;                      // tamanho efetivo do Huffman

// ============================================================
//                       FUNÇÕES AUXILIARES
// ============================================================

// Converte um valor de 0..15 para dígito hexadecimal (0..9, A..F)
char hexDigit(int x) {
    if (x < 10) return char('0' + x);
    return char('A' + (x - 10));
}

// Converte um array de bytes (data[0..size-1]) para string em hexa
// sem espaços, tudo em maiúsculo.
string toHexString(const unsigned char* data, int size) {
    string result;
    // Reservamos 2*size caracteres
    result.reserve(size * 2);
    for (int i = 0; i < size; i++) {
        unsigned char b = data[i];
        // Parte alta (4 bits)
        result.push_back(hexDigit((b >> 4) & 0xF));
        // Parte baixa (4 bits)
        result.push_back(hexDigit(b & 0xF));
    }
    return result;
}

// Converte string "AA" (hex) em valor unsigned int (0xAA)
unsigned int hexValue(const string &s) {
    // Supõe que s tem tamanho 2 e caracteres [0-9A-Fa-f]
    // (Tratamento de erro omitido para simplicidade)
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

// ============================================================
//                     COMPRESSÃO RLE
// ============================================================

void compressRLE(const unsigned char* inData, int n) {
    // rleData[] é global
    rleSize = 0;
    if (n <= 0) return;

    unsigned char current = inData[0];
    unsigned char count = 1;

    for (int i = 1; i < n; i++) {
        if (inData[i] == current && count < 255) {
            count++;
        } else {
            // grava (count, current)
            rleData[rleSize++] = count;
            rleData[rleSize++] = current;
            current = inData[i];
            count = 1;
        }
    }
    // Último par
    rleData[rleSize++] = count;
    rleData[rleSize++] = current;
}

// ============================================================
//                     COMPRESSÃO HUFFMAN
// ============================================================

// Estrutura de nó para Huffman
struct HuffNode {
    bool used;            // se já foi combinado num nó pai
    unsigned char byteVal;// válido se for folha, ou o menor byte dos filhos se for nó interno
    long long freq;       // frequência
    int left;             // índice do filho esquerdo
    int right;            // índice do filho direito
};

// Array estático de nós
static HuffNode nodes[MAX_NODES];
static int nodeCount; // quantos nós foram criados

// Reseta o array de nós para começar do zero
void initNodes() {
    memset(nodes, 0, sizeof(nodes));
    nodeCount = 0;
}

// Cria um nó e retorna seu índice
int createNode(unsigned char b, long long f) {
    int idx = nodeCount++;
    nodes[idx].used = false;
    nodes[idx].byteVal = b;
    nodes[idx].freq = f;
    nodes[idx].left = -1;
    nodes[idx].right = -1;
    return idx;
}

// Acha os dois menores nós (por freq) que ainda não foram usados.
// Em caso de empate de freq, escolhe o que tiver menor byteVal.
// Retorna índices (i1, i2) diferentes.
void getTwoSmallestIndices(int &i1, int &i2) {
    i1 = -1;
    i2 = -1;

    // Primeira passada para achar i1
    for (int i = 0; i < nodeCount; i++) {
        if (!nodes[i].used) {
            if (i1 == -1) {
                i1 = i;
            } else {
                // comparamos freq
                if (nodes[i].freq < nodes[i1].freq) {
                    i1 = i;
                } else if (nodes[i].freq == nodes[i1].freq) {
                    // desempate por byteVal
                    if (nodes[i].byteVal < nodes[i1].byteVal) {
                        i1 = i;
                    }
                }
            }
        }
    }

    // Marca i1 como usado temporariamente para não ser escolhido de novo
    nodes[i1].used = true;

    // Segunda passada para achar i2
    for (int i = 0; i < nodeCount; i++) {
        if (!nodes[i].used) {
            if (i2 == -1) {
                i2 = i;
            } else {
                if (nodes[i].freq < nodes[i2].freq) {
                    i2 = i;
                } else if (nodes[i].freq == nodes[i2].freq) {
                    if (nodes[i].byteVal < nodes[i2].byteVal) {
                        i2 = i;
                    }
                }
            }
        }
    }

    // Desmarca i1 (vai ser combinado, mas só após sabermos i2)
    // e marcaremos os dois como usados no chamador
    nodes[i1].used = false;
}

// Vamos armazenar o código de cada byte (0..255) em forma de string binária
static char hufCode[256][300]; // cada byte pode ter um código de até ~256 bits em casos extremos
static int hufCodeLen[256];    // tamanho real de cada código

// Limpa as strings de código
void resetCodes() {
    for (int i = 0; i < 256; i++) {
        hufCode[i][0] = '\0';
        hufCodeLen[i] = 0;
    }
}

// Função recursiva para gerar os códigos a partir da raiz da árvore
// Parâmetros:
//   idx = índice do nó atual
//   codeBuffer = string de bits acumulados
//   depth = quantos bits já temos
void buildCodes(int idx, char *codeBuffer, int depth) {
    // Se nó folha (sem filhos), grava o codeBuffer em hufCode do byte correspondente
    if (nodes[idx].left == -1 && nodes[idx].right == -1) {
        // Esse nó representa o byteVal
        unsigned char b = nodes[idx].byteVal;
        // Copia codeBuffer para hufCode[b]
        for (int i = 0; i < depth; i++) {
            hufCode[b][i] = codeBuffer[i];
        }
        hufCode[b][depth] = '\0';
        hufCodeLen[b] = depth;
        return;
    }

    // Se tem filho esquerdo
    if (nodes[idx].left != -1) {
        codeBuffer[depth] = '0'; // adicionar bit '0'
        buildCodes(nodes[idx].left, codeBuffer, depth + 1);
    }
    // Se tem filho direito
    if (nodes[idx].right != -1) {
        codeBuffer[depth] = '1'; // adicionar bit '1'
        buildCodes(nodes[idx].right, codeBuffer, depth + 1);
    }
}

// Constrói a árvore de Huffman no array `nodes[]` e gera `hufCode[]`
// Retorna o índice da raiz da árvore
int buildHuffmanTree(long long freq[256]) {
    initNodes();

    // Cria nó para cada byte que tenha freq > 0
    int leaves = 0;
    for (int b = 0; b < 256; b++) {
        if (freq[b] > 0) {
            int idx = createNode((unsigned char)b, freq[b]);
            leaves++;
        }
    }

    // Caso especial: se só houver 1 símbolo distinto, cria nó dummy
    if (leaves == 1) {
        // Acha o índice do único nó
        int iOnly = 0;
        while (iOnly < nodeCount && nodes[iOnly].freq == 0) iOnly++;
        // Cria um nó dummy com freq 0
        int iDummy = createNode((unsigned char)0, 0);
        // Agora combinamos manualmente
        int parent = createNode(nodes[iOnly].byteVal, nodes[iOnly].freq + nodes[iDummy].freq);
        nodes[parent].left = iOnly;
        nodes[parent].right = iDummy;
        nodes[iOnly].used = true;
        nodes[iDummy].used = true;
        return parent; // raiz
    }

    // Enquanto houver mais de 1 nó não usado, combinamos
    while (true) {
        // Conta quantos nós não usados existem
        int activeCount = 0;
        for (int i = 0; i < nodeCount; i++) {
            if (!nodes[i].used) activeCount++;
        }
        if (activeCount <= 1) {
            // acabou, temos só um nó, que é a raiz
            break;
        }

        // Acha os dois menores
        int i1, i2;
        getTwoSmallestIndices(i1, i2);

        // Cria novo nó pai
        // byteVal = min dos dois
        unsigned char newByte = (nodes[i1].byteVal < nodes[i2].byteVal) ?
                                 nodes[i1].byteVal : nodes[i2].byteVal;
        long long newFreq = nodes[i1].freq + nodes[i2].freq;

        int p = createNode(newByte, newFreq);
        nodes[p].left = i1;
        nodes[p].right = i2;

        // marca i1 e i2 como usados
        nodes[i1].used = true;
        nodes[i2].used = true;
    }

    // índice da raiz é o último nó criado que não foi "usado"
    int root = -1;
    for (int i = 0; i < nodeCount; i++) {
        if (!nodes[i].used) {
            root = i;
            break;
        }
    }
    return root;
}

// Empacota a string de bits (por exemplo "010110...") em bytes.
// Bits são inseridos do mais significativo para o menos significativo em cada byte.
// Exemplo: se os bits forem [0,1,0,1,1,0,...] => o primeiro bit é guardado em
// posição mais alta do byte.
int packBits(const string &bitString, unsigned char *out) {
    int outSize = 0;
    unsigned char currentByte = 0;
    int bitCount = 0; // quantos bits já colocamos no currentByte

    for (int i = 0; i < (int)bitString.size(); i++) {
        currentByte <<= 1;            // desloca para a esquerda
        if (bitString[i] == '1') {
            currentByte |= 1;         // seta bit
        }
        bitCount++;
        if (bitCount == 8) {
            // completamos 1 byte
            out[outSize++] = currentByte;
            currentByte = 0;
            bitCount = 0;
        }
    }
    // Se sobrou algo
    if (bitCount > 0) {
        // Preenche com zeros à esquerda
        currentByte <<= (8 - bitCount);
        out[outSize++] = currentByte;
    }
    return outSize;
}

// Função principal para comprimir com Huffman
void compressHuffman(const unsigned char* inData, int n) {
    // Se não há dados, resultado vazio
    hufSize = 0;
    if (n <= 0) return;

    // 1) Contar frequências
    long long freq[256];
    memset(freq, 0, sizeof(freq));
    for (int i = 0; i < n; i++) {
        freq[inData[i]]++;
    }

    // 2) Construir árvore
    int root = buildHuffmanTree(freq);

    // 3) Gerar códigos
    resetCodes();
    {
        char codeBuffer[300];
        buildCodes(root, codeBuffer, 0);
    }

    // 4) Montar bitstring final
    //    Para cada byte de entrada, concatenamos o código correspondente
    //    Ex: se 'A' -> "0", 'B' -> "10", etc.
    string bitString;
    bitString.reserve(n * 8); // reserva aproximada

    for (int i = 0; i < n; i++) {
        unsigned char b = inData[i];
        // hufCode[b] é a string de bits
        bitString += hufCode[b];
    }

    // 5) Empacotar bits em bytes
    hufSize = packBits(bitString, hufData);
}

// ============================================================
//                       FUNÇÃO MAIN
// ============================================================

int main(int argc, char* argv[]) {
    ios::sync_with_stdio(false);
    //inputFile.tie(NULL);

    ifstream inputFile;
    ofstream outputFile;
    
    if (argc < 3) {
        inputFile.open("compressao.input");
        outputFile.open("output.txt");
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

        // Ler T bytes em hexa
        for (int i = 0; i < T; i++) {
            string s;
            inputFile >> s; // ex: "AA", "FF", etc.
            unsigned int val = hexValue(s);
            inputData[i] = (unsigned char)val;
        }

        // ============= RLE =============
        compressRLE(inputData, T);
        // rleSize e rleData[] foram preenchidos

        // ============= HUFFMAN =============
        compressHuffman(inputData, T);
        // hufSize e hufData[] foram preenchidos

        // ============= Taxas de Compressão =============
        // De acordo com o exemplo, a taxa = (compressed_size / original_size)*100
        // Exemplo: se original=5 e comprimido=1 => 1/5=0.2 => 20.00%
        double original = (double)T;
        double rleRatio = 0.0;
        double hufRatio = 0.0;
        if (original > 0.0) {
            rleRatio = (double)rleSize / original * 100.0;
            hufRatio = (double)hufSize / original * 100.0;
        }

        // Converte a saída comprimida para hexa (sem espaços)
        string rleHex = toHexString(rleData, rleSize);
        string hufHex = toHexString(hufData, hufSize);

        // Compara tamanhos e imprime
        if (hufSize < rleSize) {
            // Huffman menor
            outputFile << seqIndex << "->HUF(" << fixed << setprecision(2) << hufRatio
                 << "%)=" << hufHex << "\n";
        } else if (rleSize < hufSize) {
            // RLE menor
            outputFile << seqIndex << "->RLE(" << fixed << setprecision(2) << rleRatio
                 << "%)=" << rleHex << "\n";
        } else {
            // Empate
            outputFile << seqIndex << "->HUF(" << fixed << setprecision(2) << hufRatio
                 << "%)=" << hufHex << "\n";
            outputFile << seqIndex << "->RLE(" << fixed << setprecision(2) << rleRatio
                 << "%)=" << rleHex << "\n";
        }
    }

    return 0;
}
