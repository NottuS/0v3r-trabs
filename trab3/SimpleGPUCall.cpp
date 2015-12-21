#include <sys/time.h>
#include <stdlib.h>
#include "cl/core/clFactory.hpp"
#include "ClGol.hpp"



int main(int argc, char *argv[]) {
	int n, m, cycles;

	n = atoi(argv[1]);
	m = atoi(argv[2]);
	cycles = atoi(argv[3]);
	ClGol *clGol =  new ClGol();

	clFactory::setDeviceType(RUN_ON_GPU);
	clFactory::startup();
	clGol->setCLQueue(clFactory::getQueue());
    clGol->initializeCL();
    clGol->runGolkernels(n, m, cycles);
	clFactory::shutdown();

	clFactory::setDeviceType(RUN_ON_CPU);
	clFactory::startup();
	clGol->setCLQueue(clFactory::getQueue());
    clGol->initializeCL();
    clGol->runGolkernels(n, m, cycles);
	clFactory::shutdown();

	delete clGol;

	return 0;
}

double timestamp(){
	struct timeval tp;
	gettimeofday(&tp, NULL);
	return (double)(tp.tv_sec +
	tp.tv_usec / 1000000.0);
}