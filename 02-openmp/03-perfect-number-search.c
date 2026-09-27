#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

#define MAX_NUMBER 1000
#define OUTER_THREADS 2
#define INNER_THREADS 4

int main() {

    omp_set_nested(1); // permitindo palelismo aninhado
    // nova equipe de theads sendo formada dentro do for

    #pragma omp parallel for num_threads(OUTER_THREADS)
    for (int i = 1; i <= MAX_NUMBER; i++) {
        int sum = 0;

        #pragma omp parallel for num_threads(INNER_THREADS) reduction(+:sum)
        for (int j = 1; j < i; j++) {
            if (i % j == 0) {
                sum += j;
            }
        }

        if (sum == i) {
            printf("PERFECT: %d\n", i);
        }
    }

    return 0;
}
