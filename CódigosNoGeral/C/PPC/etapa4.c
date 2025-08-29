/**
 * Compilar:
 * mpicc -o vc_terceiro vc_terceiro.c -lpthread
 *
 * Executar com controle de tempo (em segundos):
 * mpiexec -n 3 ./vc_terceiro tempo_produtor tempo_consumidor
 *
 */
#include <stdio.h>      // Para funções de entrada e saída padrão (printf, etc.)
#include <stdlib.h>     // Para funções gerais como atoi (converter string para inteiro) e alocação de memória.
#include <unistd.h>     // Para a função sleep().
#include <pthread.h>    // Para a criação e gerenciamento de threads (padrão POSIX).
//#include <mpi.h>        // Para a comunicação entre processos (Message Passing Interface).

#define NUM_PROCESSES 3     // Define o número total de processos na simulação.
#define QUEUE_BUFFER_SIZE 4 // Define o tamanho máximo das filas de mensagens.

// Estrutura que representa o Relógio Vetorial.
typedef struct {
    int clock[NUM_PROCESSES]; // Um array de inteiros onde cada posição representa o relógio de um processo.
} VectorClock;

// Estrutura que representa uma mensagem trocada entre os processos.
typedef struct {
    VectorClock vc;             // O relógio vetorial do remetente no momento do envio.
    int destination_rank;       // O 'rank' (ID) do processo de destino.
    char event_name;            // Um caractere para identificar a mensagem (ex: 'b' para evento B, 's' para snapshot).
} Message;

// Argumentos para a thread de lógica, permitindo passar os tempos de sleep.
typedef struct {
    int producer_sleep; // Tempo que as funções de "produção" (event, send) esperam.
    int consumer_sleep; // Tempo que a função de "consumo" (receive) espera.
} LogicThreadArgs;

// Estrutura que implementa uma fila circular segura para threads (thread-safe).
typedef struct {
    Message buffer[QUEUE_BUFFER_SIZE]; // O buffer que armazena as mensagens.
    int head, tail, count;             // Índices para controle da fila (cabeça, cauda, número de itens).
    pthread_mutex_t mutex;             // Mutex para garantir acesso exclusivo à fila.
    pthread_cond_t can_produce;        // Variável de condição para sinalizar que há espaço para produzir.
    pthread_cond_t can_consume;        // Variável de condição para sinalizar que há itens para consumir.
} Queue;

// Estrutura para armazenar o estado local (o relógio) no momento do snapshot.
typedef struct {
    VectorClock vc;
} snap;

// Estrutura para armazenar o estado de um canal de comunicação durante o snapshot.
typedef struct {
    int recorded;           // Flag: 1 se o marcador já passou por este canal, 0 caso contrário.
    Message messages[100];  // Buffer para armazenar mensagens "em trânsito" no canal.
    int count;              // Contador de mensagens armazenadas.
} ChannelState;

// --- Variáveis Globais ---
int my_rank;                            // O ID (rank) deste processo.
VectorClock local_clock;                // O relógio vetorial local deste processo.
pthread_mutex_t clock_mutex;            // Mutex para proteger o acesso ao relógio vetorial local.
Queue incoming_queue;                   // Fila de mensagens recebidas da rede, a serem processadas.
Queue outgoing_queue;                   // Fila de mensagens a serem enviadas para a rede.
volatile int simulation_finished = 0;   // Flag global para sinalizar o término da simulação.
snap snapShot;                          // Armazena o snapshot do estado local.
int snap_done;                          // Flag: 1 se este processo já salvou seu estado para o snapshot, 0 caso contrário.
ChannelState channel_states[NUM_PROCESSES]; // Array para armazenar o estado de cada canal de entrada (um por processo remoto).

// Função auxiliar para imprimir o status de ocupação das filas. Útil para debugging.
void print_queue_status(const char* context) {
    int in_count, out_count;
    // Trava o mutex de cada fila para ler o contador em segurança.
    pthread_mutex_lock(&incoming_queue.mutex); in_count = incoming_queue.count; pthread_mutex_unlock(&incoming_queue.mutex);
    pthread_mutex_lock(&outgoing_queue.mutex); out_count = outgoing_queue.count; pthread_mutex_unlock(&outgoing_queue.mutex);
    printf("     [STATUS P%d] %s | Filas -> Entrada: %d/%d, Saída: %d/%d\n",
           my_rank, context, in_count, QUEUE_BUFFER_SIZE, out_count, QUEUE_BUFFER_SIZE);
    fflush(stdout); // Garante que a saída seja impressa imediatamente.
}

