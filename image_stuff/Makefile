CC = gcc

CCFLAGS=-O3 -std=c99 -ffast-math -Wall -Wextra

LIBS = -lm -lOpenCL -fopenmp

hist.out : hist.c
	$(CC) $^ $(CCFLAGS) $(LIBS) -o hist.out

clean : 
	rm *.o *.out
