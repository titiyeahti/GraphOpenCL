/*
 * =====================================================================================
 *
 *       Filename:  m_graph.h
 *
 *    Description:  Mutable graphs, used to construct graphs structures
 *
 *        Version:  1.0
 *        Created:  11/06/2019 10:35:10
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
#include <time.h>


struct mg_t {
		
		/*-----------------------------------------------------------------------------
		 *  number of vertices, size of graph
		 *-----------------------------------------------------------------------------*/
		int size;


		/*-----------------------------------------------------------------------------
		 *  each item contain an array representing the outgoing edges from the summit.
		 *  the these array are constitued as follow:
		 *  	- graph[i][0] = number of outgoing edges for vertex i
		 *  	- graph[i][2k+1] = dest of the edge k from i
		 *  	- graph[i][2k+2] = weigth of the edge k from i
		 *  	- the destinations are ordered from smaller to bigger
		 *  so the size of these array are (graph[i][0]*2 + 1)
		 *-----------------------------------------------------------------------------*/
		int ** graph;
};				/* ----------  end of struct mg_t  ---------- */

typedef struct mg_t M_graph_t;


/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  mg_new
 *  Description:  construct a new m_graph with n vertices and 0 edges,
 *  			  each item is initiallised with a one sized array containing 0.
 * =====================================================================================
 */
M_graph_t mg_new (int n);


/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  mg_free
 *  Description:  free memory used by g
 * =====================================================================================
 */
int mg_free (M_graph_t g);


/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  mg_add_edge
 *  Description:  add the edge orig -> dest (w) to graph g.
 *  			  if orig > size - 1 || dest > size - 1 Fails
 *  			  if orig -> dest already exists only modifies w.
 * =====================================================================================
 */
int mg_add_edge (int orig, int dest, int w, M_graph_t* g);


/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  mg_add_vertex
 *  Description:  add one vertex with no edges to the graph, return its index.
 * =====================================================================================
 */
int mg_add_vertex (M_graph_t* g);


/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  mg_random
 *  Description:  return a random graph of size vertices, and max degre edge out of each 
 *  			  vertex, the weigth are between 0 and max_weight - 1 (inclusive).
 *  			  Warning : each array of graph may be a little oversized due to the
 *  			  unicity constraint.
 * =====================================================================================
 */
M_graph_t mg_random (int size, int degre, int max_weight);


/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  mg_print
 *  Description:  
 * =====================================================================================
 */
void mg_print (M_graph_t g);
