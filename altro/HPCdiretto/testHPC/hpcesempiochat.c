#include <stdio.h>
#include <mpi.h>

int main(int argc, char** argv) {
    int rank, size, tag = 0;
    int data = 0;
    MPI_Status status;

    // Inizializzazione di MPI
    MPI_Init(&argc, &argv);

    // Ottenere il numero di processi
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    // Ottenere il rank del processo corrente
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    // Se ci sono meno di due processi, uscire
    if (size < 2) {
        printf("Questo programma richiede almeno due processi.\n");
        MPI_Finalize();
        return 1;
    }

    // Processo 0 invia un messaggio a processo 1
    if (rank == 0) {
        data = 42;
        MPI_Send(&data, 1, MPI_INT, 1, tag, MPI_COMM_WORLD);
        printf("Processo %d ha inviato il valore %d.\n", rank, data);
    }
    // Processo 1 riceve il messaggio da processo 0
    else if (rank == 1) {
        MPI_Recv(&data, 1, MPI_INT, 0, tag, MPI_COMM_WORLD, &status);
        printf("Processo %d ha ricevuto il valore %d.\n", rank, data);
    }

    // Finalizzazione di MPI
    MPI_Finalize();
    return 0;
}
