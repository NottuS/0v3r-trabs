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
 * clFactory.hpp
 *
 * Created on: 15/Jul/2012
 * Author: Peter Frank Perroni (pfperroni@inf.ufpr.br)
 */

#ifndef _CLFACTORY_HPP_
#define _CLFACTORY_HPP_

#include <iostream>
#include <map>
#include <vector>
#include <omp.h>
#include <stdio.h>
#include <string.h>
#include <sstream>
#include <float.h>
#include <limits.h>

#include <CL/cl.h>
#include <CL/opencl.h>
#include <CL/cl_platform.h>
#include <CL/cl_ext.h>

#include "../../macros.hpp"
#include "Debug.hpp"

using namespace std;

//====================================================================
// STRUCT SECTION

/**
 * @brief This struct data type contains the OpenCl cl_kernel instance
 * and its mutex to synchronize the calls.
 */
typedef struct {
	char *kernelName;
	cl_kernel kernelInstance;
	cl_context context;
	cl_device_id device;
	cl_program program;
	omp_lock_t *mutex;
} kernel_t;

/**
 * @brief This struct data type contains the complete kernel configuration
 * and its mutex to synchronize the calls.
 */
typedef struct {
	map<char*, kernel_t*> kernelMap; // Key: Kernel Name
	cl_context context;
	cl_device_id device;
	cl_program program;
	omp_lock_t *mutex;
} kernel_set;

typedef struct {
	map<cl_device_id, vector<kernel_set*> > kernelSetMap;
	cl_context context;
	cl_device_id *devices;
	int n_devices;
	omp_lock_t *mutex;
} context_t;

typedef struct {
	size_t sz;
	void *ptr;
} clParam;


//====================================================================
// CLASS SECTION

/**
 * @brief This class contains an OpenCl environment ready for the use.
 */
class clQueue{
	cl_context context;
	cl_device_id device;
	cl_command_queue command_queue;

public:
	map<cl_program, vector<kernel_set*> > kernelsMap;

	clQueue(cl_context _context, cl_device_id _device, cl_command_queue _command_queue){
		context = _context;
		device = _device;
		command_queue = _command_queue;
	}

	~clQueue(){
		map<cl_program, vector<kernel_set*> >::iterator iter;
		vector<kernel_set*> kernels;
		for(iter=kernelsMap.begin(); iter != kernelsMap.end(); iter++){
			kernels = iter->second;
			for(int i=0; i < (int)kernels.size(); i++){
				delete kernels[i];
			}
		}
	}

	cl_context getContext() {return context;}

	cl_command_queue getCommandQueue() {return command_queue;}

	cl_device_id getDevice() {return device;}
};

/**
 * @brief The class is a factory for the OpenCl contexts, command queues and kernels.
 * It is also a pool manager for the queues created.
 *
 * It manages all the initialization and finalization procedures, providing a {@link #clQueue}
 * instance entirely ready for the use. Every queue is created on a distinct device (a context
 * here uses only a single device) and the devices are selected in round-robin sequence.
 */
class clFactory{
	static cl_context *context;
	static map<cl_context, context_t*> contexts;
	static map<cl_device_id, omp_lock_t*> devices;

	static omp_lock_t mutex, mutex_queue;
	static int n_contexts;
	static int n_queues;
	static int curr_queue;
	static int curr_context;
	static vector<clQueue*> queues;
	static bool kernelsAvailable;
	static bool started;
	static bool outOfOrderQueue;
	static int deviceType;

	/**
	 * @brief Initialize the GPU environment.
	 *
	 * It is responsible for reading and compiling the OpenCl source code,
	 * creating the contexts and one command queue per context, and initializing
	 * the GPU environment.
	 *
	 * @param _devices The list of devices to be used. If Null, all available devices will be used.
	 * @param clFilePath Path to the .cl file to be loaded (optional).
	 */
	static void initializeGPUs(vector<int> *_devices, char *clFilePath = NULL);

	/**
	 * @brief Create an OpenCL context on all available platforms using
	 * either a GPU or CPU, depending on what is available for this process.
	 *
	 * Only 1 single context is created per platform.
	 * For example: if there are 2 platforms available, one for each vendor (like NVidia and AMD),
	 * with every platform containing 2 distinct devices, every context will contain the
	 * 2 devices related to the same vendor.
	 *
	 * @param _devices The list of devices to be used. If Null, all available devices will be used.
	 */
	static void createContext(vector<int> *_devices);

	/**
	 * @brief Create one command queue for every device available for the context,
	 * adding every new queue to the pool.
	 *
	 * @param _context The context.
	 */
	static void  createCommandQueue(context_t *_context);

	/**
	 * @brief Create an OpenCL program from the OpenCl source file and compile it.
	 *
	 * @param _context The context where the program will be compiled.
	 * @param _device Any device from the context, only to read any compilation error.
	 * @param src A char pointer to the source code.
	 *
	 * @return The compiled program.
	 */
	static cl_program createProgram(cl_context _context, cl_device_id _device, char *src);

