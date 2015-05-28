#include <stdio.h>
#include <stdlib.h>

#include <thrust/host_vector.h>
#include <thrust/device_vector.h>
#include "matrix.h"
#include <math.h>

#define BLOCk 32

void choleskyDecomp(const float *A, float *L, int n, int nr_cols_A){
	int i,j,k;
	float sum;
	for(i = 0; i < n; i++) {
		for (j = 0; j <= i; ++j) {
			sum = A[i*n + j];
			for(k = 0; k < j; k++) {
				sum -= L[i*n +k] * L[j*n + k];
			}
			if(i == j) {
				L[i*n + j] = sqrt(sum);
			} else {
				L[i*n + j] = sum / L[j*n + j];
			}
		}
	}
}

void choleskyDecompositionGPU( int n, float *cpu_A, float * d_A, float *d_L){
	for (int i = 0; i < n; i += BLOCk) {
		int h = n - i;
		int w = h < BLOCk ? h : BLOCk
		if(i > 0){
			srku();
			mul();
			if(h > BLOCk){
				srku();
			}
		}

		choleskyDecomp();
		if(h > BLOCk){
			solve();
		}
	}
}

void invert( int n) {
	thrust::device_vector<float> d_A(n * n);
	thrust::device_vector<float> d_L(n * n);
	thrust::host_vector<float>h_A(n * n);
	thrust::host_vector<float>h_L(n * n);
	float A = thrust::raw_pointer_cast(&h_A[0]);

	srand(n);
	for( int i = 0; i < n; i++ ) {
		for (int j = i; j < n; j++) {
			A[i*n+j] = 2.0*(rand()%32768)/32768.0;
			A[j*n+i] = A[i*n+j];
		}
		A[i*n+i] += 2*sqrt((float)n);
	}


}
