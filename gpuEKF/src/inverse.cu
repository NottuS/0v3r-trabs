#include "matrix.h"

__global__ void kernelCreateIdentity(float *I, int nr_rows_I){
	int id = blockIdx.x * blockDim.x + threadIdx.x;

	if (id < nr_cols_A) {
		I[id * nr_rows_I + id] = 1;
	}
}

void createIdentity(float *I, int nr_rows_I){
	dim3 dimBlock(BLOCK_START_SIZE);
	dim3 dimGrid(ceil(nr_rows_A/dimBlock));

	kernelCreateIdentity<<<dimGrid,dimBlock>>>(I, nr_rows_I);
}

void choleskyDecomp(float *A, float *L, int nr_rows_A, int nr_cols_A){
	int i,j,k;
	for(i = 0; i < nr_rows_A; i++) {
		for (j = 0; j < i; ++j) {
			if(i == j) {
				for(j = 1; i - 2; j++){
						sum += L[i][j] << 1;
				}
				L[i][i] = sqrt(A[i][i] - sum);
				sum = 0;
			} else {
				for(j = 0; k - 2; j++){
					sum += L[k][j] * L[i][j];
				}

				L[i][k] = (A[i][k] - sum) / L[k][k];
				sum = 0;
			}
		}
	}
}

void cublasMatInverse(cublasHandle_t &handle, const float *A, int nr_rows_A, int nr_cols_A, float *result){

	const float alf = 1;
	const float *alpha = &alf;

	//create the identity matrix
	thrust::device_vector<float> I(nr_rows_A * nr_cols_A);
	thrust::host_vector<float> L(nr_rows_A * nr_cols_A);
	thust::fill(I.begin(), I.size(), 0);
	cudaDeviceSynchronize();
	createIdentity(thrust::raw_pointer_cast(&I[0]), nr_cols_A);
	cudaDeviceSynchronize();

	//get L from A = LL^T
	choleskyDecomp(A,L, nr_rows_A, nr_cols_A);
	thrust::device_vector<float> d_L(L.begin, L.begin() + L.size());

	//get Y from LY = I
	cublasStrsm_v2(handle,CUBLAS_SIDE_RIGHT, CUBLAS_FILL_MODE_LOWER,
			CUBLAS_OP_N,CUBLAS_DIAG_NON_UNIT, nr_rows_A, nr_cols_A, alf, A, nr_rows_A, I, nr_rows_A);
	cudaDeviceSynchronize();

	//get X from (L^T)X = Y
	//TODO

	d_L.shrink_to_fit();
	L.shrink_to_fit();
	result = thrust::raw_pointer_cast(&I[0]);
}
