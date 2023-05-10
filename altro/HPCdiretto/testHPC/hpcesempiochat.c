#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

int main(int argc, char** argv) {
    int rank, size;
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    int data = 0; // dati da inviare

    MPI_Status status;

    for (size_t i = 0; i < 100; i++)
    {
        data = i;
    if (rank != 0) {
        MPI_Send(&data, 1, MPI_INT, (rank + 1) % size, 0, MPI_COMM_WORLD);
    } else {
        MPI_Send(&data, 1, MPI_INT, 1, 0, MPI_COMM_WORLD);
    }

    MPI_Recv(&data, 1, MPI_INT, (rank - 1 + size) % size, 0, MPI_COMM_WORLD, &status);

    if (data == i + 1)
    {
        printf("iterazione %d di processo %d: ho ricevuto il valore %d dal processo %d\n",i, rank, data, (rank - 1 + size) % size);
    }else{
        printf("errore del cazzo");
    }
    
    
    }
    
    MPI_Finalize();
    return 0;
}


/* int main(int argc, char** argv) {
    int size, rank;
    int n = 5;  // numero di iterazioni
    int message;  // messaggio da inviare e ricevere
    MPI_Status status;  // stato del receive
    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    for (int i = 0; i < n; i++) {
        // invio il messaggio al processo successivo
        MPI_Send(&message, 1, MPI_INT, (rank + 1) % size, 0, MPI_COMM_WORLD);
        // ricevo il messaggio dal processo precedente
        MPI_Recv(&message, 1, MPI_INT, (rank - 1 + size) % size, 0, MPI_COMM_WORLD, &status);
        // controllo che il messaggio ricevuto sia quello relativo all'iterazione corrente
        int expected_message = (rank + size - i) % size;
        if (message != expected_message) {
            printf("Errore: processo %d ha ricevuto un messaggio errato: %d invece di %d\n", rank, message, expected_message);
            MPI_Abort(MPI_COMM_WORLD, 1);
        }
        printf("Iterazione %d: processo %d ha ricevuto il messaggio %d\n", i, rank, message);
    }
    MPI_Finalize();
} */