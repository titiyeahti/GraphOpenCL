/*
 * =====================================================================================
 *
 *       Filename:  dijkstra.c
 *
 *    Description:  Parallelized implementation of dijkstra's algorithm
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

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <time.h>
#include <CL/cl.h>
#include "graph.h"
#include "err_code.h"

void compare_array(int* a, int* b, int size)
{
		int i;
		for (i=0; i<size; i++)
		{
				if(a[i]!=b[i])
						printf("%d : %d != %d \n", i, a[i], b[i]);
		}

}

void print_array(int* array, int size)
{
		int i;
		for(i=0; i<size; i++)
				printf("%d ", array[i]);
		printf("\n");
}


/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  is_to_visit
 *  Description:  aux function for run_dijkstra, return 1 if some vertices are needed 
 *  			to be visited in the array
 * =====================================================================================
 */
int is_to_visit(int* array, size_t size)
{
		int i;
		for(i = 0; i<size; i++)
		{
				if (array[i]==1)
				{
						printf("To visit : %d\n", i);
						return 1;
				}
		}
		return 0;
}



/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  run_dijkstra
 *  Description:  return the weigths of the shortest paths from start_vertices to 
 *  			  every vertex. The return value must be freed.
 * =====================================================================================
 */

int*
run_dijkstra (cl_context context, cl_device_id device, Graph_t g,
				int start_vertex)
{
		FILE	*stream;								/* input-file pointer */
		char	*stream_file_name = "dijkstraker.c";	/* input-file name    */
		unsigned int filesize;                  		/* size of the .cl file */
		char * kernelsource;                    		/* string to store the code */
		int err;                                		/* the error code */
		const size_t size = g.size;
		size_t b_size = size*sizeof(int);
		int* costs;
		int* u_costs;
		int* to_visit;

		// openCL variables
		cl_command_queue queue;
		cl_program program;
		cl_kernel init, dijks1, dijks2;
		cl_mem b_costs, b_ucosts, b_tovisit, 
			   b_vertices, b_edges, b_weights;

		// Queue
		queue = clCreateCommandQueue(context, device, 0, &err);
		checkError(err, "queue\n");

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
		checkError(err, "program");

		err = clBuildProgram(program, 1, &device, NULL, NULL, NULL);
		checkError(err, "building program");

		// Kernels
		dijks1 = clCreateKernel(program, "dijkstra1", &err);
		checkError(err, "Kernel DJ1");
		dijks2 = clCreateKernel(program, "dijkstra2", &err);
		checkError(err, "Kernel DJ2");
		init = clCreateKernel(program, "init", &err);
		checkError(err, "Kernel INIT");

		// ------------------------ INIT -----------------------------
		// Allocating memory for buffers ad arrays
		
		costs	= malloc (b_size);
		if ( costs==NULL ) {
				fprintf ( stderr, "\ndynamic memory allocation failed\n" );
				exit (EXIT_FAILURE);
		}
		
		u_costs = malloc (b_size);
		if ( u_costs==NULL ) {
				fprintf ( stderr, "\ndynamic memory allocation failed\n" );
				exit (EXIT_FAILURE);
		}
		
		to_visit = malloc (b_size);
		if ( u_costs==NULL ) {
				exit (EXIT_FAILURE);
		}

		// MEMORY OBJECTS CREATION

		b_costs = clCreateBuffer(context, CL_MEM_READ_WRITE, b_size, NULL, &err);
		checkError(err, "Buffer b_costs");
		b_ucosts = clCreateBuffer(context, CL_MEM_READ_WRITE, b_size, NULL, &err);
		checkError(err, "Buffer b_ucosts");
		b_tovisit = clCreateBuffer(context, CL_MEM_READ_WRITE, b_size, NULL, &err);
		checkError(err, "Buffer b_tovisit");

		// Buffers from graph object

		b_vertices = clCreateBuffer(context, CL_MEM_READ_ONLY|CL_MEM_COPY_HOST_PTR, 
						(size+1)*sizeof(int), g.vertices, &err);
		checkError(err, "Buffer b_vertices");

		b_edges = clCreateBuffer(context, CL_MEM_READ_ONLY|CL_MEM_COPY_HOST_PTR, 
						(g.vertices[size])*sizeof(int), g.edges, &err);
		checkError(err, "Buffer b_edges");

		b_weights = clCreateBuffer(context, CL_MEM_READ_ONLY|CL_MEM_COPY_HOST_PTR, 
						(g.vertices[size])*sizeof(int), g.weights, &err);
		checkError(err, "Buffer b_weights");
		
		// Setting arguments to run the init Kernel

		err = clSetKernelArg(init, 0, sizeof(cl_mem), &b_ucosts);
		checkError(err, "Arg init 0");
		err |= clSetKernelArg(init, 1, sizeof(cl_mem), &b_tovisit);
		checkError(err, "Arg init 1");
		err |= clSetKernelArg(init, 2, sizeof(cl_mem), &b_costs);
		checkError(err, "Arg init 2");
		err |= clSetKernelArg(init, 3, sizeof(int), &start_vertex);
		checkError(err, "Arg init 3");


		err = clEnqueueNDRangeKernel (queue, init, 1, NULL,
						&size, NULL, 
						0, NULL, NULL);
		checkError(err, "Run init");
		
		err = clEnqueueBarrier(queue);
		
		checkError(err, "Finish INIT");
		
		err = clEnqueueReadBuffer(queue, b_tovisit, CL_TRUE, 0, b_size, to_visit, 
						0, NULL, NULL);
		checkError(err, "program\n");


		// main loop
		// during the loop, w enqueue arg for dijkstra1 and wait the end 
		// then we enqueue arg for dijkstra2 and run it
		// at the end we actualize to_visit array.
		err = clEnqueueBarrier(queue);
		int counter=0;
		while (is_to_visit(to_visit, size))
		{
//				print_array(to_visit, size);
				err = clSetKernelArg(dijks1, 0, sizeof(cl_mem), &b_vertices);
				err |= clSetKernelArg(dijks1, 1, sizeof(cl_mem), &b_edges);
				err |= clSetKernelArg(dijks1, 2, sizeof(cl_mem), &b_weights);
				err |= clSetKernelArg(dijks1, 3, sizeof(cl_mem), &b_costs);
				err |= clSetKernelArg(dijks1, 4, sizeof(cl_mem), &b_ucosts);
				err |= clSetKernelArg(dijks1, 5, sizeof(cl_mem), &b_tovisit);
				checkError(err, "Arguments DJ1");

				err = clEnqueueNDRangeKernel(queue, dijks1, 1, NULL, 
								(const size_t*)&size, NULL, 
								0, NULL, NULL);
				checkError(err, "Run DJ1");

				err = clEnqueueBarrier(queue);

				err = clSetKernelArg(dijks2, 0, sizeof(cl_mem), &b_ucosts);
				err |= clSetKernelArg(dijks2, 1, sizeof(cl_mem), &b_tovisit);
				err |= clSetKernelArg(dijks2, 2, sizeof(cl_mem), &b_costs);
				

				err = clEnqueueNDRangeKernel(queue, dijks2, 1, NULL, 
								(const size_t*)&size, NULL, 
								0, NULL, NULL);
				
				checkError(err, "Run DJ2");
				
				err = clEnqueueBarrier(queue);
				
				err = clEnqueueReadBuffer(queue, b_tovisit, CL_TRUE, 0, 
						b_size, to_visit, 0, NULL, NULL);
				
				err = clEnqueueBarrier(queue);
						
				counter ++;
		}
//		printf("number of loops %d\n", counter);

		err = clEnqueueBarrier(queue);
		
		err = clEnqueueReadBuffer(queue, b_costs, CL_TRUE, 0, 
				b_size, costs, 0, NULL, NULL);

		err = clFinish(queue);

		// Free
		free(kernelsource);
		free(u_costs);
		free(to_visit);

		clReleaseMemObject(b_costs);
		clReleaseMemObject(b_ucosts);
		clReleaseMemObject(b_tovisit);
		clReleaseMemObject(b_vertices);
		clReleaseMemObject(b_edges);
		clReleaseMemObject(b_weights);
		clReleaseKernel(init);
		clReleaseKernel(dijks1);
		clReleaseKernel(dijks2);
		clReleaseProgram(program);
		clReleaseCommandQueue(queue);

		return costs;
}		/* -----  end of function run_dijkstra  ----- */


