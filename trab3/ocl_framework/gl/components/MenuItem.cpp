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
 * MenuItem.cpp
 *
 * Created on: 07/Jan/2015
 * Author: Peter Frank Perroni (pfperroni@inf.ufpr.br)
 */

#include "../../include/gl/components/MenuItem.hpp"

void* MenuItem::FONT = GLUT_BITMAP_HELVETICA_12;

void MenuItem::setup(WindowDimensions* _windowDimensions, char* _label, DesignObject *_doComponent, DOManager *_doManagerComponent){
	hasCoords = false;
	labelText = NULL;
	label = NULL;
	doComponent = _doComponent;
	doManagerComponent = _doManagerComponent;

	float away = _windowDimensions->getPixelDepth() + _windowDimensions->getPixelHeight() + _windowDimensions->getPixelWidth();
	BoxParameters *boxParams = new BoxParameters();
	boxParams->setCoords(-away, -away, -away, -away, -away);
	boxParams->setRgbFill(1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f);
	boxParams->setRgbBorder(0.5f, 0.5f, 0.5f);
	boxParams->setSmooth(false);
	box = new Box(boxParams);
	addDesignElement(box);

	if(_label != NULL){
		TextParameters *textParams = new TextParameters(labelText, -1000, -1000, -away, 0, 0, 0, FONT, true);
		label = new Text(textParams);
		setLabel(_label);
		addDesignElement(label);
		label->layerUp(1);
	}
	else if(doComponent != NULL){
		addDesignElement(doComponent);
		doComponent->layerUp(1);
	}
	else if(doManagerComponent != NULL){
		addDesignElement(doManagerComponent);
		doManagerComponent->layersUp(1);
	}

//	layerUp();

	setContextualPosition(true);
	setVisible(false);
}

MenuItem::~MenuItem(){
	delete labelText;
}

void MenuItem::adjustContext(float x1, float y1, float x2, float y2, float r, float g, float b){
	RGB textColor = {(float)((int)(r)^1), (float)((int)(g)^1), (float)((int)(b)^1)};
	XYZ anchor = {x1, y1, 0.0f};

	// The Box is the area that will control the MenuItem bounds, so it must be fixed. Always.
	BoxParameters *boxParams = box->getParams();
	boxParams->setCoords(x1, y1, x2, y2);
	DesignObject::changeColorIntensity(r, g, b, 1.2f);
	boxParams->setRgbFill(r, g, b, r, g, b);
	DesignObject::changeColorIntensity(r, g, b, 1.2f);
	boxParams->setRgbBorder(r, g, b);
	box->moveAnchor(anchor);

	if(label != NULL){
		anchor.x = x1 + 2; anchor.y = y1 - 1;
		TextParameters *textParams = label->getParams();
	//	textParams->pos.x = anchor.x;
	//	textParams->pos.y = anchor.y;
		textParams->rgb = textColor;
		label->moveAnchor(anchor);
//printf("menu %s moved to [%f, %f] %f %f\n", textParams->text, textParams->pos.x, textParams->pos.y, x1, y1);
	}
	else if(doComponent != NULL){
		doComponent->moveAnchor(anchor);
	}
	else if(doManagerComponent != NULL){
		doManagerComponent->moveAnchors(anchor.x, anchor.y);
	}
	hasCoords = true;
}

void MenuItem::setLabel(char *_label){
	if(_label == NULL) return;
	if(labelText != NULL) delete labelText;
	labelText = new char[strlen(_label)];
	strcpy(labelText, _label);

	TextParameters *textParams = label->getParams();
	textParams->text = labelText;
}

void MenuItem::layerUp(){
	box->layerDown(1);
	label->layerDown(2);
}
void MenuItem::setVisible(bool _visible){
	if(hasCoords){
		DOManager::setVisible(_visible);
	}
}

int MenuItem::action(int button, int state, int x, int y){
	int ret = ACTION_CHECKED;
	if(label != NULL){
		printf("clicked menu item %s!\n", label->getParams()->text);
		ret = ACTION_TAKEN;
	}
	else if(doComponent != NULL){
		ret = doComponent->mouseButton(button, state, x, y);
	}
	else if(doManagerComponent != NULL){
		ret = doManagerComponent->mouseButton(button, state, x, y);
	}
	return ret;
}

char* MenuItem::getLabel(){
	return labelText;
}

bool MenuItem::isWithinBounds(int x, int y){
	return box->isWithinBounds(x, y);
}

int MenuItem::mouseButton(int button, int state, int x, int y){
	int ret = action(button, state, x, y);
	return ret;
}

int MenuItem::getWidthInPixels(){
	if(label != NULL) return label->getWidthInPixels();
	else if(doComponent != NULL) return doComponent->getWidthInPixels();
	else if(doManagerComponent != NULL) return doManagerComponent->getWidthInPixels();
	return 0;
}

int MenuItem::getHeightInPixels(){
	if(label != NULL) return label->getHeightInPixels();
	else if(doComponent != NULL) return doComponent->getHeightInPixels();
	else if(doManagerComponent != NULL) return doManagerComponent->getHeightInPixels();
	return 0;
}

void MenuItem::getBounds(XYZ &vertice1, XYZ &vertice2){
	box->getBounds(vertice1, vertice2);
}
