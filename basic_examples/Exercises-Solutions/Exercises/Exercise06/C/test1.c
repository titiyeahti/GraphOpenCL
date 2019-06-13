#include "matmul.h"
#include "matrix_lib.h"
#include "err_code.h"
#include "device_picker.h"




int main(int argc, char *argv[])
{
	
	//PLATFORM

	cl_platform_id platforms[2];
	cl_int err;
	cl_uint num_platforms;

	err = clGetPlatformIDs(4, platforms, &num_platforms);

	checkError(err, "Getplatform");

	int i = 0;

	printf("%d\n", num_platforms);
	
	char buffer[1024];
	size_t size;
	
	for (i =0; i<2; i++)
	{
			buffer[0] = '\0';
			err = clGetPlatformInfo(platforms[i], CL_PLATFORM_NAME, 1024, buffer, &size);
			printf("%s\n", buffer);
	}

	//DEVICE
	
	cl_device_id device;
	cl_uint num_devices;

	err = clGetDeviceIDs(platforms[0], CL_DEVICE_TYPE_ALL, 4, &device, &num_devices);

	checkError(err, "GetDev");

	for(i=0; i<num_devices; i++)
	{
			buffer[0] = '\0';
			err = clGetDeviceInfo(device, CL_DEVICE_VENDOR, 1024, buffer, &size);
			printf("%s\n", buffer);
	}

	//CONTEXT
	
	cl_context context;
	context = clCreateContext(NULL, 1, &device, NULL, NULL, &err);
	
	checkError(err, "creating context");
	//COMMAND QUEUE

	cl_command_queue queue;
	queue = clCreateCommandQueue(context, device, 0, &err);

	checkError(err, "creating queue");
	//PROGRAM

	FILE * stream = fopen("matmul1.cl", "r");
	if (stream == NULL)
			printf("error in the opening of the file\n");
	
	long filesize;
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
	//MEMORY

	float* h_A, * h_B, * h_C;
	int N;
	int dim;

	N = 1 <<10;
	dim = N*N;

	h_A = (float*)malloc (dim*sizeof(float));
	h_B = (float*)malloc (dim*sizeof(float));
	h_C = (float*)malloc (dim*sizeof(float));


	for (i = 0; i<dim; i++)
	{
			if (i % N == i/N)
					h_A[i] = 1;
			else 
					h_A[i] = 0;			// A = ID(N)
			h_B[i] = 1;					// B = Only ones
			h_C[i] = 0;					// 0
	}

	cl_mem b_A, b_B, b_C;

	b_A = clCreateBuffer(context, CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR,
					sizeof(float)*dim, h_A, &err);
	checkError(err, "Buff A");
	b_B = clCreateBuffer(context, CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR,
					sizeof(float)*dim, h_B, &err);
	checkError(err, "Buff B");
	b_C = clCreateBuffer(context, CL_MEM_WRITE_ONLY,
					sizeof(float)*dim, NULL, &err);
	checkError(err, "Buff C");

	//KERNEL

	const char* name = "mmul";
	cl_kernel kernel;
	kernel = clCreateKernel(program, name, &err);
	checkError(err, "Create kernel");

	err = clSetKernelArg(kernel, 0, sizeof(int), &N);
	checkError(err, "Set args");
	err |= clSetKernelArg(kernel , 1, sizeof(cl_mem), &b_A);
	checkError(err, "Set args");
	err |= clSetKernelArg(kernel , 2, sizeof(cl_mem), &b_B);
	checkError(err, "Set args");
	err |= clSetKernelArg(kernel , 3, sizeof(cl_mem), &b_C);
	checkError(err, "Set args");
	err |= clSetKernelArg(kernel , 4, sizeof(float)*N, NULL);

	checkError(err, "Set args");
	size_t worksize[2]; 

	worksize[0] = N;
	worksize[1] = N/64; 

	double time;

	time = wtime();
	err = clEnqueueNDRangeKernel(queue, 
					kernel, 
					1, 
					NULL, 
					worksize, 
					&(worksize[1]), 
					0,
					NULL,
					NULL);
	
	time = wtime() - time;
	checkError(err, "Enqueue Args");

	err = clEnqueueReadBuffer(
					queue, b_C, CL_TRUE, 0,
					sizeof(float) * dim, h_C,
					0, NULL, NULL);

	for(i = 0; i < dim; i++)
	{
			float val = h_C[i];
			if ((val-1)*(val-1)>0.01)
			{
					int q, r;
					q = i/N;
					r = i%N;
					printf("erreur de calcul : C[%d][%d] = %f\n", q, r, h_C[i]);
			}
	}
	printf("%f\n", time);

	results(N, h_C, time);

	
	//FREEZONE

	clReleaseContext(context);
	clReleaseCommandQueue(queue);
	clReleaseProgram(program);
	clReleaseMemObject(b_A);
	clReleaseMemObject(b_B);
	clReleaseMemObject(b_C);
	clReleaseKernel(kernel);
	free(kernelsource);
	free(h_A);
	free(h_B);
	free(h_C);
    return EXIT_SUCCESS;
}
