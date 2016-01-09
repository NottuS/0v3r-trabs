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
 * Text.cpp
 *
 * Created on: 06/Jan/2015
 * Author: Peter Frank Perroni (pfperroni@inf.ufpr.br)
 */

#include "../../include/gl/components/Text.hpp"

TextParameters* Text::getParams(){
	return (TextParameters*)DesignObject::getParams();
}

void Text::draw(){
	if(!isVisible()) return;

	TextParameters *params = getParams();

	// Draw the Letters on negative Z (front).
	glRasterPos3f(screenCoords.x, screenCoords.y, -screenCoords.z);
	glColor3f(params->rgb.r, params->rgb.g, params->rgb.b);
	char *p;
	for(p=params->text; *p != '\0'; p++){
		glutBitmapCharacter(params->font, *p);
	}

	if(params->_3D){
		// Draw a contrast shadow on positive Z (back) to make words more visible while rotating.
		glRasterPos3f(screenCoords.x, screenCoords.y, screenCoords.z);
		glColor3f(params->rgb.r, params->rgb.g, params->rgb.b);
		for(p=params->text; *p != '\0'; p++){
			glutBitmapCharacter(params->font, *p);
		}
	}
}

bool Text::isWithinBounds(int x, int y) {
	WindowDimensions *dim = getWindowDimensions();
	TextParameters* params = getParams();
	float _x = dim->getScreenCoordX(x);
	float _y = dim->getScreenCoordY(y);
	float x1 = screenCoords.x;
	float x2 = dim->getScreenCoordX(params->pos.x+15);
	float y1 = dim->getScreenCoordY(params->pos.y-20);
	float y2 = dim->getScreenCoordY(params->pos.y-5);
//printf("[%f %f %f %f - %f %f] %i\n", x1, x2, y1, y2, _x, _y, (_x >= x1 && _x <= x2 &&
//		_y <= y1 && _y >= y2));
	return (_x >= x1 && _x <= x2 &&
			_y <= y1 && _y >= y2);
}

void Text::refreshInternalDimensions(){
	TextParameters *params = getParams();
	WindowDimensions *dim = getWindowDimensions();
	screenCoords.x = dim->getScreenCoordX(params->pos.x);
	screenCoords.y = dim->getScreenCoordY(params->pos.y + getTextHeightInPixels());
	screenCoords.z = dim->getScreenCoordZ(params->pos.z);
}

int Text::mouseButton(int button, int state, int x, int y){
	int ret = ACTION_IGNORED;
	if(button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN){
		int submenuDebug;
		submenuDebug = glutCreateMenu(rightClick);
		glutAddMenuEntry("Print Text", 0);

		glutCreateMenu(Text::rightClick);
		glutAddSubMenu("Options",submenuDebug);

		FocusManager::setFocus(getWindowDimensions()->getWindowHandler(), this);
		ret |= ACTION_CHECKED;
	}
	return ret;
}

void Text::rightClick(int op){
	Text *dObj = (Text*)FocusManager::getFocus(glutGetWindow());
	FocusManager::clearFocus(glutGetWindow());
	printf("Text: [%s]\n", dObj->getParams()->text);
}

int Text::getTextWidthInPixels(char *text, void* font){
	if(text == NULL || font == NULL) return 0;
	int size = strlen(text);
	int pixels = 0;
	for(int i=0; i < size; i++){
		pixels += glutBitmapWidth(font, text[i]);
	}
	return pixels;
}

int Text::getTextHeightInPixels(void* font){
	if(font == NULL) return 0;
	return glutBitmapHeight(font);
}

int Text::getTextWidthInPixels(){
	return getTextWidthInPixels(getParams()->text, getParams()->font);
}

int Text::getTextHeightInPixels(){
	return getTextHeightInPixels(getParams()->font);
}

int Text::getWidthInPixels(){
	return getTextWidthInPixels();
}

int Text::getHeightInPixels(){
	return getTextWidthInPixels();
}

void Text::moveTo(float x, float y){
//	printf("moving Text %i to [%f,%f]\n", getUID(), x, y);
	TextParameters *params = getParams();
	params->pos.x = x;
	params->pos.y = y;
	params->pos.z = getAnchor().z;
}

void Text::getBounds(XYZ &vertice1, XYZ &vertice2){
	TextParameters *params = getParams();
	vertice1 = params->pos;
	vertice2.x = vertice1.x + getWidthInPixels();
	vertice2.y = vertice1.y + getHeightInPixels();
	vertice2.z = vertice1.z;
}
