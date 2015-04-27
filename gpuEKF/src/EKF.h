/*
 * EKF.h
 *
 *  Created on: Apr 24, 2015
 *      Author: edileuton
 */

#ifndef EKF_H_
#define EKF_H_


//Important Index
#define MEAN_X 0
#define MEAN_Y 1
#define MEAN_TETA 2
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
#define LM_IND 7//landmark index related to the mean vector

float *odometryError(const float *control, int dim);
float *moveUpadate(float *mean, const float *control);
float *jacobianG(float teta, const float *control, int dim);
float *observationError(const float *observation, int dim);
float *jacobianH(float *observed, const float *observation, int dim);
float *getExpected(const float *observation, int dim);
float *getObservation(const float *observation, int dim);
bool landmarkExist(const float *observation);
void addLandmark(float *mean, float *covariance, const float *observation, int *dim);
void EKF(int dim, float *mean, float *covariance, const float *control, const float *observation);


#endif /* EKF_H_ */
