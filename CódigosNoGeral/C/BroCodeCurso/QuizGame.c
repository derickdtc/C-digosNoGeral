#include <stdio.h>
#include <string.h>
#include <time.h>
#include <stdlib.h>
#include <ctype.h>

int main(){
    char questions[][100]={"What year did the C language debut?",
                            "Who created C?",
                            "What's the predecessor of C?"};
    char options[][100]={"A. 1969", "B. 1972", "C. 1975", "D. 1999",
                        "A. Dennis Ritchie", "B. Jeff bezos", "C. John Carmark", "D. John Lennon",
                        "A. Objectice C", "B. B", "C. C++", "D. C--", };
    char answers[3]= {'B', 'A', 'B'};
    int numberOfQ = sizeof(questions)/sizeof(questions[0]);
    char guess;
    int score=0;
    
    printf("QUIZ GAME\n");

    for (int i = 0; i < numberOfQ; i++)
    {
        printf("%s\n", questions[i]);
        for (int j = i*4; j < (i*4)+4 ; j++)
        {
            printf("%s\n", options[j]);

        }
        printf("guess: ");
        scanf("%c", &guess);
        scanf("%c");// limpa o \n do input buffer 
        guess = toupper(guess);

        if (guess = answers[i])
        {
            printf("Correct\n");
            score++;

        }else{
            printf("Wrong\n");

        }
        
    }
    printf("Final score: %d/%d", score, numberOfQ);
    
    return 0;
}