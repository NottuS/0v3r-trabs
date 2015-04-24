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

#include"matrix.h"

#define TRANS 0
#define MUL 1
#define SUM 2
#define INV 3

//Important Index
#define MEAN_X (0*3 + 0)
#define MEAN_Y (1*3 + 1)
#define MEAN_TETA (2*3 + 2)
#define CONTROL_X 0 //Robot motion in the x axis
#define CONTROL_Y 1
#define CONTROL_TETA 2
#define FOCAL_LENGTH 0
#define K_X 1//is the number of pixels per unit length(k_x, k_y)
#define K_Y 2
#define CAM_X 3
#define CAM_Y 4
#define CAM_Z 5
#define L_EXISTS 6
#define LM_IND //landmark index related to the mean vector

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
			sMatMul(NOT_TRANSP, NOT_TRANSP, thrust::raw_pointer_cast(&C[0]), thrust::raw_pointer_cast(&A[0]),
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
			pMatMul(NOT_TRANSP, NOT_TRANSP, thrust::raw_pointer_cast(&d_C[0]), thrust::raw_pointer_cast(&d_A[0]),
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

//TODO
float *odometryError(const float *control, int dim){
	float *error = (float *) calloc(dim * dim, sizeof(float));

	srand(time(NULL));
	error[0] = rand() * 0.2;
	error[dim + 1] = rand() * 0.2;
	error[2*dim + 2] = rand() * 0.2;

	return error;
}

float *moveUpadate(float *mean, const float *control){
	mean[MEAN_X] = mean[MEAN_X] + control[CONTROL_X] * cos(mean[MEAN_TETA] + control[CONTROL_TETA]/2);
	mean[MEAN_Y] = mean[MEAN_Y] + control[CONTROL_Y] * sin(mean[MEAN_TETA] + control[CONTROL_TETA]/2);
	mean[MEAN_TETA] = mean[MEAN_TETA] + control[CONTROL_TETA];
	
	return mean;
}

//TODO check if right
float *jacobianG(float teta, const float *control, int dim){
	float *G = (float *) calloc(dim * dim, sizeof(float));

	sCreateIdentity(G, dim);
	G[2] = sin(teta + control[CONTROL_TETA]/2);
	G[dim + 2] = cos(teta + control[CONTROL_TETA]/2);

	return G;
}

//TODO
float *observationError(const float *observation, int dim) {
	float *error = (float *) malloc(sizeof(float) * dim * dim);

	return error;
}

//TODO
float *jacobianH(float *observed, const float *observation, int dim){
	float *H = (float *) malloc(sizeof(float) * dim * dim);

	return H;
}

//TODO
float *getExpected(const float *observation, int dim){
	float *expected = (float *) calloc(3 * dim, sizeof(float));

	return expected;
}

//TODO
float *getObservation(const float *observation, int dim){
	float *observed = (float *) malloc(sizeof(float) * 3 * dim);

	return observed;
}

//TODO
bool landmarkExist(const float *observation) {
	if(observation[L_EXISTS] < 0)
		return false;
	return true;
}

//TODO
void addLandmark(float *mean, float *covariance, const float *observation, int *dim){
	*dim = *dim + 2;
	mean = (float *) realloc(mean, 3 * (*dim) * sizeof(float));
	covariance = (float *) realloc(covariance, sizeof(float) * (*dim) * (*dim));

	//Set the position(x,y) of the landmark in relation to the robot
	mean[(*dim - 1)*3] = mean[MEAN_X] + observation[FOCAL_LENGTH] *
			observation[K_X] * (observation[CAM_X]/observation[CAM_Z]);
	mean[(*dim - 1) * 3 + 1] = mean[MEAN_Y] + observation[FOCAL_LENGTH] *
			observation[K_Y] * (observation[CAM_Y]/observation[CAM_Z]);
}

void EKF(int dim, float *mean, float *covariance, const float *control, const float *observation){
	//******Update step******
	//u_t = g(control_t, mean_t-1)
	float teta = mean[MEAN_TETA];
	moveUpadate(mean, control);

	//TODO this block can be improved look the slides of stachness, and G is sparse
	float *partial = (float *)malloc(sizeof(float) * dim * dim);
	float *G = jacobianG(teta, control, dim);
	float *temp;
	//G * E_t-1
	sMatMul(NOT_TRANSP, NOT_TRANSP, partial, G, covariance, dim, dim, dim);
	//G * E_t-1 * G^T
	sMatMul(NOT_TRANSP, TRANSP, covariance, partial, G, dim, dim, dim);
	temp = odometryError(control, dim);
	//E = G * E_t-1 * G^T + OdometryError_t
	sMatSum(covariance, covariance, temp, dim, dim);
	free(G);
	free(temp);


	////******Matching and compute h(observation_t)******
	if(!landmarkExist(observation)){
		addLandmark(mean, covariance, observation, &dim);
		partial = (float *) realloc(partial, sizeof(float) * dim * dim);
	}
	float *expected = (float *) getExpected(observation, dim);
	

	//******Correction/Update step******
	//H is sparse, so this block can be improved...
	float *kalmanGain = (float *) malloc(sizeof(float) * dim * dim);
	float *H = jacobianH(observed, observation, dim);
	//E * H^T
	sMatMul(NOT_TRANSP, TRANSP, partial, H, covariance, dim, dim, dim);
	//H * E * H^T
	sMatMul(NOT_TRANSP, NOT_TRANSP, kalmanGain, H, partial, dim, dim, dim);
	//(H * E * H^T + observationError)
	temp = observationError(observation, dim);
	sMatSum(temp, kalmanGain, temp, dim, dim);
	//(H * E * H^T + observationError)^-1
	//check this, probabily wrong
	sMatInverse(temp, dim, dim, temp);
	//K = E * H^T * (H * E * H^T + observationError)^-1
	sMatMul(NOT_TRANSP, NOT_TRANSP, kalmanGain, partial, temp, dim, dim, dim);
	
	//z - h(u)
	float *expected = getExpected(observation, dim);
	sMatSub(observed, expected, observed, dim, 3);
	//K(z - h(u))
	sMatMul(NOT_TRANSP, NOT_TRANSP, expected, kalmanGain, observed, dim, dim, 3);
	free(observed);
	//u = u + K(z - h(u))
	sMatSum(mean, mean, expected, dim, 3);
	free(expected);

	//K * H
	sMatMul(NOT_TRANSP, NOT_TRANSP, partial, kalmanGain, H, dim, dim, dim);
	//(I - K * H)
	//temp = Identity matrix
	memset(temp, 0, sizeof(float));
	sCreateIdentity(temp, dim);
	//THIS can be improved...
	sMatSub(partial, temp, partial, dim, dim);
	free(kalmanGain);
	free(H);

	//E = (I - K * H) * E
	sMatMul(NOT_TRANSP, NOT_TRANSP, temp, partial, covariance, dim, dim, dim);
	free(covariance);
	covariance = temp;
	free(temp);
	free(partial);
}

//TODO
void parallelEKF(float *mean, float *covariance){
	
}

int main(int argc, char** argv) {
	comp(argc, argv);
}
