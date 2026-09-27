#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

#define NUM_THREADS 6

int main() {
    int number = 67;
    int sum = 0;

    // reduction evita race codition em sum pois cada thread tem seu sum local durante 
    // sua execucao, no final tudo é somado
    #pragma omp parallel for num_threads(NUM_THREADS) reduction(+:sum)
    for(int i = 1; i < number; i++){
        if(number % i == 0) {
            sum += i;
        }
    }

    if (sum == number){
        printf("%d is perfect\n", number);
    } else {
        printf("%d is not perfect\n", number);
    }



    return 0;
}

