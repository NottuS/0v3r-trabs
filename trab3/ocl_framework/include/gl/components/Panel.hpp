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
 * Panel.hpp
 *
 * Created on: 06/Jan/2015
 * Author: Peter Frank Perroni (pfperroni@inf.ufpr.br)
 */

#ifndef _PANEL_HPP_
#define _PANEL_HPP_

#include <map>
#include "../core/glWindow.hpp"
#include "../core/DOManagerGroup.hpp"
#include "Box.hpp"
#include "Hint.hpp"

class Panel : public DOManagerGroup{
private:
	Box *box1, *box2;
	Hint *hint;

	void setup(float x1, float y1, float x2, float y2, float r, float g, float b);

public:
	Panel(WindowDimensions* _windowDimensions, float x1, float y1, float x2, float y2, float r, float g, float b) :
			DOManagerGroup(_windowDimensions, "Panel"){
		setup(x1, y1, x2, y2, r, g, b);
	}
	~Panel(){}
	bool isWithinBounds(int x, int y);
	int mouseButton(int button, int state, int x, int y);
	static void rightClick(int op);
	int getWidthInPixels();
	int getHeightInPixels();
	void getBounds(XYZ &vertice1, XYZ &vertice2);
	void setHint(char *label);
	int mouseNoPassiveMove(int x, int y);
	int mousePassiveMove(int x, int y);
};

#endif /* _PANEL_HPP_ */
