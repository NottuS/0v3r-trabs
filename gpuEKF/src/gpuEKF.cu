/**
 * Copyright 1993-2012 NVIDIA Corporation.  All rights reserved.
 *
 * Please refer to the NVIDIA end user license agreement (EULA) associated
 * with this source code for terms and conditions that govern your use of
 * this software. Any use, reproduction, disclosure, or distribution of
 * this software and related documentation outside the terms of the EULA
 * is strictly prohibited.
 */
#include <stdio.h>
#include <stdlib.h>
#include <ctime>
#include <iostream>

#include<cublas_v2.h>
#include <thrust/host_vector.h>
#include <thrust/device_vector.h>

#include"matrix.h"
#define IDX2C (i ,j , ld ) ((( j )*( ld ))+( i ))
#define m 6 // a - mxk matrix
#define n 4 // b - kxn matrix
#define k 5 // c - mxn matrix


/**
 * Host function that prepares data array and passes it to the CUDA kernel.
 */
int main(int argc, char** argv) {
	cublasHandle_t handle;
	CUBLAS_CHECK_RETURN(cublasCreate(&handle));

	// Allocate 3 arrays on CPU
	int nr_rows_A, nr_cols_A, nr_rows_B, nr_cols_B, nr_rows_C, nr_cols_C;

	// for simplicity we are going to use square arrays
	nr_rows_A = nr_cols_A = nr_rows_B = nr_cols_B = nr_rows_C = nr_cols_C = 1000;

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
	sMatMul(thrust::raw_pointer_cast(&C[0]), thrust::raw_pointer_cast(&A[0]),
			thrust::raw_pointer_cast(&B[0]), nr_rows_A, nr_cols_A, nr_cols_B);
	clock_t end = clock();
	printf("seq Mul took: %f seconds \n", float(end - start) / CLOCKS_PER_SEC);
	//print_matrix(thrust::raw_pointer_cast(&C[0]), nr_rows_C, nr_cols_C);

	start = clock();
	cublasMatMul(handle, thrust::raw_pointer_cast(&d_C[0]), thrust::raw_pointer_cast(&d_A[0]),
			thrust::raw_pointer_cast(&d_B[0]),	nr_rows_A, nr_cols_A, nr_cols_B);
	end = clock();

	printf("seq Mul took: %f seconds \n", float(end - start) / CLOCKS_PER_SEC);
	//thrust::copy(d_C.begin(), d_C.end(), C.begin());
	//print_matrix(thrust::raw_pointer_cast(&C[0]), nr_rows_C, nr_cols_C);
	cublasDestroy(handle);
}
