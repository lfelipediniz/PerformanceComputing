// implementando o algoritmo de distribuicao do guided
#include <stdio.h>
#include <omp.h>

#define THREADS 4
#define N 40

omp_lock_t lock;


int main(void) {

    int proxima = 0;

    omp_lock_t lock;
    omp_init_lock(&lock);


    #pragma omp parallel num_threads(4) shared(proxima, lock)
    {
        int num_threads = omp_get_num_threads();
        int id = omp_get_thread_num();

        while(1) {
            int inicio;
            int fim;
            int chunk;
            int restantes;


            omp_set_lock(&lock);

            // reserva_trabalho();

            if (proxima >= N) {
                omp_unset_lock(&lock);
                break;
            }

            restantes = N - proxima;

            chunk = restantes / num_threads;

            if (chunk < 1) {
                chunk = 1;
            }

            inicio = proxima;
            fim = inicio + chunk;

            if (fim > N) {
                fim = N;
            }

            proxima = fim;

            printf("Thread %d recebeu iteracoes (%d, %d)\n", id, inicio, fim);
            fflush(0);

            omp_unset_lock(&lock);

            //executa_trabalho();

            for (int i = inicio; i < fim; i++) {
                printf("Thread %d executando iteracao %d\n", id, i);
                fflush(0);
            }


        }
    }

    omp_destroy_lock(&lock);

    return 0;


}