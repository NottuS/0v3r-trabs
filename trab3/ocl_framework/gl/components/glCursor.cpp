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
 * glCursor.cpp
 *
 * Created on: 06/Jan/2015
 * Author: Peter Frank Perroni (pfperroni@inf.ufpr.br)
 */

#include "../../include/gl/components/glCursor.hpp"

CursorParameters* glCursor::getParams(){
	return (CursorParameters*)DesignObject::getParams();
}

void glCursor::draw(){
	if(!isVisible())
//	 || (editable != NULL && (!editable->isVisible() || !editable->isEditable())))
	 return;

	CursorParameters *params = getParams();
	glShadeModel(GL_FLAT);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glBegin(GL_LINE_LOOP);
	glColor3f(params->rgb.r, params->rgb.g, params->rgb.b);
	glVertex3f(screenCoords[0].x, screenCoords[0].y, screenCoords[0].z);
	glVertex3f(screenCoords[1].x, screenCoords[1].y, screenCoords[1].z);
	glEnd();
}

void glCursor::refreshInternalDimensions(){
	WindowDimensions *dim = getWindowDimensions();
	if(dim == NULL) return;
	CursorParameters *params = getParams();
	screenCoords[0].x = dim->getScreenCoordX(params->coords[0].x);
	screenCoords[0].y = dim->getScreenCoordY(params->coords[0].y);
	screenCoords[0].z = dim->getScreenCoordZ(params->coords[0].z);
	screenCoords[1].x = dim->getScreenCoordX(params->coords[1].x);
	screenCoords[1].y = dim->getScreenCoordY(params->coords[1].y);
	screenCoords[1].z = dim->getScreenCoordZ(params->coords[1].z);
}

bool glCursor::isWithinBounds(int x, int y) {
	if(editable != NULL) return editable->isEditable() && editable->isWithinBounds(x, y);

	WindowDimensions *dim = getWindowDimensions();
	float _x = dim->getScreenCoordX(x);
	float _y = dim->getScreenCoordY(y);
//printf("[%f %f %f %f - %f %f]\n", screenCoords[0].x, screenCoords[0].y, screenCoords[1].x, screenCoords[1].y, _x, _y);
	return (_x >= screenCoords[0].x && _x <= screenCoords[1].x &&
			_y <= screenCoords[0].y && _y >= screenCoords[1].y);
}

void glCursor::slide(float xDist, float yDist){
	if(xDist < 0) xDist = 0;
	if(yDist < 0) yDist = 0;
	getParams()->move(getAnchor().x+xDist, getAnchor().y+yDist);
	refreshInternalDimensions();
//	glutPostRedisplay();
}

void glCursor::slideHorizontal(float dist){
	slide(dist, 0);
}

void glCursor::slideVertical(float dist){
	slide(0, dist);
}

void glCursor::moveTo(float x, float y){
	getParams()->move(x, y, getAnchor().z);
}

bool glCursor::isVisible(){
	return DesignObject::isVisible() &&
			(editable == NULL || (editable->isVisible() && editable->isEditable()));
}

int glCursor::getWidthInPixels(){
	CursorParameters *params = getParams();
	return params->coords[1].x - params->coords[0].x;
}

int glCursor::getHeightInPixels(){
	CursorParameters *params = getParams();
	return params->coords[1].y - params->coords[0].y;
}

void glCursor::getBounds(XYZ &vertice1, XYZ &vertice2){
	CursorParameters *params = getParams();
	vertice1 = params->coords[0];
	vertice2 = params->coords[1];
}
