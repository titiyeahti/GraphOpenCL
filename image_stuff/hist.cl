#define HIST_RANGE 256

__kernel
void hist(__global unsigned char *img,
						__global int *hist,
					const int img_size)
{
	__local int local_hist[HIST_RANGE];
	int i_l = get_local_id(0);
	int i_g = get_global_id(0);
	int n_l = get_local_size(0);
	int n_g = get_global_size(0);

	int i;
	

	for(i=i_l; i<HIST_RANGE; i+=n_l)
			   {
			local_hist[i] = 0;
			}
	
	barrier(CLK_LOCAL_MEM_FENCE);

	for(i=i_g; i<img_size; i+=n_g)
			   {
			atomic_add(&(local_hist[img[i]]), 1);
			}
	
	barrier(CLK_LOCAL_MEM_FENCE);

	for(i=i_l; i<HIST_RANGE; i+=n_l)
			   {
			atomic_add(&(hist[i]), local_hist[i]);
			}
}
