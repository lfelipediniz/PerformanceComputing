#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>


#define BUFFER_LEN 11
#define MAX_PRODUCED 100

int stack[BUFFER_LEN];
int item_available = 0;
int produced = 0;
int consumed = 0;

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
sem_t empty, full;

void add_stack(int item){
    stack[item_available] = item;
    item_available++;
    produced++;
}

int pop_stack(){
    item_available--;
    consumed++;

    return stack[item_available];
}

void * producer(){
    while (produced < MAX_PRODUCED) {
        int item = rand()%1000;
        // esperando alguma posicao do buffer ficar vazia
        sem_wait(&empty);

        // entrando na região critica
        pthread_mutex_lock(&mutex);
        add_stack(item);
        pthread_mutex_unlock(&mutex); // saindo regiao critica
        
        sem_post(&full); // avisando que posicao buffer esta cheia

        printf("\nO item %d produzido com o valor %d esta na posição %d buffer\n", produced, item, item_available);

    }

    pthread_exit(0);
}

void * consumer(){
    while (consumed < MAX_PRODUCED) {
        sem_wait(&full);

        pthread_mutex_lock(&mutex);
        int item = pop_stack();;
        pthread_mutex_unlock(&mutex);

        sem_post(&empty);

        printf("\nItem: %d consumido\n", item);
    }

    pthread_exit(0);
}


int main() {
    pthread_t prod_handle, cons_handle;

    // todas posicoes do buffer vazias
    sem_init(&empty, 0, BUFFER_LEN);

    // nenhum item cheio
    sem_init(&full, 0, 0);

    // criando as threads
    pthread_create(
        &prod_handle,
        NULL,
        producer, // funcao do producer
        NULL
    );

    pthread_create(
        &cons_handle,
        NULL,
        consumer,
        NULL
    );

    // esperadno as duas threads terminarem
    pthread_join(prod_handle, NULL);
    pthread_join(cons_handle, NULL);

    sem_destroy(&empty);
    sem_destroy(&full);

    pthread_mutex_destroy(&mutex);

    return 0;
}







