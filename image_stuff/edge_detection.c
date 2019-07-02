/*
 * =====================================================================================
 *
 *       Filename:  edge_detection.c
 *
 *    Description:  Usage of opencl for edges dectection algorithm.
 *    				Usage ./edges.out filename
 *
 *        Version:  1.0
 *        Created:  27/06/2019 23:25:10
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
#include <time.h>
#include <CL/cl.h>
#include "err_code.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"


unsigned char* convert_to_32bits(unsigned char* img, int x, int y)
{
		unsigned char* res = malloc(x*y*sizeof(char)*4);
		int i;
		for (i = 0; i<x*y; i++)
		{
				res[4*i] = img[3*i];
				res[4*i+1] = img[3*i+1];
				res[4*i+2] = img[3*i+2];
				res[4*i+3] = 255;
		}
		return res;
}

unsigned char* clEdgeDetection(
				char* filename, 
				cl_context context, 
				cl_command_queue queue, 
				cl_kernel kernel)
{	
		// Loading image
//		temp = stbi_load(filename, &x, &y, &n, 0);
//		input_img = convert_to_32bits(temp, x, y);
//		n++;
//		output_img = malloc(sizeof(char)*x*y*n);
//		free(temp);
		
		input_img = stbi_load(filename, &x, &y, &n, 0);


		// IMAGE PARAMTERS SETUP
		origin[0] = 0;
		origin[1] = 0;
		origin[2] = 0;
	    region[0] = (size_t)x;
		region[1] = (size_t)y;
		region[2] = 1;
		color[0] = 0;
		color[1] = 0;
		color[2] = 0;
		color[3] = 1;

		// Filling the cl_image_format
		format.image_channel_order = CL_RGBA;
		format.image_channel_data_type = CL_UNORM_INT8;

		// Filling th cl_image_desc struct 
		desc.image_type = CL_MEM_OBJECT_IMAGE2D;
		desc.image_width = (size_t)x;
		desc.image_height = (size_t)y;
		desc.image_depth = 0;
		desc.image_array_size = 1;
		desc.image_row_pitch = 0;
		desc.image_slice_pitch = 0;
		desc.num_mip_levels = 0;
		desc.num_samples = 0;
		desc.buffer = NULL;

		// CREATING IMAGE OBJECTS
		input_i = clCreateImage(context, CL_MEM_READ_ONLY | CL_MEM_USE_HOST_PTR,
						&format, &desc, input_img, &err);
		checkError(err, "Input image creation");

		output_i = clCreateImage(context, CL_MEM_WRITE_ONLY, 
						&format, &desc, NULL, &err);
		checkError(err, "Output image creation");


		// PUSHING IMAGES INTO THE DEVICE
		// CLOCK
		c0 = clock();
		err = clEnqueueWriteImage(queue, input_i, CL_FALSE, 
						(const size_t*)origin, (const size_t*)region, 0, 0, input_img,
						0, NULL, NULL);
		checkError(err, "Enqeuing input image");

		err = clEnqueueFillImage(queue, output_i, color,
						origin, region, 0, NULL, NULL); 
		checkError(err, "Enqeuing output image");

		// CREATING SAMPLER
		sampler = clCreateSampler(context, CL_FALSE, CL_ADDRESS_CLAMP_TO_EDGE,
						CL_FILTER_NEAREST, &err);
		checkError(err, "Sampler creation");

		clFinish(queue);

		// SETTING THE ARGS
		err = clSetKernelArg(kernel, 0, sizeof(cl_mem), &input_i);
		err |= clSetKernelArg(kernel, 1, sizeof(cl_mem), &output_i);
		err |= clSetKernelArg(kernel, 2, sizeof(cl_sampler), &sampler); 

		// RUNNING THE KERNELS
		worksize[0] = (size_t) x;
		worksize[1] = (size_t) y;

		printf("SEG FAULT FINDER, line : %d\n", __LINE__);
		err = clEnqueueNDRangeKernel(queue, kernel, 2, NULL, worksize, NULL,
						0, NULL, NULL);
		checkError(err, "Kernels execution");

		printf("SEG FAULT FINDER, line : %d\n", __LINE__);
		err = clFinish(queue);

		err = clEnqueueReadImage(queue, output_i, CL_FALSE, origin, region, 
						0, 0, output_img, 0, NULL, NULL);

		err = clFinish(queue);

		return output_img;
}

int main(int argc, char** argv)
{
		// VARIABLES
		char * filename = argc>1 ? argv[1]: "sputnik.jpg";
		char * kernelsource;
		int i, x, y, n;
		unsigned char* temp;
		unsigned char* input_img;
		unsigned char* output_img;
		cl_int err;
		cl_image_format format;
		cl_mem input_i;
		cl_mem output_i;
		cl_image_desc desc;
		cl_sampler sampler;
		cl_device_id device;
		cl_uint num_devices = 0;
		cl_platform_id * platforms;
		cl_uint num_platforms;
		cl_context context;
		cl_command_queue queue;
		cl_program program;
		cl_kernel kernel;
		FILE* stream;
		unsigned int filesize;
		size_t origin[3];
		size_t region[3];
		unsigned char color[4];
		size_t worksize[2];
		clock_t c0, c1;
		// END VARIABLES




		// PLATFORM 
		err = clGetPlatformIDs(0, NULL, &num_platforms);
		checkError(err, "Get num_platforms");

		platforms = (cl_platform_id*) malloc(num_platforms*sizeof(cl_platform_id));

		err = clGetPlatformIDs(num_platforms, platforms, NULL);
		checkError(err, "Get platforms");

		// DEVICES
		for(i = 0; i < num_platforms; i++)
		{
				err = clGetDeviceIDs(platforms[i], CL_DEVICE_TYPE_GPU, 0, NULL,
								&num_devices);

				if (num_devices > 0)
				{
						err = clGetDeviceIDs(platforms[i], CL_DEVICE_TYPE_GPU,
										1, &device, NULL);
						break;
				}
		}

		// CONTEXT
		context = clCreateContext(NULL, 1, &device, NULL, NULL, &err);
		checkError(err, "Create context");

		// COMMAND QUEUE
		queue = clCreateCommandQueue(context, device, 0, &err);
		checkError(err, "Create command queue");

		// PROGRAM 
		stream = fopen("edgeker.c", "r");
		if (stream == NULL)
				printf("error in the opening of the file\n");
	
		fseek(stream, 0, SEEK_END);
		filesize = ftell(stream);
		fseek(stream, 0, SEEK_SET);
    	kernelsource = malloc(sizeof(char)*filesize+1);
	
		if (filesize != fread(kernelsource, 1, filesize, stream))
				printf("error in the reading of the file\n");

		fclose(stream);
		kernelsource[filesize] = '\0';

		printf("%s\n", kernelsource);

		program = clCreateProgramWithSource(context, 1, (const char **) &kernelsource, 
						NULL, &err);
		checkError(err, "creating program");
		
		err = clBuildProgram(program, 1, &device, NULL, NULL, NULL);
		checkError(err, "Build Program");

		// KERNEL
		kernel = clCreateKernel(program, "sobel", &err);


		// SAVING THE IMAGE
		err = stbi_write_png("res.png", x, y, n, output_img, n*x);

		printf("SEG FAULT FINDER, line : %d\n", __LINE__);
		free(input_img);
		free(output_img);
		clReleaseMemObject(input_i);
		clReleaseMemObject(output_i);
		clReleaseSampler(sampler);
		clReleaseDevice(device);
		clReleaseCommandQueue(queue);
		clReleaseContext(context);
		clReleaseProgram(program);
		clReleaseKernel(kernel);

		return EXIT_SUCCESS;
}
