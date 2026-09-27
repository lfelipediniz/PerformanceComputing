#include <stdio.h>
#include <omp.h>

#define NT1 4
#define NT2 2
#define NUM_PRODUCTS 4

void process_category(const char *category, double prices[], int num_products,  double base_discount) {
    // desconto inicial definido pela categoria
    double discount = base_discount;

    // cada thread interna recebe uma copia inicializada de discount
    #pragma omp parallel num_threads(NT2) firstprivate(discount)
    {
        int channel = omp_get_thread_num();

        // cada canal modifica apenas sua propria copia do desconto
        // demosntracao que o firstprivate esta funcionando
        if (channel == 1) {
            discount += 0.05;
        }

        // distribui os produtos entre as threads internas ja criadas
        #pragma omp for
        for (int product = 0; product < num_products; product++) {

            double final_price =
                prices[product] - (prices[product] * discount);

            printf(
                "categoria=%s | produto=%d | channel=%d | "
                "desconto=%.0f%% | preco=%.2f | final=%.2f\n",
                category,
                product,
                channel,
                discount * 100,
                prices[product],
                final_price
            );
        }

        // 1. parallel cria T0 e T1
        // 2. cada thread descobre seu channel
        // 3. cada thread prepara sua copia de discount
        // 4. omp for divide os produtos entre T0 e T1
    }
}

int main() {

    double electronics[NUM_PRODUCTS] = {
        1000.0,
        2000.0,
        3500.0,
        500.0
    };

    double books[NUM_PRODUCTS] = {
        50.0,
        80.0,
        120.0,
        40.0
    };

    double clothes[NUM_PRODUCTS] = {
        100.0,
        200.0,
        150.0,
        300.0
    };

    double food[NUM_PRODUCTS] = {
        20.0,
        30.0,
        40.0,
        50.0
    };

    // permite que uma regiao paralela crie outro time de threads
    omp_set_nested(1);

    #pragma omp parallel num_threads(NT1)
    {
        // distribui as categorias entre as threads do primeiro nivel
        // cada sections sao blocos independentes de trabalho que 
        // pode ser distribuidos entre as threads 
        #pragma omp sections
        {
            #pragma omp section
            process_category( "Eletronicos", electronics, NUM_PRODUCTS, 0.10);
        

            #pragma omp section
            process_category("Livros", books, NUM_PRODUCTS, 0.20);
        

            #pragma omp section
            process_category("Roupas", clothes, NUM_PRODUCTS, 0.15);

            #pragma omp section
            process_category("Alimentos", food, NUM_PRODUCTS, 0.05 );
        
        }
    }

    return 0;
}