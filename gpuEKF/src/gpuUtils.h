#ifndef GPU_UTILS_H
#define GPU_UTILS_H

#include<curand.h>
#include "cuda_runtime.h"
#include "device_launch_parameters.h"
#include <thrust/copy.h>
#include <thrust/fill.h>
#include <thrust/sequence.h>

#define BLOCK_SIZE 16
#define BLOCK_START_SIZE 256//usually the a good number of a block size

#define CUDA_CHECK_RETURN(value) {											\
	cudaError_t _m_cudaStat = value;										\
	if (_m_cudaStat != cudaSuccess) {										\
		fprintf(stderr, "Error %s at line %d in file %s\n",					\
				cudaGetErrorString(_m_cudaStat), __LINE__, __FILE__);		\
		/*exit(1);*/															\
	}}

void GPU_fill_rand(float *A, int nr_rows_A, int nr_cols_A);

#endif
