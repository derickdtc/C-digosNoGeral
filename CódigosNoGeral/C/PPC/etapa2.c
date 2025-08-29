/* File:  
 * etapa2.c
 *
 * Purpose:
 * Implementação do modelo Produtor/Consumidor com relógios vetoriais
 * usando um pool de threads e variáveis de condição para sincronização.
 *
 * Compile:  gcc -g -Wall -o etapa2 etapa2.c -lpthread -lrt
 * Usage:    ./produtor_consumidor_vc <producer_sleep_seconds> <consumer_sleep_seconds>
 * Example (Fila Cheia): ./etapa2 1 2
 * Example (Fila Vazia): ./etapa2 2 1
 */
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h> 
#include <unistd.h>
#include <time.h>

#define NUM_PRODUCERS 3
#define NUM_CONSUMERS 3
#define BUFFER_SIZE 10      // Tamanho do buffer/fila
#define MAX_PRODUCTIONS 30  // Número total de itens a serem produzidos

// Estrutura para o Relógio Vetorial
typedef struct {
   int clock[NUM_PRODUCERS];
} VectorClock;

// Fila compartilhada e variáveis de controle
VectorClock queue[BUFFER_SIZE];
int count = 0; // Número de itens na fila
int productions_done = 0; // Contador de itens já produzidos

// Variáveis de Sincronização
pthread_mutex_t mutex;
pthread_cond_t condFull;
pthread_cond_t condEmpty;

// Protótipos das funções
void submitClock(VectorClock vc, long producer_id);
VectorClock getClock(long consumer_id);
void *produce(void *args);
void *consume(void *args);

// Argumentos para as threads
typedef struct {
    long id;
    int sleep_time;
} ThreadArgs;


int main(int argc, char* argv[]) {
    if (argc != 3) {
        fprintf(stderr, "Usage: %s <producer_sleep_seconds> <consumer_sleep_seconds>\n", argv[0]);
        return 1;
    }

    int producer_sleep = atoi(argv[1]);
    int consumer_sleep = atoi(argv[2]);

    printf("Iniciando simulação com:\n");
    printf(" - Tempo de espera do Produtor: %ds\n", producer_sleep);
    printf(" - Tempo de espera do Consumidor: %ds\n", consumer_sleep);
    printf(" - Tamanho do Buffer: %d\n", BUFFER_SIZE);
    printf(" - Total de Produções: %d\n\n", MAX_PRODUCTIONS);
    sleep(2);

    // Inicialização
    srand(time(NULL));
    pthread_mutex_init(&mutex, NULL);
    pthread_cond_init(&condEmpty, NULL);
    pthread_cond_init(&condFull, NULL);

    pthread_t producers[NUM_PRODUCERS];
    pthread_t consumers[NUM_CONSUMERS];
    ThreadArgs producer_args[NUM_PRODUCERS];
    ThreadArgs consumer_args[NUM_CONSUMERS];

    long i;
    // Criar threads produtoras
    for (i = 0; i < NUM_PRODUCERS; i++) {
        producer_args[i].id = i;
        producer_args[i].sleep_time = producer_sleep;
        if (pthread_create(&producers[i], NULL, &produce, &producer_args[i]) != 0) {
            perror("Falha ao criar a thread produtora");
        }
    }

    // Criar threads consumidoras
    for (i = 0; i < NUM_CONSUMERS; i++) {
        consumer_args[i].id = i;
        consumer_args[i].sleep_time = consumer_sleep;
        if (pthread_create(&consumers[i], NULL, &consume, &consumer_args[i]) != 0) {
            perror("Falha ao criar a thread consumidora");
        }
    }

    // Aguardar o término das threads produtoras
    for (i = 0; i < NUM_PRODUCERS; i++) {
        if (pthread_join(producers[i], NULL) != 0) {
            perror("Falha ao aguardar a thread produtora");
        }
    }
    
    // Aguardar o término das threads consumidoras
    for (i = 0; i < NUM_CONSUMERS; i++) {
        if (pthread_join(consumers[i], NULL) != 0) {
            perror("Falha ao aguardar a thread consumidora");
        }
    }

    // Limpeza
    pthread_mutex_destroy(&mutex);
    pthread_cond_destroy(&condEmpty);
    pthread_cond_destroy(&condFull);

    printf("\nSimulação concluída.\n");
    return 0;
}

