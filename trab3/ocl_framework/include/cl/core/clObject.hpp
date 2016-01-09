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
 * clObject.hpp
 *
 * Created on: 06/Jan/2015
 * Author: Peter Frank Perroni (pfperroni@inf.ufpr.br)
 */

#ifndef _CLOBJECT_HPP_
#define _CLOBJECT_HPP_

#include <stdio.h>
#include <cstdlib>
#include <omp.h>
#include <map>
#include <limits.h>
#include "../../UniqueUIDGenerator.hpp"
#include "clFactory.hpp"

using namespace std;

/**
 * @brief Class representing a set of GPU kernels and configurations.
 */
class clObject {
private:
	static map<int, clObject*> clMap;
	static omp_lock_t mutex;
	static bool lock_started;

	int UID;
	omp_lock_t mutex_self;
	clQueue *cl_queue;
	map<cl_context, vector<kernel_set*>* >* kernelGroups;
	vector<kernel_t*> kernels;

public:
	clObject();
	virtual ~clObject();

	/**
	 * @brief Method containing all code (including kernels) to be executed on GPU device.
	 *
	 * Can be called multiple times, as needed.
	 *
	 * Abstract method, must be implemented by any class extending clObject.
	 */
	virtual void runKernel() = 0;

	/**
	 * @brief Method containing all initialization code to load data and configurations to GPU device.
	 *
	 * Can be called only once.
	 *
	 * Abstract method, must be implemented by any class extending clObject.
	 */
	virtual void initializeCL() = 0;

	/**
	 * @brief Method containing updates to GPU data and configurations.
	 *
	 * Can be called multiple times, as needed.
	 *
	 * Abstract method, must be implemented by any class extending clObject.
	 */
	virtual void loadToDevice() = 0;

	/**
	 * @brief Get the clObject with the UID provided.
	 *
	 * @param UID The clObject UID.
	 * @return The clObject with the UID provided.
	 */
	static clObject* getclObject(int UID);

	/**
	 * @brief Get the UID for this clObject.
	 *
	 * @return The UID for this clObject.
	 */
	int getUID();

	/**
	 * @brief Set a clQueue for this clObject.
	 *
	 * @param _cl_queue The clQueue to be used by this clObject.
	 */
	void setCLQueue(clQueue *_cl_queue);

	/**
	 * Get the clQueue used by this clObject.
	 *
	 * @return The clQueue used by this clObject.
	 */
	clQueue* getCLQueue();

	/**
	 * @brief Startup a list of kernels.
	 *
	 * @param clFileName The .cl file containing the kernels.
	 * @param kernelNames A vector containing the kernel names which are required to be loaded.
	 */
	void startupKernels(char* clFileName, vector<char*> kernelNames);

	/**
	 * @brief Finalize all kernels that were started by this clObject.
	 */
	void finalizeKernels();

	/**
	 * @brief Create and startup a kernel_t object, which is required by clObject to add the kernel to the device.
	 *
	 * @param kernelName The kernel name.
	 * @return The kernel_t object created.
	 */
	kernel_t* createKernelObject(char *kernelName);

	/**
	 * @brief Get the first kernel instance found with the given name.
	 *
	 * There might be more than one, since the kernels are duplicated
	 * when multipĺe devices are present in the context (one per device).
	 *
	 * @param kernelName The kernel name.
	 * @return The first kernel found.
	 */
	kernel_t* getFirstKernelInstance(char *kernelName);

	/**
	 * @brief Get the kernel instance in a specific device.
	 *
	 * @param kernelName The kernel name.
	 * @param device The device id.
	 * @return The kernel.
	 */
	kernel_t* getKernelInstanceByDevice(char *kernelName, cl_device_id device);

	/**
	 * @brief Get all kernel instances found with the given name.
	 *
	 * There might be more than one, since the kernels are duplicated
	 * when multipĺe devices are present in the context (one per device).
	 *
	 * @param kernelName The kernel name.
	 * @return A vector containing the list of kernels found.
	 */
	vector<kernel_t*>* getAllKernelInstances(char *kernelName);
};

#endif /* _CLOBJECT_HPP_ */
