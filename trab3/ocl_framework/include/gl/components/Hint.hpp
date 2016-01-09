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
 * Hint.hpp
 *
 * Created on: 08/Feb/2015
 * Author: Peter Frank Perroni (pfperroni@inf.ufpr.br)
 */

#ifndef _HINT_HPP_
#define _HINT_HPP_

#include "../core/WindowDimensions.hpp"
#include "../core/DOManager.hpp"
#include "Box.hpp"
#include "Text.hpp"

class Hint : public DOManager{
private:
	static void* FONT;

	char *labelText;
	Box *box;
	Text *label;
	DesignObject *doComponent;
	DOManager *doManagerComponent;

	void setup(WindowDimensions* _windowDimensions, char* _label, DesignObject *_doComponent, DOManager *_doManagerComponent, float x1, float y1, float x2, float y2);

public:
	Hint(WindowDimensions* _windowDimensions, char* _label, float x1, float y1, float x2 = -1, float y2 = -1) : DOManager(_windowDimensions, "Hint"){
		setup(_windowDimensions, _label, NULL, NULL, x1, y1, x2, y2);
	}
	Hint(WindowDimensions* _windowDimensions, DesignObject *_doComponent, float x, float y) : DOManager(_windowDimensions, "Hint"){
		setup(_windowDimensions, NULL, _doComponent, NULL, x, y, -1, -1);
	}
	Hint(WindowDimensions* _windowDimensions, DOManager *_doManagerComponent, float x, float y) : DOManager(_windowDimensions, "Hint"){
		setup(_windowDimensions, NULL, NULL, _doManagerComponent, x, y, -1, -1);
	}
	~Hint();
	void layerUp();
	void setLabel(char *_label);
	virtual void adjustContext(float x1, float y1, float x2, float y2, float r, float g, float b);
	char* getLabel();
	bool isWithinBounds(int x, int y);
	int getWidthInPixels();
	int getHeightInPixels();
	void getBounds(XYZ &vertice1, XYZ &vertice2);
};


#endif /* _HINT_HPP_ */
