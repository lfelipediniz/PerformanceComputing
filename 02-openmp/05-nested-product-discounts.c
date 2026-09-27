#include <stdio.h>
#include <omp.h>

#define NUM_PRODUCTS 2
#define NUM_DISCOUNTS 3

int main() {

    double prices[NUM_PRODUCTS] = {100.0, 200.0};
    double discounts[NUM_DISCOUNTS] = {0.10, 0.20, 0.30};

    omp_set_nested(1);

    #pragma omp parallel for num_threads(2)
    for (int product = 0; product < NUM_PRODUCTS; product++) {

        double price = prices[product]; // ex: price = 100

        
        #pragma omp parallel for num_threads(3) firstprivate(price)
        for (int discount = 0; discount < NUM_DISCOUNTS; discount++) {

            price -= (price * discounts[discount]);

            printf(
                "product=%d discount=%.0f%% price=%.2f\n",
                product,
                discounts[discount] * 100,
                price
            );

            // firstprivate é usado porque cada thread precisa de uma cópia 
            // de price já inicializada
            // ja o private criaria a cópia sem preservar o valor atual (ex: 100)
        }
    }

    return 0;
}
