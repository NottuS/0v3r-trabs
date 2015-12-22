#include <sys/time.h>
#include <stdlib.h>
#include "cl/core/clFactory.hpp"
#include "ClGol.hpp"



int main(int argc, char *argv[]) {
	int n, m, cycles, printBoard;

	n = atoi(argv[1]);
	m = atoi(argv[2]);
	cycles = atoi(argv[3]);
	cycles = atoi(argv[4]);
	ClGol *clGol =  new ClGol();

	clFactory::setDeviceType(RUN_ON_GPU);
	clFactory::startup();
	clGol->setCLQueue(clFactory::getQueue());
    clGol->initializeCL();
    clGol->runGolkernels(n, m, cycles, printBoard);
	clFactory::shutdown();

	/*clFactory::setDeviceType(RUN_ON_CPU);
	clFactory::startup();
	clGol->setCLQueue(clFactory::getQueue());
    clGol->initializeCL();
    clGol->runGolkernels(n, m, cycles);
	clFactory::shutdown();*/

	delete clGol;

	return 0;
}