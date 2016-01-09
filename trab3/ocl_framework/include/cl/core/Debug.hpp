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
 * debug.hpp
 *
 * Created on: 17/Jul/2012
 * Author: Peter Frank Perroni (pfperroni@inf.ufpr.br)
 */

#ifndef _DEBUG_HPP_
#define _DEBUG_HPP_

#include <CL/cl.h>
#include <CL/opencl.h>
#include <CL/cl_platform.h>
#include <CL/cl_ext.h>

#include <vector_types.h>
#include <cmath>
#include <float.h>
#include <stdio.h>
#include <stdlib.h>
#include <complex>
#include <sstream>

#include "Profiling.hpp"
#include "../../config.hpp"
#include "../../macros.hpp"

using namespace std;


#ifdef _DEBUG_
#if _DEBUG_ == DEBUG_TRACE
#define TRACE(msg) { \
	ostringstream _oss; \
	_oss << msg; \
	Debug::traceInfo(_oss.str().c_str()); \
}
#else
#define TRACE(msg)
#endif
#define DEBUG_MSG(msg) Debug::debug(msg)
#define DEBUG(T, command_queue, label, cl_ptr, rows, n_vars) Debug::debug<T>(command_queue, label, cl_ptr, rows, n_vars)
#define DEBUG_COMPLEX(command_queue, label, cl_ptr, rows, n_vars) Debug::debugComplex(command_queue, label, cl_ptr, rows, n_vars)
#define DEBUG_HOST(T, label, ptr, rows, n_vars) Debug::debugHost<T>(label, ptr, rows, n_vars)
#define DEBUG_HOST_COMPLEX(label, ptr, rows, n_vars) Debug::debugHostComplex(label, ptr, rows, n_vars)
#define DEBUG_VIENNA_CL(label, ptr) std::cout << label << *ptr << std::endl;
#define DEBUG_2D(T, command_queue, label, cl_ptr, d1, d2) DEBUG_3D(T, command_queue, label, cl_ptr, d2, 1, d1)

#define DEBUG_3D(T, command_queue, label, cl_ptr, d1, d2, d3) { \
	ostringstream oss; \
	int _matrix_size = (d1) * (d2); \
	T _matrix[_matrix_size]; \
	for(int _i=0; _i < d3; _i++){ \
		oss << label << "[" << _i << "]"; \
		clMemcpyDeviceToHostOffset(command_queue, _matrix, cl_ptr, _i * _matrix_size * sizeof(T), _matrix_size * sizeof(T)); \
		DEBUG_HOST(T, oss.str().c_str(), _matrix, d2, d1); \
		oss.seekp(0); \
	} \
}

#define DEBUG_COMPLEX2D(command_queue, label, cl_ptr, d1, d2, d3) DEBUG_COMPLEX3D(command_queue, label, cl_ptr, d1, 1, d2)

#define DEBUG_COMPLEX3D(command_queue, label, cl_ptr, d1, d2, d3) { \
	ostringstream oss; \
	int _matrix_size = (d1) * (d2); \
	COMPLEX_TYPE _matrix[_matrix_size]; \
	for(int _i=0; _i < d3; _i++){ \
		oss << label << "[" << _i << "]"; \
		clMemcpyDeviceToHostOffset(command_queue, _matrix, cl_ptr, _i * _matrix_size * SIZEOF_COMPLEXTYPE, _matrix_size * SIZEOF_COMPLEXTYPE); \
		DEBUG_HOST_COMPLEX(oss.str().c_str(), _matrix, d2, d1); \
		oss.seekp(0); \
	} \
}

#else
#define TRACE(msg)
#define DEBUG_MSG(msg)
#define DEBUG(T, command_queue, label, cl_ptr, rows, n_vars)
#define DEBUG_COMPLEX(command_queue, label, cl_ptr, rows, n_vars)
#define DEBUG_HOST(T, label, ptr, rows, n_vars)
#define DEBUG_HOST_COMPLEX(label, ptr, rows, n_vars)
#define DEBUG_VIENNA_CL(label, ptr)
#define DEBUG_2D(T, command_queue, label, cl_ptr, d1, d2)
#define DEBUG_3D(T, command_queue, label, cl_ptr, d1, d2, d3)
#define DEBUG_COMPLEX2D(command_queue, label, cl_ptr, d1, d2)
#define DEBUG_COMPLEX3D(command_queue, label, cl_ptr, d1, d2, d3)
#endif


