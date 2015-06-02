/*
 * Inverse.h
 *
 *  Created on: Jun 1, 2015
 *      Author: edileuton
 */

#ifndef INVERSE_H_
#define INVERSE_H_


#define BLOCk 32

void fowardSubst(float *res, float *L, float *I, int n);
void backSubst(float *res, float *L, float *I, int n);
void choleskyDecomp(float *L, const float *A, int n);
void pMatInverse(float *A, float *L, float *I, int n);
void invert( int n);
void testInvert( int n);

#endif /* INVERSE_H_ */
