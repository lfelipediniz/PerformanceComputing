#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

#define N 1000
#define NT1 2
#define NT2 4

int main() {

    omp_set_nested(1); // permitindo palelismo aninhado
    // nova equipe de theads sendo formada dentro do for

    #pragma omp parallel for num_threads(NT1)
    for (int i = 1; i <= N; i++) {
        int sum = 0;
        int thread_i = omp_get_thread_num();

        #pragma omp parallel for num_threads(NT2) reduction(+:sum)
        for (int j = 1; j < i; j++) {

            int thread_j = omp_get_thread_num();

            // printf(
            //     "i=%d | j=%d | thread_i=%d | thread_j=%d\n",
            //     i, j, thread_i, thread_j
            // );

            if (i % j == 0) {
                sum += j;
            }
        }

        if (sum == i) {
            printf("PERFEITO: %d\n", i);
        }
    }

    return 0;
}