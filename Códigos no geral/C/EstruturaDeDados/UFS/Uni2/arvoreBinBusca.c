#include <stdio.h>
#include <stdlib.h>

typedef struct node 
{
    int data;
    struct node *left;
    struct node *right;
}tipoNode;

tipoNode* newNode(int data){
    tipoNode* no = (struct node*) malloc(sizeof(tipoNode));
    no->data = data;
    no->left = NULL;
    no->right = NULL;
    return no;
}

tipoNode* insert(tipoNode* no , int data){
    if (no == NULL)
    {
        return newNode(data);
    }
    if (data < no->data)
    {
        no->left = insert(no->left , data);
    }else if(data > no->data){
        no->right = insert(no->right , data);
    }
    return no;
    
    
}

// Função auxiliar para encontrar o nó mínimo em uma árvore

tipoNode* minValueNode(tipoNode* no){
    tipoNode* atual = no;
    while (atual && atual->left != NULL)
    {
        atual=atual->left;
    }
    return atual;
}

tipoNode* deleteNode(tipoNode* root , int data){
     // Caso base: a árvore está vazia
    if (root == NULL) return root;
    // Se o valor a ser removido é menor que o valor da raiz, ele está na subárvore esquerda
    if (data < root->data) {
        root->left = deleteNode(root->left, data);
    }
     // Se o valor a ser removido é maior que o valor da raiz, ele está na subárvore direita
    else if (data > root->data) {
        root->right = deleteNode(root->right, data);
    }
     // Se o valor for o mesmo que o valor da raiz, esse é o nó a ser removido
    else{
       // Nó com um ou nenhum filho
        if (root->left == NULL) {
            struct node* temp = root->right;
            free(root);
            return temp;
        } else if (root->right == NULL) {
            struct Node* temp = root->left;
            free(root);
            return temp;
        }

        // Nó com dois filhos: pegar o sucessor in-ordem (menor valor da subárvore direita)
        struct node* temp = minValueNode(root->right);

        // Copiar o valor do sucessor in-ordem para este nó
        root->data = temp->data;

        // Remover o sucessor in-ordem
        root->right = deleteNode(root->right, temp->data); 
    }
    return root;
}

// Função para percorrer a árvore em pré-ordem
void preOrder(struct node* root) {
    if (root == NULL)
        return;
    
    printf("%d ", root->data);
    preOrder(root->left);
    preOrder(root->right);
}

// Função para percorrer a árvore em in-ordem
void inOrder(struct node* root) {
    if (root == NULL)
        return;
    
    inOrder(root->left);
    printf("%d ", root->data);
    inOrder(root->right);
}

// Função para percorrer a árvore em pós-ordem
void postOrder(struct node* root) {
    if (root == NULL)
        return;
    
    postOrder(root->left);
    postOrder(root->right);
    printf("%d ", root->data);
}
int main(int argc, char const *argv[])
{
    struct Node* root = NULL;

    // Inserindo os nós na árvore binária de pesquisa
    root = insert(root, 50);
    insert(root, 30);
    insert(root, 20);
    insert(root, 40);
    insert(root, 70);
    insert(root, 60);
    insert(root, 80);
    
    puts("\n\n");
    printf("Árvore original (In-ordem): ");
    inOrder(root);
    printf("\n");

    // Removendo um nó
    printf("Removendo o nó 20\n");
    root = deleteNode(root, 20);
    printf("Árvore após remover 20 (In-ordem): ");
    inOrder(root);
    printf("\n");

    printf("Removendo o nó 30\n");
    root = deleteNode(root, 30);
    printf("Árvore após remover 30 (In-ordem): ");
    inOrder(root);
    printf("\n");

    printf("Removendo o nó 50\n");
    root = deleteNode(root, 50);
    printf("Árvore após remover 50 (In-ordem): ");
    inOrder(root);
    printf("\n");

    // Exibindo os nós em diferentes ordens
    printf("Percurso Pré-ordem: ");
    preOrder(root);
    printf("\n");

    printf("Percurso Pós-ordem: ");
    postOrder(root);
    printf("\n");
    return 0;
}
