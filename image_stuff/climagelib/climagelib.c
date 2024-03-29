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

const char *err_code (cl_int err_in)
{
				switch (err_in) {
								case CL_SUCCESS:
												return (char*)"CL_SUCCESS";
								case CL_DEVICE_NOT_FOUND:
												return (char*)"CL_DEVICE_NOT_FOUND";
								case CL_DEVICE_NOT_AVAILABLE:
												return (char*)"CL_DEVICE_NOT_AVAILABLE";
								case CL_COMPILER_NOT_AVAILABLE:
												return (char*)"CL_COMPILER_NOT_AVAILABLE";
								case CL_MEM_OBJECT_ALLOCATION_FAILURE:
												return (char*)"CL_MEM_OBJECT_ALLOCATION_FAILURE";
								case CL_OUT_OF_RESOURCES:
												return (char*)"CL_OUT_OF_RESOURCES";
								case CL_OUT_OF_HOST_MEMORY:
												return (char*)"CL_OUT_OF_HOST_MEMORY";
								case CL_PROFILING_INFO_NOT_AVAILABLE:
												return (char*)"CL_PROFILING_INFO_NOT_AVAILABLE";
								case CL_MEM_COPY_OVERLAP:
												return (char*)"CL_MEM_COPY_OVERLAP";
								case CL_IMAGE_FORMAT_MISMATCH:
												return (char*)"CL_IMAGE_FORMAT_MISMATCH";
								case CL_IMAGE_FORMAT_NOT_SUPPORTED:
												return (char*)"CL_IMAGE_FORMAT_NOT_SUPPORTED";
								case CL_BUILD_PROGRAM_FAILURE:
												return (char*)"CL_BUILD_PROGRAM_FAILURE";
								case CL_MAP_FAILURE:
												return (char*)"CL_MAP_FAILURE";
								case CL_MISALIGNED_SUB_BUFFER_OFFSET:
												return (char*)"CL_MISALIGNED_SUB_BUFFER_OFFSET";
								case CL_EXEC_STATUS_ERROR_FOR_EVENTS_IN_WAIT_LIST:
												return (char*)"CL_EXEC_STATUS_ERROR_FOR_EVENTS_IN_WAIT_LIST";
								case CL_INVALID_VALUE:
												return (char*)"CL_INVALID_VALUE";
								case CL_INVALID_DEVICE_TYPE:
												return (char*)"CL_INVALID_DEVICE_TYPE";
								case CL_INVALID_PLATFORM:
												return (char*)"CL_INVALID_PLATFORM";
								case CL_INVALID_DEVICE:
												return (char*)"CL_INVALID_DEVICE";
								case CL_INVALID_CONTEXT:
												return (char*)"CL_INVALID_CONTEXT";
								case CL_INVALID_QUEUE_PROPERTIES:
												return (char*)"CL_INVALID_QUEUE_PROPERTIES";
								case CL_INVALID_COMMAND_QUEUE:
												return (char*)"CL_INVALID_COMMAND_QUEUE";
								case CL_INVALID_HOST_PTR:
												return (char*)"CL_INVALID_HOST_PTR";
								case CL_INVALID_MEM_OBJECT:
												return (char*)"CL_INVALID_MEM_OBJECT";
								case CL_INVALID_IMAGE_FORMAT_DESCRIPTOR:
												return (char*)"CL_INVALID_IMAGE_FORMAT_DESCRIPTOR";
								case CL_INVALID_IMAGE_SIZE:
												return (char*)"CL_INVALID_IMAGE_SIZE";
								case CL_INVALID_SAMPLER:
												return (char*)"CL_INVALID_SAMPLER";
								case CL_INVALID_BINARY:
												return (char*)"CL_INVALID_BINARY";
								case CL_INVALID_BUILD_OPTIONS:
												return (char*)"CL_INVALID_BUILD_OPTIONS";
								case CL_INVALID_PROGRAM:
												return (char*)"CL_INVALID_PROGRAM";
								case CL_INVALID_PROGRAM_EXECUTABLE:
												return (char*)"CL_INVALID_PROGRAM_EXECUTABLE";
								case CL_INVALID_KERNEL_NAME:
												return (char*)"CL_INVALID_KERNEL_NAME";
								case CL_INVALID_KERNEL_DEFINITION:
												return (char*)"CL_INVALID_KERNEL_DEFINITION";
								case CL_INVALID_KERNEL:
												return (char*)"CL_INVALID_KERNEL";
								case CL_INVALID_ARG_INDEX:
												return (char*)"CL_INVALID_ARG_INDEX";
								case CL_INVALID_ARG_VALUE:
												return (char*)"CL_INVALID_ARG_VALUE";
								case CL_INVALID_ARG_SIZE:
												return (char*)"CL_INVALID_ARG_SIZE";
								case CL_INVALID_KERNEL_ARGS:
												return (char*)"CL_INVALID_KERNEL_ARGS";
								case CL_INVALID_WORK_DIMENSION:
												return (char*)"CL_INVALID_WORK_DIMENSION";
								case CL_INVALID_WORK_GROUP_SIZE:
												return (char*)"CL_INVALID_WORK_GROUP_SIZE";
								case CL_INVALID_WORK_ITEM_SIZE:
												return (char*)"CL_INVALID_WORK_ITEM_SIZE";
								case CL_INVALID_GLOBAL_OFFSET:
												return (char*)"CL_INVALID_GLOBAL_OFFSET";
								case CL_INVALID_EVENT_WAIT_LIST:
												return (char*)"CL_INVALID_EVENT_WAIT_LIST";
								case CL_INVALID_EVENT:
												return (char*)"CL_INVALID_EVENT";
								case CL_INVALID_OPERATION:
												return (char*)"CL_INVALID_OPERATION";
								case CL_INVALID_GL_OBJECT:
												return (char*)"CL_INVALID_GL_OBJECT";
								case CL_INVALID_BUFFER_SIZE:
												return (char*)"CL_INVALID_BUFFER_SIZE";
								case CL_INVALID_MIP_LEVEL:
												return (char*)"CL_INVALID_MIP_LEVEL";
								case CL_INVALID_GLOBAL_WORK_SIZE:
												return (char*)"CL_INVALID_GLOBAL_WORK_SIZE";
								case CL_INVALID_PROPERTY:
												return (char*)"CL_INVALID_PROPERTY";

								default:
												return (char*)"UNKNOWN ERROR";
				}
}

