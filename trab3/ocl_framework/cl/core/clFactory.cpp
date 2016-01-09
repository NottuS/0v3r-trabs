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
 * clFactory.cpp
 *
 * Created on: 16/Jul/2012
 * Author: Peter Frank Perroni (pfperroni@inf.ufpr.br)
 */

#include "../../include/cl/core/clFactory.hpp"

cl_context *clFactory::context;
omp_lock_t clFactory::mutex;
omp_lock_t clFactory::mutex_queue;
int clFactory::n_contexts;
int clFactory::n_queues;
int clFactory::curr_queue;
int clFactory::curr_context;
vector<clQueue*> clFactory::queues;
map<cl_context, context_t*> clFactory::contexts;
map<cl_device_id, omp_lock_t*> clFactory::devices;
bool clFactory::kernelsAvailable = false;
bool clFactory::started = false;
bool clFactory::outOfOrderQueue = true;
int clFactory::deviceType = RUN_ON_GPU;

void clFactory::initializeGPUs(vector<int> *_devices, char *cFilePath){
	omp_init_lock(&mutex);
	omp_init_lock(&mutex_queue);

	// Create the OpenCL contexts.
	createContext(_devices);

	// Read the .cl file.
	char *src = readCl(cFilePath);

	// Create the command queues.
	context_t *_context;
	n_queues = 0;
	map<cl_context, context_t*>::iterator iter;
	for(iter=contexts.begin(); iter != contexts.end(); iter++){
		_context = iter->second;
		createCommandQueue(_context);
		if((int)queues.size() == n_queues){
			continue;
		}

		compileClSourceIntoContext(src, _context);
		n_queues = queues.size();
	}
	delete src;

	if (n_queues == 0) {
		for(int i=0; i < n_contexts; i++){
			clReleaseContext(context[i]);
		}
		for(iter=contexts.begin(); iter != contexts.end(); iter++){
			delete iter->second->devices;
			delete iter->second;
		}
		delete context;
		printf("No device found that matches the provided parameters!\n");
		exit(1);
	}

	// Print the device information.
	// Note that only 1 command queue is created for every device during startup.
	char cBuffer[1024];
	for(int i=0; i < n_queues; i++){
		getDeviceName(queues[i], cBuffer, 1024);
		printf("Queues created on Device [%s]\n", cBuffer);
	}

	// Retain one additional reference for the first Context and for its command queue,
	// to avoid finalization errors.
	clRetainContext(queues[0]->getContext());
	clRetainCommandQueue(queues[0]->getCommandQueue());

	started = true;
}


