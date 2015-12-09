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
 * TestGPU.cpp
 *
 * Created on: 14/Jan/2015
 * Author: Peter Frank Perroni <pfperroni@inf.ufpr.br>
 */

#include "TestGPU.hpp"

bool TestGPU::started = false;

TestGPU::~TestGPU(){
	omp_set_lock(&mutex_self);
	if(started){
		for(int i=0; i < (int)queues.size(); i++){
			clFactory::disposeQueue(queues[i]);
		}
		started = false;
	}
	omp_unset_lock(&mutex_self);
}

void TestGPU::initializeCL(){
	omp_set_lock(&mutex_self);
	if(!started){
		vector<char*> kernelNames;
		kernelNames.push_back((char*)"cl_reduce");
		startupKernels((char*)"test_gpu.cl", kernelNames);
		started = true;
	}
	omp_unset_lock(&mutex_self);
}

void TestGPU::runKernel(){
	char deviceName[1024];
	int i, sz = 10*1024;
	WORD chk_sum, arr[sz];
	for(i=0; i < sz; i++) arr[i] = ((WORD)1.0)/sz;

	cl_mem cl_values, cl_sum;
	clQueue *queue = getCLQueue();
	cl_command_queue command_queue = queue->getCommandQueue();
	cl_context context = queue->getContext();
	cl_device_id device = queue->getDevice();
	kernel_t* kernel = getKernelInstanceByDevice((char*)"cl_reduce", device);

	bool contains = false;
	for(int i=0; i < (int)queues.size(); i++){
		if(queues[i] == queue){
			contains = true;
			break;
		}
	}
	if(!contains) queues.push_back(queue);

	clFactory::getDeviceName(queue, deviceName, 1024);
	printf("Validating Device [%s - Ptr %p]...\n", deviceName, device);

	CREATE_BUFFER(context, CL_MEM_WRITE_ONLY | CL_MEM_COPY_HOST_PTR, sz * SIZEOF_WORD, arr, cl_values);
	CREATE_BUFFER(context, CL_MEM_READ_WRITE, REDUCTION_NBLOCKS * SIZEOF_WORD, NULL, cl_sum);

	CALL_KERNEL(command_queue, kernel, REDUCTION_NBLOCKS*REDUCTION_BLOCKSZ, REDUCTION_BLOCKSZ, 4,
		{sizeof(cl_mem), (void*)&cl_values},
		{sizeof(cl_mem), (void*)&cl_sum},
		{REDUCTION_BLOCKSZ * SIZEOF_WORD, NULL},
		{sizeof(cl_int), (void*)&sz}
	);
	SYNC_QUEUE(command_queue);
	FINAL_REDUCTIONS(command_queue,  cl_sum, chk_sum, 1);
	chk_sum = ((int)(chk_sum*1000000))/((WORD)1000000.0); // Remove the trailing decimals.

	if(chk_sum == ((WORD)1.0)){
		printf("Device Ok.\n");
	}
	else{
		printf("Validation failed on Device [calculated %f instead of 1.0].\n", chk_sum);
	}

	clReleaseMemObject(cl_values);
	clReleaseMemObject(cl_sum);
}

int TestGPU::numDevicesTested(){
	int nDevices = 0;
	clQueue *q[queues.size()];
	for(int j, i=0; i < (int)queues.size(); i++){
		for(j=0; j < nDevices; j++){
			if(queues[i]->getDevice() == q[j]->getDevice()){
				break;
			}
		}
		if(j == nDevices){
			q[nDevices++] = queues[i];
		}
	}
	return nDevices;
}