/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  get_next_vertex
 *  Description:  aux function for seq_dijkstra
 * =====================================================================================
 */
int get_next_vertex(int * visit, int* costs, int size)
{
		int min = (1<<31) - 1;
		int res = -1;
		int i;

		for(i=0; i<size; i++)
		{
				if (visit[i])
				{
						if (costs[i] < min)
						{
								min = costs[i];
								res = i;
						}
				}
		}
		return res;
}

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  seq_dijkstra
 *  Description:  sequential version of the dijkstra algorithm
 * =====================================================================================
 */
		int *

seq_dijkstra (Graph_t g, int start_vertex)
{
		int* costs	= malloc ( sizeof(int)*g.size);
		int* to_visit	= malloc ( sizeof(int)*g.size);
		if ( to_visit==NULL ) {
				fprintf ( stderr, "\ndynamic memory allocation failed\n" );
				exit (EXIT_FAILURE);
		}

		if ( costs==NULL ) {
				fprintf ( stderr, "\ndynamic memory allocation failed\n" );
				exit (EXIT_FAILURE);
		}

		int i;
		int current;
		int edge;
		int dest;

		for (i=0; i<g.size; i++)
		{
				if (i == start_vertex)
				{
						to_visit[i] = 1;
						costs[i] = 0;
				}
				else 
				{
						to_visit[i] = 0;
						costs[i] = (1<<31) - 1;
				}
		}


		while ((current = get_next_vertex(to_visit, costs, g.size)) != -1)
		{
				to_visit[current]=0;
				int c = costs[current];
				
				for(edge = g.vertices[current];
					edge < g.vertices[current+1]; edge ++)
				{
						dest = g.edges[edge];

						if ((c+g.weights[edge])<costs[dest])
						{
								costs[dest] = c + g.weights[edge];
								to_visit[dest] = 1;
						}
				}
		}

		free ( to_visit );
		to_visit	= NULL;

		return costs;
}		/* -----  end of function sec_dijkstra  ----- */