void clFactory::createContext(vector<int> *_devices) {
	cl_int errNum;
	cl_uint numPlatforms;
	cl_platform_id platformIds[CL_PLATFORMS_TO_TRY];

	// First, select an OpenCL platform to run on.
	// This code will try up to CL_PLATFORMS_TO_TRY available platforms.
	errNum = clGetPlatformIDs(CL_PLATFORMS_TO_TRY, platformIds, &numPlatforms);
	if (errNum != CL_SUCCESS || numPlatforms <= 0) {
		std::cerr << "Failed to find any OpenCL platforms." << std::endl;
		exit(1);
	}
	if(numPlatforms > CL_PLATFORMS_TO_TRY) numPlatforms = CL_PLATFORMS_TO_TRY;

	// Create OpenCL contexts on the platform for the primary device type (GPU or CPU).
	errNum = CL_DEVICE_NOT_FOUND;
	cl_context _cl_context[numPlatforms];
	int n_found_contexts = 0;
	int i;
	for(i=0; i < (int)numPlatforms; i++){
		cl_context_properties contextProperties[] = { CL_CONTEXT_PLATFORM, (cl_context_properties) platformIds[i], 0 };
		cl_context _context = clCreateContextFromType(contextProperties, deviceType, NULL, NULL, &errNum);
		if(errNum == CL_SUCCESS){
			_cl_context[n_found_contexts++] = _context;
		}
	}
	printf(">>numPlatforms:%i, n_found_contexts:%i\n", numPlatforms, n_found_contexts);
	/* DISABLED // If all primary devices have failed, try to create it on the secondary device type.
	if(n_found_contexts == 0){
		for(i=0; i < (int)numPlatforms && errNum != CL_SUCCESS; i++){
			cl_context_properties contextProperties[] = { CL_CONTEXT_PLATFORM, (cl_context_properties) platformIds[i], 0 };
			cl_context _context = clCreateContextFromType(contextProperties, SECONDARY_DEVICE_TYPE, NULL, NULL, &errNum);
			if(errNum == CL_SUCCESS){
				_cl_context[n_found_contexts++] = _context;
			}
		}
	}*/
	if (n_found_contexts == 0) {
		std::cerr << "Failed to create an OpenCL GPU or CPU context." << std::endl;
		exit(1);
	}
	// Determine the devices that will compose each context.
	context_t *_context;
	int j, k, new_count, curr_device = 1;
	for(i=0; i < n_found_contexts; i++){
		_context = new context_t;
		getDevices(_cl_context[i], _context);
		// Only store the context if it contains at least 1 device available.
		if(_context->n_devices > 0){
			vector<cl_device_id> saved_devices;
			// If filter the devices.
			if(_devices != NULL){
				// For all devices within this context.
				for(j=0; j < _context->n_devices; j++, curr_device++){
					// For all devices to be filtered.
					for(k=0; k < (int)_devices->size(); k++){
						if(curr_device == _devices->at(k)){
							saved_devices.push_back(_context->devices[j]);
						}
					}
				}
				new_count = saved_devices.size();
				// If any device within this context shall be used.
				if(new_count > 0){
					// If any device was discarded, rebuild the pointer.
					if(new_count != _context->n_devices){
						_context->n_devices = new_count;
						delete _context->devices;
						_context->devices = new cl_device_id[_context->n_devices];
						for(j=0; j < _context->n_devices; j++){
							_context->devices[j] = saved_devices[j];
						}
					}
				}
				else{
					// Ignore this context.
					delete _context->devices;
					delete _context;
					continue;
				}
			}
			n_contexts++;
			contexts[_cl_context[i]] = _context;
		}
		else{
			delete _context;
		}
	}
	// Store the cl_context's separately for easier manipulation.
	context = new cl_context[n_contexts];
	map<cl_context, context_t*>::iterator iter;
	for(i=0, iter=contexts.begin(); iter != contexts.end(); iter++, i++){
		context[i] = iter->first;
	}
}

void clFactory::createCommandQueue(context_t *_context) {
	cl_int errNum;
	omp_lock_t *_mutex;
	cl_command_queue commandQueue = NULL;
	cl_command_queue_properties prop = (outOfOrderQueue) ? CL_QUEUE_OUT_OF_ORDER_EXEC_MODE_ENABLE : 0;
#ifdef _PROFILING_
	prop |= CL_QUEUE_PROFILING_ENABLE;
#endif

	for(int i=0; i < _context->n_devices; i++){
		commandQueue = clCreateCommandQueue(_context->context, _context->devices[i], prop, &errNum);
		if (errNum != CL_SUCCESS) {
			std::cerr << "Failed to create command queue, errno=" << errNum << "." << endl;
		}
		else if (commandQueue != NULL) {
			// Add the command queue to the pool of queues.
			queues.push_back(new clQueue(_context->context, _context->devices[i], commandQueue));
			// Create one single mutex per device.
			if((_mutex = devices[_context->devices[i]]) == NULL){
				_mutex = new omp_lock_t;
				omp_init_lock(_mutex);
				devices[_context->devices[i]] = _mutex;
			}
		}
	}
}

char* clFactory::readCl(char *cFilePath){
	FILE *fp;
	if(cFilePath == NULL) return NULL;
	if(!(fp = fopen(cFilePath, "r"))){
		cerr << "Fail reading cl file [" << cFilePath << "]: File Not Found!" << std::endl;
		return NULL;
	}
	fseek(fp, 0, SEEK_END);
	int sz1 = ftell(fp);
	rewind(fp);
	char *src = new char[sz1+1];
	int sz2 = fread(src, 1, sz1, fp);
	fclose(fp);
	if (sz1 != sz2) {
		cerr << "Fail reading cl file [" << cFilePath << "]: Cannot read the entire file!" << std::endl;
		delete src;
		return NULL;
	}
	src[sz1] = '\0';
	return src;
}

