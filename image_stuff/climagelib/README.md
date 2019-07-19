# climagelib

## Brief

This library contains some images manipulations functions running with OpenCL.

## Dependancies

To use this library, you only need `<Cl/cl.h>` and the OpenCL's drivers for your GPU. 

## Types & Structures

* `ocl_env_t`, the struct containing all OpenCL's variables.
* `img_t`, an alias for `unsigned int *`.

## Dowload & Make

0. Dowload
	* download climagelib.[hc], libker.c and Makefile.
1. Compile using gcc :
	* to compile the object code of the library : `gcc -c climagelib.c -03 -std=c99 -ffast-math`;
	* to compile your code : `gcc climagelib.o [yourfile] -03 -std=c99 -ffast-math -lm -lOpenCL -fopenmp -o [youroutput]`.
2. Compile using the Makefile :
	* suppres the `-I $(UTILS)` options from it;
	* make your receipe slightly by modifying the `test.out` receipe (replace `testzone.c` by `yourfile.c` and `test.out` by whatever you want.
	
## Usage 

0. **IMPORTANT** Make sure that `libker.c` is in the same folder as your program when you are running it (since the kernels are compiled at runtime and they are reed from thi file during `ocl_setup`);
1. **IMPORTANT** Make sure you work with 32-bit images (RGBA) with 8-bit unsigned integers for each channel;
2. `#include "climagelib.h"` on top of your file;
3. Create an `ocl_env_t`;
4. Use the functions;
5. Be aware that each function return a new pointer, so remind to free intermediate results.

## List of functions

0. Miscellanous
	* `img_t convert_to_32bits(img_t img, size_t x, size_t y)` convert the 24-Bits image `img` into a 32-Bits image by setting the alpa channel to 255;
1. OpenCL environnement functions :
	* `ocl_env_t oclSetup (void)` sets up an `ocl_env_t` for the others functions;
	* `int oclRelease (ocl_env_t* env)` frees the memory used by `env`;
2. Fixed size filters, they all take the same parameters (the ocl env, the input image, the width and the height of the input)
	* `img_t oclBlur3x3 (ocl_env_t* env, img_t input, size_t width, size_t height)` returns a blurred version of `input`;
	* `img_t oclGaussian3x3 (ocl_env_t* env, img_t input, size_t width, size_t height)` returns the input passed through a 3x3 gaussian filter;
	* `img_t oclMedian3x3 (ocl_env_t* env, img_t input, size_t width, size_t height)`;
	* `img_t oclSobel3x3 (ocl_env_t* env, img_t input, size_t width, size_t height)` edge detection filter;
3. Variable size filters, these functions take the same arguments as their 3x3 equivalent plus `n` the size of the filter :
	* `img_t oclBlurNxN (ocl_env_t* env, img_t input, size_t width, size_t height, int n)`;
	* `img_t oclGaussianNxN (ocl_env_t* env, img_t input, size_t width, size_t height, int n)`;
4. Conponent extraction, same parameters as before :
	* `img_t oclRed(ocl_env_t* env, img_t input, size_t width, size_t height)`;
	* `img_t oclGreen(ocl_env_t* env, img_t input, size_t width, size_t height)`;
	* `img_t oclBlue(ocl_env_t* env, img_t input, size_t width, size_t height)`;
	* `img_t oclGrey(ocl_env_t* env, img_t input, size_t width, size_t height)` greyscale conversion of the image;
5. Pixel by pixel mathematical and logical operations :
	* `img_t oclAdd(ocl_env_t* env, img_t input1, img_t input2, size_t width, size_t height)`;
	* `img_t oclSub(ocl_env_t* env, img_t input1, img_t input2, size_t width, size_t height)`;
	* `img_t oclAnd(ocl_env_t* env, img_t input1, img_t input2, size_t width, size_t height)`;
	* `img_t oclOr(ocl_env_t* env, img_t input1, img_t input2, size_t width, size_t height)`;
	* `img_t oclXor(ocl_env_t* env, img_t input1, img_t input2, size_t width, size_t height)`;
6. Multiplication or division by a float constant `coeff` :
	* `img_t oclMult(ocl_env_t* env, img_t input, size_t width, size_t height, float coeff)`;
	* `img_t oclDiv(ocl_env_t* env, img_t input, size_t width, size_t height, float coeff)`;
7. Scale function, returns the image resized to (xcoeff\*width)x(ycoeff\*height):
	* `img_t oclScale(ocl_env_t* env, img_t input, size_t width, size_t height, float xcoeff, float ycoeff)`;
8. Histogram, return an histogram of the image:
	* format $hist[256i+16j+k]$ contain the frequency of the pixels in the following ;set $S(i,j,k)={r,g,b \in [0,255]^3, 16i \leq r < 16(i+1),16j \leq g < 16(j+1),16k \leq b < 16(k+1)};
	* `float* oclHistogram(ocl_env_t* env, img_t input, size_t width, size_t height)`;

## CONTACT

If any problem encountered, you can ask for help at thibaut.milhaud@gmail.com.
