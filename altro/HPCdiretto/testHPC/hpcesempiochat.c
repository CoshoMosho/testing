#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

int main(int argc, char** argv) {
    int rank, size;
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    int data = rank; // dati da inviare

    MPI_Status status;
    if (rank != 0) {
        MPI_Send(&data, 1, MPI_INT, (rank + 1) % size, 0, MPI_COMM_WORLD);
    } else {
        MPI_Send(&data, 1, MPI_INT, 1, 0, MPI_COMM_WORLD);
    }

    MPI_Recv(&data, 1, MPI_INT, (rank - 1 + size) % size, 0, MPI_COMM_WORLD, &status);

    printf("Processo %d: ho ricevuto il valore %d dal processo %d\n", rank, data, (rank - 1 + size) % size);

    MPI_Finalize();
    return 0;
}
