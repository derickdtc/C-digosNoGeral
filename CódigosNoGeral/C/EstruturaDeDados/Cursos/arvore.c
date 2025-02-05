#include <stdlib.h>
#include <stdio.h>

typedef struct no {
    int valor;
    struct no *esq;
    struct no *dir;
} Node;

Node* create(int val){
    Node *n = (Node*) malloc(sizeof(Node));
    n->valor =val;
    n->esq = NULL;
    n->dir = NULL;
    return n;
}

void display(Node *n){
    if (n==NULL)
        return;
    
    printf("\n %d", n->valor);
    display(n->esq);   
    display(n->dir);
}
void add(Node *n , int val){
    if (val < n->valor )
    {
        
    }
    
}
int main(int argc, char const *argv[])
{
   Node *n5= create(5);
    Node *n2= create(2);
   Node *n1= create(1);
   Node *n8= create(8);
   Node *n4= create(4);

    n5->esq = n2;
    n2->esq = n1;
    n5->dir = n8;
    n2->dir = n4;

    display(n5);
    return 0;
}
