#include <stdio.h>
#include <stdlib.h>

#include <thrust/host_vector.h>
#include <thrust/device_vector.h>
#include "matrix.h"
#include <math.h>
#include "Inverse.h"

void fowardSubst(float *res, float *L, float *I, int n){

	for(int i = 0; i < n; i++){
		for(int j = 0; j < n; j++){
			float sum = I[j*n+i];
			for(int k = 0; k < j; k++){
				sum -= L[j*n + k]*res[k*n+i];
			}
			res[j*n + i] = sum/L[j*n+j];
		}
	}
}

__global__ void fowardSubstKernel(float *res, float *L, float *I, int n){
	int col = blockIdx.x * blockDim.x + threadIdx.x;

	if (col < n) {
		for(int j = 0; j < n; j++){
			float sum = I[j*n+col];
			for(int k = 0; k < j; k++){
				sum -= L[j*n + k]*res[k*n+col];
			}
			res[j*n + col] = sum/L[j*n+j];
		}
	}
}

void backSubst(float *res, float *L, float *I, int n){
	for(int i = 0; i < n; i++){
		for(int j = n-1; j >= 0; j--){
			float sum = I[j*n+i];
			for(int k = n - 1; k > j; k--){
				sum -= L[j*n + k]*res[k*n+i];
			}
			res[j*n + i] = sum/L[j*n+j];
		}
	}
}

__global__ void  backSubstkernel(float *res, float *L, float *I, int n){
	int col = blockIdx.x * blockDim.x + threadIdx.x;

	if (col < n) {
		for(int j = n-1; j >= 0; j--){
			float sum = I[j*n+col];
			for(int k = n - 1; k > j; k--){
				sum -= L[j*n + k]*res[k*n+col];
			}
			res[j*n + col] = sum/L[j*n+j];
		}
	}
}

void choleskyDecomp(float *L, const float *A, int n){
	int i,j,k;
	float sum;
	for(i = 0; i < n; i++) {
		for (j = 0; j <= i; ++j) {
			sum = A[i*n + j];
			for(k = 0; k < j; k++) {
				sum -= L[i*n +k] * L[j*n + k];
			}
			if(i == j) {
				L[i*n + j] = sqrt(sum);
			} else {
				L[i*n + j] = sum / L[j*n + j];
			}
		}
	}
}

__global__ void choleskyDecompKernel2(int ind, const float *A, float *L, int n){
	int row = blockIdx.y * blockDim.y + threadIdx.y;
	int col = blockIdx.x * blockDim.x + threadIdx.x;

	__shared__ float tempA[BLOCK_SIZE][BLOCK_SIZE];
	__shared__ float tempB[BLOCK_SIZE][BLOCK_SIZE];

	if (row < n && col < BLOCK_SIZE) {
		//TODO Check if all their work is done, and the indexs
		float sum = A[row * n + ind];
			for(int i = col; i < ind; i += BLOCK_SIZE){
				sum -= L[ind * n + i] * L[row * n + i];
			}



		/*for (int i = 0; i < ceil(float(ind)/(BLOCK_SIZE)); ++i) {

			/*tempA[threadIdx.y*BLOCK_SIZE + threadIdx.x] = A[row * nr_col_A + i * BLOCK_START_SIZE + threadIdx.x];
			tempB[threadIdx.y*BLOCK_SIZE + threadIdx.x] = B[(i * BLOCK_START_SIZE + threadIdx.x)* nr_cols_B *  + row];
			tempA[threadIdx.y][threadIdx.x] = A[ind * nr_cols_A + BLOCK_SIZE * i + threadIdx.x];
			tempB[threadIdx.y][threadIdx.x] = A[row * nr_cols_A + BLOCK_SIZE * i + threadIdx.x];
			__syncthreads();
			if(ind > (i + 1) * BLOCK_SIZE){
				for (int j = 0; j < BLOCK_SIZE ; ++j)
					sum -= tempA[threadIdx.y][j] * tempB[threadIdx.y][j];
			} else {
				for (int j = 0; j < ind % BLOCK_SIZE ; ++j)
					sum -= tempA[threadIdx.y][j] * tempB[threadIdx.y][j];
			}
			__syncthreads();
		}*/
		L[row * n + col] = sum;
		__syncthreads();
		if( row == ind ){
			for(int i = 0; i < BLOCK_SIZE; i++){
				sum -= L[row * n + i];
			}
			sum = sqrtf(sum);
		}
	}
}

