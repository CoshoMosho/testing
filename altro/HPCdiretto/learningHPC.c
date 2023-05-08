#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <mpi.h>

int main(int argc, char** argv) {

    clock_t inizio, fine;
    double tempo_di_esecuzione;
    inizio = clock();
    // Initialize the MPI environment
    MPI_Init(NULL, NULL);

    // Get the number of processes
    int world_size;
    MPI_Comm_size(MPI_COMM_WORLD, &world_size);

    // Get the rank of the process
    int world_rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);

    int mpiroot = 0;

    // Get the name of the processor
    char processor_name[MPI_MAX_PROCESSOR_NAME];
    int name_len;
    MPI_Get_processor_name(processor_name, &name_len);

    // Print off a hello world message
    printf("Hello world from processor %s, rank %d out of %d processors\n",
           processor_name, world_rank, world_size);

    //doStuff();

    int dati = 1242142;
    MPI_Bcast(&dati, 1, MPI_INT, 0);
    printf("from processor %s, rank %d out of %d processors with value %d\n",
           processor_name, world_rank, world_size,dati);


    // Finalize the MPI environment.
    MPI_Finalize();
    fine = clock();
    tempo_di_esecuzione = ((double)(fine - inizio) / CLOCKS_PER_SEC) * 1000;
    printf("Tempo di esecuzione: %f ms\n", tempo_di_esecuzione);
    
}

//void doStuff(){
    
//}