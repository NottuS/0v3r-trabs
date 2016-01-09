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
 * macros.hpp
 *
 * Created on: 13/Jan/2015
 * Author: Peter Frank Perroni (pfperroni@inf.ufpr.br)
 */

#ifndef _MACROS_HPP_
#define _MACROS_HPP_

#include "config.hpp"

#define ACTION_IGNORED 0x0
#define ACTION_CHECKED 0x1
#define ACTION_TAKEN 0x2
#define HAS_IGNORED_ACTION(_action_) (_action_&ACTION_IGNORED)
#define HAS_ONLY_IGNORED_ACTION(_action_) (_action_==ACTION_IGNORED)
#define HAS_CHECKED_ACTION(_action_) (_action_&ACTION_CHECKED)
#define HAS_ONLY_CHECKED_ACTION(_action_) (_action_==ACTION_CHECKED)
#define HAS_TOOK_ACTION(_action_) (_action_&ACTION_TAKEN)

// Automatic adjusts of data types.
#ifdef _DOUBLE_WORD_
#define CL_WORD cl_double
#define WORD double
#define WORD_MAX DBL_MAX
#define COMPLEX_TYPE double2
#else
#define WORD float
#define CL_WORD cl_float
#define WORD_MAX FLT_MAX
#define COMPLEX_TYPE float2
#endif

#define SIZEOF_WORD sizeof(WORD)
#define SIZEOF_COMPLEXTYPE sizeof(COMPLEX_TYPE)

#define STRINGIFY(text) #text

#define OCL_CODE(...) #__VA_ARGS__


#ifdef _PROFILING_
#define PROFILING(event) { \
	CHECK_CL_STATE( clWaitForEvents(1, &event); ) \
	cl_ulong _start = 0, _end = 0, _latest; \
	CHECK_CL_STATE( clGetEventProfilingInfo(event, CL_PROFILING_COMMAND_START, sizeof(cl_ulong), &_start, NULL); ) \
	CHECK_CL_STATE( clGetEventProfilingInfo(event, CL_PROFILING_COMMAND_END, sizeof(cl_ulong), &_end, NULL); ) \
	/* Solve an OpenCL bug which switches the start value by the end value. */ \
	if(_start > _end){ _latest = _start; _start = _end; _end = _latest; } \
	/* There is a bug that makes OpenCL Not always collect one of the profiling information */ \
	/* (data won't be available even after retrying the read on the same missing property). */ \
	if(_start > 0 && _end > 0) Profiling::increaseProcessingTime((_end - _start) * 1.0e-6f); \
	clReleaseEvent(event); \
/*cout << "###" << __FILE__ << ": " << __LINE__ << " = " << (_end - _start) * 1.0e-6f << " (" << _start << "," << _end << ")" << endl;*/ \
}
#else
#define PROFILING(event) clReleaseEvent(event)
#endif

#define clMemcpyDeviceToHost(command_queue, dest, src, size) { \
	cl_event _event; \
	CHECK_CL_STATE( clEnqueueReadBuffer(command_queue, src, CL_TRUE, 0, size, dest, 0, NULL, &_event) ) \
	PROFILING(_event); \
}

#define clMemcpyDeviceToHostOffset(command_queue, dest, src, src_offset, size) { \
	cl_event _event; \
	CHECK_CL_STATE( clEnqueueReadBuffer(command_queue, src, CL_TRUE, src_offset, size, dest, 0, NULL, &_event) ) \
	PROFILING(_event); \
}

#define clMemcpyHostToDevice(command_queue, dest, src, size) { \
	cl_event _event; \
	CHECK_CL_STATE( clEnqueueWriteBuffer(command_queue, dest, CL_TRUE, 0, size, src, 0, NULL, &_event) ) \
	PROFILING(_event); \
}

