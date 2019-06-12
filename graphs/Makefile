test2.out : test2.c m_graph.o
	gcc m_graph.o test2.c -o test2.out

test.out : test.c m_graph.o graph.o
	gcc graph.o m_graph.o test.c -o test.out

m_graph.o : m_graph.c m_graph.h
	gcc -c m_graph.c

graph.o : graph.h graph.c
	gcc -c graph.c

clean :
	rm *.o test test2