int main(int argc, char** argv)
{
		char i;
		// PLATFORM 	
		cl_platform_id * platforms;
		cl_int err;
		cl_uint num_platforms;

		err = clGetPlatformIDs(0, NULL, &num_platforms);
		checkError(err, "Get num_platforms");

		platforms = (cl_platform_id*) malloc(num_platforms*sizeof(cl_platform_id));

		err = clGetPlatformIDs(num_platforms, platforms, NULL);
		checkError(err, "Get platforms");

		// DEVICES
		

		cl_device_id device[2];
		cl_uint num_devices = 0;

		for(i = 0; i < num_platforms; i++)
		{
				err = clGetDeviceIDs(platforms[i], CL_DEVICE_TYPE_CPU, 0, NULL, &num_devices);

				if (num_devices > 0)
				{
						err = clGetDeviceIDs(platforms[i], CL_DEVICE_TYPE_CPU, 1, 
										&device[0], NULL);
						break;
				}
		}

		num_devices = 0;
		
		for(i = 0; i < num_platforms; i++)
		{
				err = clGetDeviceIDs(platforms[i], CL_DEVICE_TYPE_GPU, 0, NULL, &num_devices);

				if (num_devices > 0)
				{
						err = clGetDeviceIDs(platforms[i], CL_DEVICE_TYPE_GPU, 1,
										&device[1], NULL);
						break;
				}
		}

		// CONTEXT
		
		cl_context context[2];
		context[0] = clCreateContext(NULL, 1, &device[0], NULL, NULL, &err);
		checkError(err, "Create CPU context");

		context[1] = clCreateContext(NULL, 1, &device[1], NULL, NULL, &err);
		checkError(err, "Create CPU context");

		// DATA COLLECTING ZONE
		
		unsigned int degre = argc>1?strtoul(argv[1], NULL, 10):10;
		unsigned char max_power = argc>2?strtoul(argv[2], NULL, 10):7;
		clock_t t1, t2, t3, t4;

		size_t nb_vertices = 1 << 4;

//		M_graph_t mg = mg_random(nb_vertices, degre, 100);
//		Graph_t g = g_from_mg(mg);


		for (i=4; i<max_power; i++)
		{
				M_graph_t mg = mg_random(nb_vertices, degre, 100);

				Graph_t g = g_from_mg(mg);

				t1 = clock();
				int* seq_costs;
				if (i <= 16)
						seq_costs = seq_dijkstra(g, 0);

				t2 = clock();
				int * gpu_costs = run_dijkstra(context[1], device[1], g, 0);
	
				t3 = clock();
				int * cpu_costs = run_dijkstra(context[0], device[0], g, 0);

				t4 = clock();
				printf("%d %f %f %f\n", i,
								(double) (t2 - t1)/CLOCKS_PER_SEC,
								(double) (t3 - t2)/CLOCKS_PER_SEC,
								(double) (t4 - t3)/CLOCKS_PER_SEC);

				compare_array(cpu_costs, gpu_costs, g.size);



				
				if (i<=16)
					free(seq_costs);

				free(cpu_costs);
				free(gpu_costs);

				mg_free(mg);
				g_free(g);

				nb_vertices <<= 1;
		}

		
//		g_print(g);
//		int * cpu_costs = run_dijkstra(context[0], device[0], g, 0);
//		
//		g_free(g);
//		mg_free(mg);
		clReleaseContext(context[0]);
		clReleaseContext(context[1]);
		free(platforms);
}
