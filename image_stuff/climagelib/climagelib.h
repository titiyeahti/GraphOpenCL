/*
 * =====================================================================================
 *
 *       Filename:  climagelib.h
 *
 *    Description:  This file is the interface for climaeglib.c, an api using OpenCL
 *    							to do some images manipulations.
 *
 *          Usage:  This API will work for 32-bit img, each pixel's component being 
 *          				store as an unsigned char in an array.
 *          				To make this API work, you will also have to create an ocl_env
 *          				using the appropriate function (oclSetup).
 *
 *        Version:  1.0
 *        Created:  10/07/2019 09:56:23
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
#include <CL/cl.h>


struct ocl_env {
				cl_int err;
				cl_platform_id platform;
				cl_device_id device;
				cl_context context;
				cl_command_queue queue;
				cl_program program;
};				/* ----------  end of struct ocl_env  ---------- */

typedef struct ocl_env ocl_env_t;

/*
 * To prevent errors to happening, the type of data for pixels components
 * is fixed to unsigned char.
 */
typedef unsigned char * img_t;



// OPENCL 

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  oclSetup
 *  Description:  Sets up the OpenCL environnement for the others functions
 * =====================================================================================
 */
ocl_env_t oclSetup (void);

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  oclRelease
 *  Description:  Frees the ocl_env, return EXIT_SUCCES on success.
 * =====================================================================================
 */
int oclRelease (ocl_env_t* env);



// 3x3 FAST FILTERING FUNCTIONS

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  blur3x3
 *  Description:  Return a blurred verion of the input image where width and height 
 *  							are the image dimensions.
 * =====================================================================================
 */
img_t blur3x3 (ocl_env_t* env, img_t input, size_t width, size_t height);


/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  gaussian3x3
 *  Description:  
 * =====================================================================================
 */
img_t gaussian3x3 (ocl_env_t* env, img_t input, size_t width, size_t height);


/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  median3x3
 *  Description:  Return 
 * =====================================================================================
 */
img_t median3x3 (ocl_env_t* env, img_t input, size_t width, size_t height);



// CUSTOMIZABLE FILTERING FUNCTIONS 

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  custom_filter
 *  Description:  Applies the filter passed in argument to each pixel.
 *  							The filter should be and array of filter_range² floats.
 * =====================================================================================
 */
img_t custom_filter(ocl_env_t* env, img_t input, size_t width, size_t height, 
								float* filter, size_t filter_range);


