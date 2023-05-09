#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <mpi.h>

int main(int argc, char** argv) {

    
    // Initialize the MPI environment
    MPI_Init(NULL, NULL);
    clock_t inizio, fine;
    double tempo_di_esecuzione;
    inizio = clock();
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

    int test = 0;
    MPI_Send(&test,1,MPI_INT, world_rank + 1, 0, MPI_COMM_WORLD );


       test = test + 1;
       MPI_Recv(&test, 0, MPI_INT,  world_rank - 1. 0, MPI_COMM_WORLD, 0);

    int dati = 1242142;
    MPI_Bcast(&dati, 1, MPI_INT, 0, MPI_COMM_WORLD);
    printf("from processor %s, rank %d out of %d processors with value %d\n",
           processor_name, world_rank, world_size,test);
    fine = clock();
    tempo_di_esecuzione = ((double)(fine - inizio) / CLOCKS_PER_SEC) * 1000;
    printf("Tempo di esecuzione: %f ms\n", tempo_di_esecuzione);
    

    // Finalize the MPI environment.
    MPI_Finalize();
    
}

//void doStuff(){
    
//}