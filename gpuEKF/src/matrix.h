/*
 * matrix.h
 *
 *  Created on: Apr 8, 2015
 *      Author: nottus
 */

#ifndef MATRIX_H_
#define MATRIX_H_

#include"gpuUtils.h"
#include<curand.h>
#include<cublas_v2.h>
#include <thrust/host_vector.h>
#include <thrust/device_vector.h>
#include <iostream>

#define CUBLAS_CHECK_RETURN(value) {											\
	cublasStatus_t stat = value;										\
	if (stat != CUBLAS_STATUS_SUCCESS) {										\
		fprintf(stderr, "Error %s at line %d in file %s\n",					\
				stat, __LINE__, __FILE__);		\
		/*exit(1);*/															\
	}}

void sMatMul(float *C, const float *A, const float *B, unsigned int hA, unsigned int wA, unsigned int wB);

void pMatMul();

void cublasMatMul(cublasHandle_t &handle, float *C, const float *A, const float *B, unsigned int m, unsigned int k, unsigned int n);

void print_matrix(const float *A, int nr_rows_A, int nr_cols_A);

#endif /* MATRIX_H_ */
