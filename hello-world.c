#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

#define NUM_THREADS 4


int main(){
    printf("Hello World na parte sequencial!\n");

    #pragma omp parallel num_threads(NUM_THREADS) 
    {
        int thread_id = omp_get_thread_num(); // qual o id da thread que eu esotu agora?
        int num_threads = omp_get_num_threads(); // quantas threads estao trabalhando comigo?

        printf("Hello World! Eu estou na thread: %d, e tem %d threads trabalhando cmg\n",
        thread_id,
        num_threads
        );
    }

    return 0;
}
