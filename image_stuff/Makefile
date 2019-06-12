CC = gcc

CCFLAGS=-O3 -std=c99 -ffast-math -Wall -Wextra

LIBS = -lm -lOpenCL -fopenmp

hist : hist.c
	$(CC) $^ $(CCFLAGS) $(LIBS) -o hist
