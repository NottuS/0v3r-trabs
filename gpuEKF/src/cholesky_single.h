/*
 * cholesky_single.h
 *
 *  Created on: Apr 21, 2015
 *      Author: edileuton
 */

#ifndef CHOLESKY_SINGLE_H_
#define CHOLESKY_SINGLE_H_

static inline float quiet_float_sqrt (float x);
int gsl_linalg_float_cholesky_decomp (gsl_matrix_float * A);
int gsl_linalg_float_cholesky_invert(gsl_matrix_float * LLT);

#endif /* CHOLESKY_SINGLE_H_ */
