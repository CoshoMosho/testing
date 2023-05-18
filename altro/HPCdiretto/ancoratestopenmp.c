#include <stdio.h>
#include <omp.h>

#define SIZE 1000000000

int main() {
    int i;
    int sum = 0;
    
    double start_time = omp_get_wtime(); // Tempo di inizio
    
    #pragma omp parallel for reduction(+:sum)
    for (i = 0; i < SIZE; i++) {
        sum += i;
    }
    
    double end_time = omp_get_wtime(); // Tempo di fine
    double elapsed_time = end_time - start_time; // Tempo trascorso
    
    printf("Risultato: %d\n", sum);
    printf("Tempo di esecuzione: %.6f secondi\n", elapsed_time);
    
    return 0;
}