cl_program clFactory::createProgram(cl_context _context, cl_device_id _device, char *src) {
	cl_int errNum;
	// Create a cl_program into the context, meaning that every device present inside the context will have the compiled kernels.
	cl_program program = clCreateProgramWithSource(_context, 1, (const char**) &src, NULL, &errNum);
	if (program == NULL || errNum != CL_SUCCESS) {
		cerr << "Failed to create CL program from source (" << errNum << ")." << endl;
		return NULL;
	}

	//Passing parameters to the cl file.
	ostringstream oss;
#ifdef _DOUBLE_WORD_
	oss << "-D_DOUBLE_WORD_ -DWORD=double -DFFT_TYPE=double2 ";
#else
	oss << "-DWORD=float -DFFT_TYPE=float2 ";
#endif
	oss << "-DREDUCTION_BLOCKSZ=" << REDUCTION_BLOCKSZ << " -DREDUCTION_NBLOCKS=" << REDUCTION_NBLOCKS
			<< " -DWORD_MAX=" << WORD_MAX << " -DINT_MAX=" << INT_MAX << " -DRAND_MAX=" << RAND_MAX;

	// Build the program inside the devices under the context.
	errNum = clBuildProgram(program, 0, NULL, oss.str().c_str(), NULL, NULL);
	if (errNum != CL_SUCCESS) {
		size_t ret_val_size;
		// Get the error message from inside the device.
		//----------------------------------------------
		// Check the error message size.
		clGetProgramBuildInfo(program, _device, CL_PROGRAM_BUILD_LOG, 0, NULL, &ret_val_size);
		// Alocates only the memory needed.
		char *build_log = (char*)malloc(ret_val_size+1);
		// Read the error text.
		clGetProgramBuildInfo(program, _device, CL_PROGRAM_BUILD_LOG, ret_val_size, build_log, NULL);

		// The \0 is not mandatory in the OpenCl specification, thus we add it here for safety.
		build_log[ret_val_size] = '\0';

		printf("BUILD LOG: \n%s\n", build_log);
		free(build_log);

		return NULL;
	}
	return program;
}

cl_kernel clFactory::createKernel(cl_program program, const char *kernel_name) {
	cl_kernel kernel = clCreateKernel(program, kernel_name, NULL);
	if (kernel == NULL){
		std::cerr << "Failed to create kernel" << kernel_name << endl;
		finalize_cl();
		exit(1);
	}
	return kernel;
}

void clFactory::addKernels(kernel_set *kernels) {
	kernel_t *kernel;
	map<char*, kernel_t*>::iterator iter;
	for(iter=kernels->kernelMap.begin(); iter != kernels->kernelMap.end(); iter++){
		kernel = iter->second;
		kernel->kernelInstance = createKernel(kernels->program, kernel->kernelName);
		kernel->context = kernels->context;
		kernel->device = kernels->device;
		kernel->program = kernels->program;
		kernel->mutex = kernels->mutex;
	}

	// Store the kernels separately so they can be finalized at the end.
	cl_device_id device = kernels->device;
//	contexts[kernels->context]->kernelSetMap[device].push_back(kernels);

	// Set the correct kernels and mutex for all queues.
	for(int i=0; i < (int)queues.size(); i++){
		if(queues[i]->getDevice() == device){
			queues[i]->kernelsMap[kernels->program].push_back(clone_kernels(kernels));
		}
	}

	kernelsAvailable = true;
}

kernel_set* clFactory::clone_kernels(kernel_set *kernels) {
	kernel_set *new_kernels = new kernel_set;
	new_kernels->mutex = kernels->mutex;
	new_kernels->context = kernels->context;
	new_kernels->device = kernels->device;
	new_kernels->program = kernels->program;

	kernel_t *kernel;
	map<char*, kernel_t*>::iterator iter;
	for(iter=kernels->kernelMap.begin(); iter != kernels->kernelMap.end(); iter++){
		kernel = iter->second;
		new_kernels->kernelMap[kernel->kernelName] = kernel;
	}

	return new_kernels;
}

