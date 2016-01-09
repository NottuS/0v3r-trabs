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
 * WindowDimensions.hpp
 *
 * Created on: 06/Jan/2015
 * Author: Peter Frank Perroni (pfperroni@inf.ufpr.br)
 */


#ifndef _WINDOWDIMENSIONS_HPP_
#define _WINDOWDIMENSIONS_HPP_

#include <GL/glew.h>
#include <GL/freeglut.h>

class WindowDimensions{
private:
	int windowHandler;
	float zoom;
	float originalWindowWidth, originalWindowHeight;
	int windowWidth, windowHeight, windowDepth;
	float pixelWidth, pixelHeight, pixelDepth;
	float halfWidth, halfHeight, halfDepth;
public:
	WindowDimensions(int _windowHandler, int width, int height, int depth, float _zoom = 1.0f);
	int getWindowHandler();
	float getWindowWidth();
	float getWindowHeight();
	float getZoom();
	float getWindowDepth();
	float getPixelWidth();
	float getPixelHeight();
	float getPixelDepth();
	float getHalfPixelDepth();
	float getScreenCoordX(float x);
	float getScreenCoordY(float y);
	float getScreenCoordZ(float z);
};

#endif /* _WINDOWDIMENSIONS_HPP_ */
