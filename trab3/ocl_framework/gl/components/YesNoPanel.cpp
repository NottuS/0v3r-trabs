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
 * YesNoPanel.cpp
 *
 * Created on: 06/Jan/2015
 * Author: Peter Frank Perroni (pfperroni@inf.ufpr.br)
 */

#include "../../include/gl/components/YesNoPanel.hpp"

void YesNoPanel::setup(float x, float y, char* msgText){
	TextParameters *textParams = new TextParameters(msgText, x+5, y, getWindowDimensions()->getHalfPixelDepth(), 1, 0, 0, GLUT_BITMAP_HELVETICA_12, true);
	msg = new Text(textParams);

	yesButton = new Button(getWindowDimensions(), x+20, y+20, x+50, y+40, (char*)"Yes");
	noButton = new Button(getWindowDimensions(), x+70, y+20, x+100, y+40, (char*)"No");

	addDesignElement(yesButton);
	addDesignElement(noButton);
	addDesignElement(msg);

	setContextualPosition(true);
}

int YesNoPanel::getWidthInPixels(){
	return msg->getWidthInPixels();
}

int YesNoPanel::getHeightInPixels(){
	return msg->getHeightInPixels() + 20 + yesButton->getHeightInPixels();
}

void YesNoPanel::getBounds(XYZ &vertice1, XYZ &vertice2){
	XYZ fake;
	msg->getBounds(vertice1, fake);
	noButton->getBounds(fake, vertice2);
}

bool YesNoPanel::isWithinBounds(int x, int y){
	return yesButton->isWithinBounds(x, y) || noButton->isWithinBounds(x, y) || msg->isWithinBounds(x, y);
}
