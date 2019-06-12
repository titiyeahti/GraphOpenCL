/*
 * =====================================================================================
 *
 *       Filename:  hist.c
 *
 *    Description:  Application in Opencl Calculating histogram of a random bimtmap
 *
 *        Version:  1.0
 *        Created:  07/06/2019 12:58:44
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
#include <CL/cl.h>
#include "err_code.h"

#define HIST_RANGE 256
#define MAX_LS 256
#define MAX_WG 24

int printHist(int hist[HIST_RANGE])
{
		printf("\n\nHISTOGRAM :\n");
		int i, j, sum;
		sum = 0;
		for(i = 0; i < HIST_RANGE; i++)
		{
				if (i%1==0)
				{
						printf("%d :", i);
						for(j = 0; j < hist[i]/40; j++)
						{
								printf("+");
						}
						printf("\n");
				}
				sum += hist[i];
		}
		return sum;
}


int main(int argc, char ** argv)
{
		// GENERATION D'IMAGE

		unsigned int i;
		unsigned int img_rows;
		unsigned int img_cols;
		unsigned int img_size;
		unsigned int hist_size;

		hist_size = HIST_RANGE * sizeof(int);

		unsigned char * img;

		srand(time(NULL));

		if (argc == 2) { 
				img_rows = strtol(argv[1], NULL, 10);
				img_cols = strtol(argv[1], NULL, 10);
		}
		else if (argc == 3)	{
				img_rows = strtol(argv[1], NULL, 10);
				img_cols = strtol(argv[2], NULL, 10);
		}
		else {
				img_rows = 80000;
				img_cols = 6000;
		}

		img_size = img_rows * img_cols;

		img = (unsigned char*) malloc(img_size * sizeof(char));

		for(i = 0; i < img_size; i++)
		{
				img[i] = (rand() % HIST_RANGE)/8;
				img[i] += (rand() % HIST_RANGE)/8;
				img[i] += (rand() % HIST_RANGE)/8;
				img[i] += (rand() % HIST_RANGE)/8;
				img[i] += (rand() % HIST_RANGE)/8;
				img[i] += (rand() % HIST_RANGE)/8;
				img[i] += (rand() % HIST_RANGE)/8;
				img[i] += (rand() % HIST_RANGE)/8;
		}
		printf("i : %d\n", i);

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
				err = clGetDeviceIDs(platforms[i], CL_DEVICE_TYPE_CPU, 0, NULL, &num_devices);

				if (num_devices > 0)
				{
						err = clGetDeviceIDs(platforms[i], CL_DEVICE_TYPE_CPU, 1, &device, NULL);
						break;
				}
		}

		// CONTEXT
		
		cl_context context;
		context = clCreateContext(NULL, 1, &device, NULL, NULL, &err);
		checkError(err, "Create context");

		// COMMAND QUEUE

		cl_command_queue queue;
		queue = clCreateCommandQueue(context, device, 0, &err);
		checkError(err, "Create command queue");

		// PROGRAM 
		
		FILE * stream = fopen("hist.cl", "r");
		if (stream == NULL)
				printf("error in the opening of the file\n");
	
		unsigned int filesize;
		fseek(stream, 0, SEEK_END);
		filesize = ftell(stream);
		fseek(stream, 0, SEEK_SET);
    	char * kernelsource = malloc(sizeof(char)*filesize+1);
	
		if (filesize != fread(kernelsource, 1, filesize, stream))
				printf("error in the reading of the file\n");

		fclose(stream);
		kernelsource[filesize] = '\0';

		printf("%s\n", kernelsource);

		cl_program program;
		program = clCreateProgramWithSource(context, 1, (const char **) &kernelsource, NULL, &err);
		checkError(err, "creating program");
		
		err = clBuildProgram(program, 1, &device, NULL, NULL, NULL);
		checkError(err, "Build Program");

		// MEMORY

		cl_mem b_img;
		b_img = clCreateBuffer(context, CL_MEM_READ_ONLY, img_size, NULL, &err);
		checkError(err, "Create buffer img");

		cl_mem b_hist;
		b_hist = clCreateBuffer(context, CL_MEM_WRITE_ONLY, hist_size, NULL, &err);
		checkError(err, "Create buffer hist");

		err = clEnqueueWriteBuffer(queue, b_img, CL_TRUE, 0, img_size * sizeof(char), 
						img, 0, NULL, NULL);
		checkError(err, "Write img");

		int zero = 0;

		err = clEnqueueFillBuffer(queue, b_hist, &zero, sizeof(int), 0, hist_size,
						0, NULL, NULL);
		checkError(err, "Fill hist");

		// KERNEL
		
		cl_kernel kernel;
		kernel = clCreateKernel(program, "hist", &err);
		checkError(err, "Create kernel");

		// ARGUMENTS
		
		err = clSetKernelArg(kernel, 0, sizeof(cl_mem), &b_img);
		err |= clSetKernelArg(kernel, 1, sizeof(cl_mem), &b_hist);
		err |= clSetKernelArg(kernel, 2, sizeof(int), &img_size);
		checkError(err, "Arguments");

		// WORKSIZE
		
		size_t local_size;
		int wg;
		size_t global_size;
		int log2;

		int j;
		int tmp = img_size;

		for(i = 0; i < 1;)
		{
				if(tmp == 0)
						break;

				tmp = tmp >> 1;
				log2++;
		}

		for(i = 1; i < (unsigned int)(1 <<((log2/2) + 1)); i++)
		{
				if (img_size%i == 0)
				{
						global_size = img_size/i;
						for(j=24; j>0; j--)
						{
								if(global_size%j == 0)
								{
										local_size = global_size/i;
										wg = i;
								}
						}
						local_size = MAX_LS + 1;
						wg = MAX_WG +1;
						if (local_size <= MAX_LS && wg <= MAX_WG)
						{
								break;
						}
				}
		}


		global_size = 1024;
		local_size = 64;
		wg = 1;
		printf("\n------------------------------\n"
				"Global size : %ld \n"
				"Local size : %ld \n"
				"Work groups : %d \n"
				"--------------------------------\n"
			  , global_size, local_size, wg);

		// RUN
		
		clock_t cl = clock();

		err = clEnqueueNDRangeKernel(queue, kernel, 1, NULL,
						(const size_t *)&global_size,
						(const size_t *)&local_size,
						0, NULL, NULL);

		cl = clock() - cl;

		checkError(err, "execution");

		// RESULTATS
		
		int hist[HIST_RANGE];

		err = clEnqueueReadBuffer(queue, b_hist, CL_TRUE, 0, hist_size, hist,
						0, NULL, NULL);
		clFinish(queue);

		checkError(err, "Load of the result");

		//unsigned int total = printHist(hist);
		//printf("total : %d\n", total);
		printf("TIME : %9.8f\n", (double) cl/CLOCKS_PER_SEC);

		for (i = 0; i<HIST_RANGE; i++)
				hist[i] = 0;

		cl = clock();

		for (i = 0; i<img_size; i++)
				hist[img[i]]++;

		cl = clock() - cl;

		printf("SEQ TIME : %9.8f\n", (double) cl/CLOCKS_PER_SEC);
		// FREEZONE

		clReleaseKernel(kernel);
		clReleaseMemObject(b_img);
		clReleaseMemObject(b_hist);
		clReleaseProgram(program);
		clReleaseCommandQueue(queue);
		clReleaseContext(context);
		free(platforms);
		free(kernelsource);
		free(img);
		return 0;
}

