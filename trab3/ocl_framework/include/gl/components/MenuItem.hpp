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
 * MenuItem.hpp
 *
 * Created on: 07/Jan/2015
 * Author: Peter Frank Perroni (pfperroni@inf.ufpr.br)
 */

#ifndef _MENUITEM_HPP_
#define _MENUITEM_HPP_

#include "../core/WindowDimensions.hpp"
#include "../core/DOManager.hpp"
#include "Box.hpp"
#include "Text.hpp"

class MenuItem : public DOManager{
private:
	static void* FONT;

	char *labelText;
	Box *box;
	Text *label;
	DesignObject *doComponent;
	DOManager *doManagerComponent;
	bool hasCoords;

	void setup(WindowDimensions* _windowDimensions, char* _label, DesignObject *_doComponent, DOManager *_doManagerComponent);

public:
	MenuItem(WindowDimensions* _windowDimensions, char* _label = NULL) : DOManager(_windowDimensions, "MenuItem"){
		setup(_windowDimensions, _label, NULL, NULL);
	}
	MenuItem(WindowDimensions* _windowDimensions, DesignObject *_doComponent) : DOManager(_windowDimensions, "MenuItem"){
		setup(_windowDimensions, NULL, _doComponent, NULL);
	}
	MenuItem(WindowDimensions* _windowDimensions, DOManager *_doManagerComponent) : DOManager(_windowDimensions, "MenuItem"){
		setup(_windowDimensions, NULL, NULL, _doManagerComponent);
	}
	~MenuItem();
	void layerUp();
	void setLabel(char *_label);
	void setVisible(bool _visible);
	virtual void adjustContext(float x1, float y1, float x2, float y2, float r, float g, float b);
	virtual int action(int button, int state, int x, int y);
	char* getLabel();
	bool isWithinBounds(int x, int y);
	int mouseButton(int button, int state, int x, int y);
	int getWidthInPixels();
	int getHeightInPixels();
	void getBounds(XYZ &vertice1, XYZ &vertice2);
};

#endif /* _MENUITEM_HPP_ */
