#include"matrix.h"
#include <stdio.h>

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

void pMatMul(float *C, const float *A, const float *B, unsigned int hA, unsigned int wA, unsigned int wB){

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
