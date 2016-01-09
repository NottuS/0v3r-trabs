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
 * TextField.hpp
 *
 * Created on: 07/Jan/2015
 * Author: Peter Frank Perroni (pfperroni@inf.ufpr.br)
 */

#ifndef _TEXTFIELD_HPP_
#define _TEXTFIELD_HPP_

#include "../core/WindowDimensions.hpp"
#include "../core/DOManager.hpp"
#include "Box.hpp"
#include "Text.hpp"
#include "glCursor.hpp"

class TextField : public DOManager{
private:
	int fieldSize;
	int pos;
	int textLength;
	int border;
	char *text;
	Box *box1;
	Box *box2;
	Text *label;
	glCursor *cursor;
	bool selfPtr;
	bool onFocus;

	void select();
	void unselect();
	void setup(WindowDimensions* _windowDimensions, float x, float y, int _fieldSize, char *ptr);

public:
	static int BORDER_ALWAYS;
	static int BORDER_ON_SELECTED;
	static int BORDER_NEVER;

	TextField(WindowDimensions* _windowDimensions, float x, float y, int _fieldSize, char *ptr = NULL) : DOManager(_windowDimensions, "TextField"){
		setup(_windowDimensions, x, y, _fieldSize, ptr);
	}
	~TextField();
	virtual void executeAction();
	bool isWithinBounds(int x, int y);
	void setEditable(bool _editable);
	int mouseButton(int button, int state, int x, int y);
	int keyboard(unsigned char key, int x, int y);
	int keyboardSpecial(int key, int x, int y);
	void setText(char *_text);
	void setTextPtr(char *textPtr);
	void setBorder(int _border);
	void clearText();
	void slide(char *_text, int length);
	void requestFocus();
	void releaseFocus();
	int getWidthInPixels();
	int getHeightInPixels();
	void getBounds(XYZ &vertice1, XYZ &vertice2);
	int getSize();
	void getText(char *_text);
};

#endif /* _TEXTFIELD_HPP_ */
