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

#include "../../include/gl/components/Hint.hpp"

void* Hint::FONT = GLUT_BITMAP_HELVETICA_12;

void Hint::setup(WindowDimensions* _windowDimensions, char* _label, DesignObject *_doComponent, DOManager *_doManagerComponent, float x1, float y1, float x2, float y2){
	labelText = NULL;
	label = NULL;
	doComponent = _doComponent;
	doManagerComponent = _doManagerComponent;

	BoxParameters *boxParams = new BoxParameters();
	boxParams->setRgbFill(0.9f, 0.9f, 0.9f, 0.95f, 0.95f, 0.95f);
	boxParams->setRgbBorder(0, 0, 0);
	boxParams->setSmooth(true);
	box = new Box(boxParams);
	addDesignElement(box);

	if(_label != NULL){
		int textWidth = Text::getTextWidthInPixels(_label, FONT) + 4;
		int textHeight = Text::getTextHeightInPixels(FONT) + 4;
		float _x1 = x1;
		float _x2 = x1 + textWidth;
		if(x2 < _x2) x2 = _x2;
		else _x1 = x1 + ((_x2 - x1) / 2 - textHeight / 2);
		float _y1 = y1;
		float _y2 = y1 + textHeight;
		if(y2 < _y2) y2 = _y2;
		else _y1 = y1 + ((_y2 - y1) / 2 - textHeight / 2);
		TextParameters *textParams = new TextParameters(labelText, _x1, _y1-1, _windowDimensions->getHalfPixelDepth(), 1, 1, 1, FONT, true);
		label = new Text(textParams);
		setLabel(_label);
		addDesignElement(label);
		label->layerUp(1);
//printf("hint label=%s\n", label->getParams()->text);
	}
	else if(doComponent != NULL){
		x2 = x1 + doComponent->getWidthInPixels();
		y2 = y1 + doComponent->getHeightInPixels();
		addDesignElement(doComponent);
		doComponent->layerUp(1);
	}
	else if(doManagerComponent != NULL){
		x2 = x1 + doManagerComponent->getWidthInPixels();
		y2 = y1 + doManagerComponent->getHeightInPixels();
		addDesignElement(doManagerComponent);
		doManagerComponent->layersUp(1);
	}
	boxParams->setCoords(x1, y1, x2, y2);

//	layerUp();

	setContextualPosition(true);
	setVisible(false);
}

Hint::~Hint(){
	delete labelText;
}

void Hint::adjustContext(float x1, float y1, float x2, float y2, float r, float g, float b){
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
}

void Hint::setLabel(char *_label){
	if(_label == NULL) return;
	if(labelText != NULL) delete labelText;
	labelText = new char[strlen(_label)];
	strcpy(labelText, _label);

	TextParameters *textParams = label->getParams();
	textParams->text = labelText;
}

void Hint::layerUp(){
	box->layerDown(1);
	label->layerDown(2);
}

char* Hint::getLabel(){
	return labelText;
}

bool Hint::isWithinBounds(int x, int y){
	return box->isWithinBounds(x, y);
}

int Hint::getWidthInPixels(){
	if(label != NULL) return label->getWidthInPixels();
	else if(doComponent != NULL) return doComponent->getWidthInPixels();
	else if(doManagerComponent != NULL) return doManagerComponent->getWidthInPixels();
	return 0;
}

int Hint::getHeightInPixels(){
	if(label != NULL) return label->getHeightInPixels();
	else if(doComponent != NULL) return doComponent->getHeightInPixels();
	else if(doManagerComponent != NULL) return doManagerComponent->getHeightInPixels();
	return 0;
}

void Hint::getBounds(XYZ &vertice1, XYZ &vertice2){
	box->getBounds(vertice1, vertice2);
}