map<cl_context, vector<kernel_set*>* >* clFactory::compileClSource(char *clSrc){
	if(contexts.size() == 0) return NULL;
	map<cl_context, vector<kernel_set*>* >* ret = new map<cl_context, vector<kernel_set*>* >;
	map<cl_context, context_t*>::iterator iter;
	for(iter=contexts.begin(); iter != contexts.end(); iter++){
		(*ret)[iter->second->context] = compileClSourceIntoContext(clSrc, iter->second);
	}
	return ret;
}

vector<kernel_set*>* clFactory::compileClSourceIntoContext(char *clSrc, context_t *_context_t){
	if(clSrc == NULL || _context_t->n_devices == 0) return NULL;

	cl_program program;
	kernel_set *kernels;
	cl_context _context;
	cl_device_id device;
	vector<kernel_set*> *ret = new vector<kernel_set*>;
	for(int i=0; i < _context_t->n_devices; i++){
		// Create the OpenCL program from .cl kernel source for every device available inside the context.
		_context = _context_t->context;
		device = _context_t->devices[i];
		if ((program=createProgram(_context, device, clSrc)) == NULL) {
			delete clSrc;
			finalize_cl();
			exit(1);
		}
		kernels = new kernel_set;
		kernels->context = _context;
		kernels->device = device;
		kernels->program = program;
		kernels->mutex = devices[device];  // Using 1 single mutex per device.
		ret->push_back(kernels);

		// Add the program to the list within the context.
		contexts[_context]->kernelSetMap[device].push_back(kernels);
	}
	return ret;
}

void clFactory::finalize_cl() {
	int i;
	for(i=0; i < n_queues; i++){
		clReleaseCommandQueue(queues[i]->getCommandQueue());
		delete queues[i];
	}

	context_t *_context;
	map<cl_context, context_t*>::iterator iter1;
	map<cl_device_id, vector<kernel_set*> >::iterator iter2;
	map<char*, kernel_t*>::iterator iter3;
	map<cl_device_id, vector<kernel_set*> > kernelSetMap;
	vector<kernel_set*> kernelVec;
	kernel_set *kernels;
	cl_program program;
	for(iter1=contexts.begin(); iter1 != contexts.end(); iter1++){
		_context = iter1->second;

		// Release the kernels and respective programs.
		kernelSetMap = _context->kernelSetMap;
		for(iter2=kernelSetMap.begin(); iter2 != kernelSetMap.end(); iter2++){
			kernelVec = iter2->second;
			for(int i=0; i < (int)kernelVec.size(); i++){
				kernels = kernelVec[i];
				program = kernels->program;
				for(iter3=kernels->kernelMap.begin(); iter3 != kernels->kernelMap.end(); iter3++){
					clReleaseKernel(iter3->second->kernelInstance);
				}
				clReleaseProgram(program);
				delete kernels;
			}
		}

		delete _context->devices;

		// Release the context.
		clReleaseContext(_context->context);

		delete _context;
	}

	delete context;

	omp_lock_t *_mutex;
	map<cl_device_id, omp_lock_t*>::iterator iter4;
	for(iter4=devices.begin(); iter4 != devices.end(); iter4++){
		_mutex = iter4->second;
		omp_destroy_lock(_mutex);
		delete _mutex;
	}

	omp_destroy_lock(&mutex_queue);

	started = false;
}

void clFactory::startup(vector<int> *_devices, char *clFilePath){
	if(started) return;

	omp_set_lock(&mutex);
	n_queues = 0;
	n_contexts = 0;
	initializeGPUs(_devices, clFilePath);
	curr_queue = -1;
	curr_context = -1;
	omp_unset_lock(&mutex);
}

void clFactory::shutdown(){
	if(!started) return;

	omp_set_lock(&mutex);
	finalize_cl();
	omp_unset_lock(&mutex);

	omp_destroy_lock(&mutex);
}

