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
 * Editable.hpp
 *
 * Created on: 15/Jan/2015
 * Author: Peter Frank Perroni (pfperroni@inf.ufpr.br)
 */

#ifndef _EDITABLE_HPP_
#define _EDITABLE_HPP_

using namespace std;

/**
 * @brief Class representing an editable design object.
 */
class Editable{
private:
	bool editable;
public:
	Editable() : editable(false){}
	virtual ~Editable(){}
	virtual void setEditable(bool _editable);
	virtual bool isEditable();
	virtual void setVisible(bool _visible) = 0;
	virtual bool isVisible() = 0;
	virtual bool isWithinBounds(int x, int y) = 0;
};


#endif /* _EDITABLE_HPP_ */
