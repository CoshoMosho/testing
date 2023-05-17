#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#include <omp.h>

int main(int argc, char *argv[]) {
    int rank, size;
    int start, end, chunk_size;
    int i, sum = 0, partial_sum = 0;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    // Definisci il lavoro di ogni processo
    chunk_size = 100 / size;
    start = rank * chunk_size + 1;
    end = start + chunk_size - 1;

    // Suddivide il lavoro del for loop utilizzando OpenMP
    #pragma omp parallel for schedule(static)
    for (i = start; i <= end; i++) {
        partial_sum += i;
    }

    // Somma parziale di ogni processo
    printf("Processo %d - Somma parziale: %d\n", rank, partial_sum);

    // Riduzione delle somme parziali di ogni processo
    MPI_Allreduce(&partial_sum, &sum, 1, MPI_INT, MPI_SUM, MPI_COMM_WORLD);

    // Stampa il risultato da ogni processo
    printf("Processo %d - Somma totale: %d\n", rank, sum);

    MPI_Finalize();
    return 0;
}