#define ERROR_DETAILS(msg, file, line) { \
	ostringstream _oss; \
	_oss << "Error: [" << msg << "] at " << file << ":" << line << std::endl; \
	Debug::traceError(_oss.str().c_str()); \
}

#define ERROR(msg) ERROR_DETAILS(msg, __FILE__, __LINE__)

#ifdef _CL_STATE_
#define CHECK_CL_STATE(cmd) \
	{ \
		cl_int _status = cmd; \
		if(_status != CL_SUCCESS){ \
			ERROR_DETAILS("OpenCL call failed (error " << _status << "): " << STRINGIFY(cmd), __FILE__, __LINE__); \
			exit(1); \
		} \
	}
#else
#define CHECK_CL_STATE(cmd) cmd;
#endif

#ifdef _CHECKSUM_
#define CHECKSUM(T, command_queue, cl_ptr, size, compareTo) Debug::checkSum<T>(command_queue, cl_ptr, size, compareTo, __FILE__, __LINE__)
#define CHECKSUM_COMPLEX(command_queue, cl_ptr, size, compareTo) Debug::checkSumComplex(command_queue, cl_ptr, size, compareTo, __FILE__, __LINE__)
#else
#define CHECKSUM(T, command_queue, cl_ptr, size, compareTo)
#define CHECKSUM_COMPLEX(command_queue, cl_ptr, size, compareTo)
#endif

#ifdef _TRACK_TIME_
#define TRACK(statement) statement;
#define RESUME_TRACKING(tracker, pos) tracker[pos]->resume();
#define PAUSE_TRACKING(tracker, pos) tracker[pos]->pause();
#else
#define TRACK(statement)
#define RESUME_TRACKING(tracker, pos)
#define PAUSE_TRACKING(tracker, pos)
#endif

/**
 * @brief This static class is responsible for controlling all the
 * debug information that is produced by the program.
 */
class Debug{
	// For the methods that includes additional method call inside itself, which in turn could call a second mutex.
	static omp_lock_t mutex1;
	// For the methods that does not include additional method calls inside itself.
	static omp_lock_t mutex2;
	static bool lock_initialized;

	// Pre-allocate the debug buffer to avoid memory fragmentation.
	static char buffer[DEFAULT_BUFFER_SIZE];

public:

	/**
	 * @brief Starts up all the locks for parallel execution.
	 */
	static void startup_locks(){
		if(!lock_initialized){
			omp_init_lock(&mutex1);
			omp_init_lock(&mutex2);
			lock_initialized = true;
		}
	}

	/**
	 * @brief Destroy all the locks used for parallel execution.
	 */
	static void destroy_locks(){
		if(lock_initialized){
			omp_destroy_lock(&mutex1);
			omp_destroy_lock(&mutex2);
			lock_initialized = false;
		}
	}

	/**
	 * @brief Prinf a simple char message to the output stream.
	 *
	 * @param msg The message.
	 */
	static void debug(const char *msg){
		omp_set_lock(&mutex2);
		std::cout << msg << std::endl;
		omp_unset_lock(&mutex2);
	}

	/**
	 * @brief Print an OpenCl matrix to the standard output.
	 *
	 * @param command_queue The command queue from where to read the OpenCl matrix.
	 * @param label The output label to be printed.
	 * @param cl_ptr The OpenCl matrix.
	 * @param rows The number of rows of the matrix.
	 * @param n_vars The number of columns of the matrix.
	 */
	template<class T>
	static void debug(cl_command_queue command_queue, const char* label, cl_mem cl_ptr, int rows, int n_vars) {
		omp_set_lock(&mutex1);
		T *debug_info = (T*)buffer;
		clMemcpyDeviceToHost(command_queue, debug_info, cl_ptr, rows * n_vars * sizeof(T));
		cout << label << ":" << endl;
		for (int j, i = 0; i < rows; i++) {
			for (j = 0; j < n_vars; j++) {
				if (j > 0) {
					cout << ' ';
				}
				cout << debug_info[i * n_vars + j];
			}
			cout << endl;
		}
		omp_unset_lock(&mutex1);
	}

