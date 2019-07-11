CC= gcc

CCFLAGS=-O3 -std=c99 -ffast-math

LIBS = -lm -lOpenCL -fopenmp

UTILS=../../utils

test.out : climagelib.o testzone.c 
	$(CC) $^ $(CCFLAGS) $(LIBS) -I $(UTILS) -o $@

%.o : %.c
	gcc -c $< $(CFLAGS) -I $(UTILS) 

clean : 
	rm -rf *.o *.out