__global__ void choleskyDecompKernel(int ind, const float *A, float *L, int n){
	/*int row = blockIdx.y * blockDim.y + threadIdx.y;
	int col = blockIdx.x * blockDim.x + threadIdx.x;*/
	int row = blockIdx.x * blockDim.x + threadIdx.x;

	__shared__ float temp1[BLOCK_START_SIZE];
	//__shared__ float temp2[BLOCK_START_SIZE];

	if (row < n) {
		int x = threadIdx.x;
		float sum = A[row * n + ind];
		for (int k = 0; k < ceilf((ind + 0.0)/BLOCK_START_SIZE); ++k) {
			temp1[x] = L[ind * n+ BLOCK_START_SIZE * k + x];
			//temp2[x] = L[row * nr_cols_A + BLOCK_START_SIZE * k + 0];

			__syncthreads( );
			if((k+1)*BLOCK_START_SIZE <= ind) {
				#pragma unroll
				for(int i = 0; i < BLOCK_START_SIZE; i += 8){
					float a,b,c,d, a1, b1, c1, d1;
					a = L[row * n + k * BLOCK_START_SIZE + i];
					b = L[row * n + k * BLOCK_START_SIZE + i + 1];
					c = L[row * n + k * BLOCK_START_SIZE + i + 2];
					d = L[row * n + k * BLOCK_START_SIZE + i + 3];
					a1 = L[row * n + k * BLOCK_START_SIZE + i + 4];
					b1 = L[row * n + k * BLOCK_START_SIZE + i + 5];
					c1 = L[row * n + k * BLOCK_START_SIZE + i + 6];
					d1 = L[row * n + k * BLOCK_START_SIZE + i + 7];
					//sum -= L[row * nr_cols_A + k * BLOCK_START_SIZE + i] * temp1[i];
					//sum -= temp2[i] * temp1[i];
					sum -= a * temp1[i] + b * temp1[i+1] + c * temp1[i+2] + d * temp1[i+3] +
							a1 * temp1[i+4] + b1 * temp1[i+5] + c1 * temp1[i+6] + d1 * temp1[i+7];
				}
			} else {
				for(int i = 0; i < ind % BLOCK_START_SIZE; i += 8){
					float a,b,c,d, a1, b1, c1, d1;
					a = L[row * n + k * BLOCK_START_SIZE + i];
					b = L[row * n + k * BLOCK_START_SIZE + i + 1];
					c = L[row * n + k * BLOCK_START_SIZE + i + 2];
					d = L[row * n + k * BLOCK_START_SIZE + i + 3];
					a1 = L[row * n + k * BLOCK_START_SIZE + i + 4];
					b1 = L[row * n + k * BLOCK_START_SIZE + i + 5];
					c1 = L[row * n + k * BLOCK_START_SIZE + i + 6];
					d1 = L[row * n + k * BLOCK_START_SIZE + i + 7];
					//sum -= L[row * nr_cols_A + k * BLOCK_START_SIZE + i] * temp1[i];
					//sum -= temp2[i] * temp1[i];
					sum -= a * temp1[i] + b * temp1[i+1] + c * temp1[i+2] + d * temp1[i+3] +
							a1 * temp1[i+4] + b1 * temp1[i+5] + c1 * temp1[i+6] + d1 * temp1[i+7];
				}
			}
			__syncthreads();
		}

		if( row == ind )
			sum = sqrtf(sum);
		L[row * n + ind] = sum;
	}
}

__global__ void oldCD(int ind, const float *A, float *L, int n){
	int row = blockIdx.x * blockDim.x + threadIdx.x;

	__shared__ float temp1[BLOCK_START_SIZE];
	//__shared__ float temp2[BLOCK_START_SIZE];

	if (row < n) {
		int x = threadIdx.x;
		float sum = A[row * n + ind];
		for (int k = 0; k < ceilf((ind + 0.0)/BLOCK_START_SIZE); ++k) {
			temp1[x] = L[ind * n + BLOCK_START_SIZE * k + x];
						//temp2[x] = L[row * nr_cols_A + BLOCK_START_SIZE * k + 0];

			__syncthreads( );
			if((k+1)*BLOCK_START_SIZE <= ind) {
				#pragma unroll
				for(int i = 0; i < BLOCK_START_SIZE; i++){
					sum -= L[row * n+ k * BLOCK_START_SIZE + i] * temp1[i];
				}
			} else {
				for(int i = 0; i < ind % BLOCK_START_SIZE; i++){
					sum -= L[row * n+ k * BLOCK_START_SIZE + i] * temp1[i];
				}
			}
			__syncthreads();
		}
		if( row == ind )
			sum = sqrtf(sum);
		L[row * n + ind] = sum;
	}
}

