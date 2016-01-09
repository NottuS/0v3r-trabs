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
 * TextArea.cpp
 *
 * Created on: 03/Feb/2015
 * Author: Peter Frank Perroni (pfperroni@inf.ufpr.br)
 */

#include "../../include/gl/components/TextArea.hpp"

void TextArea::setup(WindowDimensions* _windowDimensions, float x, float y, int rows, int cols){
	omp_init_lock(&mutex_self);

	nRows = rows;
	nCols = cols;
	maxSize = nRows * nCols;
	currRow = 0;
	totalRows = rows;
	endReached = beginReached = false;

	height = 0;
	sMsg = new char[cols + 1];
	textPtr = new char[rows * cols + 1];
	currPtr = textPtr;
	lines = new TextField*[rows];
	char *ptr = textPtr;
	int i;
	for(i=0; i < rows; i++, ptr += cols){
		lines[i] = new TextField(_windowDimensions, x+2, y+2+height, cols, ptr);
		lines[i]->setBorder(TextField::BORDER_NEVER);
		height += lines[i]->getHeightInPixels();
	}
	width = lines[0]->getWidthInPixels();

	TextParameters *textParams = new TextParameters(sMsg, x+4, y+height, _windowDimensions->getHalfPixelDepth(), 0, 0, 1, GLUT_BITMAP_HELVETICA_12, true);
	msg = new Text(textParams);

	panel1 = new Panel(_windowDimensions, x, y, width+4, y+4+height+Text::getTextHeightInPixels(GLUT_BITMAP_HELVETICA_12), 0.9, 0.9, 0.9);
	panel2 = new Panel(_windowDimensions, x+2, y+2, width+2, y+2+height, 1, 1, 1);

	for(i=0; i < rows; i++){
		panel2->addDesignElement(lines[i]);
	}
	panel1->addDesignElement(msg);

	addDesignElement(panel1);
	addDesignElement(panel2);

	setEditable(false);
}

TextArea::~TextArea(){
	delete lines;
	delete textPtr;

	omp_destroy_lock(&mutex_self);
}

void TextArea::appendText(char *text){
	omp_set_lock(&mutex_self);
	if(text == NULL) return;
	int inTextLength = strlen(text);
	int textLength = strlen(textPtr);
	int total = inTextLength + textLength;
	if(total > maxSize) {
		char *ptr = textPtr;
		textPtr = new char[total + 1];
		maxSize = total;
		strcpy(textPtr, ptr);
		delete ptr;
	}
	strcpy(textPtr+textLength, text);
	currPtr = textPtr;
	currRow = 0;
	endReached = beginReached = false;

	adjustText();
	omp_unset_lock(&mutex_self);

	totalRows = countLines();
}

void TextArea::setText(char *text){
	omp_set_lock(&mutex_self);
	if(text == NULL) return;
	int textLength = strlen(text);
	if(textLength > maxSize) {
		maxSize = textLength;
		delete textPtr;
		textPtr = new char[maxSize + 1];
	}
	strcpy(textPtr, text);
	currPtr = textPtr;
	currRow = 0;
	endReached = beginReached = false;

	adjustText();
	omp_unset_lock(&mutex_self);

	totalRows = countLines();
}

void TextArea::adjustText(){
	if(currPtr == textPtr) beginReached = true;
	char *ptr = currPtr;
	char line[nCols+1];
	for(int j, i=0; i < nRows && *ptr != '\0'; i++){
		for(j=0; j < nCols && *ptr != '\0'; j++, ptr++){
			line[j] = *ptr;
			if(line[j] == '\n' || line[j] == '\r'){
				ptr++;
				break;
			}
		}
		line[j] = '\0';
		lines[i]->setText(line);
	}
	if(*ptr == '\0') endReached = true;
	sprintf(sMsg, "Line: %i-%i/%i", currRow+1, (currRow+nRows>totalRows)?totalRows:currRow+nRows, totalRows);
	if(endReached) sprintf(sMsg, "%s (End)", sMsg);
	if(beginReached) sprintf(sMsg, "%s (Begin)", sMsg);
}

void TextArea::clear(){
	omp_set_lock(&mutex_self);
	for(int i=0; i < nRows; i++){
		lines[i]->clearText();
	}
	omp_unset_lock(&mutex_self);
}

char* TextArea::getText(){
	return textPtr;
}

int TextArea::getSize(){
	omp_set_lock(&mutex_self);
	int sz = 0;
	for(int i=0; i < nRows; i++){
		sz += lines[i]->getSize();
	}
	omp_unset_lock(&mutex_self);

	return sz;
}

void TextArea::getBounds(XYZ &vertice1, XYZ &vertice2){
	XYZ fake;
	lines[0]->getBounds(vertice1, fake);
	lines[nRows-1]->getBounds(fake, vertice2);
}

