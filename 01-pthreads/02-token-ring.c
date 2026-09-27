#include <stdio.h>
#include <stdlib.h>
#include <semaphore.h>
#include <pthread.h>

#define NUM_THREADS 5

int token = 0;

pthread_t threads[NUM_THREADS];
sem_t semaphores[NUM_THREADS];

void *thread_function(void *thread_id_reference){
    int thread_id = * ((int *) thread_id_reference);

    sem_wait(&semaphores[thread_id]);

    printf("Thread %d with token %d\n", thread_id, token);
    fflush(0);
    token++;

    sem_post(&semaphores[(thread_id + 1) % NUM_THREADS]);

    return NULL;
}

int main(void){
    for (int i = 0; i < NUM_THREADS; i++){
        sem_init(&semaphores[i], 0, 0);
    }

    int thread_ids[NUM_THREADS];

    for (int i = 0; i < NUM_THREADS; i++){
        thread_ids[i] = i;

        if (pthread_create(&threads[i], 0, (void *) thread_function, (void *) &thread_ids[i]) != 0){
            printf("Error!");
            fflush(0);
        }
    }

    sem_post(&semaphores[0]);

    for (int i = 0; i < NUM_THREADS; i++){
        pthread_join(threads[i], 0);
    }

    printf("Main thread exiting");


}
