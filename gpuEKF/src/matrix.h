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
#include<cublasXt.h>
#include <thrust/host_vector.h>
#include <thrust/device_vector.h>
#include <thrust/fill.h>
#include <iostream>
#include<math.h>

#define IDX2C (i, j, ld) ((( j )*( ld ))+( i ))

#define CUBLAS_CHECK_RETURN(value) {											\
	cublasStatus_t stat = value;										\
	if (stat != CUBLAS_STATUS_SUCCESS) {										\
		fprintf(stderr, "Error %s at line %d in file %s\n",					\
				stat, __LINE__, __FILE__);		\
		/*exit(1);*/															\
	}}

void print_matrix(const float *A, int nr_rows_A, int nr_cols_A);

void sMatMul(float *C, const float *A, const float *B, unsigned int hA, unsigned int wA, unsigned int wB);
void pMatMul(float *C, const float *A, const float *B, unsigned int hA, unsigned int wA, unsigned int wB);
//BLAS uses internally column-major order storage (Fortran order) and not the typical row-major storage used in C or C++ for multidimensional arrays.
void cublasMatMul(cublasHandle_t &handle, float *C, const float *A, const float *B, unsigned int m, unsigned int k, unsigned int n);

void sMatTranspose(const float *A, int nr_rows_A, int nr_cols_A);
void pMatTranspose(const float *A, int nr_rows_A, int nr_cols_A);
void cublasMatTranspose(cublasHandle_t &handle, const float *C, const float *A, const float *B, int nr_rows_A, int nr_cols_A);

void sMatSum(const float *C, const float *A, const float *B, int nr_rows_A, int nr_cols_A);
void pMatSum(const float *C, const float *A, const float *B, int nr_rows_A, int nr_cols_A);
void cublasMatSum(cublasHandle_t &handle, const float *C, const float *A, const float *B, int nr_rows_A, int nr_cols_A);

void pMatSub(const float *C, const float *A, const float *B, int nr_rows_A, int nr_cols_A);
void pMatSub(const float *C, const float *A, const float *B, int nr_rows_A, int nr_cols_A);
void cublasMatSub(cublasHandle_t &handle, const float *C, const float *A, const float *B, int nr_rows_A, int nr_cols_A);

void sInvMat(const float *A, int nr_rows_A, int nr_cols_A);
void pInvMat(const float *A, int nr_rows_A, int nr_cols_A);
void cublasInvMat(cublasHandle_t &handle, const float *A, int nr_rows_A, int nr_cols_A);

#endif /* MATRIX_H_ */
