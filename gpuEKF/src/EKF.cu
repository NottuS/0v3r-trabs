
#include "matrix.h"
#include "EKF.h"

float *odometryError(const float *control, int dim){
	float *error = (float *) calloc(dim * dim, sizeof(float));

	error[0] = control[CONTROL_X] * 0.2;
	error[dim + 1] = control[CONTROL_Y] * 0.2;
	error[2*dim + 2] = control[CONTROL_TETA] * 0.2;

	if( error[2*dim + 2]> M_PI ){
		error[2*dim + 2] -= 2*M_PI;
	}

	if( error[2*dim + 2] < -M_PI ){
		error[2*dim + 2] += 2*M_PI;
	}

	return error;
}

float *moveUpadate(float *mean, const float *control) {
	mean[MEAN_X] = mean[MEAN_X] + control[CONTROL_X] * cos(mean[MEAN_TETA] + control[CONTROL_TETA]/2);
	mean[MEAN_Y] = mean[MEAN_Y] + control[CONTROL_Y] * sin(mean[MEAN_TETA] + control[CONTROL_TETA]/2);
	mean[MEAN_TETA] = mean[MEAN_TETA] + control[CONTROL_TETA];

	if( mean[MEAN_TETA] > M_PI ){
		mean[MEAN_TETA] -= 2*M_PI;
	}

	if( mean[MEAN_TETA] < -M_PI ){
		mean[MEAN_TETA] += 2*M_PI;
	}

	return mean;
}

// check if right
float *jacobianG(float teta, const float *control, int dim){
	float *G = (float *) calloc(dim * dim, sizeof(float));

	sCreateIdentity(G, dim);
	G[2] = -sin(teta + control[CONTROL_TETA]/2);
	G[dim + 2] = cos(teta + control[CONTROL_TETA]/2);

	return G;
}

//TODO
float *observationError(const float *observation, int dim) {
	float *error = (float *) calloc(dim * dim, sizeof(float));

	return error;
}

float *jacobianH(float *mean, const float *observation, int dim){
	float *H = (float *) calloc(dim * 2, sizeof(float));
	int landmark = observation[LM_IND] * 2 + 3;
	float x = mean[landmark] - mean[MEAN_X];
	float y = mean[landmark + 1] - mean[MEAN_X];
	float d2 = x*x - y*y;
	float d = sqrt(d2);

	H[0] = -x/d;
	H[1] = -y/d;
	H[2] = 0;
	H[dim] = y/d2;
	H[dim + 1] = -x/d2;
	H[dim + 2] = -1;
	H[landmark] = x/d;
	H[landmark + 1] = y/d;
	H[dim + landmark] = - y/d2;
	H[dim + landmark + 1] = x/d2;

	return H;
}

float *getExpected(float *mean, const float *observation, int dim){
	float *expected = (float *) calloc(dim, sizeof(float));
	int landmark = observation[LM_IND] * 2 + 3;
	float x = mean[landmark] - mean[MEAN_X];
	float y = mean[landmark + 1] - mean[MEAN_X];
	float d = sqrt( x*x - y*y);

	expected[landmark] = observation[FOCAL_LENGTH] * observation[K_X] * (observation[CAM_X]/observation[CAM_Z]);
	expected[landmark + 1] = observation[FOCAL_LENGTH] * observation[K_Y] * (observation[CAM_Y]/observation[CAM_Z]);
	expected[landmark] -= d;
	expected[landmark + 1] -= atan2(x,y) + mean[MEAN_TETA];

	if( expected[landmark + 1] > M_PI ){
		expected[landmark + 1] -= 2*M_PI;
	}

	if( expected[landmark + 1] < -M_PI ){
		expected[landmark + 1] += 2*M_PI;
	}

	return expected;
}

bool landmarkExist(const float *observation) {
	if(observation[L_EXISTS] < 0)
		return false;
	return true;
}

void addLandmark(float *mean, float *covariance, const float *observation, int *dim){
	*dim = *dim + 2;
	mean = (float *) realloc(mean, (*dim) * sizeof(float));
	covariance = (float *) realloc(covariance, sizeof(float) * (*dim) * (*dim));

	//Set the position(x,y) of the landmark in relation to the robot
	mean[*dim - 2] = mean[MEAN_X] + observation[FOCAL_LENGTH] *
			observation[K_X] * (observation[CAM_X]/observation[CAM_Z]);
	mean[*dim - 1 ] = mean[MEAN_Y] + observation[FOCAL_LENGTH] *
			observation[K_Y] * (observation[CAM_Y]/observation[CAM_Z]);
}


//TODO take care on mul transpose, the lenghs of the matrix must be swaped
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
	//TODO take care on mul transpose, the lenghs of the matrix must be swaped
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

	//******Correction/Update step******
	//H is sparse, so this block can be improved...
	float *kalmanGain = (float *) malloc(sizeof(float) * dim * dim);
	float *H = jacobianH(mean, observation, dim);
	//E * H^T
	sMatMul(NOT_TRANSP, TRANSP, partial, H, covariance, dim, dim, dim);
	//H * E * H^T
	sMatMul(NOT_TRANSP, NOT_TRANSP, kalmanGain, H, partial, dim, dim, dim);
	//(H * E * H^T + observationError)
	temp = observationError(observation, dim);
	sMatSum(temp, kalmanGain, temp, dim, dim);
	//(H * E * H^T + observationError)^-1
	//check this, probabily wrong
	sMatInverse(temp, dim, dim);
	//K = E * H^T * (H * E * H^T + observationError)^-1
	sMatMul(NOT_TRANSP, NOT_TRANSP, kalmanGain, partial, temp, dim, dim, dim);

	//z - h(u)
	float *expected = (float *) getExpected(mean, observation, dim);
	float *temp2 =  (float *) malloc(sizeof(float) * dim);
	//K(z - h(u))
	sMatMul(NOT_TRANSP, NOT_TRANSP, temp2, kalmanGain, expected, dim, dim, 1);
	//u = u + K(z - h(u))
	sMatSum(mean, mean, temp2, dim, 1);
	free(expected);
	free(temp2);
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
