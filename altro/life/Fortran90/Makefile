# DESCRIPTION:  Makefile for serial/parallel codes in Fortran 90
# AUTHOR:       Aaron Weeden, The Shodor Education Foundation, Inc.
# DATE:         September, 2011

# Code prefix
PROGRAM_PREFIX=life

# Compilers and flags
F90C=gfortran
MPIF90C=mpif90
#CFLAGS=-Wall --pedantic -fbounds-check
#CFLAGS+=-DSHOW_RESULTS # Uncomment to make the program print its results

# Source files
SRCS=$(PROGRAM_PREFIX).F90

# OpenMP
OPENMP_FLAGS=-fopenmp

# MPI
MPI_FLAGS=-DMPI

# Hybrid MPI/OpenMP
HYBRID_FLAGS=$(MPI_FLAGS) $(OPENMP_FLAGS)

EXECUTABLES=$(PROGRAM_PREFIX).{serial,openmp,mpi,hybrid}

# Make rules
$(PROGRAM_PREFIX).serial: $(SRCS)
	$(F90C) $(CFLAGS) $(SRCS) -o $(PROGRAM_PREFIX).serial

$(PROGRAM_PREFIX).openmp: $(SRCS)
	$(F90C) $(CFLAGS) $(OPENMP_FLAGS) $(SRCS) -o $(PROGRAM_PREFIX).openmp

$(PROGRAM_PREFIX).mpi: $(SRCS)
	$(MPIF90C) $(CFLAGS) $(MPI_FLAGS) $(SRCS) -o $(PROGRAM_PREFIX).mpi

$(PROGRAM_PREFIX).hybrid: $(SRCS)
	$(MPIF90C) $(CFLAGS) $(HYBRID_FLAGS) $(SRCS) -o $(PROGRAM_PREFIX).hybrid

clean:
	rm -f $(EXECUTABLES) *.o

serial:
	make $(PROGRAM_PREFIX).serial

openmp:
	make $(PROGRAM_PREFIX).openmp

mpi:
	make $(PROGRAM_PREFIX).mpi

hybrid:
	make $(PROGRAM_PREFIX).hybrid

all:
	make clean
	make $(EXECUTABLES)