clQueue* clFactory::getQueue(){
	// If the contexts were not yet initialized.
	if(n_contexts == 0){
		startup(); // No device filter.
		if(n_contexts == 0){
			return NULL;
		}
	}
	int _curr_queue;

	omp_set_lock(&mutex_queue);
	// If it's necessary to create more queues.
	if(++curr_queue == n_queues){
		do{
			// Round-robin over the contexts.
			if(++curr_context == n_contexts){
				curr_context = 0;
			}
			// Create one additional queue for every device present in the next context.
			createCommandQueue(contexts[context[curr_context]]);
		}while(n_queues == (int)queues.size());
		if(kernelsAvailable){
			// Set the correct kernels for the newly created queues.
			map<cl_device_id, vector<kernel_set*> > kernelSetMap = contexts[context[curr_context]]->kernelSetMap;
			vector<kernel_set*> kernelVec;
			// For every new queue created (one per each devices within the context).
			for(int j=n_queues; j < (int)queues.size(); j++){
				kernelVec = kernelSetMap[queues[j]->getDevice()];
				// For each kernel_set.
				for(int i=0; i < (int)kernelVec.size(); i++){
					// Set the correct mutex based on the device where the queue was created.
					queues[j]->kernelsMap[kernelVec[i]->program].push_back(clone_kernels(kernelVec[i]));
				}
			}
		}
		n_queues = queues.size();
	}
	_curr_queue = curr_queue;
	omp_unset_lock(&mutex_queue);

	return queues[_curr_queue];
}

void clFactory::disposeQueue(clQueue *clqueue){
	omp_set_lock(&mutex_queue);
	for(int i=0; i < n_queues; ++i) {
		if(queues[i] == clqueue){
			// Move the queue to the last position in the pool, recycling it.
			queues.erase(queues.begin() + i);
			queues.push_back(clqueue);
			curr_queue--;
			break;
		}
	}
	omp_unset_lock(&mutex_queue);
}

void clFactory::getDeviceName(clQueue* queue, char *deviceName, int nameSize){
	omp_set_lock(&mutex_queue);
	size_t sz;
	clGetDeviceInfo(queue->getDevice(), CL_DEVICE_NAME, nameSize, deviceName, &sz);
	deviceName[sz] = '\0';
	omp_unset_lock(&mutex_queue);
}

int clFactory::getNumAvailableContexts(){
	return n_contexts;
}

int clFactory::getNumAvailableDevices(){
	return devices.size();
}

int clFactory::countDevicesFromHardware() {
	cl_int errNum;
	size_t deviceBufferSize = -1;
	int n_devices = 0;

	for(int i=0; i < n_contexts; i++){
		// First get the size of the devices buffer
		errNum = clGetContextInfo(context[i], CL_CONTEXT_DEVICES, 0, NULL, &deviceBufferSize);
		if (errNum == CL_SUCCESS) {
			n_devices += deviceBufferSize / sizeof(cl_device_id);
		}
	}

	return n_devices;
}

void clFactory::getDevices(cl_context _context, context_t *context_data){
	context_data->context = _context;

	cl_int errNum;
	size_t deviceBufferSize = -1;

	// First get the size of the devices buffer
	errNum = clGetContextInfo(_context, CL_CONTEXT_DEVICES, 0, NULL, &deviceBufferSize);
	if (errNum != CL_SUCCESS || deviceBufferSize <= 0) {
		context_data->n_devices = 0;
		return;
	}

	// Allocate memory for the devices buffer
	context_data->n_devices = deviceBufferSize / sizeof(cl_device_id);
	context_data->devices = new cl_device_id[context_data->n_devices];
	// Get the device IDs available in the context (notice that such ids are more like internal pointers but not sequential numbers).
	errNum = clGetContextInfo(_context, CL_CONTEXT_DEVICES, deviceBufferSize, context_data->devices, NULL);
	if (errNum != CL_SUCCESS) {
		context_data->n_devices = 0;
		delete context_data->devices;
	}
}
