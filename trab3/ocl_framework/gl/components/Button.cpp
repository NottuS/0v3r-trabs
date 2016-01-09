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
 * Button.cpp
 *
 * Created on: 07/Jan/2015
 * Author: Peter Frank Perroni (pfperroni@inf.ufpr.br)
 */

#include "../../include/gl/components/Button.hpp"

void* Button::FONT = GLUT_BITMAP_HELVETICA_12;

void Button::setup(WindowDimensions* _windowDimensions, float x1, float y1, float x2, float y2, char* _label){
	if(_label != NULL){
		labelText = new char[strlen(_label)];
		strcpy(labelText, _label);
	}

	float halfZ = _windowDimensions->getHalfPixelDepth();
	BoxParameters *boxParams1 = new BoxParameters();
	boxParams1->setCoords(x1, y1, x2, y2, halfZ);
	boxParams1->setRgbFill(1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f);
	boxParams1->setRgbBorder(0.5f, 0.5f, 0.5f);
	boxParams1->setSmooth(true);
	box1 = new Box(boxParams1);

	BoxParameters *boxParams2 = new BoxParameters();
	boxParams2->setCoords(x1+2, y1+2, x2-2, y2-2, halfZ);
	boxParams2->setRgbFill(1.0f, 1.0f, 0.5f, 1.0f, 1.0f, 0.5f);
	boxParams2->setRgbBorder(1.0f, 1.0f, 0.0f);
	boxParams2->setSmooth(false);
	box2 = new Box(boxParams2);

	TextParameters *textParams = new TextParameters(labelText, x1+4, y1-1, halfZ, 1, 0, 0, FONT, true);
	label = new Text(textParams);

	addDesignElement(box1);
	addDesignElement(box2);
	addDesignElement(label);
	box2->layerUp(1);
	label->layerUp(2);

	setContextualPosition(true);
}

Button::~Button(){
	delete labelText;
}

bool Button::isWithinBounds(int x, int y) {
//BoxParameters *params = box1->getParams();
//printf("Button[%i,%i,%i,%i]\n", params->coords[0].x,params->coords[0].y,params->coords[1].x,params->coords[1].y);
	return box1->isWithinBounds(x, y);
}

int Button::mouseButton(int button, int state, int x, int y){
	if(button == GLUT_LEFT_BUTTON && state == GLUT_DOWN){
		printf("clicked button %s!\n", label->getParams()->text);
		return ACTION_TAKEN;
	}
	return ACTION_CHECKED;
}

int Button::getWidthInPixels(){
	BoxParameters *params = box1->getParams();
	return params->coords[1].x - params->coords[0].x;
}

int Button::getHeightInPixels(){
	BoxParameters *params = box1->getParams();
	return params->coords[1].y - params->coords[0].y;
}

void Button::getBounds(XYZ &vertice1, XYZ &vertice2){
	box1->getBounds(vertice1, vertice2);
}
