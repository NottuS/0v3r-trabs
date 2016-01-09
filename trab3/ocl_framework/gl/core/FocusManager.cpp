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
 * FocusManager.cpp
 *
 * Created on: 27/Jan/2015
 * Author: Peter Frank Perroni (pfperroni@inf.ufpr.br)
 */

#include "../../include/gl/core/FocusManager.hpp"

omp_lock_t FocusManager::mutex;
map<int, void*> FocusManager::focusMap;
bool FocusManager::lock_started = false;

void FocusManager::startup(){
	omp_init_lock(&mutex);
}

void FocusManager::shutdown(){
	omp_destroy_lock(&mutex);
}

void FocusManager::setFocus(int key, void *focus){
	omp_set_lock(&mutex);
	focusMap[key] = focus;
	omp_unset_lock(&mutex);
}
void FocusManager::clearFocus(int key){
	omp_set_lock(&mutex);
	map<int, void*>::iterator iter = focusMap.find(key);
	if(iter != focusMap.end()){
		focusMap.erase(iter);
	}
	omp_unset_lock(&mutex);
}
void* FocusManager::getFocus(int key){
	return focusMap[key];
}
