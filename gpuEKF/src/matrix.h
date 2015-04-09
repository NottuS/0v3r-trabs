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

#define CUBLAS_CHECK_RETURN(value) {											\
	cudaError_t _m_cudaStat = value;										\
	if (_m_cudaStat != cudaSuccess) {										\
		fprintf(stderr, "Error %s at line %d in file %s\n",					\
				cudaGetErrorString(_m_cudaStat), __LINE__, __FILE__);		\
		/*exit(1);*/															\
	}}

void sMatMul(float *C, const float *A, const float *B, unsigned int hA, unsigned int wA, unsigned int wB);

void pMatMul();

void cublasMatMul(cublasHandle_t &handle, float *C, const float *A, const float *B, unsigned int m, unsigned int k, unsigned int n);

#endif /* MATRIX_H_ */
