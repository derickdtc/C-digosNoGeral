// TEMA 10 – A concessionária de pequeno porte está em ascensão em março de 2023 com
// o crescimento na venda de veículos e serviços na região de Aracaju, capital de Sergipe.

// Crie um programa com o conceito de multilista, em que há cadastrada uma lista circular
// de vendedores e cada vendedor possue suas vendas de veículos cadastradas em uma lista
// duplamente encadeada.

// Para cada vendedor deve ser guardado nome, CPF e matrícula.
// Para cada venda deve ser guardado no da nota fiscal, valor da venda, marca, ano e modelo do veículo.

// Faça no mínimo funções para inserir e remover vendedores,
// função para cadastrar nova venda, função que exiba o total de vendas cada vendedor e 
// função para exibir o vendedor que deu mais lucro a concessionária.


#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//Lista duplamente encadeada para notas fiscais 
typedef struct notaFiscal
{
    struct notaFiscal *ant;
    struct notaFiscal *prox;
    float valor;
    char marcaCarro[21];
    char modeloCarro[21];
    int anoCarro;
}tipoNota;

//Lista circular para vendedores

typedef struct vendedor{
    char nome[31];
    char cpf[15];
    char matricula[5];
    int qtdVendas;
    float totalVendas;
    struct vendedor *proxVendedor;
    tipoNota *primeiraNota;
} tipoVendedor;

//Lista cabeça

typedef struct lista{
    tipoVendedor *ultimoVendedor;
    int tamanho;
}tipoLista;

// Criação do novo vendedor e inicialização da lista de vendas
void adicionarVendedor(tipoLista *lista) {
    tipoVendedor *novoVendedor = (tipoVendedor*)malloc(sizeof(tipoVendedor));
    if(novoVendedor == NULL) return; // Erro

    printf("\n    Adicionando vendedor\n\n");

    printf("  Digite o nome do vendedor: ");
    scanf(" %30[^\n]s", novoVendedor->nome);
    
    printf("  Digite o cpf: ");
    scanf(" %14[^\n]s", novoVendedor->cpf);
    
    printf("  Digite a matricula: ");
    scanf(" %4[^\n]s", novoVendedor->matricula);

    printf("\n");

    novoVendedor->proxVendedor = NULL;
    novoVendedor->qtdVendas = 0;
    novoVendedor->totalVendas = 0.0f;
    novoVendedor->primeiraNota = NULL;

    if (lista->ultimoVendedor == NULL) // 1° alocação
    {
        lista->ultimoVendedor = novoVendedor;
        novoVendedor->proxVendedor = novoVendedor;
    }
    else // após a 1° alocação
    {
        novoVendedor->proxVendedor = lista->ultimoVendedor->proxVendedor;
        lista->ultimoVendedor->proxVendedor = novoVendedor;
        lista->ultimoVendedor = novoVendedor;
    } 
    lista->tamanho++;
}

void mostrarVendedores(tipoLista *lista) {
    // 1° vendedor da lista
    tipoVendedor *vendedor = lista->ultimoVendedor->proxVendedor;

    for (int i = 1; i <= lista->tamanho; i++)
    {
        printf(" %d - vendedor: %s - matricula: %s\n", i, vendedor->nome, vendedor->matricula);
        vendedor = vendedor->proxVendedor;
    }
    printf("\n");
}

void adicionarNotaFiscal(tipoVendedor *vendedor) {
    tipoNota *novaNota = (tipoNota*)malloc(sizeof(tipoNota));
    if (novaNota == NULL) return; // Erro
    
    printf("  Digite a marca do carro: ");
    scanf(" %20[^\n]s", novaNota->marcaCarro);
    
    printf("  Digite o modelo do carro: ");
    scanf(" %20[^\n]s", novaNota->modeloCarro);
    
    printf("  Digite o ano do carro: ");
    scanf("%d", &novaNota->anoCarro);

    printf("  Digite o valor da venda: ");
    scanf("%f", &novaNota->valor);

    novaNota->ant = NULL;
    novaNota->prox = NULL;

    // Adicionando a nova nota fiscal à lista de notas do vendedor
    if (vendedor->primeiraNota == NULL) {
        vendedor->primeiraNota = novaNota;
    } else {
        tipoNota *atual = vendedor->primeiraNota;
        while (atual->prox != NULL) {
            atual = atual->prox;
        }
        atual->prox = novaNota;
        novaNota->ant = atual;
    }

    vendedor->totalVendas += novaNota->valor;
    vendedor->qtdVendas++;
}

