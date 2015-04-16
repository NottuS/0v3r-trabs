#include"matrix.h"
#include <stdio.h>

//Print matrix A(nr_rows_A, nr_cols_A) storage in column-major format
void print_matrix(const float *A, int nr_rows_A, int nr_cols_A) {

    for(int i = 0; i < nr_rows_A; ++i){
        for(int j = 0; j < nr_cols_A; ++j){
            std::cout << A[j * nr_rows_A + i] << " ";
        }
        std::cout << std::endl;
    }
    std::cout << std::endl;
}

__global__ void kernelCreateIdentity(float *I, int nr_rows_I){
	int id = blockIdx.x * blockDim.x + threadIdx.x;

	if (id < nr_rows_I) {
		I[id * nr_rows_I + id] = 1;
	}
}

void createIdentity(float *I, int nr_rows_I){
	dim3 dimBlock(BLOCK_START_SIZE);
	dim3 dimGrid(ceil(float(nr_rows_I)/dimBlock.x));

	kernelCreateIdentity<<<dimGrid,dimBlock>>>(I, nr_rows_I);
}

void sMatMul(float *C, const float *A, const float *B,
		unsigned int hA, unsigned int wA, unsigned int wB)
{
	for (unsigned int i = 0; i < hA; ++i)
		for (unsigned int j = 0; j < wB; ++j) {
			double sum = 0;
			for (unsigned int k = 0; k < wA; ++k) {
				double a = A[i * wA + k];
				double b = B[k * wB + j];
				sum += a * b;
			}
			C[i * wB + j] = (float)sum;
		}
}

__global__ void kernelMatMul(float *C, const float *A, const float *B, unsigned int nr_rows_A, unsigned int nr_cols_A, unsigned int nr_cols_B){
	int row = blockIdx.y * blockDim.y + threadIdx.y;
	int col = blockIdx.x * blockDim.x + threadIdx.x;


	__shared__ float tempA[BLOCK_START_SIZE];
	__shared__ float tempB[BLOCK_START_SIZE];

	float Cvalue = 0;
	int stride = 1;

	if (row < nr_rows_A && col < nr_cols_B) {
		//TODO Check if all ther work is done, and the indexs
		for (int i = 0; i < nr_rows_A/(BLOCK_START_SIZE); ++i)
		{
			tempA[threadIdx.y*BLOCK_SIZE + threadIdx.x] = A[row * nr_col_A + i * BLOCK_START_SIZE + threadIdx.x];
			tempB[threadIdx.y*BLOCK_SIZE + threadIdx.x] = B[row * nr_rows_A + i * BLOCK_START_SIZE + threadIdx.x];

			__syncthreads();
			for (int j = 0; j < BLOCK_START_SIZE; ++j)
				Cvalue += tempB[j] * tempA[j];
			stride++;
			__syncthreads();
		}
		C[row * nr_cols_A + col] = Cvalue;
		
	}
}

void pMatMul(float *C, const float *A, const float *B, unsigned int hA, unsigned int wA, unsigned int wB){
	dim3 dimBlock(BLOCK_SIZE, BLOCK_SIZE);
	dim3 dimGrid(ceil(float(nr_rows_A) / dimBlock.x), ceil(float(nr_cols_B) / dimBlock.y));

	//cudaSetDeviceFlags(cudaDeviceLmemResizeToMax);
	kernelMatMul<<<dimGrid, dimBlock>>>(C,A,B,nr_rows_A, nr_cols_A);
}

void cublasMatMul(cublasHandle_t &handle, float *C,
		const float *A, const float *B, unsigned int m, unsigned int k, unsigned int n){
	const float alf = 1;
	const float bet = 0;
	const float *alpha = &alf;
	const float *beta = &bet;

	// Do the actual multiplication
	// matrix - matrix multiplication : C = alf*A*B + bet*C
	// A -mxk matrix , B -kxn matrix , C -mxn matrix ;
	CUBLAS_CHECK_RETURN(cublasSgemm(handle, CUBLAS_OP_N, CUBLAS_OP_N,
			m, n, k, alpha, A, m, B, k, beta, C, m));
}

