/*
 * =====================================================================================
 *
 *       Filename:  djikstra.h
 *
 *    Description:  interface for djikstra.c
 *
 *        Version:  1.0
 *        Created:  24/06/2019 15:06:54
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Thibaut MILHAUD (), thibaut.milhaud@ensiie.fr
 *   Organization:  ENSIIE - SUAI
 *
 * =====================================================================================
 */
#ifndef  DJIKSTRA_H
#define  DJIKSTRA_H 

#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <CL/cl.h>
#include "graph.h"
#include "err_code.h"

int* run_djikstra(cl_context context, cl_device_id device, 
				Graph_t g, int start_vertex);

int* seq_djikstra(Graph_t g, int start_vertex);

void print_array(int* array, int size);

#endif 

