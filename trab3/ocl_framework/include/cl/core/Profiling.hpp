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
 * Profiling.hpp
 *
 * Created on: 06/Aug/2012
 * Author: Peter Frank Perroni (pfperroni@inf.ufpr.br)
 */

#include <omp.h>
#include <iostream>

#ifdef _PROFILING_

#ifndef _PROFILING_HPP_
#define _PROFILING_HPP_

/**
 * @brief This static class keeps track of the OpenCl device processing time.
 *
 * Every call of a OpenCl command will automatically increase the counter controlled by this class.
 *
 * If necessary, the {@link #reset} method can be called to clear the counter.
 */
class Profiling {
	static omp_lock_t mutex_prof;
	static double profiling_total_time;

public:
	/**
	 * @brief Initialize the locks and reset the internal counter.
	 */
	static void startup();

	/**
	 * @brief Finalize the locks.
	 */
	static void shutdown();

	/**
	 * @brief Reset the internal counter.
	 */
	static void reset();

	/**
	 * @brief Returns the total device processing time, in milliseconds.
	 *
	 * @return The total processing time, in milliseconds.
	 */
	static double getTotalProcessingTime();

	/**
	 * @brief Increase the internal counter.
	 *
	 * @param time The time to be added to the counter, in milliseconds.
	 */
	static void increaseProcessingTime(float time);
};

#endif /* _PROFILING_HPP_ */

#endif /* _PROFILING_ */
