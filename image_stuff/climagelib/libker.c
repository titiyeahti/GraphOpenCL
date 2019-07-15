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

				for(i = id0-1; i < id0 + 2; i++)
				{
								for(j = id1-1; j < id1 + 2; j++)
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

				for(i = -1; i < 2; i++)
				{
								for(j = -1; j < 2; j++)
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
				float4 array[25];

				for(i = id0-2; i < id0 + 3; i++)
				{
								for(j = id1-2; j < id1 + 3; j++)
								{
												array[k] = read_imagef(src, sampler, (int2)(i, j));
												k++;
								}
				}

				for (i=1; i<25; i++)
				{
								for (j=0; j<25-i; j++)
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
				
				if (id.x==id.y && id.x == 0)
								printf("size : %2d \n", size);

				float2 coord;
				coord.x = (float) id.x/size.x;
				coord.y = (float) id.y/size.y;

				write_imagef(dest, id, read_imagef(src, sampler, coord)); 
}