#define clMemcpyHostToDeviceOffset(command_queue, dest, dest_offset, src, size) { \
	cl_event _event; \
	CHECK_CL_STATE( clEnqueueWriteBuffer(command_queue, dest, CL_TRUE, dest_offset, size, src, 0, NULL, &_event) ) \
	PROFILING(_event); \
}

#define clMemcpyDeviceToDevice(command_queue, dest, src, size, block) { \
	cl_event _event; \
	CHECK_CL_STATE( clEnqueueCopyBuffer(command_queue, src, dest, 0, 0, size, 0, NULL, &_event) ) \
	if(block) clWaitForEvents(1, &_event); \
	PROFILING(_event); \
}

#define clMemcpyDeviceToDeviceOffset(command_queue, dest, dest_offset, src, src_offset, size, block) { \
	cl_event _event; \
	CHECK_CL_STATE( clEnqueueCopyBuffer(command_queue, src, dest, (size_t)(src_offset), (size_t)(dest_offset), size, 0, NULL, &_event) ) \
	if(block) clWaitForEvents(1, &_event); \
	PROFILING(_event); \
}

#define FINAL_REDUCE(command_queue, cl_src, result) { \
	WORD _sum[REDUCTION_NBLOCKS]; \
	clMemcpyDeviceToHost(command_queue, _sum, cl_src, REDUCTION_NBLOCKS * SIZEOF_WORD); \
	result = 0; \
	for(int _i=0; _i < REDUCTION_NBLOCKS; _i++){result += _sum[_i];} \
}

#define FINAL_REDUCTIONS(command_queue, cl_src, result, n_reductions) { \
	int sz = n_reductions * REDUCTION_NBLOCKS; \
	WORD _sum[sz]; \
	clMemcpyDeviceToHost(command_queue, _sum, cl_src, sz * SIZEOF_WORD); \
	result = 0; \
	for(int _i=0; _i < sz; _i++){result += _sum[_i];} \
}

#define FINAL_REDUCE_COMPLEX(command_queue, cl_src, result) { \
	WORD _sum[REDUCTION_NBLOCKS]; \
	clMemcpyDeviceToHost(command_queue, _sum, cl_src, REDUCTION_NBLOCKS * SIZEOF_WORD); \
	result.x = 0; result.y = 0; \
	for(int _i=0; _i < REDUCTION_NBLOCKS; _i++){result.x += _sum[_i].x; result.y += _sum[_i].y;} \
}


#ifdef _CHECK_HALT_
#define HALT(condition) \
	if(condition) { \
		ERROR_DETAILS("Halt on condition (" << STRINGIFY(condition) << ")", __FILE__, __LINE__); \
		exit(1); \
	}
#else
#define HALT(condition)
#endif

#define VA_NUM_ARGS(...) VA_NUM_ARGS_IMPL(__VA_ARGS__, 20, 19, 18, 17, 16, 15, 14, 13, 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1)
#define VA_NUM_ARGS_IMPL(_1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13, _14, _15, _16, _17, _18, _19, _20, N,...) N

#define CREATE_BUFFER(context, flags, _size, ptr_src, dest) { \
	int _size_ = (_size <= 0) ? 1 : _size; \
	cl_int ciErrNum; \
	dest = clCreateBuffer(context, flags, _size_, ptr_src, &ciErrNum); \
	TRACE("Creating buffer " << STRINGIFY(dest) << " with flags=" << STRINGIFY(flags) << ", size=" << _size_ << ((ptr_src==NULL)?" and without startup value => ":" and with contents copied from ") << STRINGIFY(ptr_src) << "."); \
	if(dest == NULL || ciErrNum != CL_SUCCESS){ \
		ERROR_DETAILS("Error creating buffer for " << STRINGIFY(dest), __FILE__, __LINE__); \
		exit(1); \
	}\
}