void sMatSum(float *C, const float *A, const float *B, int nr_rows_A, int nr_cols_A){
	for(int i = 0; i < nr_rows_A; i++){
		for (int j = 0; j < nr_cols_A; ++j) {
			C[i*nr_cols_A + j] = A[i*nr_cols_A + j] + B[i*nr_cols_A + j];
		}
	}
}

__global__ void kernelMatSum(float *C, const float *A, const float *B, int nr_rows_A, int nr_cols_A){
	int row = blockIdx.y * blockDim.y + threadIdx.y;
	int col = blockIdx.x * blockDim.x + threadIdx.x;

	if (row < nr_rows_A && col < nr_cols_A) {
		C[row*nr_cols_A + col] = A[row*nr_cols_A + col] + B[row*nr_cols_A + col];
	}
}

void pMatSum(float *C, const float *A, const float *B, int nr_rows_A, int nr_cols_A){
	dim3 dimBlock(BLOCK_SIZE, BLOCK_SIZE);
	dim3 dimGrid(ceil(float(nr_rows_A) / dimBlock.x), ceil(float(nr_cols_A) / dimBlock.y));

	//cudaSetDeviceFlags(cudaDeviceLmemResizeToMax);
	kernelMatSum<<<dimGrid, dimBlock>>>(C,A,B,nr_rows_A, nr_cols_A);
	//cudaDeviceReset();
}

void cublasMatSum(cublasHandle_t &handle, float *C, const float *A, const float *B, int nr_rows_A, int nr_cols_A){
	const float alf = 1;
	const float bet = 1;
	const float *alpha = &alf;
	const float *beta = &bet;

	CUBLAS_CHECK_RETURN(cublasSgeam(handle, CUBLAS_OP_N, CUBLAS_OP_N, nr_rows_A, nr_cols_A, alpha,
			A, nr_rows_A, beta, B, nr_rows_A, C, nr_rows_A));
}

void sMatSub(float *C, const float *A, const float *B, int nr_rows_A, int nr_cols_A){
	for(int i = 0; i < nr_rows_A; i++){
		for (int j = 0; j < nr_cols_A; ++j) {
			C[i*nr_cols_A + j] = A[i*nr_cols_A + j] - B[i*nr_cols_A + j];
		}
	}
}

__global__ void kernelMatSub(float *C, const float *A, const float *B, int nr_rows_A, int nr_cols_A){
	int row = blockIdx.y * blockDim.y + threadIdx.y;
	int col = blockIdx.x * blockDim.x + threadIdx.x;

	if (row < nr_rows_A && col < nr_cols_A) {
		C[row*nr_cols_A + col] = A[row*nr_cols_A + col] - B[row*nr_cols_A + col];
	}
}

void pMatSub(float *C, const float *A, const float *B, int nr_rows_A, int nr_cols_A){
	dim3 dimBlock(BLOCK_SIZE, BLOCK_SIZE);
	dim3 dimGrid(ceil(float(nr_rows_A) / dimBlock.x), ceil(float(nr_cols_A) / dimBlock.y));


	//cudaSetDeviceFlags(cudaDeviceLmemResizeToMax);
	kernelMatSub<<<dimGrid, dimBlock>>>(C,A,B,nr_rows_A, nr_cols_A);
	//cudaDeviceReset();
}

void cublasMatSub(cublasHandle_t &handle, float *C, const float *A, const float *B, int nr_rows_A, int nr_cols_A){
	const float alf = 1;
	const float bet = -1;
	const float *alpha = &alf;
	const float *beta = &bet;

	CUBLAS_CHECK_RETURN(cublasSgeam(handle, CUBLAS_OP_N, CUBLAS_OP_N, nr_rows_A, nr_cols_A, alpha,
			A, nr_rows_A, beta, B, nr_rows_A, C, nr_rows_A));
}

void sMatTranspose(float *C, const float *A, int nr_rows_A, int nr_cols_A){
	for (int i = 0; i < nr_cols_A; ++i) {
		for (int j = 0; j < nr_rows_A; ++j) {
			C[i * nr_cols_A + j] = A[j * nr_rows_A + i];
		}
	}
}

