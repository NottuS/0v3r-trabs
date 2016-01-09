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
 * config.hpp
 *
 * Created on: 27/May/2012
 * Author: Peter Frank Perroni (pfperroni@inf.ufpr.br)
 */

#ifndef _CONFIG_HPP_
#define _CONFIG_HPP_

enum {DEBUG_NONE, DEBUG_NORMAL, DEBUG_TRACE};

/**
 * @brief The rand seed, to keep the random number generation under control.
 * To be used Only for testing purposes.
 * Once the code is tested, it must be commented out.
 */
#ifndef RAND_SEED
#define RAND_SEED rand(); //10
#endif

/**
 * @brief Set the GPU as the OpenCl device type.
 */
#define RUN_ON_GPU CL_DEVICE_TYPE_GPU

/**
 * @brief Set the CPU as the OpenCl device type.
 */
#define RUN_ON_CPU CL_DEVICE_TYPE_CPU

/**
 * @brief Set the debug behavior.
 */
//#define _DEBUG_ DEBUG_TRACE

/**
 * @brief Define if the program will verify the OpenCl states.
 */
#define _CL_STATE_

/**
 * @brief Define if the program will be enabling the halt conditions.
 */
#define _CHECK_HALT_

/**
 * @brief Define is the OpenCl reductions will be checked
 * by doing classical CPU sums.
 */
#define _CHECKSUM_

/**
 * @brief Define if the time will be tracked during the program execution.
 */
#define _TRACK_TIME_

/**
 * @brief Define if the OpenCl Profiling will be enabled.
 */
//#define _PROFILING_

/**
 * @brief Define if the WORD size will be double or float.
 */
//#define _DOUBLE_WORD_

/**
 * @brief Define the default block size.
 */
#define DEFAULT_BLOCKSZ 256

/**
 * @brief Define the number of blocks to be used for reduction.
 */
#define REDUCTION_NBLOCKS 16

/**
 * @brief Define the block size to be used for reduction.
 */
#define REDUCTION_BLOCKSZ 64

/**
 * @brief Define the block size to compute matrix operations.
 */
#define MATRIX_OP_BLOCKSZ 96

/**
 * @brief Define the number of OpenCl platforms to try.
 */
#define CL_PLATFORMS_TO_TRY 10

/**
 * @brief Define the default buffer size to be allocated.
 * Instead of doing several memory allocations/releases, a big chunck
 * of memory allow for a better memory mapping, avoiding its fragmentation.
 */
#define DEFAULT_BUFFER_SIZE ((long)(30 * 1024 * 1024))

#endif /* _CONFIG_HPP_ */