#define CALL_KERNEL2D(_command_queue, _kernel, _globalWorkSizeD1, _globalWorkSizeD2, _localWorkSizeD1, _localWorkSizeD2, _nParams, ...) \
	{ \
		const size_t _globalWorkSize2D[2] = {((size_t)_globalWorkSizeD1), ((size_t)_globalWorkSizeD2)}; \
		const size_t _localWorkSize2D[2] = {((size_t)_localWorkSizeD1), ((size_t)_localWorkSizeD2)}; \
		bool _hasLocalWorkSize = (_localWorkSizeD1>0&&_localWorkSizeD2>0); \
		if(_hasLocalWorkSize){ \
			TRACE("Kernel " << _kernel->kernelName << ", globalWorkSize=[" << _globalWorkSize2D[0] << "," << _globalWorkSize2D[1] \
                << "], localWorkSize=[" << _localWorkSize2D[0] << "," << _localWorkSize2D[1] << "]"); \
		} \
		else { \
			TRACE("Kernel " << _kernel->kernelName << ", globalWorkSize=[" << _globalWorkSize2D[0] << "," << _globalWorkSize2D[1] \
                << "] and no localWorkSize"); \
		} \
		_CALL_KERNEL(_command_queue, _kernel, 2, _globalWorkSize2D, ((_localWorkSizeD1>0&&_localWorkSizeD2>0)?_localWorkSize2D:NULL), _nParams, __VA_ARGS__) \
	}

#define CALL_KERNEL(_command_queue, _kernel, _globalWorkSize, _localWorkSize, _nParams, ...) \
	{ \
		const size_t _globalWorkSize1D[1] = {((size_t)_globalWorkSize)}; \
		const size_t _localWorkSize1D[1] = {((size_t)_localWorkSize)}; \
		TRACE("Kernel " << _kernel->kernelName << ", globalWorkSize=" << _globalWorkSize1D[0] << ", localWorkSize=" << _localWorkSize1D[0]); \
		_CALL_KERNEL(_command_queue, _kernel, 1, _globalWorkSize1D, _localWorkSize1D, _nParams, __VA_ARGS__) \
	}

#define _CALL_KERNEL(_command_queue, _kernel, _nDim, _globalWorkSize, _localWorkSize, _nParams, ...) \
	omp_set_lock(_kernel->mutex); /* Mandatory to submit Only One Kernel at a time to the queue! */ \
	cl_int _err; \
	clParam _params[_nParams] = {__VA_ARGS__}; \
	TRACE("Setting " << _nParams << " parameters to the kernel " << _kernel->kernelName); \
	for(int _i=0; _i < _nParams; _i++){ \
		_err  = clSetKernelArg(_kernel->kernelInstance, _i, _params[_i].sz, _params[_i].ptr); \
		if (_err != CL_SUCCESS){ \
			ERROR("Error (" << _err << ") setting parameter [" << (_i+1) << "] for kernel " << _kernel->kernelName); \
			exit(1); \
		} \
	} \
	TRACE("Calling the kernel " << _kernel->kernelName); \
	cl_event _event; \
	_err = clEnqueueNDRangeKernel(_command_queue, _kernel->kernelInstance, _nDim, NULL, _globalWorkSize, _localWorkSize, 0, NULL, &_event); \
	if (_err != CL_SUCCESS){ \
		ERROR("Error (" << _err << ") calling kernel: " << _kernel->kernelName); \
		exit(1); \
	} \
	PROFILING(_event); \
	omp_unset_lock(_kernel->mutex);
	/*nanosleep(&(struct timespec){0, 10000000}, NULL);*/

#define ADJUST_COMPLEX(command_queue, cl_complex, sz) { \
	int round = 0; \
	int globalWorkSize = (sz < GENPHASE_BLOCKSZ) ? sz : ceil(sz / (double)GENPHASE_BLOCKSZ) * GENPHASE_BLOCKSZ; \
	int localWorkSize = (sz < GENPHASE_BLOCKSZ) ? sz : GENPHASE_BLOCKSZ; \
	CALL_KERNEL(command_queue, kernels->cl_adjust_complex, globalWorkSize, localWorkSize, 3, \
		{sizeof(cl_mem), (void*)&cl_complex}, \
		{sizeof(cl_int), (void*)&round}, \
		{sizeof(cl_int), (void*)&sz} \
	); \
}

