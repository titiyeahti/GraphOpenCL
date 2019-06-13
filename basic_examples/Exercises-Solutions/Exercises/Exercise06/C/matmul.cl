__kernel void mmul(const int N,
							__global float *A,
							__global float *B,
							__global float *C)
{
		int i, j, k;
		i = get_global_id(0);
		j = get_global_id(1);
		float temp;
		
		temp = 0.0f;
		for(k=0; k<N; k++)
				temp += A[i*N+k]*B[k*N+j];	
		
		C[N*i+j] = temp;
}
