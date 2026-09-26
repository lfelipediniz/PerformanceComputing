#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <stdbool.h>


#define BUFFER_LEN 7
#define MAX_PRODUCED 100

#define NUM_PROD 3
#define NUM_CONS 4

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

void * producer(void * arg){
    int id = * ((int *)arg);

    while(true) {
        int item = rand()%1000;

        sem_wait(&empty);
        pthread_mutex_lock(&mutex);

        if (produced >= MAX_PRODUCED){
            pthread_mutex_unlock(&mutex);
            sem_post(&empty);

            break;
        } else {
            add_stack(item);
        }

        printf(
            "\nProdutor %d produziu o item %d com valor %d na posição %d do buffer\n",
            id,
            produced,
            item,
            item_available
        );

        pthread_mutex_unlock(&mutex);
        sem_post(&full);
    }

    pthread_exit(0);
}

void * consumer(void *arg) {
    int id = * ((int *)arg);
    int item;

    while(true) {
        sem_wait(&full); // reserva um item
        pthread_mutex_lock(&mutex);

        if (consumed >= MAX_PRODUCED){
            pthread_mutex_unlock(&mutex);
            sem_post(&full);
            
            break;
        } else {
            item = pop_stack();
        }

        printf(
            "\nConsumidor %d consumiu o item %d com valor %d na posição %d do buffer\n",
            id,
            consumed,
            item,
            item_available
        );
        pthread_mutex_unlock(&mutex);

        // como retirou um item, liberou uma posição do buffer
        sem_post(&empty);

        // edge case quadno o cunsumidor pega o ultimo produto
        if (consumed >= MAX_PRODUCED){
            sem_post(&full);

        }
    }

    pthread_exit(0);
}

int main(){
    pthread_t prod_handle[NUM_PROD], cons_handle[NUM_CONS];

    int prod_id[NUM_PROD];
    int cons_id[NUM_CONS];

    // todas posicoes do buffer vazias
    sem_init(&empty, 0, BUFFER_LEN);

    // nenhum item cheio
    sem_init(&full, 0, 0);

    for (int i = 0; i < NUM_PROD; i++){
        prod_id[i] = i;
        pthread_create(
            &prod_handle[i],
            NULL,
            producer, //funcao do producer
            &prod_id[i]
        );
    }

    for (int i = 0; i < NUM_CONS; i++){
        cons_id[i] = i;
        pthread_create(
            &cons_handle[i],
            NULL,
            consumer, 
            &cons_id[i]
        );
    }
    
    for (int i = 0; i < NUM_PROD; i++){
        pthread_join(
            prod_handle[i],
            NULL
        );
    }

    for (int i = 0; i < NUM_CONS; i++){
        pthread_join(
            cons_handle[i],
            NULL
        );
    }

    sem_destroy(&empty);
    sem_destroy(&full);

    pthread_mutex_destroy(&mutex);



    return 0;
}