#define ROUND_COMPLEX(command_queue, cl_complex, sz) { \
	int round = 1; \
	int globalWorkSize = (sz < GENPHASE_BLOCKSZ) ? sz : ceil(sz / (double)GENPHASE_BLOCKSZ) * GENPHASE_BLOCKSZ; \
	int localWorkSize = (sz < GENPHASE_BLOCKSZ) ? sz : GENPHASE_BLOCKSZ; \
	CALL_KERNEL(command_queue, kernels->cl_adjust_complex, globalWorkSize, localWorkSize, 3, \
		{sizeof(cl_mem), (void*)&cl_complex}, \
		{sizeof(cl_int), (void*)&round}, \
		{sizeof(cl_int), (void*)&sz} \
	); \
}

#define SYNC_QUEUE(command_queue) { \
	cl_int _status = CL_SUCCESS; \
	if((_status=clFinish(command_queue)) != CL_SUCCESS){ \
	/*if(clEnqueueBarrier(command_queue) != CL_SUCCESS){*/ \
		ostringstream __oss; \
		__oss << "Error: [" << (int)_status << "] synchronizing the kernel!" << std::endl; \
		ERROR(__oss.str().c_str()); \
		exit(1); \
	} \
}

#define COMPLEX2REAL(z) sqrt(z.x * z.x + z.y + z.y)

#define REAL2COMPLEX_GPU_VIA_CPU(command_queue, cl_src, cl_dest, sz) { \
	WORD tmp_r[sz]; \
	COMPLEX_TYPE tmp_z[sz]; \
	clMemcpyDeviceToHost(command_queue, tmp_r, cl_src, sz * SIZEOF_WORD); \
	for(int _i=0; _i < sz; _i++){tmp_z.x = tmp_r[_i]; tmp_z.y = 0;} \
	clMemcpyHostToDevice(command_queue, tmp_Z, cl_DEST, sz * SIZEOF_COMPLEXTYPE); \
}

#define REAL2COMPLEX_GPU_VIA_GPU(command_queue, cl_src, cl_dest, sz) { \
	int globalWorkSize = (sz < MATRIX_OP_BLOCKSZ) ? sz : ceil(sz / (double)MATRIX_OP_BLOCKSZ) * MATRIX_OP_BLOCKSZ; \
	int localWorkSize = (sz < MATRIX_OP_BLOCKSZ) ? sz : MATRIX_OP_BLOCKSZ; \
	int _sz = sz; \
	CALL_KERNEL(command_queue, kernels->cl_real2complex, globalWorkSize, localWorkSize, 3, \
		{sizeof(cl_mem), (void*)&cl_src}, \
		{sizeof(cl_mem), (void*)&cl_dest}, \
		{sizeof(cl_int), (void*)&_sz} \
	); \
}

#define COMPLEX2REAL_GPU_VIA_GPU(command_queue, cl_src, cl_dest, sz) { \
	int globalWorkSize = (sz < MATRIX_OP_BLOCKSZ) ? sz : ceil(sz / (double)MATRIX_OP_BLOCKSZ) * MATRIX_OP_BLOCKSZ; \
	int localWorkSize = (sz < MATRIX_OP_BLOCKSZ) ? sz : MATRIX_OP_BLOCKSZ; \
	int _sz = sz; \
	CALL_KERNEL(command_queue, kernels->cl_complex2real, globalWorkSize, localWorkSize, 3, \
		{sizeof(cl_mem), (void*)&cl_src}, \
		{sizeof(cl_mem), (void*)&cl_dest}, \
		{sizeof(cl_int), (void*)&_sz} \
	); \
}

#define DOUBLE2FLOAT(d, f, size) for (int _i=0; _i < size; _i++) f[_i] = d[_i]


#endif /* _MACROS_HPP_ */
