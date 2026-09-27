#include <stdio.h>
#include <omp.h>

#define NUM_ORDERS 8
#define N1 6

void process_order(int order_id) {
    printf(
        "Thread %d processing order %d\n",
        omp_get_thread_num(),
        order_id
    );
}

int main() {

    #pragma omp parallel num_threads(N1)
    {
        // apenas uma thread entra aqui para criar as tarefas
        #pragma omp single
        {
            printf(
                "Thread %d is creating the tasks\n\n",
                omp_get_thread_num()
            );

            for (int order_id = 1; order_id <= NUM_ORDERS; order_id++) {
                #pragma omp task
                process_order(order_id);
            }

            // espera todas as tarefas criadas acima terminarem
            #pragma omp taskwait

            printf(
                "\nThread %d: all orders were processed\n",
                omp_get_thread_num()
            );
        }
    }

    return 0;
}