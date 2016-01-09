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
 * TextArea.hpp
 *
 * Created on: 03/Feb/2015
 * Author: Peter Frank Perroni (pfperroni@inf.ufpr.br)
 */

#ifndef _TEXTAREA_HPP_
#define _TEXTAREA_HPP_

#include "../core/DOManagerGroup.hpp"
#include "TextField.hpp"
#include "Panel.hpp"
#include "Text.hpp"

class TextArea : public DOManagerGroup{
private:
	omp_lock_t mutex_self;
	Panel *panel1, *panel2;
	TextField **lines;
	Text *msg;
	char *textPtr, *currPtr, *sMsg;
	int nRows, nCols, maxSize;
	int currRow, totalRows;
	int width, height;
	bool endReached, beginReached;

	void setup(WindowDimensions* _windowDimensions, float x, float y, int rows, int cols);
	void adjustText();
	int returnLines(int nLines);
	int advanceLines(int nLines);

public:
	TextArea(WindowDimensions* _windowDimensions, float x, float y, int rows, int cols) : DOManagerGroup(_windowDimensions, "TextArea"){
		setup(_windowDimensions, x, y, rows, cols);
	}
	~TextArea();
	void setText(char *text);
	void appendText(char *text);
	void clear();
	char* getText();
	int getSize();
	int getWidthInPixels();
	int getHeightInPixels();
	void getBounds(XYZ &vertice1, XYZ &vertice2);
	void setEditable(bool _editable);
	void disableEdition();
	void setVisible(bool _visible);
	int keyboard(unsigned char key, int x, int y);
	int keyboardSpecial(int key, int x, int y);
	int countLines();
	int mouseButton(int button, int state, int x, int y);
	static void rightClick(int op);
};


#endif /* _TEXTAREA_HPP_ */
