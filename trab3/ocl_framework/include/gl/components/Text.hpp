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
 * Text.hpp
 *
 * Created on: 06/Jan/2015
 * Author: Peter Frank Perroni (pfperroni@inf.ufpr.br)
 */

#ifndef _TEXT_HPP_
#define _TEXT_HPP_

#include "../core/DesignObject.hpp"
#include "../core/WindowFactory.hpp"

struct TextParameters: DOParameters{
	char *text;
	XYZ pos;
	RGB rgb;
	void *font;
	bool _3D;

	TextParameters(char * _text, float _x, float _y, float _z, float r, float g, float b, void *_font, bool _3d) {
		text = _text;
		pos.x = _x; pos.y = _y; pos.z = _z;
		rgb.r = r; rgb.g = g; rgb.b = b;
		font = _font;
		_3D = _3d;
	}
};

class Text: public DesignObject{
private:
	XYZ screenCoords;
	void refreshInternalDimensions();
public:
	Text(TextParameters *params) : DesignObject(params->pos, params, "Text") {
		setContextualPosition(true);
	}
	TextParameters* getParams();
	void loadToDevice(){}
	void draw();
	int mouseButton(int button, int state, int x, int y);
	static void rightClick(int op);
	bool isWithinBounds(int x, int y);
	static int getTextWidthInPixels(char *text, void* font);
	static int getTextHeightInPixels(void* font);
	int getTextWidthInPixels();
	int getTextHeightInPixels();
	int getWidthInPixels();
	int getHeightInPixels();
	void moveTo(float x, float y);
	void getBounds(XYZ &vertice1, XYZ &vertice2);
};


#endif /* _TEXT_HPP_ */
