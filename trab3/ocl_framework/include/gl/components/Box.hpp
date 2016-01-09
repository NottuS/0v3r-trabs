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
 * Box.hpp
 *
 * Created on: 06/Jan/2015
 * Author: Peter Frank Perroni (pfperroni@inf.ufpr.br)
 */

#ifndef _BOX_HPP_
#define _BOX_HPP_

#include "../core/DesignObject.hpp"

struct BoxParameters: DOParameters{
	XYZ coords[2];
	RGB rgbFill[2];
	RGB rgbBorder;
	bool smooth;
	BoxParameters() : smooth(false) {}
	void setCoords(float x1, float y1, float x2, float y2) {
		setCoords(x1, y1, x2, y2, coords[0].z);
	}
	void setCoords(float x1, float y1, float x2, float y2, float z) {
		coords[0].x = x1; coords[0].y = y1; coords[0].z = z;
		coords[1].x = x2; coords[1].y = y2; coords[1].z = z;
	}
	void setRgbFill(float r1, float g1, float b1, float r2, float g2, float b2) {
		rgbFill[0].r = r1; rgbFill[0].g = g1; rgbFill[0].b = b1;
		rgbFill[1].r = r2; rgbFill[1].g = g2; rgbFill[1].b = b2;
	}
	void setRgbBorder(float r, float g, float b) {
		rgbBorder.r = r; rgbBorder.g = g; rgbBorder.b = b;
	}
	void setSmooth(bool _smooth) {
		smooth = _smooth;
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

class Box: public DesignObject{
private:
	XYZ screenCoords[2];
	void refreshInternalDimensions();
public:
	Box(BoxParameters *params) : DesignObject(params->coords[0], params, "Box") {}
	BoxParameters* getParams();
	void loadToDevice(){}
	void draw();
	bool isWithinBounds(int x, int y);
	void moveTo(float x, float y);
	int getWidthInPixels();
	int getHeightInPixels();
	void getBounds(XYZ &vertice1, XYZ &vertice2);
};


#endif /* _BOX_HPP_ */
