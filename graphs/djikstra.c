/*
 * =====================================================================================
 *
 *       Filename:  djikstra.c
 *
 *    Description:  Parallelized implementation of djikstra's algorithm
 *
 *        Version:  1.0
 *        Created:  13/06/2019 11:56:58
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Thibaut MILHAUD (), thibaut.milhaud@ensiie.fr
 *   Organization:  ENSIIE - SUAI
 *
 * =====================================================================================
 */

#include <CL/cl.h>
#include "graph.h"

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  run_djikstra
 *  Description:  return the weigths of the shortest paths from start_vertices to 
 *  			  every vertex. The return value must be freed.
 * =====================================================================================
 */
		int*
run_djikstra (cl_context context, cl_device_id device, Graph_t g,
				int start_vertex)
{
		int* res										/* the return value   */
		FILE	*stream;								/* input-file pointer */
		char	*stream_file_name = "djikstra.cl";		/* input-file name    */
		unsigned int filesize;                  /* size of the .cl file */
		char * kernelsource;                    /* string to store the code */
		int err;                                /* the error code */


		// openCL variables
		cl_command_queue queue;
		cl_program program;
		cl_kernel init, djiks1, djiks2;

		res	= malloc ( sizeof(int)*g.size);
		if ( res==NULL ) {
				fprintf ( stderr, "\ndynamic memory allocation failed\n" );
				exit (EXIT_FAILURE);
		}

		// Queue
		queue = clCreateCommandQueue(context, device, 0, &err);

		// Loading .cl source files


		stream	= fopen( stream_file_name, "r" );
		if ( stream == NULL ) {
				fprintf ( stderr, "couldn't open file '%s'; %s\n",
								stream_file_name, strerror(errno) );
				exit (EXIT_FAILURE);
		}
		
		fseek(stream, 0, SEEK_END);
		filesize = ftell(stream);
		fseek(stream, 0, SEEK_SET);
    	kernelsource = malloc(sizeof(char)*filesize+1);
		
		if (filesize != fread(kernelsource, 1, filesize, stream))
				printf("error in the reading of the file\n");
		kernelsource[filesize] = '\0';

		if( fclose(stream) == EOF ) {			/* close input file   */
				fprintf ( stderr, "couldn't close file '%s'; %s\n",
								stream_file_name, strerror(errno) );
				exit (EXIT_FAILURE);
		}

		// Creating and building program
		program = clCreateProgramWithSource(context, 1, (const char **) &kernelsource, 
						NULL, &err);

		err = clBuildProgram(program, 1, &device, NULL, NULL, NULL);

		// Kernels
		djiks1 = clCreateKernel(program, "djikstra1", &err);
		djiks2 = clCreateKernel(program, "djikstra2", &err);
		init = clCreateKernel(program, "init", &err);

		// ------------------------ INIT -----------------------------


		// Free
		clReleaseKernel(init);
		clReleaseKernel(djiks1);
		clReleaseKernel(djiks2);
		clReleaseProgram(program);
		clReleaseCommandQueue(queue);

		return res;
}		/* -----  end of function run_djikstra  ----- */
