
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


