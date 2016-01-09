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
 * Button.hpp
 *
 * Created on: 07/Jan/2015
 * Author: Peter Frank Perroni (pfperroni@inf.ufpr.br)
 */

#ifndef _BUTTON_HPP_
#define _BUTTON_HPP_

#include "../core/WindowDimensions.hpp"
#include "../core/DOManager.hpp"
#include "Box.hpp"
#include "Text.hpp"

class Button : public DOManager{
private:
	static void* FONT;
	char *labelText;
	Box *box1;
	Box *box2;
	Text *label;

	void setup(WindowDimensions* _windowDimensions, float x1, float y1, float x2, float y2, char* _label);

public:
	Button(WindowDimensions* _windowDimensions, float x1, float y1, float x2, float y2, char* _label) : DOManager(_windowDimensions, "Button"){
		setup(_windowDimensions, x1, y1, x2, y2, _label);
	}
	Button(WindowDimensions* _windowDimensions, float x, float y, char* _label) : DOManager(_windowDimensions, "Button"){
		setup(_windowDimensions, x, y, x + Text::getTextWidthInPixels(_label, FONT) + 8,
				y + Text::getTextHeightInPixels(FONT) + 4, _label);
	}
	~Button();
	bool isWithinBounds(int x, int y);
	int mouseButton(int button, int state, int x, int y);
	int getWidthInPixels();
	int getHeightInPixels();
	void getBounds(XYZ &vertice1, XYZ &vertice2);
};

#endif /* _BUTTON_HPP_ */
