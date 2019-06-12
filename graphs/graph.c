/*
 * =====================================================================================
 *
 *       Filename:  graph.c
 *
 *    Description:  Implementation of graph.h 
 *
 *        Version:  1.0
 *        Created:  10/06/2019 11:04:47
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YOUR NAME (), 
 *   Organization:  
 *
 * =====================================================================================
 */

#include "graph.h"

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  g_free
 *  Description:  release the memory used by g
 * =====================================================================================
 */
		int
g_free (Graph_t g)
{
		free(g.vertices);
		free(g.edges);
		free(g.weights);
		return EXIT_SUCCESS;
}


/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  g_new
 *  Description:  return a new graph with n vertices and 0 edges  
 * =====================================================================================
 */
		Graph_t
g_new (int n)
{
		int i;
		Graph_t g;
		g.size = n;
		
		g.vertices	= malloc ( sizeof(int)*(n+1));
		if ( g.vertices==NULL ) {
				fprintf ( stderr, "\ndynamic memory allocation failed\n" );
				exit (EXIT_FAILURE);
		}

		for(i=0; i<n+1; i++)
				g.vertices[i] = 0;

		g.edges = NULL;
		g.weights = NULL;

		return g;
}


/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  g_add_vertex
 *  Description:  add vertex to g and return his index
 * =====================================================================================
 */
		int
g_add_vertex (Graph_t* g)
{
		if (!g->vertices)
		{
				g_free(*g);
				*g = g_new(1);
				return 0;
		}

		int i;
		int * new_vertices;
		g->size ++;
		new_vertices = (int*) malloc((g->size + 1)*sizeof(int));
		
		for(i=0; i<g->size; i++)
				new_vertices[i] = g->vertices[i];

		new_vertices[g->size] = new_vertices[g->size-1]; /* no edges atm */
		
		free(g->vertices);

		g->vertices = new_vertices;

		return i-1; 
}


/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  g_add_edge
 *  Description:  add i -> j with weigth w in g, return EXIT_FAILURE on failure. 
 * =====================================================================================
 */
		int
g_add_edge (int i, int j, int w, Graph_t* g)
{
		int l;

		if ( i>g->size - 1 || j > g->size - 1)
				return EXIT_FAILURE;

		if (g->edges) {
				int k;
				int * new_edges;
				int * new_weights;

				int n_edges = g->vertices[g->size] + 1;

				new_edges = (int*) malloc(sizeof(int)*n_edges);
				new_weights= (int*) malloc(sizeof(int)*n_edges);

				k = g->vertices[i];

				new_edges[k] = j;
				new_weights[k] = w;

				// Copy
				for(l=0; l<n_edges-1; l++)
				{
						if (l<k)
						{
								new_edges[l] = g->edges[l];
								new_weights[l] = g->weights[l];
						}
						else
						{
								new_edges[l+1] = g->edges[l];
								new_weights[l+1] = g->weights[l];
						}
				}

				free(g->edges);
				free(g->weights);

				g->edges = new_edges;
				g->weights = new_weights;
		}
		else 
		{
				free(g->edges);
				free(g->weights);

				g->edges = (int*) malloc(sizeof(int));
				g->weights = (int*) malloc(sizeof(int));
				g->edges[0] = j;
				g->weights[0] = w;
		}

		for(l = i+1; l < g->size+1; l++)
				g->vertices[l]++;

		return EXIT_SUCCESS;
}

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  g_print
 *  Description:  print a representation of graph g
 * =====================================================================================
 */
		void
g_print (Graph_t g)
{
		int i, j;
		for(i = 0; i<g.size; i++)
		{
				printf("%d\t|", i);
				for(j = g.vertices[i]; j < g.vertices[i+1]; j++)
				{
						printf("%d[%d]\t", g.edges[j], g.weights[j]);
				
				}
				printf("\n");
		}
}


/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  g_from_mg
 *  Description:  Construct a graph from a m_graph
 * =====================================================================================
 */
		Graph_t

g_from_mg (M_graph_t mg)
{
		int i, j, n, pos, count;
		n = mg.size;
		
		Graph_t g;
		g.size = n;
		
		g.vertices	= malloc ( sizeof(int)*(n+1));
		if ( g.vertices==NULL ) {
				fprintf ( stderr, "\ndynamic memory allocation failed\n" );
				exit (EXIT_FAILURE);
		}

		count = 0;
		for(i = 0; i<n; i++)
		{
				g.vertices[i] = count;
				count += mg.graph[i][0]; //the number of edges
		}

		g.vertices[n] = count;

		g.edges	= (int*)malloc ( sizeof(int)*count);

		if ( g.edges==NULL ) {
				fprintf ( stderr, "\ndynamic memory allocation failed\n" );
				exit (EXIT_FAILURE);
		}

		g.weights	= (int*)malloc ( sizeof(int)*count);
		if ( g.weights==NULL ) {
				fprintf ( stderr, "\ndynamic memory allocation failed\n" );
				exit (EXIT_FAILURE);
		}

		for(i=0; i<n; i++)
		{
				pos = g.vertices[i];
				for(j=0; j< mg.graph[i][0]; j++)
				{
						g.edges[pos+j] = mg.graph[i][2*j+1];
						g.weights[pos+j] = mg.graph[i][2*j+2];
				}
		}

		return g;
}		/* -----  end of function g_from_mg  ----- */
