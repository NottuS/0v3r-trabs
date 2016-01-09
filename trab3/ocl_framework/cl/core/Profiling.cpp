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
 * Profiling.cpp
 *
 * Created on: 06/Aug/2012
 * Author: Peter Frank Perroni (pfperroni@inf.ufpr.br)
 */

#include "../../include/cl/core/Profiling.hpp"

#ifdef _PROFILING_

omp_lock_t Profiling::mutex_prof;
double Profiling::profiling_total_time = 0;

void Profiling::startup(){
	omp_init_lock(&mutex_prof);
	reset();
}

void Profiling::shutdown(){
	omp_destroy_lock(&mutex_prof);
}

void Profiling::reset(){
	omp_set_lock(&mutex_prof);
	profiling_total_time = 0;
	omp_unset_lock(&mutex_prof);
}

double Profiling::getTotalProcessingTime(){
	return profiling_total_time;
}

void Profiling::increaseProcessingTime(float time){
	omp_set_lock(&mutex_prof);
	profiling_total_time += time;
	omp_unset_lock(&mutex_prof);
}

#endif
