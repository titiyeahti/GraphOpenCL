/*
 * =====================================================================================
 *
 *       Filename:  libker.c
 *
 *    Description:  OpenCL code for the climagelib 
 *
 *        Version:  1.0
 *        Created:  11/07/2019 11:00:20
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Thibaut MILHAUD (), thibaut.milhaud@ensiie.fr
 *   Organization:  ENSIIE - SUAI
 *
 * =====================================================================================
 */

#define	CONST_SIZE 3
#define	CONST_SIZE2 CONST_SIZE*CONST_SIZE

				__kernel
void blur33(__read_only image2d_t src,
								__write_only image2d_t dest,
								sampler_t sampler)
{

				int id0 = get_global_id(0);
				int id1 = get_global_id(1);
				int i, j;

				float4 temp;
				temp.x = 0;
				temp.y = 0;
				temp.z = 0;
				temp.w = 0;

				for(i = -(CONST_SIZE-2); i < (CONST_SIZE-1); i++)
				{
								for(j = -(CONST_SIZE-2); j < (CONST_SIZE-1); j++)
								{
												temp += read_imagef(src, sampler, (int2)(i, j));
								}
				}

				temp = temp/9.0f;

				write_imagef(dest, (int2) (id0, id1), temp);
};

				__kernel
void gaussian33(__read_only image2d_t src,
								__write_only image2d_t dest,
								sampler_t sampler)
{

				int id0 = get_global_id(0);
				int id1 = get_global_id(1);
				int i, j;
				float sum, val;

				sum = 0;
				float4 temp;
				temp.x = 0;
				temp.y = 0;
				temp.z = 0;
				temp.w = 0;

				for(i = -(CONST_SIZE-2); i < (CONST_SIZE-1); i++)
				{
								for(j = -(CONST_SIZE-2); j < (CONST_SIZE-1); j++)
								{
												val = exp2(-(float)(i*i+j*j)/2);
												sum += val;
												temp += read_imagef(src, sampler, (int2)(id0+i, id1+j))*val;
								}
				}

				temp = temp/sum;
				write_imagef(dest, (int2) (id0, id1), temp);
};

// TODO improve the pixels' comparison
				__kernel
void median33(__read_only image2d_t src,
								__write_only image2d_t dest,
								sampler_t sampler)
{
				int id0 = get_global_id(0);
				int id1 = get_global_id(1);
				int i, j, k;

				k = 0;
				float4 temp;
				float4 array[9];

				for(i = -(CONST_SIZE-2); i < (CONST_SIZE-1); i++)
				{
								for(j = -(CONST_SIZE-2); j < (CONST_SIZE-1); j++)
								{
												array[k] = read_imagef(src, sampler, (int2)(i, j));
												k++;
								}
				}

				for (i=1; i<CONST_SIZE2; i++)
				{
								for (j=0; j<CONST_SIZE2-i; j++)
								{
												if ((array[j].x > array[j+1].x)
																				|| (fabs(array[j].x - array[j+1].x) <= 0.01  && 
																								(array[j].y > array[j+1].y))
																				|| (fabs(array[j].x - array[j+1].x) <= 0.01 && 
																								fabs(array[j].y - array[j+1].y) <= 0.01 && 
																								array[j].z > array[j+1].z))
												{
																temp = array[j];
																array[j] = array[j+1];
																array[j+1] = temp;
												}
								}
				}


				write_imagef(dest, (int2) (id0, id1), array[12]);
};

				__kernel 
void red(__read_only image2d_t src,
								__write_only image2d_t dest,
								sampler_t sampler)
{
				int id0 = get_global_id(0);
				int id1 = get_global_id(1);

				float4 temp = read_imagef(src, sampler, (int2)(id0, id1));
				temp.y = 0;
				temp.z = 0;
				write_imagef(dest, (int2) (id0, id1), temp);
}

				__kernel 
void green(__read_only image2d_t src,
								__write_only image2d_t dest,
								sampler_t sampler)
{
				int id0 = get_global_id(0);
				int id1 = get_global_id(1);

				float4 temp = read_imagef(src, sampler, (int2)(id0, id1));
				temp.x = 0;
				temp.z = 0;
				write_imagef(dest, (int2) (id0, id1), temp);
}

				__kernel 
void blue(__read_only image2d_t src,
								__write_only image2d_t dest,
								sampler_t sampler)
{
				int id0 = get_global_id(0);
				int id1 = get_global_id(1);

				float4 temp = read_imagef(src, sampler, (int2)(id0, id1));
				temp.y = 0;
				temp.x = 0;
				write_imagef(dest, (int2) (id0, id1), temp);
}

				__kernel 
void grey(__read_only image2d_t src,
								__write_only image2d_t dest,
								sampler_t sampler)
{
				int id0 = get_global_id(0);
				int id1 = get_global_id(1);
				float mean;
				float4 temp = read_imagef(src, sampler, (int2)(id0, id1));

				mean = temp.x;
				mean += temp.y;
				mean += temp.z;
				mean = mean / 3;

				temp.x = mean;
				temp.y = mean;
				temp.z = mean;

				write_imagef(dest, (int2) (id0, id1), temp);
}

				__kernel
