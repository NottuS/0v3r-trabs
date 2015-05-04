#include"matrix.h"
#include <stdio.h>

//using namespace Eigen;

//Print matrix A(nr_rows_A, nr_cols_A) storage in column-major format
void print_matrix(const float *A, int nr_rows_A, int nr_cols_A) {

    for(int i = 0; i < nr_rows_A; ++i){
        for(int j = 0; j < nr_cols_A; ++j){
            std::cout << A[i * nr_rows_A + j] << " ";
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

void sCreateIdentity(float *I, int nr_rows_I){
	for(int i = 0; i < nr_rows_I; i++){
		I[i*nr_rows_I + i] = 1;
	}
}

//TODO allow transpose on multiplication
void sMatMul(int transp_1, int transp_2,float *C, const float *A, const float *B,
		unsigned int hA, unsigned int wA, unsigned int wB)
{
	for (unsigned int i = 0; i < hA; ++i)
		for (unsigned int j = 0; j < wB; ++j) {
			double sum = 0;
			for (unsigned int k = 0; k < wA; ++k) {
				double a;
				double b;
				if(! transp_1)
					a = A[i * wA + k];
				else
					a = A[k * wA + i];
				if(! transp_2)
					b = B[k * wB + j];
				else
					b = B[j * wB + k];
				sum += a * b;
			}
			C[i * wB + j] = (float)sum;
		}
}

__global__ void kernelMatMul(float *C, const float *A,
		const float *B, unsigned int nr_rows_A, unsigned int nr_cols_A, unsigned int nr_cols_B){
	int row = blockIdx.y * blockDim.y + threadIdx.y;
	int col = blockIdx.x * blockDim.x + threadIdx.x;

	__shared__ float tempA[BLOCK_SIZE][BLOCK_SIZE];
	__shared__ float tempB[BLOCK_SIZE][BLOCK_SIZE];

	float Cvalue = 0;

	if (row < nr_rows_A && col < nr_cols_B) {
		//TODO Check if all their work is done, and the indexs
		for (int i = 0; i < ceil(float(nr_rows_A)/(BLOCK_SIZE)); ++i)
		{
			/*tempA[threadIdx.y*BLOCK_SIZE + threadIdx.x] = A[row * nr_col_A + i * BLOCK_START_SIZE + threadIdx.x];
			tempB[threadIdx.y*BLOCK_SIZE + threadIdx.x] = B[(i * BLOCK_START_SIZE + threadIdx.x)* nr_cols_B *  + row];*/
			tempA[threadIdx.y][threadIdx.x] = A[threadIdx.y * nr_cols_A + BLOCK_SIZE * i + threadIdx.x];
			tempB[threadIdx.y][threadIdx.x] = B[(BLOCK_SIZE * i + threadIdx.y) * nr_cols_B + threadIdx.x];
			__syncthreads();

			#pragma unroll
			for (int j = 0; j < BLOCK_SIZE; ++j)
				Cvalue += 1*tempA[threadIdx.y][j] * tempB[j][threadIdx.x] + 0*C[row * nr_cols_A + col] ;

			__syncthreads();
		}
		C[row * nr_cols_A + col] = Cvalue;
		
	}
}

__global__ void transp1kernelMatMul(float *C, const float *A,
		const float *B, unsigned int nr_rows_A, unsigned int nr_cols_A, unsigned int nr_cols_B){
	int row = blockIdx.y * blockDim.y + threadIdx.y;
	int col = blockIdx.x * blockDim.x + threadIdx.x;

	__shared__ float tempA[BLOCK_SIZE][BLOCK_SIZE];
	__shared__ float tempB[BLOCK_SIZE][BLOCK_SIZE];

	float Cvalue = 0;

	if (row < nr_rows_A && col < nr_cols_B) {
		//TODO Check if all their work is done, and the indexs
		for (int i = 0; i < ceil(float(nr_rows_A)/(BLOCK_SIZE)); ++i)
		{
			/*tempA[threadIdx.y*BLOCK_SIZE + threadIdx.x] = A[row * nr_col_A + i * BLOCK_START_SIZE + threadIdx.x];
			tempB[threadIdx.y*BLOCK_SIZE + threadIdx.x] = B[(i * BLOCK_START_SIZE + threadIdx.x)* nr_cols_B *  + row];*/
			tempA[threadIdx.y][threadIdx.x] = A[(BLOCK_SIZE * i + threadIdx.y) * nr_cols_A + threadIdx.x];
			tempB[threadIdx.y][threadIdx.x] = B[(BLOCK_SIZE * i + threadIdx.y) * nr_cols_B + threadIdx.x];
			__syncthreads();

			#pragma unroll
			for (int j = 0; j < BLOCK_SIZE; ++j)
				Cvalue += 1*tempA[j][threadIdx.y] * tempB[j][threadIdx.x] + 0*C[row * nr_cols_A + col] ;

			__syncthreads();
		}
		C[row * nr_cols_A + col] = Cvalue;

	}
}

__global__ void transp2kernelMatMul(float *C, const float *A,
		const float *B, unsigned int nr_rows_A, unsigned int nr_cols_A, unsigned int nr_cols_B){
	int row = blockIdx.y * blockDim.y + threadIdx.y;
	int col = blockIdx.x * blockDim.x + threadIdx.x;

	__shared__ float tempA[BLOCK_SIZE][BLOCK_SIZE];
	__shared__ float tempB[BLOCK_SIZE][BLOCK_SIZE];

	float Cvalue = 0;

	if (row < nr_rows_A && col < nr_cols_B) {
		//TODO Check if all their work is done, and the indexs
		for (int i = 0; i < ceil(float(nr_rows_A)/(BLOCK_SIZE)); ++i)
		{
			/*tempA[threadIdx.y*BLOCK_SIZE + threadIdx.x] = A[row * nr_col_A + i * BLOCK_START_SIZE + threadIdx.x];
			tempB[threadIdx.y*BLOCK_SIZE + threadIdx.x] = B[(i * BLOCK_START_SIZE + threadIdx.x)* nr_cols_B *  + row];*/
			tempA[threadIdx.y][threadIdx.x] = A[threadIdx.y * nr_cols_A + BLOCK_SIZE * i + threadIdx.x];
			tempB[threadIdx.y][threadIdx.x] = B[threadIdx.y * nr_cols_B + BLOCK_SIZE * i + threadIdx.x];
			__syncthreads();

			#pragma unroll
			for (int j = 0; j < BLOCK_SIZE; ++j)
				Cvalue += 1*tempA[threadIdx.y][j] * tempB[threadIdx.x][j] + 0*C[row * nr_cols_A + col] ;

			__syncthreads();
		}
		C[row * nr_cols_A + col] = Cvalue;

	}
}

//TODO allow transpose on multiplication
void pMatMul(int transp_1, int transp_2, float *C, const float *A, const float *B,
		unsigned int nr_rows_A, unsigned int nr_cols_A, unsigned int nr_cols_B){
	dim3 dimBlock(BLOCK_SIZE, BLOCK_SIZE);
	dim3 dimGrid(ceil(float(nr_rows_A) / dimBlock.x), ceil(float(nr_cols_B) / dimBlock.y));

	//cudaSetDeviceFlags(cudaDeviceLmemResizeToMax);
	if(transp_1)
		transp1kernelMatMul<<<dimGrid, dimBlock>>>(C, A, B, nr_rows_A, nr_cols_A, nr_cols_B);
	else if(transp_2)
		transp2kernelMatMul<<<dimGrid, dimBlock>>>(C, A, B, nr_rows_A, nr_cols_A, nr_cols_B);
	else
		kernelMatMul<<<dimGrid, dimBlock>>>(C, A, B, nr_rows_A, nr_cols_A, nr_cols_B);
}

void cublasMatMul(cublasHandle_t &handle, int transp_1, int transp_2, float *C,
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
	for(int i = 0; i < nr_rows_A * nr_cols_A; i++){
		C[i] = A[i] + B[i];
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
	for(int i = 0; i < nr_rows_A * nr_cols_A; i++){
		C[i] = A[i] - B[i];
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



void sMatInverse(float *A, int nr_rows_A, int nr_cols_A, float *resultado){
	/*thrust::device_vector<float> I(nr_rows_A * nr_cols_A);
	thrust::host_vector<float> L(nr_rows_A * nr_cols_A);
	thrust::fill(I.begin(), I.end(), 0);
	cudaDeviceSynchronize();
	createIdentity(thrust::raw_pointer_cast(&I[0]), nr_rows_A);
	cudaDeviceSynchronize();*/

	//get L from A = LL^T
	/*choleskyDecomp(A, thrust::raw_pointer_cast(&L[0]), nr_rows_A, nr_cols_A);
*/
	gsl_matrix_float_view gA = gsl_matrix_float_view_array(A, nr_rows_A, nr_cols_A);
	//gsl_matrix_float *gA = gsl_matrix_float_alloc (nr_rows_A, nr_cols_A);
	gsl_linalg_float_cholesky_decomp(&gA.matrix);
	gsl_linalg_float_cholesky_invert(&gA.matrix);
}

void choleskyDecomp(const float *A, float *L, int nr_rows_A, int nr_cols_A){
	int i,j,k;
	float sum;
	for(i = 0; i < nr_rows_A; i++) {
		for (j = 0; j <= i; ++j) {
			sum = 0;
			for(k = 0; k < j; k++) {
				sum += L[i*nr_cols_A +k] * L[j*nr_cols_A + k];
			}
			sum -= A[i*nr_cols_A + j];
			if(i == j) {
				L[i*nr_cols_A + j] = sqrt(sum);
			} else {
				L[i*nr_cols_A + j] = sum / L[j*nr_cols_A + j];
			}
		}
	}
}

__global__ void choleskyDecompKernel(int ind, const float *A, float *L, float diagonal, int nr_rows_A, int nr_cols_A){
	/*int row = blockIdx.y * blockDim.y + threadIdx.y;
	int col = blockIdx.x * blockDim.x + threadIdx.x;*/
	int row = blockIdx.x * blockDim.x + threadIdx.x;
	__shared__ float temp[BLOCK_START_SIZE];
	
	if (row < nr_rows_A) {
		int x = threadIdx.x, y = threadIdx.y, problem = blockIdx.x;
		//temp[x] = -A[row + ind];
		for(int i = 0; i < ind; i++){
			L[row + ind] += L[row + i] * L[ ind * nr_cols_A + i];
		}
		//run Gauss-Jordan in shared memory (see next slide)
		#pragma unroll
		for( int i = 0; i < BLOCK_SIZE; i++ ) {
			if( y == i ) temp[y][x] /= temp[i][i];
			__syncthreads( );
			if( y != i ) temp[y][x] -= temp[y][i]*temp[i][x];
		}
		//copy result to global memory
		L[32*32*problem+32*y+x] = temp[y][x];
	}
}


//TODO
void pMatInverse(const float *A, int nr_rows_A, int nr_cols_A, float *result){
	dim3 dimBlock(BLOCK_SIZE, BLOCK_SIZE);
	dim3 dimGrid(ceil(float(nr_rows_A) / dimBlock.x), ceil(float(nr_cols_A) / dimBlock.y));
}

//TODO correct this one
void cublasMatInverse(cublasHandle_t &handle, const float *A, int nr_rows_A, int nr_cols_A, float *result){

	const float alf = 1;
	const float *alpha = &alf;

	//create the identity matrix
	thrust::device_vector<float> I(nr_rows_A * nr_cols_A);
	thrust::host_vector<float> L(nr_rows_A * nr_cols_A);
	thrust::fill(I.begin(), I.end(), 0);
	cudaDeviceSynchronize();
	createIdentity(thrust::raw_pointer_cast(&I[0]), nr_rows_A);
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


