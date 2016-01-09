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
 * clObject.cpp
 *
 * Created on: 07/Jan/2015
 * Author: Peter Frank Perroni (pfperroni@inf.ufpr.br)
 */

#include "../../include/cl/core/clObject.hpp"

omp_lock_t clObject::mutex;
bool clObject::lock_started = false;
map<int, clObject*> clObject::clMap;

clObject::~clObject(){
	finalizeKernels();
	omp_destroy_lock(&mutex_self);
	if(lock_started){
		omp_destroy_lock(&mutex);
		lock_started = false;
	}
}

clObject::clObject(){
	// Generate the Unique Identifier for this object.
	UID = UniqueUIDGenerator::nextUniqueObjectUID();

	if(!lock_started){
		omp_init_lock(&mutex);
		lock_started = true;
	}
	omp_set_lock(&mutex);
	clMap[UID] = this;
	omp_unset_lock(&mutex);

	omp_init_lock(&mutex_self);

	cl_queue = NULL;
	kernelGroups = NULL;
}

void clObject::setCLQueue(clQueue *_cl_queue){
	omp_set_lock(&mutex_self);
	cl_queue = _cl_queue;
	omp_unset_lock(&mutex_self);
}

kernel_t* clObject::createKernelObject(char *kernelName){
	omp_set_lock(&mutex_self);
	kernel_t* kernel = new kernel_t;
	kernel->kernelName = kernelName;
	kernels.push_back(kernel);
	omp_unset_lock(&mutex_self);
	return kernel;
}

clObject* clObject::getclObject(int UID){
	return clMap[UID];
}

int clObject::getUID(){
	return UID;
}

clQueue* clObject::getCLQueue(){
	return cl_queue;
}

void clObject::startupKernels(char* clFileName, vector<char*> kernelNames){
	char *src = clFactory::readCl(clFileName);
	if(src == NULL){
		return;
	}
	omp_set_lock(&mutex_self);
	kernelGroups = clFactory::compileClSource(src);
	omp_unset_lock(&mutex_self);
	delete src;
	map<cl_context, vector<kernel_set*>* >::iterator iter;
	vector<kernel_set*>* kernelGroup;
	kernel_set *kernelList;
	// For each context.
	int i, j;
	for(iter=kernelGroups->begin(); iter != kernelGroups->end(); iter++){
		kernelGroup = iter->second;
		// For each device within the context.
		for(i=0; i < (int)kernelGroup->size(); i++){
			kernelList = (*kernelGroup)[i];
			for(j=0; j < (int)kernelNames.size(); j++){
				kernelList->kernelMap[kernelNames[j]] = createKernelObject(kernelNames[j]);
			}
			// A separate instance of every kernel for each device.
			omp_set_lock(&mutex_self);
			clFactory::addKernels(kernelList);
			omp_unset_lock(&mutex_self);
		}
	}
}

void clObject::finalizeKernels(){
	omp_set_lock(&mutex_self);

	for(int i=kernels.size()-1; i >= 0; i--){
		delete kernels[i];
		kernels.erase(kernels.end()-1);
	}

	if(kernelGroups == NULL) return;
	map<cl_context, vector<kernel_set*>* >::iterator iter;
	for(iter=kernelGroups->begin(); iter != kernelGroups->end(); iter++){
		delete iter->second;
	}
	delete kernelGroups;
	kernelGroups = NULL;

	omp_unset_lock(&mutex_self);
}


kernel_t* clObject::getFirstKernelInstance(char *kernelName){
	for(int i=0; i < (int)kernels.size(); i++){
		if(strcmp(kernels[i]->kernelName, kernelName) == 0){
			return kernels[i];
		}
	}
	return NULL;
}

vector<kernel_t*>* clObject::getAllKernelInstances(char *kernelName){
	vector<kernel_t*>* ret = NULL;
	for(int i=0; i < (int)kernels.size(); i++){
		if(strcmp(kernels[i]->kernelName, kernelName) == 0){
			if(ret == NULL){
				ret = new vector<kernel_t*>;
			}
			ret->push_back(kernels[i]);
		}
	}
	return ret;
}

kernel_t* clObject::getKernelInstanceByDevice(char *kernelName, cl_device_id device){
	kernel_t* ret = NULL;
	for(int i=0; i < (int)kernels.size(); i++){
		if(strcmp(kernels[i]->kernelName, kernelName) == 0 && kernels[i]->device == device){
			return kernels[i];
		}
	}
	return ret;
}

