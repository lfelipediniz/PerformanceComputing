#include <stdio.h>
#include <omp.h>

#define NUM_THREADS 4

int main() {
    double balance;

    #pragma omp parallel num_threads(NUM_THREADS)
    {
        #pragma omp single
        {
            // eu sou a task reposavel por produzir um valor pra balance
            #pragma omp task depend(out: balance)
            {
                balance = 1000.0;
                printf(
                    "Thread %d loaded balance: %.2f\n",
                    omp_get_thread_num(),
                    balance
                );
            }

            // eu preciso que alguem tenha produzido balance antes de executar (nao vou alterar o seu valor)
            #pragma omp task depend(in: balance)
            printf("Thread %d checked balance: %.2f\n", omp_get_thread_num(), balance);


            // eu preciso que alguem tenha produzido balance e vou modifica-lo
            #pragma omp task depend(inout: balance)
            {
                balance -= 200.0;

                printf(
                    "Thread %d processed payment. New balance: %.2f\n",
                    omp_get_thread_num(),
                    balance
                );
            }

            // vai esperar o inout modificar o balance para poder ler ele novamente
            #pragma omp task depend(in: balance)
            printf("Thread %d final balance: %.2f\n", omp_get_thread_num(), balance );
        }
    }

    return 0;
}