void adicionarVenda(tipoLista *lista) {
    mostrarVendedores(lista);

    int opcVendedor;
    printf("Digite um número: ");
    scanf("%d", &opcVendedor);
    printf("\n");

    tipoVendedor *vendedor = lista->ultimoVendedor->proxVendedor;

    for (int i = 2; i <= opcVendedor; i++)
    {
        vendedor = vendedor->proxVendedor;
    }

    printf("\n  Vendedor escolhido:\n");
    printf("  Nome: %s\n", vendedor->nome);
    printf("  Matricula: %s\n\n", vendedor->matricula);

    adicionarNotaFiscal(vendedor);
}

void exibirTotalVendasPorVendedor(tipoLista *lista) {
    tipoVendedor *vendedor = lista->ultimoVendedor->proxVendedor;

    for (int i = 1; i <= lista->tamanho; i++)
    {
        printf("Vendedor: %s - Total de Vendas: %.2f\n", vendedor->nome, vendedor->totalVendas);
        vendedor = vendedor->proxVendedor;
    }
}

void exibirVendedorComMaisLucro(tipoLista *lista) {
    if (lista->tamanho == 0) return;

    tipoVendedor *vendedor = lista->ultimoVendedor->proxVendedor;
    tipoVendedor *vendedorMaisLucro = vendedor;

    for (int i = 1; i <= lista->tamanho; i++)
    {
        if (vendedor->totalVendas > vendedorMaisLucro->totalVendas) {
            vendedorMaisLucro = vendedor;
        }
        vendedor = vendedor->proxVendedor;
    }

    printf("\nVendedor com mais lucro:\n");
    printf("Nome: %s - Total de Vendas: %.2f\n", vendedorMaisLucro->nome, vendedorMaisLucro->totalVendas);
}

void removerVendedor(tipoLista *lista) {
    if (lista->ultimoVendedor == NULL) {
        printf("A lista de vendedores está vazia!\n");
        return;
    }

    mostrarVendedores(lista);

    int opcVendedor;
    printf("Digite o número do vendedor a ser removido: ");
    scanf("%d", &opcVendedor);
    printf("\n");

    tipoVendedor *vendedor = lista->ultimoVendedor->proxVendedor;
    tipoVendedor *anterior = lista->ultimoVendedor;

    for (int i = 1; i < opcVendedor; i++) {
        anterior = vendedor;
        vendedor = vendedor->proxVendedor;
    }

    // Ajustando os ponteiros dos vendedores, mantendo a circularidade
    if (vendedor == anterior) { // Caso haja apenas um vendedor na lista
        lista->ultimoVendedor = NULL;
    } else {
        anterior->proxVendedor = vendedor->proxVendedor;
        if (vendedor == lista->ultimoVendedor) { // Se for o último vendedor
            lista->ultimoVendedor = anterior;
        }
    }

    // Liberando as vendas associadas ao vendedor
    tipoNota *notaAtual = vendedor->primeiraNota;
    while (notaAtual != NULL) {
        tipoNota *proximaNota = notaAtual->prox;
        free(notaAtual);
        notaAtual = proximaNota;
    }

    // Liberando a memória do vendedor
    free(vendedor);
    lista->tamanho--;

    printf("Vendedor removido com sucesso!\n");
}


void menu() {
    printf("\n**********************************\n");
    printf("*        Concessionária UFS      *\n");
    printf("*   1 - Adicionar vendedor       *\n");
    printf("*   2 - Adicionar venda          *\n");
    printf("*   3 - Exibir total vendas      *\n");
    printf("*   4 - Vendedor com mais lucro  *\n");
    printf("*   5 - Remover vendedor         *\n");
    printf("*   0 - Sair                     *\n");
    printf("**********************************\n");

    printf("\nEscolha uma opção: ");
}

// Inicializa a lista cabeça
void inicializarLista(tipoLista *lista) {
    lista->ultimoVendedor = NULL;
    lista->tamanho = 0;
}

int main()
{
    tipoLista listaVendedores;
    inicializarLista(&listaVendedores);

    int opcMenu;
    
    do
    {
        menu();
        scanf("%d", &opcMenu);
        printf("\n");

        if ((&listaVendedores)->tamanho == 0 && opcMenu > 1){
            printf("    Lista sem Vendedores!\n");
        } else {
            switch (opcMenu) 
            {
            case 1:
                adicionarVendedor(&listaVendedores);
                break;
            
            case 2:
                adicionarVenda(&listaVendedores);
                break;

            case 3:
                exibirTotalVendasPorVendedor(&listaVendedores);
                break;

            case 4:
                exibirVendedorComMaisLucro(&listaVendedores);
                break;

            case 5:
                removerVendedor(&listaVendedores);
                break;

            default:
                break;
            }
        }

    } while (opcMenu != 0);

    return 0;
}