__global__ void updateCholesky(int ind, float *L, int n){
	int row = blockIdx.x * blockDim.x + threadIdx.x;

	if (row < n && row != ind ) {
		if( row < ind )
			L[row * n + ind] = 0;
		else
			L[row * n+ ind] /=  L[ind * n + ind];
	}
}

//TODO
void pMatInverse(float *A, float *L, float *I, int n){
	dim3 dimBlock(BLOCK_START_SIZE);
//	dim3 dimBlock2(BLOCK_SIZE, BLOCK_SIZE);
	dim3 dimGrid(ceil(float(n) / dimBlock.x));
	//dim3 dimGrid2(ceil(float(n) / dimBlock.y), BLOCK_SIZE);

	for(int i = 0; i < n; i++){
		choleskyDecompKernel<<<dimGrid, dimBlock>>>(i, A, L, n);
		CUDA_CHECK_RETURN(cudaDeviceSynchronize());
		updateCholesky<<<dimGrid, dimBlock>>>(i, L, n);
		CUDA_CHECK_RETURN(cudaDeviceSynchronize());
	}

	fowardSubstKernel<<<dimGrid, dimBlock>>>(A, L, I, n);
	CUDA_CHECK_RETURN(cudaDeviceSynchronize());
	pMatTranspose(I, L, n, n);
	CUDA_CHECK_RETURN(cudaDeviceSynchronize());
	backSubstkernel<<<dimGrid, dimBlock>>>(L, I, A, n);
	CUDA_CHECK_RETURN(cudaDeviceSynchronize());

}

/*void choleskyDecompositionGPU( int n, float *cpu_A, float * d_A, float *d_L){
	for (int i = 0; i < n; i += BLOCk) {
		int h = n - i;
		int w = h < BLOCk ? h : BLOCk
		if(i > 0){
			srku();
			mul();
			if(h > BLOCk){
				srku();
			}
		}

		choleskyDecomp();
		if(h > BLOCk){
			solve();
		}
	}
}*/



void testInvert( int n) {
	thrust::device_vector<float> d_A(n * n);
	thrust::device_vector<float> d_L(n * n);
	thrust::device_vector<float> d_I(n * n);
	thrust::host_vector<float>h_A(n * n);
	thrust::host_vector<float>h_L(n * n);
	thrust::host_vector<float>h_I(n*n);
	thrust::host_vector<float>b_A(n*n);

	float *A = thrust::raw_pointer_cast(&h_A[0]);
	float *I = thrust::raw_pointer_cast(&h_I[0]);
	float *L = thrust::raw_pointer_cast(&h_L[0]);

	float *dA = thrust::raw_pointer_cast(&d_A[0]);
	float *dI = thrust::raw_pointer_cast(&d_I[0]);
	float *dL = thrust::raw_pointer_cast(&d_L[0]);

	srand(n);
	for( int i = 0; i < n; i++ ) {
		for (int j = i; j < n; j++) {
			A[i*n+j] = 2.0*(rand()%32768)/32768.0;
			A[j*n+i] = A[i*n+j];
		}
		A[i*n+i] += 2*sqrt((float)n);
	}

	thrust::fill(h_I.begin(), h_I.end(), 0);
	for(int i = 0; i < n; i++)
		I[i *n +i] = 1;
	thrust::copy(h_A.begin(), h_A.end(), b_A.begin());
	thrust::copy(h_A.begin(), h_A.end(), d_A.begin());
	thrust::copy(h_I.begin(), h_I.end(), d_I.begin());

	/*clock_t start = clock();
	choleskyDecomp(L, A, n);
	//print_matrix(L, n, n);
	fowardSubst(A, L, I, n);
	sMatTranspose(I, L, n, n);
	backSubst(L, I, A,  n);
	clock_t end = clock();
	printf("seq took: %f seconds \n", float(end - start) / CLOCKS_PER_SEC);*/
	//print_matrix(L, n, n);

	//print_matrix(L, n, n);


	thrust::copy(b_A.begin(), b_A.end(), h_A.begin());
	clock_t start = clock();
	sMatInverse(A, n,n , L);
	clock_t end = clock();
	//print_matrix(A, n, n);
	printf("seq2 took: %f seconds \n", float(end - start) / CLOCKS_PER_SEC);

	start = clock();
	pMatInverse(dA, dL, dI, n);
	end = clock();
	printf("par took: %f seconds \n", float(end - start) / CLOCKS_PER_SEC);
	thrust::copy(d_L.begin(), d_L.end(), h_A.begin());

}
