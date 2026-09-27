#include <stdio.h>
#include <unistd.h>
#include <omp.h>

#define NUM_THREADS 4
#define NUM_ORDERS 8

void process_order(int order_id) {

    // simula o tempo necessário para processar um pedido
    usleep(500000);

    printf(
        "    Worker thread %d processed order %d\n",
        omp_get_thread_num(),
        order_id
    );
}

int main() {

    #pragma omp parallel num_threads(NUM_THREADS)
    {
        // apenas uma thread cria as tasks
        // nowait permite que as outras threads não esperem o single terminar
        #pragma omp single nowait
        {
            printf(
                "Thread %d is the task producer\n\n",
                omp_get_thread_num()
            );

            for (int order_id = 1; order_id <= NUM_ORDERS; order_id++) {

                printf(
                    "Producer thread %d created order %d\n",
                    omp_get_thread_num(),
                    order_id
                );

                #pragma omp task
                process_order(order_id);

                // deixa a criação mais lenta para visualizar a sobreposição
                usleep(200000);
            }
        }
    }

    return 0;
}