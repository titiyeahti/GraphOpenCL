
UTILS=../utils
CCFLAGS=-std=c99
LIBS= -lOpenCL 

test2.out : test2.c m_graph.o
	gcc $^ -o $@

test.out : test.c m_graph.o graph.o
	gcc $^ -o $@

djikstra.out : djikstra.c graph.o m_graph.o
	gcc $^ $(CFLAGS) $(LIBS) -I $(UTILS) -o $@

%.o : %.c
	gcc -c $< $(CFLAGS) -I $(UTILS) 

clean :
	rm -rf *.o *.out
