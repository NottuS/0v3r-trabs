/*
 * pEKF.h
 *
 *  Created on: Jun 7, 2015
 *      Author: edileuton
 */

#ifndef PEKF_H_
#define PEKF_H_

float *pOdometryError(const float *control, int dim);
float *pMoveUpadate(float *mean, const float *control);
float *pJacobianG(float teta, const float *control, int dim);
float *pObservationError(const float *observation, int dim);
float *pJacobianH(float *observed, const float *observation, int dim);
float *pGetExpected(const float *observation, int dim);
float *pGetObservation(const float *observation, int dim);
bool pLandmarkExist(const float *observation);
void pAddLandmark(float *mean, float *covariance, const float *observation, int *dim);
void EKF(int dim, float *mean, float *covariance, const float *control, const float *observation);


#endif /* PEKF_H_ */
