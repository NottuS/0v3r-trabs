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
 * TestGPU.hpp
 *
 * Created on: 14/Jan/2015
 * Author: Peter Frank Perroni <pfperroni@inf.ufpr.br>
 */

#ifndef _TESTGPU_HPP_
#define _TESTGPU_HPP_

#include "cl/core/clObject.hpp"

class TestGPU : public clObject{
private:
	static bool started;

	vector<clQueue*> queues;
	omp_lock_t mutex_self;

public:
	~TestGPU();
	void initializeCL();
	void runKernel();
	void loadToDevice(){}
	int numQueuesTested(){return queues.size();}
	int numDevicesTested();
};

#endif /* _TESTGPU_HPP_ */