__global__ void kernelTranspose(float *C, const float *A, int nr_rows_A, int nr_cols_A){
	int row = blockIdx.y * blockDim.y + threadIdx.y;
	int col = blockIdx.x * blockDim.x + threadIdx.x;

	__shared__ float temp[BLOCK_SIZE][BLOCK_SIZE+1];

	if (row < nr_rows_A && col < nr_cols_A) {
		int id_in = row * nr_cols_A + col;
		temp[threadIdx.y][threadIdx.x] = A[id_in];
	}

	__syncthreads();

	col = blockIdx.y * blockDim.y + threadIdx.y;
	row = blockIdx.x * blockDim.x + threadIdx.x;

	if (row < nr_cols_A && col < nr_rows_A) {
		int id_out = col * nr_rows_A + row;
		C[id_out] = temp[threadIdx.x][threadIdx.y];
	}
}

void pMatTranspose(float *C, const float *A, int nr_rows_A, int nr_cols_A){
	dim3 dimBlock(BLOCK_SIZE, BLOCK_SIZE);
	dim3 dimGrid(ceil(float(nr_rows_A) / dimBlock.x), ceil(float(nr_cols_A) / dimBlock.y));

	kernelTranspose<<<dimGrid, dimBlock>>>(C, A, nr_rows_A, nr_cols_A);
}

void cublasMatTranspose(cublasHandle_t &handle, float *C, const float *A, const float *B, int nr_rows_A, int nr_cols_A){
	const float alf = 1;
	const float bet = 0;
	const float *alpha = &alf;
	const float *beta = &bet;

	CUBLAS_CHECK_RETURN(cublasSgeam(handle, CUBLAS_OP_T, CUBLAS_OP_N, nr_rows_A, nr_cols_A, alpha,
			A, nr_rows_A, beta, B, nr_rows_A, C, nr_rows_A));
}

void sInvMat(const float *A, int nr_rows_A, int nr_cols_A){

}
void pInvMat(const float *A, int nr_rows_A, int nr_cols_A){

}

void choleskyDecomp(const float *A, float *L, int nr_rows_A, int nr_cols_A){
	int i,j,k;
	float sum;
	for(i = 0; i < nr_rows_A; i++) {
		for (j = 0; j < i; ++j) {
			if(i == j) {
				for(j = 1; i - 2; j++){
						sum += L[i*nr_cols_A + j] * L[i*nr_cols_A + j];
				}
				L[i*nr_cols_A + i] = sqrt(A[i*nr_cols_A + i] - sum);
				sum = 0;
			} else {
				for(j = 0; k - 2; j++){
					sum += L[k*nr_cols_A +j] * L[i*nr_cols_A + j];
				}
				L[i*nr_cols_A + k] = (A[i*nr_cols_A + k] - sum) / L[k*nr_cols_A + k];
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
	thrust::fill(I.begin(), I.end(), 0);
	cudaDeviceSynchronize();
	createIdentity(thrust::raw_pointer_cast(&I[0]), nr_cols_A);
	cudaDeviceSynchronize();

	//get L from A = LL^T
	choleskyDecomp(A, thrust::raw_pointer_cast(&L[0]), nr_rows_A, nr_cols_A);
	thrust::device_vector<float> d_L(L.begin(), L.begin() + L.size());

	//get Y from LY = I
	cublasStrsm_v2(handle,CUBLAS_SIDE_RIGHT, CUBLAS_FILL_MODE_LOWER,
			CUBLAS_OP_N,CUBLAS_DIAG_NON_UNIT, nr_rows_A, nr_cols_A, alpha, A,
			nr_rows_A, thrust::raw_pointer_cast(&I[0]), nr_rows_A);
	cudaDeviceSynchronize();

	//get X from (L^T)X = Y
	cublasStrsm_v2(handle, CUBLAS_SIDE_RIGHT, CUBLAS_FILL_MODE_UPPER,
				CUBLAS_OP_T,CUBLAS_DIAG_NON_UNIT, nr_rows_A, nr_cols_A,	alpha,
				thrust::raw_pointer_cast(&L[0]), nr_rows_A, thrust::raw_pointer_cast(&I[0]), nr_rows_A);
	cudaDeviceSynchronize();
	d_L.shrink_to_fit();
	L.shrink_to_fit();
	result = thrust::raw_pointer_cast(&I[0]);
}