// --- Funções da Fila Thread-Safe ---
void queue_init(Queue* q) { q->head = 0; q->tail = 0; q->count = 0; pthread_mutex_init(&q->mutex, NULL); pthread_cond_init(&q->can_produce, NULL); pthread_cond_init(&q->can_consume, NULL); }
void queue_destroy(Queue* q) { pthread_mutex_destroy(&q->mutex); pthread_cond_destroy(&q->can_produce); pthread_cond_destroy(&q->can_consume); }
void enqueue(Queue* q, Message item) {
    pthread_mutex_lock(&q->mutex); // Trava o mutex para acesso exclusivo.
    while (q->count == QUEUE_BUFFER_SIZE) { // Se a fila está cheia...
        pthread_cond_wait(&q->can_produce, &q->mutex); // ...espera pelo sinal 'can_produce'.
    }
    q->buffer[q->tail] = item; // Adiciona o item na cauda.
    q->tail = (q->tail + 1) % QUEUE_BUFFER_SIZE; // Atualiza o índice da cauda (circular).
    q->count++; // Incrementa o contador.
    pthread_cond_signal(&q->can_consume); // Sinaliza a uma thread consumidora que há um item disponível.
    pthread_mutex_unlock(&q->mutex); // Libera o mutex.
}
Message dequeue(Queue* q) {
    pthread_mutex_lock(&q->mutex); // Trava o mutex.
    while (q->count == 0) { // Se a fila está vazia...
        pthread_cond_wait(&q->can_consume, &q->mutex); // ...espera pelo sinal 'can_consume'.
    }
    Message item = q->buffer[q->head]; // Pega o item da cabeça.
    q->head = (q->head + 1) % QUEUE_BUFFER_SIZE; // Atualiza o índice da cabeça (circular).
    q->count--; // Decrementa o contador.
    pthread_cond_signal(&q->can_produce); // Sinaliza a uma thread produtora que há espaço disponível.
    pthread_mutex_unlock(&q->mutex); // Libera o mutex.
    return item;
}

// Função para imprimir o estado do relógio vetorial de um processo.
void print_clock(int rank, VectorClock* vc) {
    printf("P%d, Relógio: (%d, %d, %d)\n",
        rank, vc->clock[0], vc->clock[1], vc->clock[2]);
    fflush(stdout);
}

// --- Funções de Lógica do Relógio Vetorial ---

// Simula um evento local (Regra 1 do Relógio Vetorial).
void event(int rank, int producer_sleep){
    pthread_mutex_lock(&clock_mutex);  // Protege o acesso ao relógio local.
    local_clock.clock[rank]++;         // Incrementa o relógio da própria posição no vetor.
    printf("Event: ");
    print_clock(rank, &local_clock);   // Imprime o novo estado do relógio.
    pthread_mutex_unlock(&clock_mutex); // Libera o mutex.
    sleep(producer_sleep); // Pausa para controlar o ritmo da simulação.
}

// Prepara e enfileira uma mensagem para envio (Regra 2 do Relógio Vetorial).
void send(int rank, int rankSend, int producer_sleep){
    pthread_mutex_lock(&clock_mutex); // Protege o acesso ao relógio local.
    local_clock.clock[rank]++;        // Incrementa o relógio local antes de enviar.
    // Cria a mensagem, copiando o estado atual do relógio vetorial para ela.
    Message msg_b = {.vc = local_clock, .destination_rank = rankSend, .event_name = 'b'};
    enqueue(&outgoing_queue, msg_b); // Coloca a mensagem na fila de saída.
    printf("Send: ");
    print_queue_status("enfileirou para OUT"); // Imprime status da fila.
    print_clock(rank, &local_clock); // Imprime o estado do relógio após o evento de envio.
    pthread_mutex_unlock(&clock_mutex); // Libera o mutex.
    sleep(producer_sleep); // Pausa.
}

