/*
 * =====================================================================================
 *
 *       Filename:  test.c
 *
 *    Description:  first tests on graph.c
 *
 *        Version:  1.0
 *        Created:  10/06/2019 13:26:32
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YOUR NAME (), 
 *   Organization:  
 *
 * =====================================================================================
 */


#include	<stdlib.h>
#include "graph.h"
/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  main
 *  Description:  
 * =====================================================================================
 */
		int
main ( int argc, char *argv[] )
{
		int i;
		M_graph_t mg = mg_random(1000, 20, 10);	

		printf ( "\n---------------------\n" );

		Graph_t g = g_from_mg(mg);

		g_print(g);

		mg_free(mg);
		g_free(g);

		return EXIT_SUCCESS;
}				/* ----------  end of function main  ---------- */