	/**
	 * @brief Print a CPU matrix to the standard output.
	 *
	 * @param label The output label to be printed.
	 * @param ptr The Host pointer for the matrix.
	 * @param rows The number of rows of the matrix.
	 * @param n_vars The number of columns of the matrix.
	 */
	template<class T>
	static void debugHost(const char* label, void* ptr, int rows, int n_vars) {
		omp_set_lock(&mutex2);
		T *debug_info = (T*)ptr;
		cout << label << ":" << endl;
		for (int j, i = 0; i < rows; i++) {
			for (j = 0; j < n_vars; j++) {
				if (j > 0) {
					cout << ' ';
				}
				cout << debug_info[i * n_vars + j];
			}
			cout << endl;
		}
		omp_unset_lock(&mutex2);
	}

	/**
	 * @brief Print an OpenCl matrix of complex numbers (COMPLEX_TYPE) to the standard output.
	 *
	 * @param command_queue The command queue from where to read the OpenCl matrix.
	 * @param label The output label to be printed.
	 * @param cl_ptr The OpenCl matrix.
	 * @param rows The number of rows of the matrix.
	 * @param n_vars The number of columns of the matrix.
	 */
	static void debugComplex(cl_command_queue command_queue, const char* label, cl_mem cl_ptr, int rows, int n_vars) {
		omp_set_lock(&mutex1);
		COMPLEX_TYPE *debug_info = (COMPLEX_TYPE*)buffer;
		clMemcpyDeviceToHost(command_queue, debug_info, cl_ptr, rows * n_vars * SIZEOF_COMPLEXTYPE);
		cout << label << ":" << endl;
		for (int j, i = 0; i < rows; i++) {
			for (j = 0; j < n_vars; j++) {
				if (j > 0) {
					cout << ' ';
				}
				cout << debug_info[i * n_vars + j].x << "," << debug_info[i * n_vars + j].y;
			}
			cout << endl;
		}
		cout << endl;
		omp_unset_lock(&mutex1);
	}

	/**
	 * @brief Print a CPU matrix of complex numbers (COMPLEX_TYPE) to the standard output.
	 *
	 * @param label The output label to be printed.
	 * @param ptr The Host pointer for the matrix.
	 * @param rows The number of rows of the matrix.
	 * @param n_vars The number of columns of the matrix.
	 */
	static void debugHostComplex(const char* label, COMPLEX_TYPE* ptr, int rows, int n_vars) {
		omp_set_lock(&mutex2);
		cout << label << ":" << endl;
		for (int j, i = 0; i < rows; i++) {
			for (j = 0; j < n_vars; j++) {
				if (j > 0) {
					cout << ' ';
				}
				cout << ptr[i * n_vars + j].x << "," << ptr[i * n_vars + j].y;
			}
			cout << endl;
		}
		cout << endl;
		omp_unset_lock(&mutex2);
	}

	/**
	 * @brief Prinf a simple char message as Trace information to the output stream.
	 *
	 * @param msg The message.
	 */
	static void traceInfo(const char *msg){
		omp_set_lock(&mutex2);
		std::cout << "Trace: [" << msg << "]" << std::endl;
		omp_unset_lock(&mutex2);
	}

	/**
	 * @brief Prinf a simple char message as Error information to the error stream.
	 *
	 * @param msg The message.
	 */
	static void traceError(const char *msg){
		omp_set_lock(&mutex2);
		std::cerr << "Trace: [" << msg << "]" << std::endl;
		omp_unset_lock(&mutex2);
	}

