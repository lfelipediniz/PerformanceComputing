#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <stdbool.h>


#define BUFFER_SIZE 7
#define MAX_ITEMS 100

#define NUM_PRODUCERS 3
#define NUM_CONSUMERS 4

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

void * producer(void * arg){
    int id = * ((int *)arg);

    while(true) {
        int item = rand()%1000;

        sem_wait(&empty_slots);
        pthread_mutex_lock(&buffer_mutex);

        if (produced_count >= MAX_ITEMS){
            pthread_mutex_unlock(&buffer_mutex);
            sem_post(&empty_slots);

            break;
        } else {
            push_item(item);
        }

        printf(
            "\nProducer %d produced item %d with value %d at buffer position %d\n",
            id,
            produced_count,
            item,
            available_items
        );

        pthread_mutex_unlock(&buffer_mutex);
        sem_post(&filled_slots);
    }

    pthread_exit(0);
}

void * consumer(void *arg) {
    int id = * ((int *)arg);
    int item;

    while(true) {
        sem_wait(&filled_slots); // reserva um item
        pthread_mutex_lock(&buffer_mutex);

        if (consumed_count >= MAX_ITEMS){
            pthread_mutex_unlock(&buffer_mutex);
            sem_post(&filled_slots);
            
            break;
        } else {
            item = pop_item();
        }

        printf(
            "\nConsumer %d consumed item %d with value %d at buffer position %d\n",
            id,
            consumed_count,
            item,
            available_items
        );
        pthread_mutex_unlock(&buffer_mutex);

        // como retirou um item, liberou uma posição do buffer
        sem_post(&empty_slots);

        // edge case quadno o cunsumidor pega o ultimo produto
        if (consumed_count >= MAX_ITEMS){
            sem_post(&filled_slots);

        }
    }

    pthread_exit(0);
}

int main(){
    pthread_t producer_threads[NUM_PRODUCERS], consumer_threads[NUM_CONSUMERS];

    int producer_ids[NUM_PRODUCERS];
    int consumer_ids[NUM_CONSUMERS];

    // todas posicoes do buffer vazias
    sem_init(&empty_slots, 0, BUFFER_SIZE);

    // nenhum item cheio
    sem_init(&filled_slots, 0, 0);

    for (int i = 0; i < NUM_PRODUCERS; i++){
        producer_ids[i] = i;
        pthread_create(
            &producer_threads[i],
            NULL,
            producer, //funcao do producer
            &producer_ids[i]
        );
    }

    for (int i = 0; i < NUM_CONSUMERS; i++){
        consumer_ids[i] = i;
        pthread_create(
            &consumer_threads[i],
            NULL,
            consumer, 
            &consumer_ids[i]
        );
    }
    
    for (int i = 0; i < NUM_PRODUCERS; i++){
        pthread_join(
            producer_threads[i],
            NULL
        );
    }

    for (int i = 0; i < NUM_CONSUMERS; i++){
        pthread_join(
            consumer_threads[i],
            NULL
        );
    }

    sem_destroy(&empty_slots);
    sem_destroy(&filled_slots);

    pthread_mutex_destroy(&buffer_mutex);



    return 0;
}

