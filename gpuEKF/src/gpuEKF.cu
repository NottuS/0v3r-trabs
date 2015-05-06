/**
* Implement a parallel and a sequencial version of the EKF for SLAM.
*
* EKF(mean_t-1, covariance_t-1, control_t, observation_t)
* 
* 	mean'_t = g(control_t, mean_t-1)
* 	covariance'_t = Jacobian(g()) * covariance_t-1 * Jacobian(g())^T + OdometryError_t
* 
* 	KalmanGain = covariance'_t * Jacobian(h(observation_t)) * 
* 		(Jacobian(h(observation_t)) * covariance'_t * Jacobian(h(observation_t)) + ObservatioError_t)^-1
* 	mean_t = mean'_t + KalmanGain * (observation_t - h(observation_t))
* 	covariance_t = (I - KalmanGain* Jacobian(h(observation_t))) * covariance'_t
* 	return mean_t, covariance_t
 */
#include <stdio.h>
#include <stdlib.h>
#include <ctime>
#include <iostream>
#include<math.h>

#include<cublas_v2.h>
#include <thrust/host_vector.h>
#include <thrust/device_vector.h>

#include "matrix.h"
#include "EKF.h"

#define TRANS 0
#define MUL 1
#define SUM 2
#define INV 3

void comp(int argc, char** argv){
	cublasHandle_t handle;
	CUBLAS_CHECK_RETURN(cublasCreate(&handle));
	int operation;
	// Allocate 3 arrays on CPU
	int nr_rows_A, nr_cols_A, nr_rows_B, nr_cols_B, nr_rows_C, nr_cols_C;

	if(argc < 3){
		nr_rows_A = nr_cols_A = nr_rows_B = nr_cols_B = nr_rows_C = nr_cols_C = 3;
		operation = 1;
	} else {
		nr_rows_A = nr_cols_A = nr_rows_B = nr_cols_B = nr_rows_C = nr_cols_C = atoi(argv[1]);
		operation = atoi(argv[2]);
	}

	thrust::device_vector<float> d_A(nr_rows_A * nr_cols_A),
			d_B(nr_rows_B * nr_cols_B), d_C(nr_rows_C * nr_cols_C), d_T(nr_rows_C * nr_cols_C);

	// Fill the arrays A and B on GPU with random numbers
	GPU_fill_rand(thrust::raw_pointer_cast(&d_A[0]), nr_rows_A, nr_cols_A);
	GPU_fill_rand(thrust::raw_pointer_cast(&d_B[0]), nr_rows_B, nr_cols_B);
	//GPU_fill_rand(thrust::raw_pointer_cast(&d_T[0]), nr_rows_B, nr_cols_B);
	pMatMul(NOT_TRANSP, TRANSP, thrust::raw_pointer_cast(&d_T[0]), thrust::raw_pointer_cast(&d_A[0]), thrust::raw_pointer_cast(&d_A[0]), nr_rows_A, nr_cols_A, nr_cols_A);
	//thrust::device_vector<int> vec(d_A, d_A + n);
	cudaDeviceSynchronize();
	
	thrust::host_vector<float>A(d_A.begin(), d_A.begin() + d_A.size());
	thrust::host_vector<float>B(d_B.begin(), d_B.begin() + d_B.size());
	thrust::host_vector<float>T(d_T.begin(), d_T.begin() + d_T.size());
	thrust::host_vector<float>C(nr_rows_C * nr_cols_C);
	sMatMul(NOT_TRANSP, TRANSP, thrust::raw_pointer_cast(&T[0]), thrust::raw_pointer_cast(&A[0]), thrust::raw_pointer_cast(&A[0]), nr_rows_A, nr_cols_A, nr_cols_A);

	clock_t start = clock();
	switch (operation) {
		case TRANS:
			sMatTranspose(thrust::raw_pointer_cast(&C[0]),
					thrust::raw_pointer_cast(&A[0]), nr_rows_A, nr_cols_A);
			break;
		case MUL:
			sMatMul(NOT_TRANSP, NOT_TRANSP, thrust::raw_pointer_cast(&C[0]), thrust::raw_pointer_cast(&A[0]),
					thrust::raw_pointer_cast(&B[0]), nr_rows_A, nr_cols_A, nr_cols_A);
			break;
		case SUM:
			sMatSum(thrust::raw_pointer_cast(&C[0]), thrust::raw_pointer_cast(&A[0]),
					thrust::raw_pointer_cast(&B[0]), nr_rows_A, nr_cols_A);
			break;
		case INV:
			/*sMatInverse(thrust::raw_pointer_cast(&A[0]),
					nr_rows_A, nr_cols_A, thrust::raw_pointer_cast(&C[0]));*/
			choleskyDecomp(thrust::raw_pointer_cast(&T[0]), thrust::raw_pointer_cast(&C[0]), nr_rows_A, nr_cols_A);
			break;
		default:
			thrust::host_vector<float>T(A.begin(), A.begin() + A.size());
			sMatMul(NOT_TRANSP, TRANSP, thrust::raw_pointer_cast(&T[0]), thrust::raw_pointer_cast(&A[0]), thrust::raw_pointer_cast(&A[0]), nr_rows_A, nr_cols_A, nr_cols_B);
			choleskyDecomp(thrust::raw_pointer_cast(&T[0]), thrust::raw_pointer_cast(&C[0]), nr_rows_A, nr_cols_A);
			print_matrix(thrust::raw_pointer_cast(&C[0]), nr_rows_A, nr_cols_A);
			gsl_matrix_float_view gA = gsl_matrix_float_view_array(thrust::raw_pointer_cast(&T[0]), nr_rows_A, nr_cols_A);
			gsl_linalg_float_cholesky_decomp(&gA.matrix);
			print_matrix(thrust::raw_pointer_cast(&T[0]), nr_rows_A, nr_cols_A);

			break;
	}
	clock_t end = clock();
	printf("seq Mul took: %f seconds \n", float(end - start) / CLOCKS_PER_SEC);
	if(argc < 4 || atoi(argv[3]) != 0){
		print_matrix(thrust::raw_pointer_cast(&C[0]), nr_rows_A, nr_cols_A);
	}
	start = clock();
	switch (operation) {
		case TRANS:
			cublasMatTranspose(handle, thrust::raw_pointer_cast(&d_C[0]), thrust::raw_pointer_cast(&d_A[0]),
					thrust::raw_pointer_cast(&d_B[0]), nr_rows_A, nr_cols_A);
			break;
		case MUL:
			cublasMatMul(handle, NOT_TRANSP, NOT_TRANSP, thrust::raw_pointer_cast(&d_C[0]), thrust::raw_pointer_cast(&d_A[0]),
					thrust::raw_pointer_cast(&d_B[0]),	nr_rows_A, nr_cols_A, nr_cols_A);
			break;
		case SUM:
			cublasMatSum(handle, thrust::raw_pointer_cast(&d_C[0]), thrust::raw_pointer_cast(&d_A[0]),
					thrust::raw_pointer_cast(&d_B[0]), nr_rows_A, nr_cols_A);
			break;
		case INV:

			break;
		default:
			break;
	}

	end = clock();
	printf("par Mul took: %f seconds \n", float(end - start) / CLOCKS_PER_SEC);
	thrust::copy(d_C.begin(), d_C.end(), C.begin());
	// deallocate any capacity which may currently be associated with vec
	//vec.shrink_to_fit();

	if(argc < 4 || atoi(argv[3]) != 0){
		print_matrix(thrust::raw_pointer_cast(&C[0]), nr_rows_A, nr_cols_A);
	}

	start = clock();
	switch (operation) {
		case TRANS:
			pMatTranspose(thrust::raw_pointer_cast(&d_C[0]),
					thrust::raw_pointer_cast(&d_A[0]), nr_rows_A, nr_cols_A);
			break;
		case MUL:
			pMatMul(NOT_TRANSP, TRANSP, thrust::raw_pointer_cast(&d_C[0]), thrust::raw_pointer_cast(&d_A[0]),
					thrust::raw_pointer_cast(&d_B[0]),	nr_rows_A, nr_cols_A, nr_cols_A);
			break;
		case SUM:
			pMatSum(thrust::raw_pointer_cast(&d_C[0]), thrust::raw_pointer_cast(&d_A[0]),
					thrust::raw_pointer_cast(&d_B[0]), nr_rows_A, nr_cols_A);
			break;
		case INV:
				pMatInverse(thrust::raw_pointer_cast(&d_T[0]), thrust::raw_pointer_cast(&d_C[0]), nr_rows_A, nr_cols_A);
			break;
		default:
			break;
	}

	end = clock();
	printf("My par Mul took: %f seconds \n", float(end - start) / CLOCKS_PER_SEC);
	thrust::copy(d_C.begin(), d_C.end(), C.begin());
	// deallocate any capacity which may currently be associated with vec
	//vec.shrink_to_fit();
	if(argc < 4 || atoi(argv[3]) != 0){
		print_matrix(thrust::raw_pointer_cast(&C[0]), nr_rows_A, nr_cols_A);
	}


	/*multComp(handle, A, B, C, d_A, d_B, d_C, nr_rows_A, nr_cols_A);
	sumComp(handle, A, B, C, d_A, d_B, d_C, nr_rows_A, nr_cols_A);*/
	cublasDestroy(handle);
}

//TODO
void parallelEKF(float *mean, float *covariance){
	
}

int main(int argc, char** argv) {
	comp(argc, argv);
}
