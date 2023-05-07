#include <mpi.h>
#include <stdio.h>

#define N 1000
#define BLOCK_SIZE 250

int main(int argc, char** argv) {
    int num_processes, my_rank;
    int A[BLOCK_SIZE][N];
    int x[N], y[BLOCK_SIZE];
    int i, j;

    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &num_processes);
    MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);

    // Inizializza la matrice A e il vettore x
    for (i = 0; i < BLOCK_SIZE; i++) {
        for (j = 0; j < N; j++) {
            A[i][j] = i + j;
        }
    }
    for (i = 0; i < N; i++) {
        x[i] = i;
    }

    // Calcola il prodotto matrice-vettore distribuito tra i processi
    for (i = 0; i < BLOCK_SIZE; i++) {
        y[i] = 0;
        for (j = 0; j < N; j++) {
            y[i] += A[i][j] * x[j];
        }
    }

    // Invia i risultati parziali ad un processo master per la somma finale
    if (my_rank == 0) {
        int recv_buf[BLOCK_SIZE];
        for (i = 1; i < num_processes; i++) {
            MPI_Recv(recv_buf, BLOCK_SIZE, MPI_INT, i, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            for (j = 0; j < BLOCK_SIZE; j++) {
                y[i * BLOCK_SIZE + j] = recv_buf[j];
            }
        }
    } else {
        MPI_Send(y, BLOCK_SIZE, MPI_INT, 0, 0, MPI_COMM_WORLD);
    }

    MPI_Finalize();
    return 0;
}
