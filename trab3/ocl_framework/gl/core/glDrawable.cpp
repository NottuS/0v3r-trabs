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
 * glDrawable.cpp
 *
 * Created on: 05/Feb/2015
 * Author: Peter Frank Perroni (pfperroni@inf.ufpr.br)
 */

#include "../../include/gl/core/glDrawable.hpp"

int glDrawable::DESIGN_ELEMENT_DO = 0;
int glDrawable::DESIGN_ELEMENT_DOMANAGER = 1;
int glDrawable::DESIGN_ELEMENT_DOMANAGERGROUP = 2;

glDrawable::glDrawable(){
	selfElement = new DesignElement;
	selfElement->drawable = this;
	selfElement->prev = selfElement->next = NULL;
	type = -1;
}
glDrawable::~glDrawable(){
	delete selfElement;
}
DesignElement *glDrawable::getSelfDesignElement(){
	return selfElement;
}

void glDrawable::setDrawableType(int _type){
	type = _type;
}

int glDrawable::getDrawableType(){
	return type;
}
