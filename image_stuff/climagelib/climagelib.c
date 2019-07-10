/*
 * =====================================================================================
 *
 *       Filename:  climagelib.c
 *
 *    Description:  Implementation of climagelib.h.
 *
 *        Version:  1.0
 *        Created:  10/07/2019 13:40:06
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Thibaut MILHAUD (), thibaut.milhaud@ensiie.fr
 *   Organization:  ENSIIE - SUAI
 *
 * =====================================================================================
 */

#define	__DEV__			/* var used for the dev part of the project */
#include "climagelib.h"


ocl_env_t oclSetup()
{
				ocl_env_t env;

				// PLATFORM
				cl_platform_id* platforms;
				int num_platforms;
				int num_devices;
				int i;

				env.err = clGetPlatformIDs(0, NULL, &num_platforms);

				platforms = (cl_platform_id*) malloc(num_platforms*sizeof(cl_platform_id));

				env.err = clGetPlatformIDs(num_platforms, platforms, NULL);

				// DEVICES
				for(i = 0; i < num_platforms; i++)
				{
								env.err = clGetDeviceIDs(platforms[i], CL_DEVICE_TYPE_GPU, 0, NULL,
																&num_devices);

								if (num_devices > 0)
								{
												env.platform = platforms[i];
												env.err = clGetDeviceIDs(env.platform, CL_DEVICE_TYPE_GPU,
																				1, env.device, NULL);
												break;
								}
				}

				free(platforms);

				// CONTEXT
				env.context = clCreateContext(NULL, 1, &env.device, NULL, NULL, &env.err);

				// COMMAND QUEUE
				env.queue = clCreateCommandQueue(env.context, env.device, 0, &env.err);

				// Program

				char* kernelsource;
#ifndef __DEV__

				kernelsource = ""; // COPY the file in this string
#else      /* -----  __DEV__  ----- */

				FILE* stream;
				size_t filesize;

				stream = fopen("libker.c", "r");
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

#endif     /* -----  __DEV__  ----- */

				env.program = clCreateProgramWithSource(env.context, 1, (const char **) &kernelsource, 
												NULL, &env.err);

				env.err = clBuildProgram(env.program, 1, &env.device, NULL, NULL, NULL);

#ifdef __DEV__
				free(kernelsource);
#endif
				return env;
}

int oclRelease(ocl_env_t* env)
{
				env->err = clReleaseDevice(env->device);
				env->err |= clReleaseContext(env->context);
				env->err |= clReleaseCommandQueue(env->queue);
				env->err |= clReleaseProgram(env->program);

				return  env->err;
}

img_t blur3x3 (ocl_env_t* env, img_t input, size_t width, size_t height)
{
				img_t output;
				cl_image_format format;
				cl_mem input_i;
				cl_mem output_i;
				cl_image_desc desc;
				size_t origin[3];
				size_t region[3];
				unsigned char color[4];
				size_t worksize[2];

				cl_kernel blur33;

		output = malloc(sizeof(char)*width*height*4);
		
		c = clock();
		// IMAGE PARAMTERS SETUP
		origin[0] = 0;
		origin[1] = 0;
		origin[2] = 0;
	  region[0] = width;
		region[1] = height;
		region[2] = 1;
		color[0] = 0;
		color[1] = 0;
		color[2] = 0;
		color[3] = 1;

		// FORMAT
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
		// TODO chekc the second parameter. seems OK. double check at some point.
		input_i = clCreateImage(env->context, CL_MEM_READ_ONLY | CL_MEM_USE_HOST_PTR,
						&format, &desc, input, &env->err);

		output_i = clCreateImage(env->context, CL_MEM_WRITE_ONLY, 
						&format, &desc, NULL, &env->err);

		// PUSHING IMAGES INTO THE DEVICE
		err = clEnqueueWriteImage(env->queue, input_i, CL_FALSE, 
						(const size_t*)origin, (const size_t*)region, 0, 0, input,
						0, NULL, NULL);

		err = clEnqueueFillImage(env->queue, output_i, color,
						origin, region, 0, NULL, NULL); 

		// SAMPLER CREATION
		sampler = clCreateSampler(context, CL_FALSE, CL_ADDRESS_CLAMP_TO_EDGE,
						CL_FILTER_NEAREST, &err);

		// KERNEL CREATION
		blur33 = clCreateKernel(env->program, "blur33", &env->err);

		clFinish(env->queue);

		// KERNEL ARGS
		env->err = clSetKernelArg(blur33, 0, sizeof(cl_mem), &input_i);
		env->err |= clSetKernelArg(blur33, 1, sizeof(cl_mem), &output_i);
		env->err |= clSetKernelArg(blur33, 2, sizeof(cl_sampler), &sampler);
		return output;
}

img_t gaussian3x3 (ocl_env_t* env, img_t input, size_t width, size_t height)
{
				//TODO
				img_t output;
				return output;
}

img_t median3x3 (ocl_env_t* env, img_t input, size_t width, size_t height)
{
				//TODO
				img_t output;
				return output;
}
