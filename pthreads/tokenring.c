#include <stdio.h>
#include <stdlib.h>
#include <semaphore.h>
#include <pthread.h>

#define T 5

int token = 0;

pthread_t thread_handler[T];
sem_t sempahore_handler[T];

void *thread_function(void *p_ref){
    int p = * ((int *) p_ref);  

    sem_wait(&sempahore_handler[p]);

    printf("Thread %d with token %d\n", p, token);
    fflush(0);
    token++;

    sem_post(&sempahore_handler[(p + 1) % T]);
}

int main(void){
    for (int i = 0; i < T; i++){
        sem_init(&sempahore_handler[i], 0, 0);
    }

    int p[T];

    for (int i = 0; i < T; i++){
        p[i] = i;

        if (pthread_create(&thread_handler[i], 0, (void *) thread_function, (void *) &p[i]) != 0){
            printf("Error!");
            fflush(0);
        }
    }

    sem_post(&sempahore_handler[0]);

    for (int i = 0; i < T; i++){
        pthread_join(thread_handler[i], 0);
    }

    printf("Main thread exiting");


}