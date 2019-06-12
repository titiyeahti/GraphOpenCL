/*
 * =====================================================================================
 *
 *       Filename:  test2.c
 *
 *    Description:  Test de m_graph.h
 *
 *        Version:  1.0
 *        Created:  11/06/2019 13:06:57
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YOUR NAME (), 
 *   Organization:  
 *
 * =====================================================================================
 */

#include "m_graph.h"


#include	<stdlib.h>

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  main
 *  Description:  
 * =====================================================================================
 */
		int
main ( int argc, char *argv[] )
{
		M_graph_t g = mg_random(150, 6, 10);
		mg_print(g);
		mg_free(g);

		return EXIT_SUCCESS;
}				/* ----------  end of function main  ---------- */
