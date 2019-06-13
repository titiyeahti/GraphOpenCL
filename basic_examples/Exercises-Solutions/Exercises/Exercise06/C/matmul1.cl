__kernel void mmul(const int N,
							__global float *A,
							__global float *B,
							__global float *C,
							__local float *col_B)
{
		int i, j, k, iloc, nloc;
		float row_A[1024];
		float temp;

		i = get_global_id(0);
		iloc = get_local_id(0);
		nloc = get_local_size(0);
		
		//printf("%d %d %d\n",i ,nloc, iloc);
		for(k = 0; k < N ;k++)
			  row_A[k] = A[N*i+k];

		
		for(j=0; j<N; j++)
		{
				for(k=iloc; k<N; k+=nloc)
						col_B[k] = B[k*N+j];

				barrier(CLK_LOCAL_MEM_FENCE);
				temp = 0.0f;

				for(k=0; k<N; k++)
						temp += row_A[k]*col_B[k];	
				
				C[N*i+j] = temp;
				barrier(CLK_LOCAL_MEM_FENCE);
		}
}
