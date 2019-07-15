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

// Utils functions

img_t convert_to_32bits(img_t img, size_t x, size_t y);

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
 *         Name:  oclRelease
 *  Description:  Frees the ocl_env, return EXIT_SUCCES on success.
 * =====================================================================================
 */
img_t blur3x3 (ocl_env_t* env, img_t input, size_t width, size_t height);

img_t gaussian3x3 (ocl_env_t* env, img_t input, size_t width, size_t height);

img_t median3x3 (ocl_env_t* env, img_t input, size_t width, size_t height);

img_t sobel3x3 (ocl_env_t* env, img_t input, size_t width, size_t height);


/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  red, green, blue, grey
 *  Description:  extract one component of an image, or for the grey function , return 
 *  							the greyscale image correcponding to the input;
 * =====================================================================================
 */
img_t red(ocl_env_t* env, img_t input, size_t width, size_t height);

img_t green(ocl_env_t* env, img_t input, size_t width, size_t height);

img_t blue(ocl_env_t* env, img_t input, size_t width, size_t height);

img_t grey(ocl_env_t* env, img_t input, size_t width, size_t height);


/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  add, sub, and, or, xor
 *  Description:  pixel by pixel mathematical operations;
 * =====================================================================================
 */

img_t add(ocl_env_t* env, img_t input1, img_t input2,
								size_t width, size_t height);

img_t sub(ocl_env_t* env, img_t input1, img_t input2,
								size_t width, size_t height);

img_t and(ocl_env_t* env, img_t input1, img_t input2, 
								size_t width, size_t height);

img_t or(ocl_env_t* env, img_t input1, img_t input2,
								size_t width, size_t height);

img_t xor(ocl_env_t* env, img_t input1, img_t input2,
								size_t width, size_t height);


/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  scale
 *  Description:  return the image with the new dims : xcoeff*width, ycoeff*height.
 * =====================================================================================
 */
img_t scale(ocl_env_t* env, img_t input, size_t width, 
								size_t height, float xcoeff, float ycoeff);



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


