#include <stdlib.h>
#include "cl/core/clFactory.hpp"
#include "ClGol.hpp"



int main(int argc, char *argv[]) {
	int n, m, cycles, printBoard;

	if(argc < 5){
		printf("usage: ./trab3 linhas colunas ciclos flag \n");
		printf("\t flag:0 - não  imprime resultado, 1 - para imprimir resultado.\n");
		exit(1);
	}

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
	ClGol *clGol =  new ClGol();
	clGol->setCLQueue(clFactory::getQueue());
    clGol->initializeCL();
    clGol->runGolkernels(n, m, cycles, printBoard);
	clFactory::shutdown();

	delete clGol;*/

	return 0;
}