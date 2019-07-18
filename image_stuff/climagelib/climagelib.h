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


/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  oclBlur3x3, oclMedian3x3, oclGaussian3x3, oclSobel3x3
 *  Description:  apply a 3x3 filter on each pixel of the image.
 * =====================================================================================
 */
img_t oclBlur3x3 (ocl_env_t* env, img_t input, size_t width, size_t height);

img_t oclGaussian3x3 (ocl_env_t* env, img_t input, size_t width, size_t height);

img_t oclMedian3x3 (ocl_env_t* env, img_t input, size_t width, size_t height);

img_t oclSobel3x3 (ocl_env_t* env, img_t input, size_t width, size_t height);


/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  oclBlurNxN, oclGaussianNxN, oclSobelNxN
 *  Description:  apply a nxn filter on each pixel of the image.
 * =====================================================================================
 */
img_t oclBlurNxN (ocl_env_t* env, img_t input, size_t width, size_t height, int n);

img_t oclGaussianNxN (ocl_env_t* env, img_t input, size_t width, size_t height, int n);


/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  oclRed, oclGreen, oclBlue, oclGrey
 *  Description:  extract one component of an image, or for the grey function, return 
 *  							the greyscale image correcponding to the input;
 * =====================================================================================
 */
img_t oclRed(ocl_env_t* env, img_t input, size_t width, size_t height);

img_t oclGreen(ocl_env_t* env, img_t input, size_t width, size_t height);

img_t oclBlue(ocl_env_t* env, img_t input, size_t width, size_t height);

img_t oclGrey(ocl_env_t* env, img_t input, size_t width, size_t height);


/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  add, sub, and, or, xor
 *  Description:  pixel by pixel mathematical operations;
 * =====================================================================================
 */

img_t oclAdd(ocl_env_t* env, img_t input1, img_t input2,
								size_t width, size_t height);

img_t oclSub(ocl_env_t* env, img_t input1, img_t input2,
								size_t width, size_t height);

img_t oclAnd(ocl_env_t* env, img_t input1, img_t input2, 
								size_t width, size_t height);

img_t oclOr(ocl_env_t* env, img_t input1, img_t input2,
								size_t width, size_t height);

img_t oclXor(ocl_env_t* env, img_t input1, img_t input2,
								size_t width, size_t height);


/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  oclMult, oclDiv
 *  Description:  multiply or divide each pixel of the image by coeff;
 * =====================================================================================
 */
img_t oclMult(ocl_env_t* env, img_t input, 
								size_t width, size_t height, float coeff);

img_t oclDiv(ocl_env_t* env, img_t input, 
								size_t width, size_t height, float coeff);

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  scale
 *  Description:  return the image with the new dims : xcoeff*width, ycoeff*height.
 * =====================================================================================
 */
img_t oclScale(ocl_env_t* env, img_t input, size_t width, 
								size_t height, float xcoeff, float ycoeff);



/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  histogram
 *  Description:  return the histogram of the input image.
 *  							the format is the following :
 *  							hist[256*i + 16*j + k] contain the frequency of the pixels in the
 *  							following set (S(i, j, k) = {r, g, b \in [0, 255]^3,
 *  									16*i <= r < 16(i+1) 
 *  									16*j <= g < 16(j+1) 
 *  									16*k <= b < 16(k+1)} 
 * =====================================================================================
 */
float* oclHistogram(ocl_env_t* env, img_t input, size_t width, 
								size_t height);