// Processa uma mensagem recebida (Regra 3 do Relógio Vetorial).
void receive(int rank, int consumer_sleep){
    Message msg_h = dequeue(&incoming_queue); // Retira uma mensagem da fila de entrada.
    pthread_mutex_lock(&clock_mutex); // Protege o acesso ao relógio local.
    // Atualiza o relógio local: para cada posição, pega o valor máximo entre o relógio local e o da mensagem.
    for(int i=0; i<NUM_PROCESSES; i++) { local_clock.clock[i] = (msg_h.vc.clock[i] > local_clock.clock[i]) ? msg_h.vc.clock[i] : local_clock.clock[i]; }
    local_clock.clock[rank]++; // Incrementa o relógio local após a fusão.
    printf("Receive: ");
    print_clock(rank, &local_clock); // Imprime o novo estado do relógio.
    pthread_mutex_unlock(&clock_mutex); // Libera o mutex.
    sleep(consumer_sleep); // Pausa.
}

// --- Funções do Algoritmo de Snapshot Chandy-Lamport ---

// Função para o processo que INICIA o snapshot.
void iniciadorSnapshot(){
    if(snap_done == 1) return; // Se já iniciou o snapshot, não faz nada.

    pthread_mutex_lock(&clock_mutex); // Protege o acesso às variáveis globais de snapshot e relógio.
    snap_done = 1; // Marca que este processo já salvou seu estado.

    // 1. Salva o estado local (copia o relógio vetorial local para a variável de snapshot).
    for (int i = 0; i < NUM_PROCESSES; i++){
        snapShot.vc.clock[i] = local_clock.clock[i];
    }
    // Inicializa o estado dos canais de entrada (nenhum marcador recebido ainda).
    for (int i = 0; i < NUM_PROCESSES; i++) {
        if (i == my_rank) continue; // Pula o canal para si mesmo.
            channel_states[i].recorded = 0;
            channel_states[i].count = 0;
    }
    printf("P%d salvou clock local no snapshot\n", my_rank);
    fflush(stdout);
    
    // 2. Envia mensagens marcadoras para todos os outros processos.
    Message marcador;
    marcador.event_name = 's'; // 's' para snapshot/marcador.
    // O relógio do marcador não importa, então pode ser preenchido com valores sentinela.
    for (int i = 0; i < NUM_PROCESSES; i++){
        marcador.vc.clock[i] = -1;
    }
    
    // Enfileira um marcador para cada outro processo.
    for(int i = 0; i < NUM_PROCESSES; i++){
        if(i == my_rank) continue;
        marcador.destination_rank = i;
        enqueue(&outgoing_queue, marcador);
        printf("P%d enviou marcador para P%d\n", my_rank, i);
    }

    pthread_mutex_unlock(&clock_mutex); // Libera o mutex.
}

// Função para um processo PARTICIPANTE (que recebe um marcador).
void participanteSnapshot(MPI_Status status){
    // Caso 1: O processo JÁ salvou seu estado e recebe um marcador do processo 'status.MPI_SOURCE'.
    if(snap_done == 1){
        // Se este canal ainda estava aberto para gravação...
        if(!channel_states[status.MPI_SOURCE].recorded){
            // ...fecha o canal. A partir de agora, mensagens deste canal não são mais parte do snapshot.
            channel_states[status.MPI_SOURCE].recorded = 1;
            printf("P%d fechou gravação do canal P%d->P%d\n", my_rank, status.MPI_SOURCE, my_rank);
        }
        return; // Termina a função.
    } 

    // Caso 2: É o PRIMEIRO marcador que este processo recebe.
    pthread_mutex_lock(&clock_mutex); // Protege o acesso às variáveis globais.
    snap_done = 1; // Marca que salvou o estado.

    // 1. Salva o estado local (exatamente como o iniciador).
    for (int i = 0; i < NUM_PROCESSES; i++){
        snapShot.vc.clock[i] = local_clock.clock[i];
    }
    // Inicializa o estado dos canais.
    for (int i = 0; i < NUM_PROCESSES; i++) {
        if (i == my_rank) continue;
            channel_states[i].recorded = 0;
            channel_states[i].count = 0;
    }
    printf("P%d salvou clock local no snapshot\n", my_rank);
    fflush(stdout);
    
    // 2. Envia mensagens marcadoras para todos os outros processos (exatamente como o iniciador).
    Message marcador;
    marcador.event_name = 's';
    for (int i = 0; i < NUM_PROCESSES; i++){
        marcador.vc.clock[i] = -1;
    }
    
    for(int i = 0; i < NUM_PROCESSES; i++){
        if(i == my_rank) continue;
        marcador.destination_rank = i;
        enqueue(&outgoing_queue, marcador);
        printf("P%d enviou marcador para P%d\n", my_rank, i);
        fflush(stdout);
    }

    pthread_mutex_unlock(&clock_mutex); // Libera o mutex.
}

