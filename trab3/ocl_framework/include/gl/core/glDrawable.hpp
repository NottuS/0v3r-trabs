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
 * glDrawable.hpp
 *
 * Created on: 05/Feb/2015
 * Author: Peter Frank Perroni (pfperroni@inf.ufpr.br)
 */

#ifndef _GLDRAWABLE_HPP_
#define _GLDRAWABLE_HPP_

#include "../../types.hpp"

class DesignObject;
class DOManager;
class DOManagerGroup;

/**
 * @brief Class representing a drawable design object.
 *
 * It can be either directly drawable or a container of directly drawable objects.
 */
class glDrawable{
private:
	DesignElement *selfElement;
	int type;

public:
	static int DESIGN_ELEMENT_DO;
	static int DESIGN_ELEMENT_DOMANAGER;
	static int DESIGN_ELEMENT_DOMANAGERGROUP;
	glDrawable();
	virtual ~glDrawable();

	/**
	 * @brief Get the self DesignElement object associated to this drawable instance.
	 *
	 * @return The DesignElement object associated to this drawable instance.
	 */
	DesignElement *getSelfDesignElement();

	/**
	 * @brief Set the drawable type for this instance.
	 *
	 * @param _type A drawable type from: DESIGN_ELEMENT_DO, DESIGN_ELEMENT_DOMANAGER, DESIGN_ELEMENT_DOMANAGERGROUP.
	 */
	void setDrawableType(int _type);
	int getDrawableType();
	virtual void scheduleAddDesignElement(DesignElement *elem) = 0;
	virtual void removeDesignElement(DesignElement *elem) = 0;
	virtual int getUID() = 0;
};


#endif /* _GLDRAWABLE_HPP_ */
