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
 * TrackXY.hpp
 *
 * Created on: 07/Jan/2015
 * Author: Peter Frank Perroni (pfperroni@inf.ufpr.br)
 */

#ifndef _TRACKXY_HPP_
#define _TRACKXY_HPP_

#include "../core/WindowDimensions.hpp"
#include "../core/DOManager.hpp"
#include "Text.hpp"

class TrackXY : public DOManager{
private:
	char posText[30];
	struct TextParameters *textParams;
	Text *pos;

	int mousePassiveMove(int x, int y);

public:
	TrackXY(WindowDimensions* _windowDimensions, float x, float y, float r, float g, float b) : DOManager(_windowDimensions, "TrackXY"){
		posText[0] = '\0';
		textParams = new TextParameters(posText, x, y, _windowDimensions->getHalfPixelDepth(), r, g, b, GLUT_BITMAP_HELVETICA_12, true);
		pos = new Text(textParams);
		addDesignElement(pos);
	}
	bool isWithinBounds(int x, int y);
	int getWidthInPixels();
	int getHeightInPixels();
	void getBounds(XYZ &vertice1, XYZ &vertice2);
};

#endif /* _TRACKXY_HPP_ */
