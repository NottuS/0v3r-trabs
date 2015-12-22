
#ifndef _CLGOL_HPP_
#define _CLGOL_HPP_

#include "cl/core/clObject.hpp"
#include <sys/time.h>

#define BLOCKSIZE 256

class ClGol : public clObject{
private:
	static bool started;

	vector<clQueue*> queues;
	omp_lock_t mutex_self;

public:
	~ClGol();
	void initializeCL();
	void runGolkernels(unsigned int n, unsigned int m, unsigned int cycles, int printBoard);
	void print_matrix(int *matrix, int n, int m);
	double timestamp();
	void runKernel();
	void loadToDevice(){}
};

#endif /* _CLGOL_HPP_ */
