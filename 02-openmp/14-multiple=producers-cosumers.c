#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include <stdbool.h>

#define BUFFER_SIZE 7
#define MAX_ITEMS 100

#define NUM_PRODUCERS 3
#define NUM_CONSUMERS 4

int buffer[BUFFER_SIZE];

int available_items = 0;
int produced_count = 0;
int consumed_count = 0;

omp_lock_t buffer_lock;


void push_item(int item) {
    buffer[available_items] = item;
    available_items++;
    produced_count++;
}


int pop_item() {
    available_items--;
    consumed_count++;

    return buffer[available_items];
}


void producer(int id) {

    while (true) {
        int finished = false;

        // entrando na região crítica
        omp_set_lock(&buffer_lock);

        if (produced_count >= MAX_ITEMS) {
            finished = true;

        } else if (available_items < BUFFER_SIZE) {
            int item = rand() % 1000;

            push_item(item);

            printf(
                "Producer %d produced item %d with value %d at buffer position %d\n",
                id,
                produced_count,
                item,
                available_items
            );
        }

        // saindo da região crítica
        omp_unset_lock(&buffer_lock);

        if (finished) {
            break;
        }
    }
}


void consumer(int id) {
    while (true) {
        int finished = false;

        // entrando na região crítica
        omp_set_lock(&buffer_lock);

        if (consumed_count >= MAX_ITEMS) {
            finished = true;
        } else if (available_items > 0) {

            int item = pop_item();

            printf(
                "Consumer %d consumed item %d with value %d at buffer position %d\n",
                id,
                consumed_count,
                item,
                available_items
            );

        } else if ( produced_count >= MAX_ITEMS && available_items == 0) {
            finished = true;
        }

        // saindo da região crítica
        omp_unset_lock(&buffer_lock);

        if (finished) {
            break;
        }
    }
}


int main() {
    omp_init_lock(&buffer_lock);

    #pragma omp parallel num_threads(NUM_PRODUCERS + NUM_CONSUMERS)
    {
        int id = omp_get_thread_num();

        if (id < NUM_PRODUCERS) {
            producer(id);

        } else {
            consumer(id - NUM_PRODUCERS);
        }
    }

    omp_destroy_lock(&buffer_lock);

    return 0;
}