	/**
	 * @brief Compare the OpenCl reduction result with a classical CPU sum.
	 *
	 * @param command_queue The command queue from where to read the OpenCl matrix.
	 * @param cl_ptr The OpenCl matrix.
	 * @param size The size of the matrix.
	 * @param compareTo The OpenCl result to be compared with this classical sum.
	 * @param file The source code file name where this method was called.
	 * @param line The source code line where this method was called.
	 */
	template<class T>
	static void checkSum(cl_command_queue command_queue, cl_mem cl_ptr, int size, double compareTo, const char *file, int line) {
		omp_set_lock(&mutex1);
		T *values = (T*)buffer;
		double sum = 0;
		clMemcpyDeviceToHost(command_queue, values, cl_ptr, size * sizeof(T));
		for (int i = 0; i < size; i++) {
			sum += values[i];
		}
		omp_unset_lock(&mutex1);

	#ifdef _DOUBLE_WORD_
		if((float)sum != (float)compareTo) {
			ERROR("CHECKSUM ERROR: correct=" << sum << ", found=" << compareTo);
			exit(1);
		}
	#else
		int error = 0;
		// This is Entirely ad-hoc, since the GPU floats will never match with CPU calculations.
		// Thus, this is much more to have some reasonable approximation from the expected result.
		if (sum >= 1e+8 && abs(sum / 100000 - compareTo / 100000) > 1)
			error = 1;
		if (sum < 1e+8 && sum >= 1e+7 && abs(sum / 10000 - compareTo / 10000) > 1)
			error = 2;
		if (sum < 1e+7 && sum >= 1e+6 && abs(sum / 1000 - compareTo / 1000) > 1)
			error = 3;
		if (sum < 1e+6 && sum >= 1e+5 && abs(sum / 100 - compareTo / 100) > 1)
			error = 4;
		if (sum < 1e+5 && sum >= 1e+4 && abs(sum / 10 - compareTo / 10) > 1)
			error = 5;
		if (sum < 1e+4 && sum >= 1e+3 && abs(sum - compareTo) > 1)
			error = 6;
		if (sum < 1e+3 && sum >= 1e+2 && abs(sum * 10 - compareTo * 10) > 1)
			error = 7;
		if (sum < 1e+2 && sum >= 1e+1 && abs(sum * 100 - compareTo * 100) > 1)
			error = 8;
		if (sum < 1e+1 && abs(sum * 1000 - compareTo * 1000) > 1)
			error = 9;
		/*
		 if((sum >= 1e+8 && (int)(sum/100000) != (int)(compareTo/100000)))
		 error = 1;
		 if((sum < 1e+8 && sum >= 1e+7 && (int)(sum/10000) != (int)(compareTo/10000)))
		 error = 2;
		 if((sum < 1e+7 && sum >= 1e+6 && (int)(sum/1000) != (int)(compareTo/1000)))
		 error = 3;
		 if((sum < 1e+6 && sum >= 1e+5 && (int)(sum/100) != (int)(compareTo/100)))
		 error = 4;
		 if((sum < 1e+5 && sum >= 1e+4 && (int)(sum/100) != (int)(compareTo/100)))
		 error = 5;
		 if((sum < 1e+4 && sum >= 1e+3 && abs(sum-compareTo) > 1))
		 error = 6;
		 if((sum < 1e+3 && sum >= 1e+2 && (int)(sum*10) != (int)(compareTo*10)))
		 error = 7;
		 if((sum < 1e+2 && sum >= 1e+1 && (int)(sum*100) != (int)(compareTo*100)))
		 error = 8;
		 if((sum < 1e+1 && (int)(sum*1000) != (int)(compareTo*1000)))
		 error = 9;
		 */
		if (error) {
			ERROR_DETAILS("CHECKSUM ERROR: correct=" << (WORD)sum << ", found=" << (WORD)compareTo << ", error=" << error, file, line);
			exit(1);
		}
	#endif
	}

	/**
	 * @brief Compare the OpenCl reduction result of complex numbers (COMPLEX_TYPE) with a classical CPU sum.
	 *
	 * @param command_queue The command queue from where to read the OpenCl matrix.
	 * @param cl_ptr The OpenCl matrix.
	 * @param size The size of the matrix.
	 * @param compareTo The OpenCl result to be compared with this classical sum.
	 * @param file The source code file name where this method was called.
	 * @param line The source code line where this method was called.
	 */
	static void checkSumComplex(cl_command_queue command_queue, cl_mem cl_ptr, int size, COMPLEX_TYPE compareTo, const char *file, int line) {
		omp_set_lock(&mutex1);
		COMPLEX_TYPE *values = (COMPLEX_TYPE*)buffer;
		COMPLEX_TYPE sum = {0, 0};
		clMemcpyDeviceToHost(command_queue, values, cl_ptr, size * SIZEOF_COMPLEXTYPE);
		for (int i = 0; i < size; i++) {
			sum.x += values[i].x;
			sum.y += values[i].y;
		}
		omp_unset_lock(&mutex1);
		// Not sure if the comparison below will work for float2 data types.
		if(sum.x != compareTo.x || sum.x != compareTo.x) {
			ERROR_DETAILS("CHECKSUM ERROR: correct=" << sum.x << "," << sum.y
				<< "; found=" << compareTo.x << "," << compareTo.y, file, line);
			exit(1);
		}
	}
};

#endif /* _DEBUG_HPP_ */
