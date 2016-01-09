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
 * glCursor.hpp
 *
 * Created on: 06/Jan/2015
 * Author: Peter Frank Perroni (pfperroni@inf.ufpr.br)
 */

#ifndef _GLCURSOR_HPP_
#define _GLCURSOR_HPP_

#include "../core/DesignObject.hpp"
#include "../core/DOManager.hpp"

struct CursorParameters: DOParameters{
	XYZ coords[2];
	RGB rgb;
	void setCoords(float x1, float y1, float x2, float y2, float z) {
		coords[0].x = x1; coords[0].y = y1; coords[0].z = z;
		coords[1].x = x2; coords[1].y = y2; coords[1].z = z;
	}
	void setRgb(float r, float g, float b) {
		rgb.r = r; rgb.g = g; rgb.b = b;
	}
	void move(float x, float y){
		move(x, y, coords[0].z);
	}
	void move(float x, float y, float z){
		float diffX = coords[1].x - coords[0].x;
		float diffY = coords[1].y - coords[0].y;
		coords[0].x = x; coords[0].y = y; coords[0].z = z;
		coords[1].x = x+diffX; coords[1].y = y+diffY; coords[1].z = z;
	}
};

class glCursor: public DesignObject{
private:
	XYZ screenCoords[2];
	Editable *editable;
	void refreshInternalDimensions();

public:
	glCursor(CursorParameters *params) : DesignObject(params->coords[0], params, "glCursor"){
		editable = NULL;
	}
	glCursor(Editable *_editable, CursorParameters *params) : DesignObject(params->coords[0], params, "glCursor"){
		editable = _editable;
	}
	CursorParameters* getParams();
	void loadToDevice(){}
	void draw();
	bool isWithinBounds(int x, int y);
	bool isVisible();
	void slideHorizontal(float dist);
	void slideVertical(float dist);
	void slide(float xDist, float yDist);
	void moveTo(float x, float y);
	int getWidthInPixels();
	int getHeightInPixels();
	void getBounds(XYZ &vertice1, XYZ &vertice2);
};


#endif /* _GLCURSOR_HPP_ */
