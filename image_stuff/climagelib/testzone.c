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

#include "climagelib.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"


int main(int argc, char** arvg)
{
				ocl_env_t env;
				int x, y, n;
				
				env = oclSetup();

				img_t temp = stbi_load("../medtest2.jpg", &x, &y, &n, 0);

				printf("x = %d, y = %d\n", x, y);
				img_t input = convert_to_32bits(temp, (size_t) x, (size_t) y);
				n++;

				free(temp);

				img_t output = blur3x3(&env, input, (size_t)x, (size_t)y);

				stbi_write_jpg("blur.jpg", x, y, n, output, 100);
				
				free(output);
				
				output = median3x3(&env, input, (size_t)x, (size_t)y);
				stbi_write_jpg("med.jpg", x, y, n, output, 100);
				
				free(output);
				
				output = gaussian3x3(&env, input, (size_t)x, (size_t)y);
				stbi_write_jpg("gauss.jpg", x, y, n, output, 100);
				
				free(output);

				free(input);
				return oclRelease(&env);
}
