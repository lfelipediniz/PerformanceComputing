#include <stdio.h>
#include <omp.h>

#define NUM_OPERATIONS 100000

double balance = 1000.0;
omp_lock_t balance_lock;

int main() {

    omp_init_lock(&balance_lock); // iniciando o lock

    #pragma omp parallel for num_threads(4)
    for (int i = 0; i < NUM_OPERATIONS; i++) {

        // so pra vermos diferenca nos valores (e realmente muda com a ausencia do lock)
        double value = (i % 2 == 0) ? 10.0 : -5.0;

        omp_set_lock(&balance_lock); // regiao critica
        // atualiza o recurso compartilhado
        balance += value;

        omp_unset_lock(&balance_lock);
    }

    omp_destroy_lock(&balance_lock);

    printf("Final balance: %.2f\n", balance);

    return 0;
}