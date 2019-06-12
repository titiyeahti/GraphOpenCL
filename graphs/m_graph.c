/*
 * =====================================================================================
 *
 *       Filename:  m_graph.c
 *
 *    Description:  implementation of m_graph.h
 *
 *        Version:  1.0
 *        Created:  11/06/2019 16:35:10
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YOUR NAME (), 
 *   Organization:  
 *
 * =====================================================================================
 */

#include "m_graph.h"

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  mg_new
 *  Description:  construct a new m_graph with n vertices and 0 edges,
 *  			  each item is initiallised with a one sized array containing 0.
 * =====================================================================================
 */
		M_graph_t

mg_new (int n)
{
		int i;
		M_graph_t g;
		
		g.size = n;		
		g.graph	= malloc ( sizeof(int *)*n);
		if ( g.graph==NULL ) {
				fprintf ( stderr, "\ndynamic memory allocation failed\n" );
				exit (EXIT_FAILURE);
		}

		for(i=0; i<n; i++)
		{
				g.graph[i]	= malloc ( sizeof(int) );
				if ( g.graph[i]==NULL ) {
						fprintf ( stderr, "\ndynamic memory allocation failed\n" );
						exit (EXIT_FAILURE);
				}
				g.graph[i][0] = 0;
		}


		return g;
}		/* -----  end of function mg_new  ----- */


/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  mg_free
 *  Description:  free memory used by g
 * =====================================================================================
 */
		int

mg_free (M_graph_t g)
{
		int i;
		for(i=0; i<g.size; i++)
		{
				free(g.graph[i]);
				g.graph[i] = NULL;
		}

		free(g.graph);
		g.graph = NULL;

		return EXIT_SUCCESS;
}		/* -----  end of function mg_free  ----- */


/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  mg_add_edge
 *  Description:  add the edge orig -> dest (w) to graph g.
 *  			  if orig > size - 1 || dest > size - 1 Fails
 *  			  if orig -> dest already exists only modifies w.
 * =====================================================================================
 */
		int

mg_add_edge (int orig, int dest, int w, M_graph_t* g)
{
		int i, j;
		int max = g->size - 1;

		if (orig > max || dest > max)
		{
				fprintf(stderr, "\n mg_add_edge : dest or orig > max \n" \
								"Line : %d\n", __LINE__);
				return EXIT_FAILURE;
		}

		i = 1;
		int length = g->graph[orig][0]*2+1;
		int current = 0;
		while(i < length && dest >= current)
		{
				current = g->graph[orig][i];
				if (current == dest)
				{
						g->graph[orig][i+1] = w;
						return EXIT_SUCCESS;
				}
				i += 2;
		}

		int * new_edges;

		new_edges	= malloc (sizeof(int)*(length+2));
		if ( new_edges==NULL ) {
				fprintf ( stderr, "\ndynamic memory allocation failed\n" );
				exit (EXIT_FAILURE);
		}

		new_edges[0] = g->graph[orig][0] + 1;
		new_edges[i] = dest;
		new_edges[i+1] = w;

		// COPY
		for(j = 1; j < length; j += 2)
		{
				if (j<i)
				{
						new_edges[j] = g->graph[orig][j];
						new_edges[j+1] = g->graph[orig][j+1];
				}
				else
				{
						new_edges[j+2] = g->graph[orig][j];
						new_edges[j+3] = g->graph[orig][j+1];
				}
		}

		free(g->graph[orig]);
		g->graph[orig] = new_edges;

		return EXIT_SUCCESS;
}		/* -----  end of function mg_add_edge  ----- */


/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  mg_add_vertex
 *  Description:  add one vertex with no edges to the graph, return its index.
 * =====================================================================================
 */
		int

mg_add_vertex (M_graph_t* g)
{
		int i;
		int n = g->size;
		int** temp;
		temp	= malloc ( sizeof(int*)*(n+1));
		if ( temp==NULL ) {
				fprintf ( stderr, "\ndynamic memory allocation failed\n" );
				exit (EXIT_FAILURE);
		}

		
		temp[n]	= malloc ( sizeof(int) );
		if ( temp==NULL ) {
				fprintf ( stderr, "\ndynamic memory allocation failed\n" );
				exit (EXIT_FAILURE);
		}

		temp[n][0] = 0;


		for (i = 0; i<n; i++)
				temp[i] = g->graph[i];
		
		g->size ++;
		free(g->graph);
		g->graph = temp;

		return g->size - 1;
}		/* -----  end of function mg_add_vertex  ----- */


/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  insert
 *  Description:  insert a value in the odd index of a sorted array.
 *  			  return success if the value is inserted and failure elsewhere.
 *  			  the array must be large enougth.
 * =====================================================================================
 */
		int

insert (int v, int size, int spot, int* array)
{
		int i, a, b;
		a = v;

		for(i=1; i<size; i += 2)
		{
				if (i == spot)
				{
						array[i] = a;
						return EXIT_SUCCESS;
				}else 
				if (array[i] == v) 
				{
						return EXIT_FAILURE;
				}else 
				if (array[i] > v)
				{
						b = array[i];
						array[i] = a;
						a = b;
				}
		}
		return EXIT_SUCCESS;
}		/* -----  end of function insert  ----- */


/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  mg_random
 *  Description:  return a random graph of size vertices, and max degre edge out of each 
 *  			  vertex, the weigth are between 0 and max_weight - 1 (inclusive).
 *  			  Warning : each array of graph may be a little oversized due to the
 *  			  unicity constraint.
 * =====================================================================================
 */

		M_graph_t

mg_random (int size, int degre, int max_weight)
{
		int i, j, count, length;
		time_t t;
		srand(time(&t));
		length = degre*2+1;
		M_graph_t g;

		g.size = size;		
		g.graph	= (int**)malloc ( sizeof(int *)*size);
		
		if ( g.graph==NULL ) {
				fprintf ( stderr, "\ndynamic memory allocation failed\n" );
				exit (EXIT_FAILURE);
		}

		for(i=0; i<size; i++)
		{
				count = 0;
				g.graph[i]	= (int*)malloc ((size_t)length*sizeof(int));
				
				if ( g.graph[i]==NULL ) {
						fprintf ( stderr, "\ndynamic memory allocation failed\n" );
						exit (EXIT_FAILURE);
				}

				for(j=0; j<degre; j++)
				{
						if(!insert((int)rand()%size, length, 2*count+1, g.graph[i]))
								count ++;
				}
				for(j=0; j < count; j++)
						g.graph[i][2*j+2] = rand()%max_weight;

				g.graph[i][0] = count;

		}

		return g;
}		/* -----  end of function mg_random  ----- */


/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  mg_print
 *  Description:  
 * =====================================================================================
 */
		void
mg_print (M_graph_t g)
{
		int i, j;
		for (i=0; i < g.size; i++)
		{
				printf("%d\t|", i);
				for (j=0; j<g.graph[i][0]; j++)
						printf("%d[%d]\t", g.graph[i][2*j+1], g.graph[i][2*j+2]);

				printf("\n");
		}
}		/* -----  end of function mg_print  ----- */
