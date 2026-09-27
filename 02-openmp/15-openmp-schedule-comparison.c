#include <stdio.h>
#include <omp.h>

#define N 16
#define NUM_THREADS 4

void trabalho(int milissegundos) {
    double inicio = omp_get_wtime();

    // simula uma computação que ocupa a cpu
    while ((omp_get_wtime() - inicio) < milissegundos / 1000.0) {
    }
}

void mostrar_resultado(const char *nome, int thread_de_cada_iteracao[], double tempo) {
    printf("\n============================\n");
    printf("schedule(%s)\n", nome);
    printf("============================\n");

    for (int i = 0; i < N; i++) {
        printf(
            "iteracao %2d | custo %3d ms | thread %d\n",
            i,
            (i + 1) * 10,
            thread_de_cada_iteracao[i]
        );
    }

    printf("tempo total: %.3f segundos\n", tempo);
}

void testar_static() {
    int thread_de_cada_iteracao[N];

    double inicio = omp_get_wtime();

    #pragma omp parallel for schedule(static)
    for (int i = 0; i < N; i++) {

        int id = omp_get_thread_num();

        thread_de_cada_iteracao[i] = id;

        trabalho((i + 1) * 10);
    }

    double fim = omp_get_wtime();

    mostrar_resultado(
        "static",
        thread_de_cada_iteracao,
        fim - inicio
    );
}

void testar_dynamic() {
    int thread_de_cada_iteracao[N];

    double inicio = omp_get_wtime();

    #pragma omp parallel for schedule(dynamic, 1)
    for (int i = 0; i < N; i++) {

        int id = omp_get_thread_num();

        thread_de_cada_iteracao[i] = id;

        trabalho((i + 1) * 10);
    }

    double fim = omp_get_wtime();

    mostrar_resultado(
        "dynamic",
        thread_de_cada_iteracao,
        fim - inicio
    );
}

void testar_guided() {
    int thread_de_cada_iteracao[N];

    double inicio = omp_get_wtime();

    #pragma omp parallel for schedule(guided, 1)
    for (int i = 0; i < N; i++) {

        int id = omp_get_thread_num();

        thread_de_cada_iteracao[i] = id;

        trabalho((i + 1) * 10);
    }

    double fim = omp_get_wtime();

    mostrar_resultado(
        "guided",
        thread_de_cada_iteracao,
        fim - inicio
    );
}

int main() {

    omp_set_dynamic(0);
    omp_set_num_threads(NUM_THREADS);

    testar_static();
    testar_dynamic();
    testar_guided();

    return 0;
}