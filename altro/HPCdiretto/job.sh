#!/bin/bash
#SBATCH -N 4  # Numero di nodi richiesti
#SBATCH -n 16  # Numero totale di processi MPI (processi totali = nodi x processi per nodo)
#SBATCH -c 4   # Numero di thread OpenMP per processo
#SBATCH -t 00:30:00  # Tempo di esecuzione previsto
#SBATCH -p EPYC

# Carica il modulo MPI
module load mpi

# Compila il programma
mpicc -fopenmp mpi_openmp_example.c -o mpi_openmp_example

# Esegui il programma con MPI
srun ./mpi_openmp_example
