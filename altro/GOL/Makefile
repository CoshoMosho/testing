#*------------------------------------------------------------------------------|
#Authors 	:Anestis Bechtsoudis 						|
#Capacity	:Undergraduate Student						|
#Foundation 	:Computer Engineering and Informatics Department (CEID)		|
#		:University of Patras						|
#Release Date	:23 - 05 - 2010							|
#Description	:Makefile to compile the sources				|
#-------------------------------------------------------------------------------|

CC=gcc
CFLAGS=-Wall -ggdb

all: GOLThreads GOLOpenMP

GOLThreads: gol_threads.c 
	$(CC) $(CFLAGS) -o GOLPT gol_threads.c -lpthread

GOLOpenMP: gol_openmp.c
	$(CC) $(CFLAGS) -o GOLOMP gol_openmp.c -fopenmp