// Função da thread produtora
void *produce(void *args) {
    ThreadArgs* p_args = (ThreadArgs*) args;
    long id = p_args->id;
    int sleep_time = p_args->sleep_time;
    int personal_clock = 0;

    while (1) {
        // Bloqueia para verificar a condição de parada
        pthread_mutex_lock(&mutex);
        if (productions_done >= MAX_PRODUCTIONS) {
            pthread_mutex_unlock(&mutex);
            break; // Termina a produção
        }
        pthread_mutex_unlock(&mutex);

        // Gera um novo relógio vetorial
        personal_clock++;
        VectorClock vc;
        for (int i = 0; i < NUM_PRODUCERS; i++) {
            if (i == id) {
                vc.clock[i] = personal_clock;
            } else {
                vc.clock[i] = rand() % 100; // Valores aleatórios para outros processos
            }
        }
        
        submitClock(vc, id);
        sleep(sleep_time);
    }

    return NULL;
}

// Função da thread consumidora
void *consume(void *args) {
    ThreadArgs* c_args = (ThreadArgs*) args;
    long id = c_args->id;
    int sleep_time = c_args->sleep_time;

    while (1) {
        pthread_mutex_lock(&mutex);
        // Se a produção acabou e a fila está vazia, a thread pode terminar.
        if (productions_done >= MAX_PRODUCTIONS && count == 0) {
            pthread_mutex_unlock(&mutex);
            break; 
        }
        pthread_mutex_unlock(&mutex);

        VectorClock vc = getClock(id);
        
        // getClock pode fazer a thread sair se a condição de término for atingida
       
        int is_done;
        pthread_mutex_lock(&mutex);
        is_done = (productions_done >= MAX_PRODUCTIONS && count == 0);
        pthread_mutex_unlock(&mutex);
        if(!is_done){
             printf("Consumidor %ld consumiu: (%d, %d, %d)\n", id, vc.clock[0], vc.clock[1], vc.clock[2]);
             fflush(stdout);
             sleep(sleep_time);
        }
    }
    
    // Acorda outras threads consumidoras que possam estar presas em condEmpty
    // para que elas também possam verificar a condição de término e sair.
    pthread_cond_broadcast(&condEmpty);
    return NULL;
}

// Adiciona um relógio vetorial na fila
void submitClock(VectorClock vc, long producer_id) {
    pthread_mutex_lock(&mutex);

    // Espera se a fila estiver cheia
    while (count == BUFFER_SIZE) {
        printf("Produtor %ld: Fila CHEIA. Esperando...\n", producer_id);
        fflush(stdout);
        pthread_cond_wait(&condFull, &mutex);
    }

    // Adiciona o item
    queue[count] = vc;
    count++;
    productions_done++;
    
    printf("Produtor %ld produziu: (%d, %d, %d). Itens na fila: %d\n", producer_id, vc.clock[0], vc.clock[1], vc.clock[2], count);
    fflush(stdout);

    // Sinaliza que a fila não está mais vazia
    pthread_cond_signal(&condEmpty);
    pthread_mutex_unlock(&mutex);
}

// Pega um relógio vetorial da fila
VectorClock getClock(long consumer_id) {
    pthread_mutex_lock(&mutex);

    // Espera se a fila estiver vazia
    while (count == 0) {
        // Verifica se a produção terminou. Se sim, não há mais o que esperar.
        if (productions_done >= MAX_PRODUCTIONS) {
            pthread_mutex_unlock(&mutex);
            // Acorda outras consumidoras para que também saiam
            pthread_cond_broadcast(&condEmpty); 
            pthread_exit(NULL); // Termina a thread
        }
        printf("Consumidor %ld: Fila VAZIA. Esperando...\n", consumer_id);
        fflush(stdout);
        pthread_cond_wait(&condEmpty, &mutex);
    }

    // Remove o item
    VectorClock vc = queue[0];
    for (int i = 0; i < count - 1; i++) {
        queue[i] = queue[i+1];
    }
    count--;

    // Sinaliza que a fila não está mais cheia
    pthread_cond_signal(&condFull);
    pthread_mutex_unlock(&mutex);
    return vc;
}