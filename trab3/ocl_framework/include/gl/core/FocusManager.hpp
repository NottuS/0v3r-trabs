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
 * FocusManager.hpp
 *
 * Created on: 27/Jan/2015
 * Author: Peter Frank Perroni (pfperroni@inf.ufpr.br)
 */

#ifndef _FOCUSMANAGER_HPP_
#define _FOCUSMANAGER_HPP_

#include <omp.h>
#include <map>

using namespace std;

/**
 * @brief Class that manages all focus events.
 */
class FocusManager{
private:
	static omp_lock_t mutex;
	static bool lock_started;
	static map<int, void*> focusMap;

public:
	/**
	 * @brief Start up the focus manager.
	 */
	static void startup();

	/**
	 * @brief Shut down the focus manager.
	 */
	static void shutdown();

	/**
	 * @brief Assign the focus method for the key provided.
	 *
	 * @param key The focus unique key.
	 * @param focus The focus method.
	 */
	static void setFocus(int key, void *focus);

	/**
	 * @brief Clear the focus method for the key provided.
	 *
	 * @param key The focus unique key.
	 */
	static void clearFocus(int key);

	/**
	 * @brief Get the focus method assigned to the key provided.
	 *
	 * @param key The focus unique key.
	 * @return The focus method.
	 */
	static void* getFocus(int key);

	/**
	 * @brief Get the focus method assigned to the key provided.
	 *
	 * @param key The focus unique key.
	 * @return The focus method, casted to the type specified.
	 */
	template<class T>
	static T getTypedFocus(int key){return static_cast<T>(focusMap[key]);}
};


#endif /* _FOCUSMANAGER_HPP_ */
