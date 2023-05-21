#include <stdio.h>
#include <omp.h>

int main() {
    int num_threads, i;
    int num_iterations = 100;

    #pragma omp parallel
    {
        num_threads = omp_get_num_threads(); // Ottieni il numero di thread paralleli disponibili

        #pragma omp for
        for (i = 0; i < num_iterations; i++) {
            int thread_id = omp_get_thread_num(); // Ottieni l'ID del thread corrente
            printf("Thread %d esegue l'iterazione %d\n", thread_id, i);
            // Esegui il lavoro corrispondente all'iterazione
        }
    }

    return 0;
}
