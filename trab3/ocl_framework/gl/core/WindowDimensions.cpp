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
 * WindowDimensions.cpp
 *
 * Created on: 06/Jan/2015
 * Author: Peter Frank Perroni (pfperroni@inf.ufpr.br)
 */

#include "../../include/gl/core/WindowDimensions.hpp"

WindowDimensions::WindowDimensions(int _windowHandler, int width, int height, int depth, float _zoom){
	windowHandler = _windowHandler;
	windowWidth = width;
	windowHeight = height;
	windowDepth = depth;
	originalWindowWidth = glutGet(GLUT_WINDOW_WIDTH);
	originalWindowHeight = glutGet(GLUT_WINDOW_HEIGHT);
	halfWidth = windowWidth / 2.0f;
	halfHeight = windowHeight / 2.0f;
	halfDepth = windowDepth / 2.0f;
	zoom = (_zoom <= 0.0f) ? 0.1f : _zoom;
	pixelWidth = (2.0f * zoom) / windowWidth;
	pixelHeight = (2.0f * zoom) / windowHeight;
	pixelDepth = (2.0f * zoom) / windowDepth;
}
int WindowDimensions::getWindowHandler(){
	return windowHandler;
}

float WindowDimensions::getWindowWidth(){
	return windowWidth;
}

float WindowDimensions::getWindowHeight(){
	return windowHeight;
}

float WindowDimensions::getZoom(){
	return zoom;
}

float WindowDimensions::getWindowDepth(){
	return windowDepth;
}

float WindowDimensions::getPixelWidth(){
	return pixelWidth;
}

float WindowDimensions::getPixelHeight(){
	return pixelHeight;
}

float WindowDimensions::getPixelDepth(){
	return pixelDepth;
}

float WindowDimensions::getHalfPixelDepth(){
	return pixelDepth / 2.0f;
}

float WindowDimensions::getScreenCoordX(float x){
	return ((x - halfWidth) * pixelWidth);
}

float WindowDimensions::getScreenCoordY(float y){
	return ((halfHeight - y) * pixelHeight);
}

float WindowDimensions::getScreenCoordZ(float z){
	return ((halfDepth - z) * pixelDepth);
}
