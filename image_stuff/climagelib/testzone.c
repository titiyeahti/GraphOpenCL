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
				img_t output = blur3x3(&env, input, (size_t)x, (size_t)y);
				c = clock() - c;
				printf("Blur : %f\n", (double) c/CLOCKS_PER_SEC);

//				stbi_write_jpg("img/blur.jpg", x, y, n, output, 100);

				free(output);
			

				c = clock();
				output = median3x3(&env, input, (size_t)x, (size_t)y);
				c = clock() - c;
				printf("median : %f\n", (double) c/CLOCKS_PER_SEC);
//				stbi_write_jpg("img/med.jpg", x, y, n, output, 100);
				
				free(output);
				
				c = clock();
				output = gaussian3x3(&env, input, (size_t)x, (size_t)y);
				c = clock() - c;
				printf("gaussian : %f\n", (double) c/CLOCKS_PER_SEC);
//				stbi_write_jpg("img/gauss.jpg", x, y, n, output, 100);
				
				free(output);

				c = clock();
				output = red(&env, input, (size_t)x, (size_t)y);
				c = clock() - c;
				printf("red : %f\n", (double) c/CLOCKS_PER_SEC);
//				stbi_write_jpg("img/red.jpg", x, y, n, output, 100);
				
				free(output);

				c = clock();
				output = green(&env, input, (size_t)x, (size_t)y);
				c = clock() - c;
				printf("green : %f\n", (double) c/CLOCKS_PER_SEC);
//				stbi_write_jpg("img/green.jpg", x, y, n, output, 100);
				
				free(output);

				c = clock();
				output = blue(&env, input, (size_t)x, (size_t)y);
				c = clock() - c;
				printf("blue : %f\n", (double) c/CLOCKS_PER_SEC);
//				stbi_write_jpg("img/blue.jpg", x, y, n, output, 100);

				free(output);

				c = clock();
				output = grey(&env, input, (size_t)x, (size_t)y);
				c = clock() - c;
				printf("grey : %f\n", (double) c/CLOCKS_PER_SEC);
//				stbi_write_jpg("img/grey.jpg", x, y, n, output, 100);
				
				free(output);

				c = clock();
				output = sobel3x3(&env, input, (size_t)x, (size_t)y);
				c = clock() - c;
				printf("sobel : %f\n", (double) c/CLOCKS_PER_SEC);
//				stbi_write_jpg("img/sobel.jpg", x, y, n, output, 100);
				
				img_t input1 = output;

				c = clock();
				output = add(&env, input, input1, (size_t)x, (size_t)y);
				c = clock() - c;
				printf("add : %f\n", (double) c/CLOCKS_PER_SEC);
//				stbi_write_jpg("img/add.jpg", x, y, n, output, 100);
				
				free(output);

				c = clock();
				output = sub(&env, input, input1, (size_t)x, (size_t)y);
				c = clock() - c;
				printf("sub : %f\n", (double) c/CLOCKS_PER_SEC);
//				stbi_write_jpg("img/sub.jpg", x, y, n, output, 100);
				
				free(output);

				c = clock();
				output = and(&env, input, input1, (size_t)x, (size_t)y);
				c = clock() - c;
				printf("and : %f\n", (double) c/CLOCKS_PER_SEC);
//				stbi_write_jpg("img/and.jpg", x, y, n, output, 100);
				
				free(output);

				c = clock();
				output = or(&env, input, input1, (size_t)x, (size_t)y);
				c = clock() - c;
				printf("or : %f\n", (double) c/CLOCKS_PER_SEC);
//				stbi_write_jpg("img/or.jpg", x, y, n, output, 100);
				
				free(output);

				c = clock();
				output = xor(&env, input, input1, (size_t)x, (size_t)y);
				c = clock() - c;
				printf("xor : %f\n", (double) c/CLOCKS_PER_SEC);
//				stbi_write_jpg("img/xor.jpg", x, y, n, output, 100);
				
				free(output);

				c = clock();
				output = scale(&env, input, (size_t)x, (size_t)y, 0.5, 1.5);
				c = clock() - c;
				printf("scale : %f\n", (double) c/CLOCKS_PER_SEC);
				stbi_write_jpg("img/scale.jpg", (int)(x/2), (int)(y*1.5), n, output, 100);

				float* hist = histogram(&env, input, (size_t)x, (size_t)y);

				int i;
				float sum=0;
				for(i=0; i<4096; i++)
				{
//								printf("%f\n", hist[i]);
								sum += hist[i];
				}
				printf("somme hist = %f\n", sum);
				
				free(output);

				free(input);

				free(input1);
				return oclRelease(&env);
}
