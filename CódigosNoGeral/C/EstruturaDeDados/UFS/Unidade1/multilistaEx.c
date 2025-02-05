
#include <stdio.h>
#include <stdlib.h>
#include <string.h>




typedef struct student
{
    char name[30];
    int registerNumber;
    struct student *nextStudent;

}typeStudent;

typedef struct subject
{
    char nameSubject[25];
    int code;    
    struct student *firstStudent;
    struct student *lastStudent;
    int quantStudent;
    struct subject *nextSubject;

}typeSubject;

typedef struct subjectManager
{
    struct subject *firstSubject;
    struct subject *lastSubject;
    int quant;
    
}subManager;

void createSubject (subManager *subList , int cod , char nameSub[]){
    typeSubject *newSub ;
    
    if (subList->firstSubject == NULL)// Caso a lista de matérias esteja vazia
    {
     newSub = (typeSubject*) malloc(sizeof(typeSubject));   
    newSub->code = cod ; 
    strcpy(newSub->nameSubject , nameSub);
    newSub->nextSubject = NULL;
    newSub->firstStudent = NULL;

    subList->firstSubject = newSub;
    subList->lastSubject = newSub;
    subList->quant++;

    }else{// O else vai inserir a nova matéria sempre na frente da lista
    newSub = (typeSubject*) malloc(sizeof(typeSubject));   
    newSub->code = cod;
    strcpy(newSub->nameSubject , nameSub);
    newSub->nextSubject = NULL;
    (*newSub).firstStudent = NULL;
    
    newSub->nextSubject = subList->firstSubject;
    subList->firstSubject = newSub;
    
    }
    

}
void startSubject(typeSubject *sub){
    sub->firstStudent = NULL;
    sub->lastStudent = NULL;
    int quantStudent = 0;
    (*sub).nextSubject= NULL;
}
void addStudent(subManager *subList , typeSubject *sub, char nameStudent[] , int registerNum ){
    typeStudent *newStudent = (typeStudent*) malloc(sizeof(typeStudent));
     if (newStudent == NULL) {
        printf("Erro ao alocar memória para novo estudante.\n");
        return;
    }
        //Atribuindo os valores dos atributos do estudante
        newStudent->registerNumber = registerNum;
        strcpy(newStudent->name , nameStudent);
        newStudent->nextStudent = NULL;

    if (sub->firstStudent == NULL)//Caso a matéria(lista de estudantes) esteja vazia
    {    
       
        sub->firstStudent = newStudent;
        sub->lastStudent = newStudent;
    }else{// Caso já hajam estutandes na matéria, adiciona um estudante na frente
       
        newStudent->nextStudent = sub->firstStudent;
        sub->firstStudent = newStudent;
    }

        sub->quantStudent++;

}

void removeLastStudent(subManager *subList , typeSubject *sub){
    typeStudent *atual;
    typeStudent *antNo;
    if (sub->firstStudent == NULL)
    {
        return;
    }
    atual = sub->firstStudent;
    while (atual->nextStudent !=NULL)
    {
        antNo = atual;
        atual = atual->nextStudent;
    }
    antNo->nextStudent = NULL;
    sub->lastStudent = antNo;
    sub->quantStudent--;
    free(atual);
    
}

void printTotalList(subManager *subList){
     typeSubject *sub = subList->firstSubject;

    if (sub == NULL) {
        printf("Nenhuma matéria cadastrada.\n");
        return;
    }

    while (sub != NULL) {
        printf("Matéria: %s (Código: %d)\n", sub->nameSubject, sub->code);
        //printf("Quantidade de estudantes: %d\n", sub->quantStudent);
        
        typeStudent *stud = sub->firstStudent;
        while (stud != NULL) {
            printf("\tEstudante: %s (Matrícula: %d)\n", stud->name, stud->registerNumber);
            stud = stud->nextStudent;
        }
        sub = sub->nextSubject;
        printf("\n");
    }
}
int main(){
   subManager subList;
    
    // Inicializando os ponteiros e contadores dentro de subList
    subList.firstSubject = NULL;
    subList.lastSubject = NULL;
    subList.quant = 0;

    // Criando matérias
    createSubject(&subList, 4, "Estrutura de Dados");
    createSubject(&subList, 3, "POO");
    createSubject(&subList, 2, "Sistemas Digitais");
    createSubject(&subList, 1, "Cálculo C");

    // Adicionando estudantes às matérias
    addStudent(&subList, subList.firstSubject, "Alice", 1001);
    addStudent(&subList, subList.firstSubject, "Leonardo", 1002);
    addStudent(&subList, subList.firstSubject->nextSubject, "Davi", 1003);
    addStudent(&subList, subList.firstSubject->nextSubject->nextSubject, "José", 1004);
    addStudent(&subList , subList.firstSubject->nextSubject->nextSubject->nextSubject , "Maria" , 1808);

    // Imprimindo a lista completa de matérias e estudantes
    printTotalList(&subList);
    
    return 0;
}
