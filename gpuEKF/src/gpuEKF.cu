/**
* Implement a parallel and a sequencial version of the EKF for SLAM.
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

#include<cublas_v2.h>
#include <thrust/host_vector.h>
#include <thrust/device_vector.h>

#include"matrix.h"
#define m 6 // a - mxk matrix
#define n 4 // b - kxn matrix
#define k 5 // c - mxn matrix
#define TRANS 0
#define MUL 1
#define SUM 2
#define INV 3

void sumComp(cublasHandle_t handle, thrust::host_vector<float>A, thrust::host_vector<float> B, thrust::host_vector<float> C,
		thrust::device_vector<float> d_A, thrust::device_vector<float>d_B,
		thrust::device_vector<float>d_C, int nr_rows_A, int nr_cols_A){

}

float *odometryError(float *control, int dim){
		
}

float *observationError(float *observation, int dim) {
	
}

float *jacobianG(int dim){
		
}

float *jacobianH(int dim){
		
}

float *moveUpadate(float *mean, float *control){
	int x = 0*3 + 0;
	int y = 1*3 + 1;
	int teta = 2*3 + 2;
	
	mean[x] = mean[x] + control[0] * cos(mean[teta] + control[2]/2);
	mean[y] = mean[y] + control[1] * sin(mean[teta] + control[2]/2);
	mean[teta] = media[teta] + control[2];
	
	return mean;
}

bool lanmarkExist(float *observation) {
	return true;
}

void addLandmark(float *mean, float *covariance, int *dim){
	
}

void EKF(int dim, float *mean, float *covariance, const float *control, const float *observation){
	//mean'_t = g(control_t, mean_t-1)
	float *mean = moveUpadate(mean, control);
	float *G = jacobianG(dim);
	
	//Update step
	float *partial = malloc(sizeof(float) * dim * dim);
	sMatMul(partial, G, covariance, dim, dim, dim);
	
	//vai dar bosta esse transpose tem q trocar o g
	sMatTranspose(G, G, dim, dim);
	sMatMul(covariance, partial, G, dim, dim, dim);
	sMatSum(covariance, covariance, odometryError(control, dim));

	//Matching and compute h(observation_t)
	if(!lanmarkExist(observation)){
		addLandmark(mean, covariance, &dim);
		partial = (float *) realloc(partial, sizeof(float) * dim * dim);
	}
	//compute h(observation_t)
	
	//Correction/update step
	float *kalmanGain = malloc(sizeof(float) * dim * dim);
	float H = jacobianH(dim);
	float H_t = malloc(sizeof(float) * dim * dim);
	
	sMatTranspose(H_t, H, dim, dim);
	sMatMul(partial, H_t, covariance);
	sMatMul(kalmanGain, H, partial);
	sMatSum(kalmanGain, kalmanGain, observationError(observation));
	//check this probabily wrong
	sMatMul(kalmanGain, Partial, kalmanGain);
	
	//missing compute mean
	
	sMatMul(partial, kalmanGain, H);
	//criar um special sub
	//sMatSub(partial, identity, partial);
	//check this probabily wrong
	sMatMul(covariance, partial, covariance);
}

void parallelEKF(float *mean, float *covariance){
	
}



int main(int argc, char** argv) {
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
			d_B(nr_rows_B * nr_cols_B), d_C(nr_rows_C * nr_cols_C);

	// Fill the arrays A and B on GPU with random numbers
	GPU_fill_rand(thrust::raw_pointer_cast(&d_A[0]), nr_rows_A, nr_cols_A);
	GPU_fill_rand(thrust::raw_pointer_cast(&d_B[0]), nr_rows_B, nr_cols_B);
	//thrust::device_vector<int> vec(d_A, d_A + n);

	thrust::host_vector<float>A(d_A.begin(), d_A.begin() + d_A.size());
	thrust::host_vector<float>B(d_B.begin(), d_B.begin() + d_B.size());
	thrust::host_vector<float>C(nr_rows_C * nr_cols_C);


	clock_t start = clock();
	switch (operation) {
		case TRANS:
			sMatTranspose(thrust::raw_pointer_cast(&C[0]),
					thrust::raw_pointer_cast(&A[0]), nr_rows_A, nr_cols_A);
			break;
		case MUL:
			sMatMul(thrust::raw_pointer_cast(&C[0]), thrust::raw_pointer_cast(&A[0]),
					thrust::raw_pointer_cast(&B[0]), nr_rows_A, nr_cols_A, nr_cols_A);
			break;
		case SUM:
			sMatSum(thrust::raw_pointer_cast(&C[0]), thrust::raw_pointer_cast(&A[0]),
					thrust::raw_pointer_cast(&B[0]), nr_rows_A, nr_cols_A);
			break;
		case INV:
			sMatInverse(thrust::raw_pointer_cast(&A[0]),
					nr_rows_A, nr_cols_A, thrust::raw_pointer_cast(&C[0]));
			break;
		default:
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
			cublasMatMul(handle, thrust::raw_pointer_cast(&d_C[0]), thrust::raw_pointer_cast(&d_A[0]),
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
			pMatMul(thrust::raw_pointer_cast(&d_C[0]), thrust::raw_pointer_cast(&d_A[0]),
					thrust::raw_pointer_cast(&d_B[0]),	nr_rows_A, nr_cols_A, nr_cols_A);
			break;
		case SUM:
			pMatSum(thrust::raw_pointer_cast(&d_C[0]), thrust::raw_pointer_cast(&d_A[0]),
					thrust::raw_pointer_cast(&d_B[0]), nr_rows_A, nr_cols_A);
			break;
		case INV:

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
