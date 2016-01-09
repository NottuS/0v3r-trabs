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
 * Box.cpp
 *
 * Created on: 06/Jan/2015
 * Author: Peter Frank Perroni (pfperroni@inf.ufpr.br)
 */

#include "../../include/gl/components/Box.hpp"

BoxParameters* Box::getParams(){
	return (BoxParameters*)DesignObject::getParams();
}

void Box::draw(){
	if(!isVisible()) return;

	BoxParameters *params = getParams();
	if(params->smooth) glShadeModel(GL_SMOOTH);
	else glShadeModel(GL_FLAT);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glBegin(GL_POLYGON);
//	float zCoord = getZCoord();
	glColor3f(params->rgbFill[0].r, params->rgbFill[0].g, params->rgbFill[0].b);
	glVertex3f(screenCoords[0].x, screenCoords[0].y, screenCoords[0].z);
	glColor3f(params->rgbFill[1].r, params->rgbFill[1].g, params->rgbFill[1].b);
	glVertex3f(screenCoords[1].x, screenCoords[0].y, screenCoords[0].z);
	glColor3f(params->rgbFill[2].r, params->rgbFill[2].g, params->rgbFill[2].b);
	glVertex3f(screenCoords[1].x, screenCoords[1].y, screenCoords[1].z);
	glColor3f(params->rgbFill[3].r, params->rgbFill[3].g, params->rgbFill[3].b);
	glVertex3f(screenCoords[0].x, screenCoords[1].y, screenCoords[1].z);

	glEnd();
	glShadeModel(GL_FLAT);

	// Draw the box.
	glBegin(GL_LINE_LOOP);
	glColor3f(params->rgbBorder.r, params->rgbBorder.g, params->rgbBorder.b);
	glVertex3f(screenCoords[0].x, screenCoords[0].y, screenCoords[0].z);
	glVertex3f(screenCoords[1].x, screenCoords[0].y, screenCoords[0].z);
	glVertex3f(screenCoords[1].x, screenCoords[1].y, screenCoords[1].z);
	glVertex3f(screenCoords[0].x, screenCoords[1].y, screenCoords[1].z);
	glEnd();
}

void Box::refreshInternalDimensions(){
	BoxParameters *params = getParams();
	WindowDimensions *dim = getWindowDimensions();
	screenCoords[0].x = dim->getScreenCoordX(params->coords[0].x);
	screenCoords[0].y = dim->getScreenCoordY(params->coords[0].y);
	screenCoords[0].z = dim->getScreenCoordZ(params->coords[0].z);
	screenCoords[1].x = dim->getScreenCoordX(params->coords[1].x);
	screenCoords[1].y = dim->getScreenCoordY(params->coords[1].y);
	screenCoords[1].z = dim->getScreenCoordZ(params->coords[1].z);
//printf("Z[%f,%f,%f][%f,%f,%f]", screenCoords[0].x, screenCoords[0].y, screenCoords[0].z, screenCoords[1].x, screenCoords[1].y, screenCoords[1].z);
//printf("[%f,%f,%f][%f,%f,%f]\n", params->coords[0].x, params->coords[0].y, params->coords[0].z, params->coords[1].x, params->coords[1].y, params->coords[1].z);
}

bool Box::isWithinBounds(int x, int y) {
	WindowDimensions *dim = getWindowDimensions();
	float _x = dim->getScreenCoordX(x);
	float _y = dim->getScreenCoordY(y);
//printf("[%f %f %f %f - %f %f]\n", screenCoords[0].x, screenCoords[0].y, screenCoords[1].x, screenCoords[1].y, _x, _y);
	return (_x >= screenCoords[0].x && _x <= screenCoords[1].x &&
			_y <= screenCoords[0].y && _y >= screenCoords[1].y);
}

void Box::moveTo(float x, float y){
//	printf("moving box %i to [%f,%f]\n", getUID(), x, y);
	getParams()->move(x, y, getAnchor().z);
}

int Box::getWidthInPixels(){
	BoxParameters *params = getParams();
	return params->coords[1].x - params->coords[0].x;
}

int Box::getHeightInPixels(){
	BoxParameters *params = getParams();
	return params->coords[1].y - params->coords[0].y;
}

void Box::getBounds(XYZ &vertice1, XYZ &vertice2){
	BoxParameters *params = getParams();
	vertice1 = params->coords[0];
	vertice2 = params->coords[1];
}