// --- Threads Principais ---

// Thread "Ouvido": Fica escutando por mensagens da rede (MPI).
void* receiver_thread(void* args) {
    while (!simulation_finished) {
        Message msg;
        MPI_Status status;
        // Fica bloqueada aqui esperando uma mensagem de qualquer processo (MPI_ANY_SOURCE).
        MPI_Recv(&msg, sizeof(Message), MPI_BYTE, MPI_ANY_SOURCE, 0, MPI_COMM_WORLD, &status);
        if (simulation_finished) break; // Se a simulação acabou, sai do loop.

        // Se a mensagem recebida é um marcador...
        if(msg.event_name == 's'){
            participanteSnapshot(status); // ...chama a lógica de snapshot do participante.
            continue; // Pula o resto do loop e espera a próxima mensagem.
        }

        // Se a mensagem é NORMAL (não é um marcador):
        // Verifica se é uma mensagem "em trânsito" para o snapshot.
        // Condição: "eu já salvei meu estado" E "o canal do remetente ainda está aberto".
        if(snap_done && !channel_states[status.MPI_SOURCE].recorded){
            // Se for, armazena a mensagem no estado do canal.
            channel_states[status.MPI_SOURCE].messages[channel_states[status.MPI_SOURCE].count++] = msg;
            printf("P%d gravou msg '%c' no estado do canal P%d->P%d\n", my_rank, msg.event_name, status.MPI_SOURCE, my_rank);
        }

        printf("[RECV] P%d <-- P%d: Mensagem '%c' recebida.\n", my_rank, status.MPI_SOURCE, msg.event_name);
        fflush(stdout);
        enqueue(&incoming_queue, msg); // Coloca a mensagem na fila de entrada para a logic_thread processar.
        print_queue_status("Após enfileirar em IN");
    }
    return NULL;
}

// Thread "Boca": Envia mensagens que estão na fila de saída.
void* sender_thread(void* args) {
    while (!simulation_finished) {
        // Fica bloqueada aqui esperando por uma mensagem na fila de saída.
        Message msg = dequeue(&outgoing_queue);
        // Condição de parada: a simulação terminou E a mensagem é a de finalização ('X').
        if (simulation_finished && msg.event_name == 'X') break;

        printf("[SEND] P%d --> P%d: Enviando mensagem '%c'.\n", my_rank, msg.destination_rank, msg.event_name);
        fflush(stdout);
        // Envia a mensagem para o destino usando MPI.
        MPI_Send(&msg, sizeof(Message), MPI_BYTE, msg.destination_rank, 0, MPI_COMM_WORLD);
    }
    return NULL;
}

// Thread "Cérebro": Orquestra a sequência de eventos da simulação para este processo.
void* logic_thread(void* args) {
    LogicThreadArgs* thread_args = (LogicThreadArgs*) args;
    int producer_sleep = thread_args->producer_sleep;
    int consumer_sleep = thread_args->consumer_sleep;
    snap_done = 0; // Garante que o estado do snapshot comece como não concluído.

    MPI_Barrier(MPI_COMM_WORLD); // Sincroniza todos os processos antes de começar a lógica.
    if(my_rank == 0) sleep(1);   // Pequena pausa para o processo 0, ajuda na formatação da saída.

    // A lógica de cada processo é pré-definida e diferente para cada rank.
    if (my_rank == 0) {
        // Evento A
        event(my_rank, producer_sleep);
        // Evento B 
        send(my_rank, 1, producer_sleep);
        // Evento C 
        receive(my_rank, consumer_sleep);
        // Inicia o snapshot!
        iniciadorSnapshot();
        // Evento D
        send(my_rank, 2, producer_sleep);
        // Evento E
        receive(my_rank, consumer_sleep);
        // Evento F
        send(my_rank, 1, producer_sleep);
        // Evento G
        event(my_rank, producer_sleep);

    } else if (my_rank == 1) {
        // Evento H
        send(my_rank, 0, producer_sleep);
        // Evento I
        receive(my_rank, consumer_sleep);
        // Evento J
        receive(my_rank, consumer_sleep);

    } else if (my_rank == 2) {
        // Evento k
        event(my_rank, producer_sleep);
        // Envio para l
        send(my_rank, 0, producer_sleep);
        // Evento M
        receive(my_rank, consumer_sleep);
    }
    return NULL; // A thread de lógica termina após executar sua sequência.
}

