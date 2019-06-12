/*
 * =====================================================================================
 *
 *       Filename:  graph.h
 *
 *    Description:  Interface for graphs structures
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

#include <stdio.h>
#include <stdlib.h>
#include "m_graph.h"

struct graph_t {
		int size;
		/*-----------------------------------------------------------------------------
		 *  Array of size N + 1. vertices[i] contain the index of the 
		 *  first outcoming edge from this vertex.
		 *  vertices[N] contain the size of edges.
		 *  vertices[i+1]-vertices[i] = number of outcomming edge from vertex i.
		 *-----------------------------------------------------------------------------*/
		int* vertices;
		/*-----------------------------------------------------------------------------
		 *  Array of size M. edges[i] contain the index of the destination vertex for
		 *  the edge i.
		 *-----------------------------------------------------------------------------*/
		int* edges;
		/*-----------------------------------------------------------------------------
		 *  weight[i] = weigth of edge i 
		 *-----------------------------------------------------------------------------*/
		int* weights;
};				/* ----------  end of struct graph_t  ---------- */

typedef struct graph_t Graph_t;


/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  g_free
 *  Description:  release the memory used by g
 * =====================================================================================
 */
		int
g_free (Graph_t g);


/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  g_new
 *  Description:  return a new graph with n vertices and 0 edges  
 * =====================================================================================
 */
		Graph_t
g_new (int n);


/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  g_add_vertex
 *  Description:  add vertex to g and return his index
 * =====================================================================================
 */
		int
g_add_vertex (Graph_t* g);

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  g_add_edge
 *  Description:  add i -> j with weigth w in g, return EXIT_FAILURE on failure. 
 * =====================================================================================
 */
		int
g_add_edge (int i, int j, int w, Graph_t* g);

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  g_print
 *  Description:  print a representation of graph g
 * =====================================================================================
 */
		void
g_print (Graph_t g);


/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  g_from_mg
 *  Description:  construct a graph from a m_graph
 * =====================================================================================
 */
		Graph_t

g_from_mg (M_graph_t mg);