	/**
	 * @brief Create a OpenCl kernel for a program.
	 *
	 * @param program The OpenCl program instance.
	 * @param kernel_name The kernel name.
	 *
	 * @return The kernel instance.
	 */
	static cl_kernel createKernel(cl_program program, const char *kernel_name);

	/**
	 * @brief Create a new copy of the provided kernel_set.
	 *
	 * The new copy is basically a copy of pointers, so there's no duplication of data on device.
	 * The returned must be deleted manually later.
	 *
	 * @param kernels The kernel_set to be copied.
	 * @return The new copy.
	 */
	static kernel_set* clone_kernels(kernel_set *kernels);

	/**
	 * @brief Finalize the OpenCl instances.
	 */
	static void finalize_cl();

	/**
	 * @brief Read directly from the hardware the number of devices available for use.
	 *
	 * @return The number of devices available for use, without consulting the internal cached data.
	 */
	static int countDevicesFromHardware();

	/**
	 * @brief Get all devices available within a context.
	 *
	 * @param _context The context where to look for devices.
	 * @param context_data The context_t pointer where to set the data. This pointer must be previously allocated.
	 */
	static void getDevices(cl_context _context, context_t *context_data);

public:
	static void startup() {startup(NULL);}

	/**
	 * @brief Prepare this factory for the use, filtering only the devices specified by the vector.
	 *
	 * It's mandatory to call this method Before calling any other method of this factory.
	 *
	 * @param _devices The list of devices to be used (optional).
	 * @param clFilePath The path for the .cl file (optional).
	 */
	static void startup(vector<int> *_devices, char *clFilePath = NULL);

	/**
	 * @brief Finalize this factory.
	 *
	 * It's mandatory to call this method after all the work is done!
	 */
	static void shutdown();

	/**
	 * @brief Read the OpenCl source code.
	 *
	 * @return A char pointer to the source code. This pointer must be deleted later, after the use.
	 */
	static char* readCl(char *cFilePath);

	/**
	 * @brief Compile the .cl file into every context available.
	 *
	 * @param clSrc The .cl source file.
	 * @return A map containing the kernel_set objects created (one kernel_set per device, once vector per context).
	 */
	static map<cl_context, vector<kernel_set*>* >* compileClSource(char *clSrc);

	/**
	 * @brief Compile the .cl file into the context specified.
	 *
	 * @param clSrc The .cl source file.
	 * @param _context The context where to compile the .cl file.
	 * @return A vector containing the kernel_set objects created (one kernel_set per device).
	 */
	static vector<kernel_set*>* compileClSourceIntoContext(char *clSrc, context_t *_context);

	/**
	 * @brief Initialize the OpenCl instances into a given context.
	 * The instances that are initialized are the kernel instances.
	 * Also their mutexes are initialized at this method.
	 *
	 * @param kernels A kernel_set pointer containing the configuration to load a list of kernels under a cl program.
	 */
	static void addKernels(kernel_set *kernels);

	/**
	 * @brief Get the next command queue available for the use.
	 * If none is available on the pool, just create one on the next
	 * available context.
	 *
	 * @return The {@link #clQueue} instance containing the command queue
	 * and all its configuration.
	 */
	static clQueue* getQueue();

	/**
	 * @brief Dispose the clQueue instance, putting it back to the pool.
	 *
	 * @param clqueue The clQueue instance to be released.
	 */
	static void disposeQueue(clQueue *clqueue);

	/**
	 * @brief Get the device name from the queue provided.
	 *
	 * @param queue The queue.
	 * @param deviceName A pointer where to put the device name.
	 * @param nameSize The memory size where the pointer points to.
	 */
	static void getDeviceName(clQueue* queue, char *deviceName, int nameSize);

	/**
	 * @brief Return the number of contexts available for use.
	 *
	 * @return The number of contexts available for use.
	 */
	static int getNumAvailableContexts();

	/**
	 * @brief Return the number of devices available for use.
	 *
	 * @return The number of devices available for use.
	 */
	static int getNumAvailableDevices();

	/**
	 * @brief Set the device type where OpenCL will execute the calculations.
	 * Default is RUN_ON_GPU.
	 *
	 * Any change to the default value MUST be done Before calling startup().
	 *
	 * @param type Either RUN_ON_GPU OR RUN_ON_CPU.
	 */
	static void setDeviceType(int type){deviceType = type;}

	/**
	 * @brief Set the queues to be created as CL_QUEUE_OUT_OF_ORDER_EXEC_MODE_ENABLE.
	 * Default is true.
	 *
	 * Any change to the default value MUST be done Before calling startup().
	 *
	 * @param outOfOrder Boolean value enabling/disabling out-of-order queues.
	 */
	static void setOutOfOrderQueue(bool outOfOrder){outOfOrderQueue = outOfOrder;}
};


#endif /* _CLFACTORY_HPP_ */
