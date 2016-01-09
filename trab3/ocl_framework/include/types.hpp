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
 * types.hpp
 *
 * Created on: 27/Jan/2015
 * Author: Peter Frank Perroni (pfperroni@inf.ufpr.br)
 */

#ifndef _TYPES_HPP_
#define _TYPES_HPP_

using namespace std;

#include <stddef.h>

#define DESIGN_LIST_DRAWABLES 0
#define DESIGN_LIST_ADDS 1
#define DESIGN_LIST_DELETES 2

class glDrawable;

/**
 * @brief A node in a list of glDrawable's.
 */
struct DesignElement{
	glDrawable *drawable;
	struct DesignElement *next;
	struct DesignElement *prev;
};

/**
 * @brief Root of a glDrawable's list.
 *
 * When drawing components, the sequence of painting is important.
 * Therefore, they are stored in the order they were added to their containers.
 */
struct DesignList{
	int type;
	struct DesignElement *first;
	struct DesignElement *last;
};

/**
 * @brief The RGB color.
 */
typedef struct _RGB{
	float r;
	float g;
	float b;
} RGB;

/**
 * @brief The 3D coordinate.
 */
typedef struct _XYZ{
	float x;
	float y;
	float z;
} XYZ;

#endif /* _TYPES_HPP_ */
