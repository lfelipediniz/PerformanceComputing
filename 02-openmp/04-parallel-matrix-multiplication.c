#include <stdio.h>
#include <stdlib.h>

#define ROWS_A 2
#define COLS_A 3

#define ROWS_B 3
#define COLS_B 4

#define NUM_THREADS 8

int main() {
    int matrix_a[ROWS_A][COLS_A];
    int matrix_b[ROWS_B][COLS_B];
    int result_matrix[ROWS_A][COLS_B];

    // preenchendo a matriz A
    #pragma omp parallel for num_threads(NUM_THREADS) collapse(2)
    for (int i = 0; i < ROWS_A; i++) {
        for (int j = 0; j < COLS_A; j++) {
            matrix_a[i][j] = i + 1;
        }
    }

    // explicacao do collapse
    // collapse permite uma melhor divisao do trabalho entre as threads
    // ele junta logicamente os lacos aninhados em um unico espaco de iteracoes
    //
    // sem collapse o OpenMP dividiria apenas o primeiro for
    //
    // exemplo:
    // i = 2
    // j = 1000
    //
    // sem collapse:
    // apenas 2 iteracoes seriam divididas entre as threads
    // entao mesmo tendo 8 threads, apenas 2 poderiam receber trabalho
    //
    // com collapse(2):
    // o OpenMP considera todas as combinacoes de i e j
    //
    // 2 * 1000 = 2000 trabalhos
    //
    // importante:
    // collapse nao transforma a matriz fisicamente em um vetor
    // ele apenas trata os lacos como um unico espaco de iteracoes para distribuir o trabalho

    // preenchendo a matriz B
    #pragma omp parallel for num_threads(NUM_THREADS) collapse(2)
    for (int i = 0; i < ROWS_B; i++) {
        for (int j = 0; j < COLS_B; j++) {
            matrix_b[i][j] = j + 1;
        }
    }

    // multiplicando A por B
    #pragma omp parallel for num_threads(NUM_THREADS) collapse(2)
    for (int i = 0; i < ROWS_A; i++) {
        for (int j = 0; j < COLS_B; j++) {

            int sum = 0;

            // explicacao da divisao do trabalho
            //
            // i e j escolhem qual posicao da matriz C sera calculada
            //
            // exemplo:
            // i = 0 e j = 0 -> calcula C[0][0]
            // i = 0 e j = 1 -> calcula C[0][1]
            // i = 1 e j = 0 -> calcula C[1][0]
            //
            // essas posicoes sao independentes umas das outras
            // por isso i e j podem ser paralelizados com collapse(2)
            //
            // k tem uma funcao diferente
            // ele nao escolhe uma nova posicao de C
            // ele calcula as parcelas que formam a mesma posicao C[i][j]
            //
            // exemplo:
            //
            // C[i][j] =
            // A[i][0] * B[0][j]
            // +
            // A[i][1] * B[1][j]
            // +
            // A[i][2] * B[2][j]
            //
            // por isso nao estamos usando collapse(3)
            // todos os valores de k contribuem para o mesmo sum

            #pragma omp simd reduction(+:sum)
            for (int k = 0; k < COLS_A; k++) {
                sum += matrix_a[i][k] * matrix_b[k][j];
            }

            // explicacao do simd reduction
            //
            // simd permite que varias iteracoes de k sejam processadas
            // usando instrucoes vetoriais do processador
            //
            // por exemplo, para um mesmo C[i][j]:
            //
            // k = 0 -> A[i][0] * B[0][j]
            // k = 1 -> A[i][1] * B[1][j]
            // k = 2 -> A[i][2] * B[2][j]
            //
            // essas multiplicacoes podem ser feitas usando SIMD
            //
            // reduction(+:sum) informa que todas essas operacoes
            // contribuem para a mesma variavel sum
            //
            // cada resultado parcial e somado corretamente no final
            //
            // entao temos dois niveis de paralelismo:
            //
            // collapse(2) -> paralelismo entre diferentes C[i][j]
            // simd reduction -> paralelismo dentro do calculo de um C[i][j]

            result_matrix[i][j] = sum;
        }
    }

    // imprimindo a matriz A
    printf("Matrix A:\n");

    for (int i = 0; i < ROWS_A; i++) {
        printf("| ");

        for (int j = 0; j < COLS_A; j++) {
            printf("%d ", matrix_a[i][j]);
        }

        printf("|\n");
    }

    // imprimindo a matriz B
    printf("\nMatrix B:\n");

    for (int i = 0; i < ROWS_B; i++) {
        printf("| ");

        for (int j = 0; j < COLS_B; j++) {
            printf("%d ", matrix_b[i][j]);
        }

        printf("|\n");
    }

    // imprimindo a matriz C
    printf("\nMatrix C = A x B:\n");

    for (int i = 0; i < ROWS_A; i++) {
        printf("| ");

        for (int j = 0; j < COLS_B; j++) {
            printf("%d ", result_matrix[i][j]);
        }

        printf("|\n");
    }

    return 0;
}