img_t convert_to_32bits(img_t img, size_t x, size_t y)
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

void check_error(cl_int err, const char *operation, char *filename, int line)
{
				if (err != CL_SUCCESS)
				{
								fprintf(stderr, "Error during operation '%s', ", operation);
								fprintf(stderr, "in '%s' on line %d\n", filename, line);
								fprintf(stderr, "Error code was \"%s\" (%d)\n", err_code(err), err);
								exit(EXIT_FAILURE);
				}
}


#define checkError(E, S) check_error(E,S,__FILE__,__LINE__)

ocl_env_t oclSetup()
{
				ocl_env_t env;

				// PLATFORM
				cl_platform_id* platforms;
				cl_uint num_platforms;
				cl_uint num_devices;
				int i;

				env.err = clGetPlatformIDs(0, NULL, &num_platforms);

				platforms = (cl_platform_id*) malloc(num_platforms*sizeof(cl_platform_id));

				env.err = clGetPlatformIDs(num_platforms, platforms, NULL);
				checkError(env.err, "platforms");


				num_devices = 0;
				// DEVICES
				for(i = 0; i < num_platforms; i++)
				{
								env.err = clGetDeviceIDs(platforms[i], CL_DEVICE_TYPE_GPU, 0, NULL,
																&num_devices);
								//checkError(env.err, "num_device");

								if (num_devices > 0)
								{
												env.platform = platforms[i];
												env.err = clGetDeviceIDs(env.platform, CL_DEVICE_TYPE_GPU,
																				1, &env.device, NULL);
												break;
								}
				}

				free(platforms);

				// CONTEXT
				env.context = clCreateContext(NULL, 1, &env.device, NULL, NULL, &env.err);

				checkError(env.err, "context");

				// COMMAND QUEUE
				env.queue = clCreateCommandQueue(env.context, env.device, 0, &env.err);
				checkError(env.err, "queue");

				// Program

				char* kernelsource;
#ifndef __DEV__
				printf("bad place\n");
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
				checkError(env.err, "program creation");

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

/* ======================================================================================
 * ==================================== IMAGE FUNCTIONS =================================
 * ======================================================================================*/


// templates
img_t oneImgNoParam(ocl_env_t* env, img_t input, size_t width, size_t height,
								const char* kername)
{
				img_t output;
				cl_image_format format;
				cl_mem input_i;
				cl_mem output_i;
				cl_image_desc desc;
				cl_sampler sampler;
				size_t origin[3];
				size_t region[3];
				unsigned char color[4];
				size_t worksize[2];

				cl_kernel kernel;

				output = malloc(sizeof(char)*width*height*4);

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
				desc.image_width = width;
				desc.image_height = height;
				desc.image_depth = 0;
				desc.image_array_size = 1;
				desc.image_row_pitch = 0;
				desc.image_slice_pitch = 0;
				desc.num_mip_levels = 0;
				desc.num_samples = 0;
				desc.buffer = NULL;

				// CREATING IMAGE OBJECTS
				input_i = clCreateImage(env->context, CL_MEM_READ_ONLY | CL_MEM_USE_HOST_PTR,
												&format, &desc, input, &env->err);

				output_i = clCreateImage(env->context, CL_MEM_WRITE_ONLY, 
												&format, &desc, NULL, &env->err);

				// PUSHING IMAGES INTO THE DEVICE
				env->err = clEnqueueWriteImage(env->queue, input_i, CL_FALSE, 
												(const size_t*)origin, (const size_t*)region, 0, 0, input,
												0, NULL, NULL);

				//		env->err = clEnqueueFillImage(env->queue, output_i, color,
				//						origin, region, 0, NULL, NULL); 

				// SAMPLER CREATION
				sampler = clCreateSampler(env->context, CL_FALSE, CL_ADDRESS_CLAMP_TO_EDGE,
												CL_FILTER_NEAREST, &env->err);

				// KERNEL CREATION
				kernel = clCreateKernel(env->program, kername, &env->err);
				checkError(env->err, "kernel creation");

				clFinish(env->queue);

				// KERNEL ARGS
				env->err = clSetKernelArg(kernel, 0, sizeof(cl_mem), &input_i);
				env->err |= clSetKernelArg(kernel, 1, sizeof(cl_mem), &output_i);
				env->err |= clSetKernelArg(kernel, 2, sizeof(cl_sampler), &sampler);

				// 
				worksize[0] = width;
				worksize[1] = height;

				env->err = clEnqueueNDRangeKernel(env->queue, kernel, 2, NULL, worksize, NULL,
												0, NULL, NULL);
				checkError(env->err, "kernel ndranged");

				env->err = clFinish(env->queue);

				env->err = clEnqueueReadImage(env->queue, output_i, CL_FALSE, origin, region, 
												0, 0, output, 0, NULL, NULL);

				env->err = clFinish(env->queue);

				clReleaseKernel(kernel);
				clReleaseMemObject(input_i);
				clReleaseMemObject(output_i);
				clReleaseSampler(sampler);

				return output;
}

img_t oneImgOneInt(ocl_env_t* env, img_t input, size_t width, size_t height,
								int param, const char* kername)
{
				img_t output;
				cl_image_format format;
				cl_mem input_i;
				cl_mem output_i;
				cl_image_desc desc;
				cl_sampler sampler;
				size_t origin[3];
				size_t region[3];
				unsigned char color[4];
				size_t worksize[2];

				cl_kernel kernel;

				output = malloc(sizeof(char)*width*height*4);

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
				desc.image_width = width;
				desc.image_height = height;
				desc.image_depth = 0;
				desc.image_array_size = 1;
				desc.image_row_pitch = 0;
				desc.image_slice_pitch = 0;
				desc.num_mip_levels = 0;
				desc.num_samples = 0;
				desc.buffer = NULL;

				// CREATING IMAGE OBJECTS
				input_i = clCreateImage(env->context, CL_MEM_READ_ONLY | CL_MEM_USE_HOST_PTR,
												&format, &desc, input, &env->err);

				output_i = clCreateImage(env->context, CL_MEM_WRITE_ONLY, 
												&format, &desc, NULL, &env->err);

				// PUSHING IMAGES INTO THE DEVICE
				env->err = clEnqueueWriteImage(env->queue, input_i, CL_FALSE, 
												(const size_t*)origin, (const size_t*)region, 0, 0, input,
												0, NULL, NULL);

				//		env->err = clEnqueueFillImage(env->queue, output_i, color,
				//						origin, region, 0, NULL, NULL); 

				// SAMPLER CREATION
				sampler = clCreateSampler(env->context, CL_FALSE, CL_ADDRESS_CLAMP_TO_EDGE,
												CL_FILTER_NEAREST, &env->err);

				// KERNEL CREATION
				kernel = clCreateKernel(env->program, kername, &env->err);
				checkError(env->err, "kernel creation");

				clFinish(env->queue);

				// KERNEL ARGS
				env->err = clSetKernelArg(kernel, 0, sizeof(cl_mem), &input_i);
				env->err |= clSetKernelArg(kernel, 1, sizeof(cl_mem), &output_i);
				env->err |= clSetKernelArg(kernel, 2, sizeof(int), &param);
				env->err |= clSetKernelArg(kernel, 3, sizeof(cl_sampler), &sampler);

				worksize[0] = width;
				worksize[1] = height;

				env->err = clEnqueueNDRangeKernel(env->queue, kernel, 2, NULL, worksize, NULL,
												0, NULL, NULL);
				checkError(env->err, "kernel ndranged");

				env->err = clFinish(env->queue);

				env->err = clEnqueueReadImage(env->queue, output_i, CL_FALSE, origin, region, 
												0, 0, output, 0, NULL, NULL);

				env->err = clFinish(env->queue);

				clReleaseKernel(kernel);
				clReleaseMemObject(input_i);
				clReleaseMemObject(output_i);
				clReleaseSampler(sampler);

				return output;
}

img_t oneImgOneFloat(ocl_env_t* env, img_t input, size_t width, size_t height,
								float param, const char* kername)
{
				img_t output;
				cl_image_format format;
				cl_mem input_i;
				cl_mem output_i;
				cl_image_desc desc;
				cl_sampler sampler;
				size_t origin[3];
				size_t region[3];
				unsigned char color[4];
				size_t worksize[2];

				cl_kernel kernel;

				output = malloc(sizeof(char)*width*height*4);

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
				desc.image_width = width;
				desc.image_height = height;
				desc.image_depth = 0;
				desc.image_array_size = 1;
				desc.image_row_pitch = 0;
				desc.image_slice_pitch = 0;
				desc.num_mip_levels = 0;
				desc.num_samples = 0;
				desc.buffer = NULL;

				// CREATING IMAGE OBJECTS
				input_i = clCreateImage(env->context, CL_MEM_READ_ONLY | CL_MEM_USE_HOST_PTR,
												&format, &desc, input, &env->err);

				output_i = clCreateImage(env->context, CL_MEM_WRITE_ONLY, 
												&format, &desc, NULL, &env->err);

				// PUSHING IMAGES INTO THE DEVICE
				env->err = clEnqueueWriteImage(env->queue, input_i, CL_FALSE, 
												(const size_t*)origin, (const size_t*)region, 0, 0, input,
												0, NULL, NULL);

				//		env->err = clEnqueueFillImage(env->queue, output_i, color,
				//						origin, region, 0, NULL, NULL); 

				// SAMPLER CREATION
				sampler = clCreateSampler(env->context, CL_FALSE, CL_ADDRESS_CLAMP_TO_EDGE,
												CL_FILTER_NEAREST, &env->err);

				// KERNEL CREATION
				kernel = clCreateKernel(env->program, kername, &env->err);
				checkError(env->err, "kernel creation");

				clFinish(env->queue);

				// KERNEL ARGS
				env->err = clSetKernelArg(kernel, 0, sizeof(cl_mem), &input_i);
				env->err |= clSetKernelArg(kernel, 1, sizeof(cl_mem), &output_i);
				env->err |= clSetKernelArg(kernel, 2, sizeof(float), &param);
				env->err |= clSetKernelArg(kernel, 3, sizeof(cl_sampler), &sampler);

				worksize[0] = width;
				worksize[1] = height;

				env->err = clEnqueueNDRangeKernel(env->queue, kernel, 2, NULL, worksize, NULL,
												0, NULL, NULL);
				checkError(env->err, "kernel ndranged");

				env->err = clFinish(env->queue);

				env->err = clEnqueueReadImage(env->queue, output_i, CL_FALSE, origin, region, 
												0, 0, output, 0, NULL, NULL);

				env->err = clFinish(env->queue);

				clReleaseKernel(kernel);
				clReleaseMemObject(input_i);
				clReleaseMemObject(output_i);
				clReleaseSampler(sampler);

				return output;
}

img_t twoImgNoParam(ocl_env_t* env, img_t input1, img_t input2, 
								size_t width, size_t height, char* kername)
{
				img_t output;
				cl_image_format format;
				cl_mem input1_i;
				cl_mem input2_i;
				cl_mem output_i;
				cl_image_desc desc;
				cl_sampler sampler;
				size_t origin[3];
				size_t region[3];
				unsigned char color[4];
				size_t worksize[2];

				cl_kernel kernel;

				output = malloc(sizeof(char)*width*height*4);

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
				format.image_channel_data_type = CL_UNSIGNED_INT8;

				// Filling th cl_image_desc struct 
				desc.image_type = CL_MEM_OBJECT_IMAGE2D;
				desc.image_width = width;
				desc.image_height = height;
				desc.image_depth = 0;
				desc.image_array_size = 1;
				desc.image_row_pitch = 0;
				desc.image_slice_pitch = 0;
				desc.num_mip_levels = 0;
				desc.num_samples = 0;
				desc.buffer = NULL;

				// CREATING IMAGE OBJECTS
				input1_i = clCreateImage(env->context, CL_MEM_READ_ONLY | CL_MEM_USE_HOST_PTR,
												&format, &desc, input1, &env->err);

				input2_i = clCreateImage(env->context, CL_MEM_READ_ONLY | CL_MEM_USE_HOST_PTR,
												&format, &desc, input2, &env->err);

				output_i = clCreateImage(env->context, CL_MEM_WRITE_ONLY, 
												&format, &desc, NULL, &env->err);

				// PUSHING IMAGES INTO THE DEVICE
				env->err = clEnqueueWriteImage(env->queue, input1_i, CL_FALSE, 
												(const size_t*)origin, (const size_t*)region, 0, 0, input1,
												0, NULL, NULL);

				env->err = clEnqueueWriteImage(env->queue, input2_i, CL_FALSE, 
												(const size_t*)origin, (const size_t*)region, 0, 0, input2,
												0, NULL, NULL);
				//		env->err = clEnqueueFillImage(env->queue, output_i, color,
				//						origin, region, 0, NULL, NULL); 

				// SAMPLER CREATION
				sampler = clCreateSampler(env->context, CL_FALSE, CL_ADDRESS_CLAMP_TO_EDGE,
												CL_FILTER_NEAREST, &env->err);

				// KERNEL CREATION
				kernel = clCreateKernel(env->program, kername, &env->err);
				checkError(env->err, "kernel creation");

				clFinish(env->queue);

				// KERNEL ARGS
				env->err = clSetKernelArg(kernel, 0, sizeof(cl_mem), &input1_i);
				env->err = clSetKernelArg(kernel, 1, sizeof(cl_mem), &input2_i);
				env->err |= clSetKernelArg(kernel, 2, sizeof(cl_mem), &output_i);
				env->err |= clSetKernelArg(kernel, 3, sizeof(cl_sampler), &sampler);

				// 
				worksize[0] = width;
				worksize[1] = height;

				env->err = clEnqueueNDRangeKernel(env->queue, kernel, 2, NULL, worksize, NULL,
												0, NULL, NULL);
				checkError(env->err, "kernel ndranged");

				env->err = clFinish(env->queue);

				env->err = clEnqueueReadImage(env->queue, output_i, CL_FALSE, origin, region, 
												0, 0, output, 0, NULL, NULL);

				env->err = clFinish(env->queue);

				clReleaseKernel(kernel);
				clReleaseMemObject(input1_i);
				clReleaseMemObject(input2_i);
				clReleaseMemObject(output_i);
				clReleaseSampler(sampler);

				return output;
}

// RESIZE 

img_t oclScale(ocl_env_t* env, img_t input, size_t width, 
								size_t height, float xcoeff, float ycoeff)
{
				img_t output;
				cl_image_format format;
				cl_mem input_i;
				cl_mem output_i;
				cl_image_desc desc;
				cl_image_desc dest_desc;
				cl_sampler sampler;
				size_t dest_width, dest_height;
				size_t origin[3];
				size_t region[3];
				size_t dest_region[3];
				unsigned char color[4];
				size_t worksize[2];

				dest_width = (size_t) (xcoeff * width);
				dest_height = (size_t) (ycoeff * height);

				cl_kernel kernel;

				output = malloc(sizeof(char)*dest_width*dest_height*4);

				// IMAGE PARAMTERS SETUP
				origin[0] = 0;
				origin[1] = 0;
				origin[2] = 0;
				
				region[0] = width;
				region[1] = height;
				region[2] = 1;
				
				dest_region[0] = dest_width;
				dest_region[1] = dest_height;
				dest_region[2] = 1;
				
				color[0] = 0;
				color[1] = 0;
				color[2] = 0;
				color[3] = 1;

				// FORMAT
				format.image_channel_order = CL_RGBA;
				format.image_channel_data_type = CL_UNORM_INT8;

				// Filling th cl_image_desc struct 
				desc.image_type = CL_MEM_OBJECT_IMAGE2D;
				desc.image_width = width;
				desc.image_height = height;
				desc.image_depth = 0;
				desc.image_array_size = 1;
				desc.image_row_pitch = 0;
				desc.image_slice_pitch = 0;
				desc.num_mip_levels = 0;
				desc.num_samples = 0;
				desc.buffer = NULL;

				dest_desc.image_type = CL_MEM_OBJECT_IMAGE2D;
				dest_desc.image_width = dest_width;
				dest_desc.image_height = dest_height;
				dest_desc.image_depth = 0;
				dest_desc.image_array_size = 1;
				dest_desc.image_row_pitch = 0;
				dest_desc.image_slice_pitch = 0;
				dest_desc.num_mip_levels = 0;
				dest_desc.num_samples = 0;
				dest_desc.buffer = NULL;

				// CREATING IMAGE OBJECTS
				input_i = clCreateImage(env->context, CL_MEM_READ_ONLY | CL_MEM_USE_HOST_PTR,
												&format, &desc, input, &env->err);

				output_i = clCreateImage(env->context, CL_MEM_WRITE_ONLY, 
												&format, &dest_desc, NULL, &env->err);

				// PUSHING IMAGES INTO THE DEVICE
				env->err = clEnqueueWriteImage(env->queue, input_i, CL_FALSE, 
												(const size_t*)origin, (const size_t*)region, 0, 0, input,
												0, NULL, NULL);

				//		env->err = clEnqueueFillImage(env->queue, output_i, color,
				//						origin, region, 0, NULL, NULL); 

				// SAMPLER CREATION
				sampler = clCreateSampler(env->context, CL_TRUE, CL_ADDRESS_CLAMP_TO_EDGE,
												CL_FILTER_NEAREST, &env->err);

				// KERNEL CREATION
				kernel = clCreateKernel(env->program, "scale", &env->err);
				checkError(env->err, "kernel creation");

				clFinish(env->queue);

				// KERNEL ARGS
				env->err = clSetKernelArg(kernel, 0, sizeof(cl_mem), &input_i);
				env->err |= clSetKernelArg(kernel, 1, sizeof(cl_mem), &output_i);
				env->err |= clSetKernelArg(kernel, 2, sizeof(cl_sampler), &sampler);

				// 
				worksize[0] = dest_width;
				worksize[1] = dest_height;

				env->err = clEnqueueNDRangeKernel(env->queue, kernel, 2, NULL, worksize, NULL,
												0, NULL, NULL);
				checkError(env->err, "kernel ndranged");

				env->err = clFinish(env->queue);

				env->err = clEnqueueReadImage(env->queue, output_i, CL_FALSE, origin, dest_region, 
												0, 0, output, 0, NULL, NULL);

				env->err = clFinish(env->queue);

				clReleaseKernel(kernel);
				clReleaseMemObject(input_i);
				clReleaseMemObject(output_i);
				clReleaseSampler(sampler);

				return output;
}

float* oclHistogram(ocl_env_t* env, img_t input, size_t width, size_t height)
{
				const char * kername = "hist";
				cl_image_format format;
				cl_mem input_i;
				cl_mem hist_i;
				cl_mem hist_o;
				cl_image_desc desc;
				cl_sampler sampler;
				size_t hist_size = sizeof(float)*4096;
				float* hist = malloc(hist_size);
				size_t origin[3];
				size_t region[3];
				size_t worksize[2];

				cl_kernel kernel;
				cl_kernel convert;

				// IMAGE PARAMTERS SETUP
				origin[0] = 0;
				origin[1] = 0;
				origin[2] = 0;
				region[0] = width;
				region[1] = height;
				region[2] = 1;

				// FORMAT
				format.image_channel_order = CL_RGBA;
				format.image_channel_data_type = CL_UNSIGNED_INT8;

				// Filling th cl_image_desc struct 
				desc.image_type = CL_MEM_OBJECT_IMAGE2D;
				desc.image_width = width;
				desc.image_height = height;
				desc.image_depth = 0;
				desc.image_array_size = 1;
				desc.image_row_pitch = 0;
				desc.image_slice_pitch = 0;
				desc.num_mip_levels = 0;
				desc.num_samples = 0;
				desc.buffer = NULL;

				// CREATING IMAGE OBJECTS
				input_i = clCreateImage(env->context, CL_MEM_READ_ONLY | CL_MEM_USE_HOST_PTR,
												&format, &desc, input, &env->err);

				hist_i = clCreateBuffer(env->context, CL_MEM_READ_WRITE, hist_size,
												NULL, &env->err);
				
				hist_o = clCreateBuffer(env->context, CL_MEM_WRITE_ONLY, hist_size,
												NULL, &env->err);

				// PUSHING IMAGE INTO THE DEVICE
				env->err = clEnqueueWriteImage(env->queue, input_i, CL_FALSE, 
												(const size_t*)origin, (const size_t*)region, 0, 0, input,
												0, NULL, NULL);

				int zero = 0;
				env->err = clEnqueueFillBuffer(env->queue, hist_i, &zero, 
												sizeof(int), 0, hist_size, 0, NULL, NULL);


				// SAMPLER CREATION
				sampler = clCreateSampler(env->context, CL_FALSE, CL_ADDRESS_CLAMP_TO_EDGE,
												CL_FILTER_NEAREST, &env->err);

				// KERNEL CREATION
				kernel = clCreateKernel(env->program, kername, &env->err);
				checkError(env->err, "kernel creation");

				convert = clCreateKernel(env->program, "convert", &env->err);
				checkError(env->err, "kernel creation");

				clFinish(env->queue);

				// KERNEL ARGS
				env->err = clSetKernelArg(kernel, 0, sizeof(cl_mem), &input_i);
				env->err |= clSetKernelArg(kernel, 1, sizeof(cl_mem), &hist_i);
				env->err |= clSetKernelArg(kernel, 2, sizeof(cl_sampler), &sampler);

				worksize[0] = width;
				worksize[1] = height;

				env->err = clEnqueueNDRangeKernel(env->queue, kernel, 2, NULL, worksize, NULL,
												0, NULL, NULL);
				checkError(env->err, "kernel ndranged");

				env->err = clFinish(env->queue);


				size_t img_size = width*height;

				env->err = clSetKernelArg(convert, 0, sizeof(cl_mem), &hist_i);
				env->err |= clSetKernelArg(convert, 1, sizeof(cl_mem), &hist_o);
				env->err |= clSetKernelArg(convert, 2, sizeof(size_t), &img_size);

				size_t size_for_kernel = 4096;
				env->err = clEnqueueNDRangeKernel(env->queue, convert, 1, NULL, 
												&size_for_kernel, NULL, 0, NULL, NULL);

				env->err = clFinish(env->queue);
				
				env->err = clEnqueueReadBuffer(env->queue, hist_o, CL_FALSE, 0, 
												hist_size, hist, 0, NULL, NULL);

				env->err = clFinish(env->queue);

				clReleaseKernel(kernel);
				clReleaseMemObject(input_i);
				clReleaseMemObject(hist_i);
				clReleaseMemObject(hist_o);
				clReleaseSampler(sampler);

				return hist;
}
// 3x3filtering

img_t oclBlur3x3 (ocl_env_t* env, img_t input, size_t width, size_t height)
{
				return oneImgNoParam(env, input, width, height, "blur33"); 
}

img_t oclGaussian3x3 (ocl_env_t* env, img_t input, size_t width, size_t height)
{
				return oneImgNoParam(env, input, width, height, "gaussian33"); 
}

img_t oclMedian3x3 (ocl_env_t* env, img_t input, size_t width, size_t height)
{
				return oneImgNoParam(env, input, width, height, "median33"); 
}

img_t oclSobel3x3(ocl_env_t* env, img_t input, size_t width, size_t height)
{
				return oneImgNoParam(env, input, width, height, "sobel"); 
}

// NxN filtering
img_t oclBlurNxN (ocl_env_t* env, img_t input, size_t width, size_t height, int n)
{
				return oneImgOneInt(env, input, width, height, n, "blurnn");
}

img_t oclGaussianNxN (ocl_env_t* env, img_t input, size_t width, size_t height, int n)
{
				return oneImgOneInt(env, input, width, height, n, "gaussnn");
}

// component extration

img_t oclRed(ocl_env_t* env, img_t input, size_t width, size_t height)
{
				return oneImgNoParam(env, input, width, height, "red"); 
}

img_t oclGreen(ocl_env_t* env, img_t input, size_t width, size_t height)
{
				return oneImgNoParam(env, input, width, height, "green"); 
}

img_t oclBlue(ocl_env_t* env, img_t input, size_t width, size_t height)
{
				return oneImgNoParam(env, input, width, height, "blue"); 
}

img_t oclGrey(ocl_env_t* env, img_t input, size_t width, size_t height)
{
				return oneImgNoParam(env, input, width, height, "grey"); 
}

// math & logical operations
img_t oclAdd(ocl_env_t* env, img_t input1, img_t input2,
								size_t width, size_t height)
{
				return twoImgNoParam(env, input1, input2, width, height, "add");
}

img_t oclSub(ocl_env_t* env, img_t input1, img_t input2,
								size_t width, size_t height)
{
				return twoImgNoParam(env, input1, input2, width, height, "sub");
}

img_t oclAnd(ocl_env_t* env, img_t input1, img_t input2,
								size_t width, size_t height)
{
				return twoImgNoParam(env, input1, input2, width, height, "and");
}

img_t oclOr(ocl_env_t* env, img_t input1, img_t input2,
								size_t width, size_t height)
{
				return twoImgNoParam(env, input1, input2, width, height, "or");
}

img_t oclXor(ocl_env_t* env, img_t input1, img_t input2,
								size_t width, size_t height)
{
				return twoImgNoParam(env, input1, input2, width, height, "xor");
}

// MULT & DIVIDE BY A CONSTANT

img_t oclMult(ocl_env_t* env, img_t input, 
								size_t width, size_t height, float coeff)
{
				return oneImgOneFloat(env, input, width, height, coeff, "mult");
}

img_t oclDiv(ocl_env_t* env, img_t input, 
								size_t width, size_t height, float coeff)
{
				float new_coeff = 1.0/coeff;
				return oclMult(env, input, width, height, new_coeff);
}
