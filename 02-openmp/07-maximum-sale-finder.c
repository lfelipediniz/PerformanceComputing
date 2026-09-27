#include <stdio.h>
#include <omp.h>

#define NUM_SALES 8
#define NUM_THREADS 4

int main() {

    double sales[NUM_SALES] = {
        1200.0,
        850.0,
        3100.0,
        1700.0,
        4500.0,
        900.0,
        2800.0,
        3900.0
    };

    double max_sale = 0;
    int best_seller = -1;

    #pragma omp parallel num_threads(NUM_THREADS)
    {
        double local_max = 0;
        int local_seller = -1;

        // distribuindo trabalho entre as threads
        #pragma omp for
        for (int i = 0; i < NUM_SALES; i++) {
            if (sales[i] > local_max) {
                local_max = sales[i];
                local_seller = i;
            }
        }

        // cria regicao sequencial para evitar race condition
        // combinando resultados globais
        #pragma omp critical
        {
            if (local_max > max_sale) {
                max_sale = local_max;
                best_seller = local_seller;
            }
        }
    }

    printf(
        "Highest sale: %.2f | Seller: %d\n",
        max_sale,
        best_seller
    );

    return 0;
}
