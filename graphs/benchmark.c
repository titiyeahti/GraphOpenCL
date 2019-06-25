/*
 * =====================================================================================
 *
 *       Filename:  benchmark.c
 *
 *    Description:  comparison between 2 implementations of djikstra's algorithm
 *    				
 *
 *        Version:  1.0
 *        Created:  24/06/2019 15:11:25
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Thibaut MILHAUD (), thibaut.milhaud@ensiie.fr
 *   Organization:  ENSIIE - SUAI
 *
 * =====================================================================================
 */

#include "djikstra.h"

int main(int argc, char** argv)
{
		int i;
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
		

		cl_device_id device;
		cl_uint num_devices = 0;

		for(i = 0; i < num_platforms; i++)
		{
				err = clGetDeviceIDs(platforms[i], CL_DEVICE_TYPE_GPU, 0, NULL, &num_devices);

				if (num_devices > 0)
				{
						err = clGetDeviceIDs(platforms[i], CL_DEVICE_TYPE_GPU, 1, &device, NULL);
						break;
				}
		}

		// CONTEXT
		
		cl_context context;
		context = clCreateContext(NULL, 1, &device, NULL, NULL, &err);
		checkError(err, "Create context");
	
		printf("coucou\n");
		// TEST ZONE;

		M_graph_t mg = mg_random(5, 3, 10);

		Graph_t g = g_from_mg(mg);

		g_print(g);
		printf("coucou\n");

		int * seq_costs = seq_djikstra(g, 0);

		int * costs = run_djikstra(context, device, g, 0);

		print_array(seq_costs, 5);
		print_array(costs, 5);

		free(seq_costs);
		free(costs);
		
		clReleaseContext(context);
		free(platforms);
}
