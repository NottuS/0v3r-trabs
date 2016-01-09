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
 * Panel.cpp
 *
 * Created on: 06/Jan/2015
 * Author: Peter Frank Perroni (pfperroni@inf.ufpr.br)
 */

#include "../../include/gl/components/Panel.hpp"

void Panel::setup(float x1, float y1, float x2, float y2, float r, float g, float b){
	hint = NULL;

	BoxParameters *params2 = new BoxParameters();
	params2->setCoords(x1+2, y1+2, x2-2, y2-2, getWindowDimensions()->getHalfPixelDepth());
	params2->setRgbFill(r, g, b, r, g, b);
	float colorRate = (r+g+b>2.7f)?0.9f:1.1f;
	DesignObject::changeColorIntensity(r, g, b, colorRate);
	params2->setRgbBorder(r, g, b);
	params2->setSmooth(false);
	box2 = new Box(params2);

	BoxParameters *params1 = new BoxParameters();
	params1->setCoords(x1, y1, x2, y2, getWindowDimensions()->getHalfPixelDepth());
	colorRate = (r+g+b>2.7f)?0.9f:1.1f;
	DesignObject::changeColorIntensity(r, g, b, colorRate);
	params1->setRgbFill(r, g, b, r, g, b);
	colorRate = (r+g+b>2.5f)?0.8f:1.2f;
	DesignObject::changeColorIntensity(r, g, b, colorRate);
	params1->setRgbBorder(r, g, b);
	params1->setSmooth(false);
	box1 = new Box(params1);

	addDesignElement(box1);
	addDesignElement(box2);

	box2->layerUp(1);

	setContextualPosition(true);
}

int Panel::mouseButton(int button, int state, int x, int y){
	int ret = ACTION_IGNORED;
	if(button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN && FocusManager::getFocus(glutGetWindow()) == NULL){
		int submenuRotate;
		submenuRotate = glutCreateMenu(rightClick);
		glutAddMenuEntry("X", 0);
		glutAddMenuEntry("Y", 1);
		glutAddMenuEntry("Z", 2);
		glutAddMenuEntry("XY", 3);
		glutAddMenuEntry("XZ", 4);
		glutAddMenuEntry("YZ", 5);
		glutAddMenuEntry("XYZ", 6);
		glutAddMenuEntry("Reset", 7);

		glutCreateMenu(Panel::rightClick);
		glutAddSubMenu("Rotate", submenuRotate);
		ret |= ACTION_CHECKED;
	}

	ret |= DOManagerGroup::mouseButton(button, state, x, y);
	return ret;
}

bool Panel::isWithinBounds(int x, int y){
	return (isContextualPosition() && isOnMove()) || box1->isWithinBounds(x, y);
}

void Panel::rightClick(int op){
	glTranslatef(0, 0, 0);
	if(op == 0) glRotatef(5, 1, 0, 0);
	else if(op == 1) glRotatef(5, 0, 1, 0);
	else if(op == 2) glRotatef(5, 0, 0, 1);
	else if(op == 3) glRotatef(5, 1, 1, 0);
	else if(op == 4) glRotatef(5, 1, 0, 1);
	else if(op == 5) glRotatef(5, 0, 1, 1);
	else if(op == 6) glRotatef(5, 1, 1, 1);
	else if(op == 7) glLoadIdentity();
	printf("Selected option %i.\n", op);
}

int Panel::getWidthInPixels(){
	BoxParameters *params = box1->getParams();
	return params->coords[1].x - params->coords[0].x;
}

int Panel::getHeightInPixels(){
	BoxParameters *params = box1->getParams();
	return params->coords[1].y - params->coords[0].y;
}

void Panel::getBounds(XYZ &vertice1, XYZ &vertice2){
	box1->getBounds(vertice1, vertice2);
}

void Panel::setHint(char *label){
	if(hint == NULL){
		XYZ pos1, pos2;
		getBounds(pos1, pos2);
		hint = new Hint(getWindowDimensions(), label, pos1.x + (pos2.x - pos1.x)/4, pos1.y + (pos2.y - pos1.y)/4);
		addDesignElement(hint);
		hint->layersUp(3);
		hint->setVisible(false);
	}
	else{
		hint->setLabel(label);
	}
}

int Panel::mouseNoPassiveMove(int x, int y){
	if(hint != NULL){
		hint->setVisible(false);
		return ACTION_TAKEN;
	}
	return DOManagerGroup::mouseNoPassiveMove(x, y);
}

int Panel::mousePassiveMove(int x, int y){
	if(hint != NULL){
		hint->setVisible(true);
		return ACTION_TAKEN;
	}
	return DOManagerGroup::mousePassiveMove(x, y);
}
