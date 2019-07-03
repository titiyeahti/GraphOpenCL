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
#include <math.h>
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


unsigned char* edgeDetection(
				unsigned char* input_img, 
				int y, int x, int n)
{
		int i, j, k;
		clock_t c;
		unsigned char* output_img;
		float colorv[4];
		float colorh[4];
		float temp;
		
		colorv[3] = 1;
		colorh[3] = 1;
		
		output_img = malloc(sizeof(char)*x*y*n);
		printf("table size :%ld\n", (size_t) x*n*y);
		
		c = clock();
		for(i=0; i<x; i++)
		{
				for(j=0; j<y; j++)
				{
						for(k=0; k<3; k++)
						{
								colorh[k] = 0;
								colorv[k] = 0;
						}

						for(k=0; k<3; k++)
						{

								colorv[k] += i+1<x?
										((float)input_img[(y*(i+1)+j)*n+k]*2/255):0;
								colorv[k] += (i+1<x)&&(j-1>0)?
										((float)input_img[(y*(i+1)+j-1)*n+k]/255):0;
								colorv[k] += (i+1<x)&&(j+1<y)?
										((float)input_img[(y*(i+1)+j+1)*n+k]/255):0;

								colorv[k] -= i-1>0?
										((float)input_img[(y*(i-1)+j)*n+k]*2/255):0;
								colorv[k] -= (i-1>0)&&(j-1>0)?
										((float)input_img[(y*(i-1)+j-1)*n+k]/255):0;
								colorv[k] -= (i-1>0)&&(j+1<y)?
										((float)input_img[(y*(i-1)+j+1)*n+k]/255):0;
								
								
								colorh[k] += j+1<y?
										((float)input_img[(y*i+j+1)*n+k]*2/255):0;
								colorh[k] += (j+1<y)&&(i-1>0)?
										((float)input_img[(y*(i-1)+j+1)*n+k]/255):0;
								colorh[k] += (j+1<y)&&(i+1<x)?
										((float)input_img[(y*(i+1)+j+1)*n+k]/255):0;

								colorh[k] -= j-1>0?
										((float)input_img[(y*i+j-1)*n+k]*2/255):0;
								colorh[k] -= (j-1>0)&&(i-1>0)?
										((float)input_img[(y*(i-1)+j-1)*n+k]/255):0;
								colorh[k] -= (j-1>0)&&(i+1<x)?
										((float)input_img[(y*(i+1)+j-1)*n+k]/255):0;
						}

						for(k=0; k<3; k++)
						{
								temp = 64*sqrt(colorh[k]*colorh[k] + colorv[k]*colorv[k]);
								
								output_img[(y*i+j)*n+k] = (unsigned char)temp;
						}
						output_img[3] = input_img[3];
				}
		}
		c = clock() -c;

		printf("SEQ TIME %d %f\n", x*y,((float)c/ CLOCKS_PER_SEC));

		return output_img;
}

unsigned char* clEdgeDetection(
				unsigned char* input_img, 
				cl_context context, 
				cl_command_queue queue, 
				cl_kernel kernel,
				int x, int y, int n)
{	
		// VARS
		unsigned char* output_img;
		cl_int err;
		cl_image_format format;
		cl_mem input_i;
		cl_mem output_i;
		cl_image_desc desc;
		cl_sampler sampler;
		size_t origin[3];
		size_t region[3];
		unsigned char color[4];
		size_t worksize[2];
		clock_t c;


		output_img = malloc(sizeof(char)*x*y*n);
		
		c = clock();
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

		err = clEnqueueNDRangeKernel(queue, kernel, 2, NULL, worksize, NULL,
						0, NULL, NULL);
		checkError(err, "Kernels execution");

		err = clFinish(queue);

		err = clEnqueueReadImage(queue, output_i, CL_FALSE, origin, region, 
						0, 0, output_img, 0, NULL, NULL);

		err = clFinish(queue);
		
		c = clock() -c;

		printf("Parralel TIME %d %f\n", x*y,((float)c/ CLOCKS_PER_SEC));

		clReleaseMemObject(input_i);
		clReleaseMemObject(output_i);
		clReleaseSampler(sampler);
		return output_img;
}

int main(int argc, char** argv)
{
		// VARIABLES
		cl_int err;
		char * filename = argc>1 ? argv[1]: "sputnik.jpg";
		char * kernelsource;
		int i, x, y, n;
		unsigned char* temp;
		unsigned char* input_img;
		unsigned char* output_img;
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
		// END VARIABLES


		// Loading image
		temp = stbi_load(filename, &x, &y, &n, 0);
		input_img = convert_to_32bits(temp, x, y);
		n++;
		free(temp);
		
		//input_img = stbi_load(filename, &x, &y, &n, 0);



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

		// CALCUL 
		//output_img = clEdgeDetection(input_img, context, queue, kernel, x, y, n);
		//free(output_img);
		//output_img = NULL;
		output_img = edgeDetection(input_img, x, y, n);

		// SAVING THE IMAGE
		err = stbi_write_jpg("res.jpg", x, y, n, output_img, 100);

		free(input_img);
		free(output_img);
		clReleaseDevice(device);
		clReleaseCommandQueue(queue);
		clReleaseContext(context);
		clReleaseProgram(program);
		clReleaseKernel(kernel);

		return EXIT_SUCCESS;
}
