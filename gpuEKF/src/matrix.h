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
#include <eigen/Eigen/Dense>
#include <eigen/Eigen/Core>
#include <gsl/gsl_blas.h>
#include <gsl/gsl_linalg.h>
#include <gsl/gsl_matrix.h>
#include "cholesky_single.h"

#define TRASN 1
#define NOT_TRASN 0

#define IDX2C (i, j, ld) ((( j )*( ld ))+( i ))

#define CUBLAS_CHECK_RETURN(value) {											\
	cublasStatus_t stat = value;										\
	if (stat != CUBLAS_STATUS_SUCCESS) {										\
		fprintf(stderr, "Error %d at line %d in file %s\n",					\
				stat, __LINE__, __FILE__);		\
		/*exit(1);*/															\
	}}

void print_matrix(const float *A, int nr_rows_A, int nr_cols_A);
void sCreateIdentity(float *I, int nr_rows_I);
void createIdentity(float *I, int nr_rows_I);

void sMatMul(int trans_1, int trans_2, float *C, const float *A, const float *B, unsigned int hA, unsigned int wA, unsigned int wB);
void pMatMul(int trans_1, int trans_2, float *C, const float *A, const float *B, unsigned int hA, unsigned int wA, unsigned int wB);
//BLAS uses internally column-major order storage (Fortran order) and not the typical row-major storage used in C or C++ for multidimensional arrays.
void cublasMatMul(cublasHandle_t &handle, int trans_1, int trans_2, float *C, const float *A, const float *B, unsigned int m, unsigned int k, unsigned int n);

void sMatTranspose(int trans_1, int trans_2, float *C, const float *A, int nr_rows_A, int nr_cols_A);
void pMatTranspose(int trans_1, int trans_2, float *C, const float *A, int nr_rows_A, int nr_cols_A);
void cublasMatTranspose(cublasHandle_t &handle, float *C, const float *A, const float *B, int nr_rows_A, int nr_cols_A);

void sMatSum(float *C, const float *A, const float *B, int nr_rows_A, int nr_cols_A);
void pMatSum(float *C, const float *A, const float *B, int nr_rows_A, int nr_cols_A);
void cublasMatSum(cublasHandle_t &handle, float *C, const float *A, const float *B, int nr_rows_A, int nr_cols_A);

void pMatSub(float *C, const float *A, const float *B, int nr_rows_A, int nr_cols_A);
void pMatSub(float *C, const float *A, const float *B, int nr_rows_A, int nr_cols_A);
void cublasMatSub(cublasHandle_t &handle, float *C, const float *A, const float *B, int nr_rows_A, int nr_cols_A);

void sMatInverse(float *A, int nr_rows_A, int nr_cols_A, float *resultado);
void pMatInverse(const float *A, int nr_rows_A, int nr_cols_A, float *resultado);
void cublasMatInverse(cublasHandle_t &handle, const float *A, int nr_rows_A, int nr_cols_A, float *result);

#endif /* MATRIX_H_ */