// --- Função Principal ---
int main(int argc, char* argv[]) {
    // Verifica se os argumentos de linha de comando (tempos de sleep) foram passados.
    if (argc != 3) {
        MPI_Init(&argc, &argv);
        int temp_rank;
        MPI_Comm_rank(MPI_COMM_WORLD, &temp_rank);
        if (temp_rank == 0) { // Apenas o processo 0 imprime a mensagem de erro.
            fprintf(stderr, "Uso: %s <tempo_produtor_segundos> <tempo_consumidor_segundos>\n", argv[0]);
            fprintf(stderr, "Exemplo: mpiexec -n 3 %s 2 1\n", argv[0]);
        }
        MPI_Finalize();
        return 1;
    }
    // Converte os argumentos de string para inteiro.
    int producer_sleep = atoi(argv[1]);
    int consumer_sleep = atoi(argv[2]);

    int mpi_provided;
    // Inicializa o MPI, solicitando suporte para múltiplas threads.
    MPI_Init_thread(&argc, &argv, MPI_THREAD_MULTIPLE, &mpi_provided);
    if (mpi_provided < MPI_THREAD_MULTIPLE) { // Verifica se o suporte foi concedido.
        fprintf(stderr, "Erro: MPI não oferece suporte total a threads.\n");
        MPI_Finalize(); return 1;
    }

    MPI_Comm_rank(MPI_COMM_WORLD, &my_rank); // Obtém o rank do processo atual.
    // Inicializa o relógio vetorial local com zeros.
    for (int i = 0; i < NUM_PROCESSES; i++) local_clock.clock[i] = 0;

    // Inicializa os mutexes e as filas.
    pthread_mutex_init(&clock_mutex, NULL);
    queue_init(&incoming_queue);
    queue_init(&outgoing_queue);

    if (my_rank == 0) { // Processo 0 imprime o cabeçalho da simulação.
        printf("--- Simulação iniciada ---\n");
        printf("Tempo Produtor: %ds | Tempo Consumidor: %ds\n\n", producer_sleep, consumer_sleep);
    }
    
    pthread_t receiver, sender, logic; // Declara as variáveis das threads.
    
    // Prepara os argumentos para a logic_thread.
    LogicThreadArgs args;
    args.producer_sleep = producer_sleep;
    args.consumer_sleep = consumer_sleep;
    
    // Cria as três threads principais.
    pthread_create(&receiver, NULL, receiver_thread, NULL);
    pthread_create(&sender, NULL, sender_thread, NULL);
    pthread_create(&logic, NULL, logic_thread, &args);

    // O fluxo principal (main) espera a thread de LÓGICA terminar.
    pthread_join(logic, NULL);
    
    
    // --- Lógica de Finalização ---
    simulation_finished = 1; // Sinaliza para todas as threads que a simulação acabou.
    MPI_Barrier(MPI_COMM_WORLD); // Espera todos os processos chegarem a este ponto.
    
    // O processo 0 envia mensagens de finalização para todos (incluindo ele mesmo)
    // para desbloquear qualquer MPI_Recv que ainda esteja esperando.
    if (my_rank == 0) {
        for (int i = 0; i < NUM_PROCESSES; i++) {
            Message final_msg = {.event_name = 'X'};
            MPI_Send(&final_msg, sizeof(Message), MPI_BYTE, i, 0, MPI_COMM_WORLD);
        }
    }
    // Enfileira uma mensagem de finalização na fila de saída para desbloquear a sender_thread, caso ela esteja esperando.
    enqueue(&outgoing_queue, (Message){.event_name = 'X'});
    
    // Espera as threads de comunicação terminarem.
    pthread_join(receiver, NULL);
    pthread_join(sender, NULL);
    
    if (my_rank == 0) printf("\n--- Simulação Concluída ---\n");
    
    // Libera os recursos (mutexes, filas).
    pthread_mutex_destroy(&clock_mutex);
    queue_destroy(&incoming_queue);
    queue_destroy(&outgoing_queue);
    MPI_Finalize(); // Finaliza o ambiente MPI.
    return 0;
}