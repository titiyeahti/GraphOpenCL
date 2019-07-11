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

				for(i = -2; i < 3; i++)
				{
								for(j = -2; j < 3; j++)
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

