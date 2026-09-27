#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>


#define BUFFER_SIZE 11
#define MAX_ITEMS 100

int buffer[BUFFER_SIZE];
int available_items = 0;
int produced_count = 0;
int consumed_count = 0;

pthread_mutex_t buffer_mutex = PTHREAD_MUTEX_INITIALIZER;
sem_t empty_slots, filled_slots;

void push_item(int item){
    buffer[available_items] = item;
    available_items++;
    produced_count++;
}

int pop_item(){
    available_items--;
    consumed_count++;

    return buffer[available_items];
}

void * producer(){
    while (produced_count < MAX_ITEMS) {
        int item = rand()%1000;
        // esperando alguma posicao do buffer ficar vazia
        sem_wait(&empty_slots);

        // entrando na região critica
        pthread_mutex_lock(&buffer_mutex);
        push_item(item);
        pthread_mutex_unlock(&buffer_mutex); // saindo regiao critica
        
        sem_post(&filled_slots); // avisando que posicao buffer esta cheia

        printf("\nItem %d was produced with value %d at buffer position %d\n", produced_count, item, available_items);

    }

    pthread_exit(0);
}

void * consumer(){
    while (consumed_count < MAX_ITEMS) {
        sem_wait(&filled_slots);

        pthread_mutex_lock(&buffer_mutex);
        int item = pop_item();;
        pthread_mutex_unlock(&buffer_mutex);

        sem_post(&empty_slots);

        printf("\nItem %d was consumed\n", item);
    }

    pthread_exit(0);
}


int main() {
    pthread_t producer_thread, consumer_thread;

    // todas posicoes do buffer vazias
    sem_init(&empty_slots, 0, BUFFER_SIZE);

    // nenhum item cheio
    sem_init(&filled_slots, 0, 0);

    // criando as threads
    pthread_create(
        &producer_thread,
        NULL,
        producer, // funcao do producer
        NULL
    );

    pthread_create(
        &consumer_thread,
        NULL,
        consumer,
        NULL
    );

    // esperadno as duas threads terminarem
    pthread_join(producer_thread, NULL);
    pthread_join(consumer_thread, NULL);

    sem_destroy(&empty_slots);
    sem_destroy(&filled_slots);

    pthread_mutex_destroy(&buffer_mutex);

    return 0;
}






