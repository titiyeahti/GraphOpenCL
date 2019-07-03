C= gcc

CCFLAGS=-O3 -std=c99 -ffast-math

LIBS = -lm -lOpenCL -fopenmp

UTILS=../utils

edge.out : edge_detection.c
	$(CC) $^ $(CCFLAGS) $(LIBS) -I $(UTILS) -o $@

hist.out : hist.c
	$(CC) $^ $(CCFLAGS) $(LIBS) -I $(UTILS) -o $@

clean : 
	rm -rf *.o *.out