int TextArea::getWidthInPixels(){
	return width;
}

int TextArea::getHeightInPixels(){
	return height;
}

void TextArea::setVisible(bool _visible){
	DOManagerGroup::setVisible(_visible);
	if(_visible){
		for(int i=0; i < nRows; i++){
			lines[i]->setBorder(TextField::BORDER_NEVER);
		}
	}
}

void TextArea::setEditable(bool _editable){
	omp_set_lock(&mutex_self);
	for(int i=0; i < nRows; i++){
		lines[i]->setEditable(_editable);
	}
	Editable::setEditable(_editable);
	omp_unset_lock(&mutex_self);
}

int TextArea::keyboard(unsigned char key, int x, int y){
	return DOManagerGroup::keyboard(key, x, y);
}

int TextArea::keyboardSpecial(int key, int x, int y){
	int modifier = glutGetModifiers();  //ALT=4  SHIFT=1  CTRL=2
    if(modifier != 0){
          switch(modifier){
             case 1:  break; // SHIFT
             case 2: // CTRL
             	switch(key){
         			case GLUT_KEY_HOME:
    					beginReached = true;
    					endReached = false;
    					currPtr = textPtr;
    					currRow = 0;
         				break;

         			case GLUT_KEY_END:
    					beginReached = false;
    					endReached = true;
    					currPtr = textPtr+maxSize;
    					currRow = countLines();
        				returnLines(nRows);
         				break;
         		}

            	 break;
             case 4:  break; // ALT
          }
    }
    else{
    	switch(key){
			case GLUT_KEY_UP:
				if(!beginReached){
					endReached = false;
					returnLines(1);
				}
				break;

			case GLUT_KEY_DOWN:
				if(!endReached){
					beginReached = false;
					advanceLines(1);
				}
				break;

			case GLUT_KEY_PAGE_UP:
				if(!beginReached){
					endReached = false;
					returnLines(nRows);
				}
				break;

			case GLUT_KEY_PAGE_DOWN:
				if(!endReached){
					beginReached = false;
					advanceLines(nRows);
				}
				break;
    	}
    }
    adjustText();
    return ACTION_TAKEN;
}

int TextArea::advanceLines(int nLines){
	omp_set_lock(&mutex_self);
	int count;
	for(count=0; count < nLines && !endReached; count++){
		currRow++;
		currPtr += lines[0]->getSize();
		if(*currPtr == '\n' || *currPtr == '\r') currPtr++;
		adjustText();
	}
	omp_unset_lock(&mutex_self);

	return count;
}

int TextArea::returnLines(int nLines){
	omp_set_lock(&mutex_self);
	int count;
	for(count=0; count < nLines && currPtr != textPtr; count++){
		currRow--;
		if(*(currPtr-1) == '\n' || *(currPtr-1) == '\r'){
			currPtr -= (currPtr-1 != textPtr)? 2 : 1;
		}
		for(int i=0; i < nCols && currPtr != textPtr; i++, currPtr--){
			if(*currPtr == '\n' || *currPtr == '\r'){
				currPtr++;
				break;
			}
		}
	}
	if(currPtr == textPtr) beginReached = true;
	omp_unset_lock(&mutex_self);

	return count;
}

int TextArea::countLines(){
	omp_set_lock(&mutex_self);
	char *ptr = textPtr;
	int count;
	for(count=0; *ptr != '\0'; count++){
		for(int j=0; j < nCols && *ptr != '\0'; j++, ptr++){
			if(*ptr == '\n' || *ptr == '\r'){
				ptr++;
				break;
			}
		}
	}
	omp_unset_lock(&mutex_self);

	return count;
}

int TextArea::mouseButton(int button, int state, int x, int y){
	int ret = ACTION_IGNORED;
	if(button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN){
		int submenuTextArea;
		submenuTextArea = glutCreateMenu(TextArea::rightClick);
		glutAddMenuEntry("Print to Standard Output", 0);
		glutAddMenuEntry("Remove from the screen", 1);
		glutAddMenuEntry("Hide", 2);

		glutCreateMenu(TextArea::rightClick);
		glutAddSubMenu("TextArea", submenuTextArea);

		FocusManager::setFocus(getWindowDimensions()->getWindowHandler(), this);
		ret |= ACTION_CHECKED;
	}

//	ret |= DOManagerGroup::mouseButton(button, state, x, y);
	return ret;
}

void TextArea::rightClick(int op){
	TextArea *textArea = (TextArea*)FocusManager::getFocus(glutGetWindow());
	FocusManager::clearFocus(glutGetWindow());
	if(op == 0){
		printf("TextArea stored log: [%s]\n", textArea->getText());
	}
	else if(op == 1){
		textArea->removeMe();
	}
	else if(op == 2){
		textArea->setVisible(false);
	}
}
