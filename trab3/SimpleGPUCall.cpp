#include <stdlib.h>
#include "cl/core/clFactory.hpp"
#include "ClGol.hpp"



int main(int argc, char *argv[]) {
	int n, m, cycles, printBoard;

	n = atoi(argv[1]);
	m = atoi(argv[2]);
	cycles = atoi(argv[3]);
	printBoard = atoi(argv[4]);
	clFactory::setDeviceType(RUN_ON_GPU);
	clFactory::startup();
	ClGol *clGol =  new ClGol();


	clGol->setCLQueue(clFactory::getQueue());
    clGol->initializeCL();
    clGol->runGolkernels(n, m, cycles, printBoard);
	clFactory::shutdown();

	delete clGol;

	/*clFactory::setDeviceType(RUN_ON_CPU);
	clFactory::startup();
	clGol =  new ClGol();
	clGol->setCLQueue(clFactory::getQueue());
    clGol->initializeCL();
    clGol->runGolkernels(n, m, cycles, printBoard);
	clFactory::shutdown();

	delete clGol;*/

	return 0;
}