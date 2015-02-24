#include "gpuIntegral.h"
#include "gpuUtils.h"
#include <stdio.h>
#include <stdlib.h>

__global__ void rowsSum(float *data, float *r_data2, int height, int width) {
	int row = blockIdx.y * blockDim.y + threadIdx.y;
	int col = blockIdx.x * blockDim.x + threadIdx.x;

	if (row < height && col < width) {
		r_data2[row*width + col] = 0;
		for (int i = col - 1; i >= 0; i--)
			r_data2[row*width + col] += data[row * width + i];
	}
}

__global__ void colsSum(float *data, float *r_data1, int height, int width){
	int row = blockIdx.y * blockDim.y + threadIdx.y;
	int col = blockIdx.x * blockDim.x + threadIdx.x;
	
	if (row < height && col < width) {
		r_data1[row*width + col] = 0;
		for (int i = row - 1; i >= 0; i--)
			r_data1[row*width + col] += data[i * width + col];
	}
}

__global__ void matrixSum(float *data, float *r_data1, float *r_data2, int height, int width){
	int row = blockIdx.y * blockDim.y + threadIdx.y;
	int col = blockIdx.x * blockDim.x + threadIdx.x;

	if (row < height && col < width) {
		r_data1[row*width + col] = data[row*width + col] + r_data1[row*width + col] + r_data2[row*width + col];
	}
}

__global__ void integralImage(float *r_data1, float *r_data2, int height, int width){
	int row = blockIdx.y * blockDim.y + threadIdx.y;
	int col = blockIdx.x * blockDim.x + threadIdx.x;

	if (row < height && col < width) {
		r_data2[row*width + col] = r_data1[row*width + col];
		for (int i = 1; i <= row && i <= col; i++)
			r_data2[row*width + col] +=  r_data1[(row - i)*width + col - i];
	}
}

void gpuIntegralImage(float *data, float *r_data, int height, int width){
	dim3 dimBlock(BLOCK_SIZE, BLOCK_SIZE);
	dim3 dimGrid(width / dimBlock.x, height / dimBlock.y);
	/*int dimBlock = BLOCK_START_SIZE;
	int dimGrid = ceil(height * width / (float) dimBlock);*/
	float *d_data = NULL; float *d_r_data1 = NULL; float *d_r_data2 = NULL;
	float *teste = (float *)calloc(height * width, sizeof(float));

	CUDA_CHECK_RETURN(cudaSetDevice(0));
	CUDA_CHECK_RETURN(cudaMalloc((void **)&d_data, height * width * sizeof(float))) ;
	CUDA_CHECK_RETURN(cudaMalloc((void **)&d_r_data1, height * width * sizeof(float)));
	CUDA_CHECK_RETURN(cudaMalloc((void **)&d_r_data2, height * width * sizeof(float)));
	CUDA_CHECK_RETURN(cudaMemcpy(d_data, data, height * width * sizeof(float), cudaMemcpyHostToDevice));
	
	rowsSum <<<dimGrid, dimBlock>>>(d_data, d_r_data1, height, width);
	for (int i = 0; i < height; i++) {
		//teste[i*width + j] = 0;
		for (int j = 1; j < width; j++)
		{
			teste[i*width + j] += data[i*width + j - 1] + teste[i*width + j - 1];
		}
	}

	colsSum <<<dimGrid, dimBlock>>>(d_data, d_r_data2, height, width);
	CUDA_CHECK_RETURN(cudaDeviceSynchronize());	// Wait for the GPU launched work to complete
	matrixSum <<<dimGrid, dimBlock>>>(d_data, d_r_data1, d_r_data2, height, width);
	CUDA_CHECK_RETURN(cudaDeviceSynchronize());	// Wait for the GPU launched work to complete
	integralImage <<<dimGrid, dimBlock>>>(d_r_data1, d_r_data2, height, width);
	CUDA_CHECK_RETURN(cudaDeviceSynchronize());	// Wait for the GPU launched work to complete

	CUDA_CHECK_RETURN(cudaMemcpy(r_data, d_r_data2, height * width * sizeof(float), cudaMemcpyDeviceToHost)); //<--CORRETO
	//CUDA_CHECK_RETURN(cudaMemcpy(r_data, d_r_data1, height * width * sizeof(float), cudaMemcpyDeviceToHost));// <--- TESTE
	/*for (int i = 0; i < height * width; i++) {
		if (teste[i] != r_data[i])
			int y;//return FALSE;
	}*/
	cudaFree(d_data);
	cudaFree(d_r_data1);
	cudaFree(d_r_data2);
}
