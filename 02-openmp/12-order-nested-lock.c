#include <stdio.h>
#include <omp.h>

double order_total = 1000.0;
omp_nest_lock_t order_lock;

void apply_discount(double percentage) {
    int id = omp_get_thread_num();

    printf("[thread %d] apply_discount: tentando pegar o lock novamente\n", id);

    omp_set_nest_lock(&order_lock);

    printf("[thread %d] apply_discount: pegou o lock pela segunda vez\n", id);
    printf("[thread %d] apply_discount: total antes = %.2f\n", id, order_total);

    // aplica o desconto
    order_total -= order_total * percentage;

    printf("[thread %d] apply_discount: total depois = %.2f\n", id, order_total);
    printf("[thread %d] apply_discount: liberando o lock interno\n", id);

    omp_unset_nest_lock(&order_lock);
}

void update_order(int thread_id) {

    printf("\n[thread %d] update_order: tentando pegar o lock\n", thread_id);

    omp_set_nest_lock(&order_lock);

    printf("[thread %d] update_order: pegou o lock\n", thread_id);
    printf("[thread %d] update_order: total antes = %.2f\n",
           thread_id, order_total);

    // acrescenta um item ao pedido
    order_total += 100.0;

    printf("[thread %d] update_order: adicionou 100. total = %.2f\n",
           thread_id, order_total);

    if (thread_id % 2 == 0) {
        printf("[thread %d] update_order: thread par, chamando apply_discount\n",
               thread_id);

        apply_discount(0.10);

        printf("[thread %d] update_order: voltou de apply_discount\n",
               thread_id);
    }

    printf("[thread %d] update_order: liberando o lock externo\n",
           thread_id);

    omp_unset_nest_lock(&order_lock);
}

int main() {

    omp_init_nest_lock(&order_lock);

    #pragma omp parallel num_threads(4)
    {
        int id = omp_get_thread_num();
        update_order(id);
    }

    omp_destroy_nest_lock(&order_lock);

    printf("\nFinal order total: %.2f\n", order_total);

    return 0;
}