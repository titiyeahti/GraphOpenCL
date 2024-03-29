/*
 * =====================================================================================
 *
 *       Filename:  testzone.c
 *
 *    Description:  A place to test climagelib's functions.
 *
 *        Version:  1.0
 *        Created:  11/07/2019 10:37:26
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Thibaut MILHAUD (), thibaut.milhaud@ensiie.fr
 *   Organization:  ENSIIE - SUAI
 *
 * =====================================================================================
 */
#include <time.h>
#include "climagelib.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"


int main(int argc, char** arvg)
{
				ocl_env_t env;
				int x, y, n;
				clock_t c;

				img_t output;
				c = clock();
				env = oclSetup();

				img_t temp = stbi_load("img/src.jpg", &x, &y, &n, 0);

				printf("x = %d, y = %d\n", x, y);
				img_t input = convert_to_32bits(temp, (size_t) x, (size_t) y);
				n++;

				free(temp);
				c = clock() - c;
				printf("Init : %f\n", (double) c/CLOCKS_PER_SEC);

				c = clock();
				output = oclBlur3x3(&env, input, (size_t)x, (size_t)y);
				c = clock() - c;
				printf("Blur : %f\n", (double) c/CLOCKS_PER_SEC);

				stbi_write_jpg("img/blur.jpg", x, y, n, output, 100);

				free(output);
			

				c = clock();
				output = oclMedian3x3(&env, input, (size_t)x, (size_t)y);
				c = clock() - c;
				printf("median : %f\n", (double) c/CLOCKS_PER_SEC);
				stbi_write_jpg("img/med.jpg", x, y, n, output, 100);
				
				free(output);
				
				c = clock();
				output = oclGaussian3x3(&env, input, (size_t)x, (size_t)y);
				c = clock() - c;
				printf("gaussian : %f\n", (double) c/CLOCKS_PER_SEC);
				stbi_write_jpg("img/gauss.jpg", x, y, n, output, 100);
				
				free(output);

				c = clock();
				output = oclRed(&env, input, (size_t)x, (size_t)y);
				c = clock() - c;
				printf("red : %f\n", (double) c/CLOCKS_PER_SEC);
				stbi_write_jpg("img/red.jpg", x, y, n, output, 100);
				
				free(output);

				c = clock();
				output = oclGreen(&env, input, (size_t)x, (size_t)y);
				c = clock() - c;
				printf("green : %f\n", (double) c/CLOCKS_PER_SEC);
				stbi_write_jpg("img/green.jpg", x, y, n, output, 100);
				
				free(output);

				c = clock();
				output = oclBlue(&env, input, (size_t)x, (size_t)y);
				c = clock() - c;
				printf("blue : %f\n", (double) c/CLOCKS_PER_SEC);
				stbi_write_jpg("img/blue.jpg", x, y, n, output, 100);

				free(output);

				c = clock();
				output = oclGrey(&env, input, (size_t)x, (size_t)y);
				c = clock() - c;
				printf("grey : %f\n", (double) c/CLOCKS_PER_SEC);
				stbi_write_jpg("img/grey.jpg", x, y, n, output, 100);
				
				free(output);

				c = clock();
				output = oclSobel3x3(&env, input, (size_t)x, (size_t)y);
				c = clock() - c;
				printf("sobel : %f\n", (double) c/CLOCKS_PER_SEC);
				stbi_write_jpg("img/sobel.jpg", x, y, n, output, 100);
				
				img_t input1 = output;

				c = clock();
				output = oclAdd(&env, input, input1, (size_t)x, (size_t)y);
				c = clock() - c;
				printf("add : %f\n", (double) c/CLOCKS_PER_SEC);
				stbi_write_jpg("img/add.jpg", x, y, n, output, 100);
				
				free(output);

				c = clock();
				output = oclSub(&env, input, input1, (size_t)x, (size_t)y);
				c = clock() - c;
				printf("sub : %f\n", (double) c/CLOCKS_PER_SEC);
				stbi_write_jpg("img/sub.jpg", x, y, n, output, 100);
				
				free(output);

				c = clock();
				output = oclAnd(&env, input, input1, (size_t)x, (size_t)y);
				c = clock() - c;
				printf("and : %f\n", (double) c/CLOCKS_PER_SEC);
				stbi_write_jpg("img/and.jpg", x, y, n, output, 100);
				
				free(output);

				c = clock();
				output = oclOr(&env, input, input1, (size_t)x, (size_t)y);
				c = clock() - c;
				printf("or : %f\n", (double) c/CLOCKS_PER_SEC);
				stbi_write_jpg("img/or.jpg", x, y, n, output, 100);
				
				free(output);

				c = clock();
				output = oclXor(&env, input, input1, (size_t)x, (size_t)y);
				c = clock() - c;
				printf("xor : %f\n", (double) c/CLOCKS_PER_SEC);
				stbi_write_jpg("img/xor.jpg", x, y, n, output, 100);
				
				free(output);

				c = clock();
				output = oclScale(&env, input, (size_t)x, (size_t)y, 0.5, 1.5);
				c = clock() - c;
				printf("scale : %f\n", (double) c/CLOCKS_PER_SEC);
				stbi_write_jpg("img/scale.jpg", (int)(x/2), (int)(y*1.5), n, output, 100);
				
				c = clock();
				output = oclMult(&env, input, (size_t)x, (size_t)y, 2.0);
				c = clock() - c;
				printf("mult : %f\n", (double) c/CLOCKS_PER_SEC);
				stbi_write_jpg("img/mult2.jpg", x, y, n, output, 100);

				c = clock();
				output = oclDiv(&env, input, (size_t)x, (size_t)y, 2.0);
				c = clock() - c;
				printf("div : %f\n", (double) c/CLOCKS_PER_SEC);
				stbi_write_jpg("img/div2.jpg", x, y, n, output, 100);

				c = clock();
				float* hist = oclHistogram(&env, input, (size_t)x, (size_t)y);
				c = clock() - c;
				printf("Histogram : %f\n", (double) c/CLOCKS_PER_SEC);

				int i;
				float sum=0;
				for(i=0; i<4096; i++)
				{
								sum += hist[i];
				}
				printf("somme hist = %f\n", sum);
				
				free(output);

				free(input);

//				free(input1);
				return oclRelease(&env);
}