void sobel(__read_only image2d_t src,
								__write_only image2d_t dest,
								sampler_t sampler)
{

				float vert_filter[9] = {1.0f, 2.0f, 1.0f,
								0.0f, 0.0f, 0.0f,
								-1.0f, -2.0f, -1.0f};

				float horiz_filter[9] = {1.0f, 0.0f, -1.0f,
								2.0f, 0.0f, -2.0f,
								1.0f, 0.0f, -1.0f};

				int id0 = get_global_id(0);
				int id1 = get_global_id(1);
				int i, j, k;

				float4 vert = (float4) (0.0f, 0.0f, 0.0f, 0.0f);
				float4 horiz = (float4) (0.0f, 0.0f, 0.0f, 0.0f);
				float4 temp;
				k = 0;

				for(i = id0-1; i < id0 + 2; i++)
				{
								for(j = id1-1; j < id1 + 2; j++)
								{
												temp = read_imagef(src, sampler, (int2)(i, j));

												vert += vert_filter[k] * temp;
												horiz += horiz_filter[k] * temp;

												k++;
								}
				}

				temp = sqrt((horiz*horiz + vert*vert)/16.0f);
				temp.w = 1;
				write_imagef(dest, (int2) (id0, id1), temp);
}

__kernel
void add(__read_only image2d_t src1,
								__read_only image2d_t src2,
								__write_only image2d_t dest,
								sampler_t sampler)
{
				int id0 = get_global_id(0);
				int id1 = get_global_id(1);

				int4 temp = read_imagei(src1, sampler, (int2)(id0, id1));
				temp += read_imagei(src2, sampler, (int2)(id0, id1));
				temp.w = 1;
				write_imagei(dest, (int2) (id0, id1), temp);
}

__kernel
void sub(__read_only image2d_t src1,
								__read_only image2d_t src2,
								__write_only image2d_t dest,
								sampler_t sampler)
{
				int id0 = get_global_id(0);
				int id1 = get_global_id(1);

				int4 temp = read_imagei(src1, sampler, (int2)(id0, id1));
				temp -= read_imagei(src2, sampler, (int2)(id0, id1));
				temp.w = 1;
				write_imagei(dest, (int2) (id0, id1), temp);
}

__kernel
void and(__read_only image2d_t src1,
								__read_only image2d_t src2,
								__write_only image2d_t dest,
								sampler_t sampler)
{
				int id0 = get_global_id(0);
				int id1 = get_global_id(1);

				int4 temp = read_imagei(src1, sampler, (int2)(id0, id1));
				temp &= read_imagei(src2, sampler, (int2)(id0, id1));
				temp.w = 1;
				write_imagei(dest, (int2) (id0, id1), temp);
}

__kernel
void or(__read_only image2d_t src1,
								__read_only image2d_t src2,
								__write_only image2d_t dest,
								sampler_t sampler)
{
				int id0 = get_global_id(0);
				int id1 = get_global_id(1);

				int4 temp = read_imagei(src1, sampler, (int2)(id0, id1));
				temp |= read_imagei(src2, sampler, (int2)(id0, id1));
				temp.w = 1;
				write_imagei(dest, (int2) (id0, id1), temp);
}

__kernel
void xor(__read_only image2d_t src1,
								__read_only image2d_t src2,
								__write_only image2d_t dest,
								sampler_t sampler)
{
				int id0 = get_global_id(0);
				int id1 = get_global_id(1);

				int4 temp = read_imagei(src1, sampler, (int2)(id0, id1));
				temp ^= read_imagei(src2, sampler, (int2)(id0, id1));
				temp.w = 1;
				write_imagei(dest, (int2) (id0, id1), temp);
}

__kernel
void scale(__read_only image2d_t src,
								__write_only image2d_t dest,
								sampler_t sampler)
{
				int2 id;
				id.x = get_global_id(0);
				id.y = get_global_id(1);


				int2 size;
				size.x = get_global_size(0);
				size.y = get_global_size(1);
				
				float2 coord;
				coord.x = (float) id.x/size.x;
				coord.y = (float) id.y/size.y;

				write_imagef(dest, id, read_imagef(src, sampler, coord)); 
}

__kernel
void hist(__read_only image2d_t src,
								__global int* histo,
								sampler_t sampler)
{
				int size = get_global_size(1);
				int id0 = get_global_id(0);
				int id1 = get_global_id(1);
				int i, j, k;

				int4 color;
				color = read_imagei(src, sampler, (int2)(id0, id1));


				i = color.x / 16;
				j = color.y / 16;
				k = color.z / 16;
				
//				if (id0 == id1)
//				{
//								printf("color : %4d\n(i, j, k) : %3d\n\n", color,
//															(int3)(i, j, k));
//				}
//				
				atomic_add(&histo[256*i+16*j+k],1);
}

__kernel
void convert(__global int* input,
								__global float* output,
								unsigned long int size)
{
				int id = get_global_id(0);

				output[id] = (float)input[id]/(float)size; 
}
