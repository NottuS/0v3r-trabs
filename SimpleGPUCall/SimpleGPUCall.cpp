/*
 * Copyright (c) 2015, Peter Frank Perroni
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */

/**
 * SimpleGPUCall.cpp
 *
 * Created on: 07/Jan/2015
 * Author: Peter Frank Perroni <pfperroni@inf.ufpr.br>
 */

#include "cl/core/clFactory.hpp"
#include "TestGPU.hpp"

void testDevices(int nTests);

int main(int argc, char *argv[]) {
	clFactory::startup();
	testDevices(10);
	clFactory::shutdown();

	return 0;
}

double timestamp(){
	struct timeval tp;
	gettimeofday(&tp, NULL);
	return (double)(tp.tv sec +
	tp.tv usec / 1000000.0);
}

void testDevices(int nTests){
	TestGPU *testGPU = new TestGPU();
	for(int i=0; i < nTests; i++){
		testGPU->setCLQueue(clFactory::getQueue());
		testGPU->initializeCL();
		testGPU->runKernel();
	}
	printf("Number of queues tested: %i\n", testGPU->numQueuesTested());
	printf("Number of devices tested: %i\n", testGPU->numDevicesTested());
	delete testGPU;
}
