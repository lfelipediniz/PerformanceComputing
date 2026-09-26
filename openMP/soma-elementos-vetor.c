 #include <stdio.h>
 #include <stdlib.h>
 #include <omp.h>

#define NUM_THREADS 10

 int main() {
    #define ARRAY_LEN 10

    int values[10];
    int sum = 0;
    int sum_par = 0;

    #pragma omp parallel for num_threads(NUM_THREADS) 
    for (int i = 0; i < ARRAY_LEN; i++){ // como eu declaro o i aqui dentro ele já é private
            values[i] = rand() % 1000;
    }

    # pragma omp parallel for num_threads(NUM_THREADS) reduction(+:sum)
    for (int i = 0; i < ARRAY_LEN; i++){
        sum += values[i];
    }

    printf("\n\n%d", sum);

    return 0;
}