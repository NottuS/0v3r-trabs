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
 * TextField.cpp
 *
 * Created on: 07/Jan/2015
 * Author: Peter Frank Perroni (pfperroni@inf.ufpr.br)
 */

#include "../../include/gl/components/TextField.hpp"

int TextField::BORDER_ALWAYS = 0;
int TextField::BORDER_ON_SELECTED = 1;
int TextField::BORDER_NEVER = 2;

void TextField::setup(WindowDimensions* _windowDimensions, float x, float y, int _fieldSize, char *ptr){
	fieldSize = _fieldSize;
	if(ptr != NULL){
		text = ptr;
		selfPtr = false;
	}
	else{
		text = new char[fieldSize+1];
		selfPtr = true;
	}

	border = BORDER_ALWAYS;

	char bitmapCheck[fieldSize];
	for(int i=0; i < fieldSize; i++) bitmapCheck[i] = 'M';
	bitmapCheck[fieldSize] = '\0';
	int x2 = x+Text::getTextWidthInPixels(bitmapCheck, GLUT_BITMAP_HELVETICA_12)+10;
	int y2 = y+Text::getTextHeightInPixels(GLUT_BITMAP_HELVETICA_12) + 4;
	float halfZ = _windowDimensions->getHalfPixelDepth();
	BoxParameters *boxParams1 = new BoxParameters();
	boxParams1->setCoords(x, y, x2, y2, halfZ);
	boxParams1->setRgbFill(1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f);
	boxParams1->setRgbBorder(0.5f, 0.5f, 0.5f);
	boxParams1->setSmooth(true);
	box1 = new Box(boxParams1);

	BoxParameters *boxParams2 = new BoxParameters();
	boxParams2->setCoords(x+2, y+2, x2-2, y2-2, halfZ);
	boxParams2->setRgbFill(1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f);
	boxParams2->setRgbBorder(1.0f, 1.0f, 0.0f);
	boxParams2->setSmooth(false);
	box2 = new Box(boxParams2);

	TextParameters *textParams = new TextParameters(text, x+4, y-1, halfZ, 1, 0, 0, GLUT_BITMAP_HELVETICA_12, true);
	label = new Text(textParams);

	CursorParameters *cursorParams = new CursorParameters();
	cursorParams->setCoords(x+3, y+3, x+4, y2-3, halfZ);
	cursorParams->setRgb(0.0f, 0.0f, 0.0f);
	cursor = new glCursor(this, cursorParams);

	addDesignElement(box1);
	addDesignElement(box2);
	addDesignElement(label);
	addDesignElement(cursor);

	box2->layerUp(1);
	label->layerUp(2);
	cursor->layerUp(3);

	setEditable(true);
	setText((char*)"");
	setContextualPosition(true);
	cursor->setVisible(false);
	onFocus = false;
}

TextField::~TextField(){
	if(selfPtr) delete text;
}

int TextField::mouseButton(int button, int state, int x, int y){
	select();
	requestFocus();
	return ACTION_CHECKED;
}

int TextField::keyboard(unsigned char key, int x, int y){
	if(!isEditable() || !onFocus) return ACTION_CHECKED;
	switch(key){
		case 8: // Backspace
			if(textLength == 0 || pos == 0) break;
			for(int i=pos-1; i < textLength; i++) text[i] = text[i+1];
			pos--;
			textLength--;
			slide(text, pos);
			break;

		case 127: // Delete
			if(pos == textLength) break;
			for(int i=pos; i < textLength; i++) text[i] = text[i+1];
			textLength--;
			slide(text, pos);
			break;

		case 13: // Enter
			releaseFocus();
			executeAction();
			unselect();
			break;

		case 27: // Esc
			releaseFocus();
			unselect();
			break;

		default:
			if(textLength == fieldSize) break;
			else if(pos == textLength){
				text[pos++] = key;
				text[pos] = '\0';
				textLength++;
			}
			else{
				textLength++;
				for(int i=textLength; i >= pos; i--) text[i+1] = text[i];
				text[pos++] = key;
			}
			slide(text, pos);
			break;
	}
	return ACTION_TAKEN;
}

int TextField::keyboardSpecial(int key, int x, int y){
	if(!isEditable() || !onFocus) return ACTION_CHECKED;

	int i;
	int modifier = glutGetModifiers();  //ALT=4  SHIFT=1  CTRL=2
    if(modifier != 0){
          switch(modifier){
             case 1:  break; // SHIFT
             case 2: // CTRL
             	switch(key){
         			case GLUT_KEY_LEFT:
         				if(textLength == 0 || pos == 0) break;
         				for(i=pos-1; i > 0 && text[i-1] != ' '; i--);
         				pos = i;
         				slide(text, pos);
         				break;

         			case GLUT_KEY_RIGHT:
         				if(pos == textLength) break;
         				for(i=pos+1; i < textLength && text[i-1] != ' '; i++);
         				pos = i;
         				slide(text, pos);
         				break;

         		}

            	 break;
             case 4:  break; // ALT
          }
    }
    else{
    	switch(key){
			case GLUT_KEY_LEFT:
				if(textLength == 0 || pos == 0) break;
				pos--;
				slide(text, pos);
				break;

			case GLUT_KEY_RIGHT:
				if(pos == textLength) break;
				pos++;
				slide(text, pos);
				break;

			case GLUT_KEY_HOME:
				pos = 0;
				slide(text, pos);
				break;

			case GLUT_KEY_END:
				pos = textLength;
				slide(text, pos);
				break;
		}
    }
    return ACTION_TAKEN;
}

void TextField::setText(char *_text){
	if(_text == NULL) return;
	if(!selfPtr){
		text = new char[fieldSize+1];
		TextParameters *params = label->getParams();
		params->text = text;
		selfPtr = true;
	}
	textLength = strlen(_text);
	if(textLength > fieldSize) {
		textLength = fieldSize;
	}
	strncpy(text, _text, textLength);
	text[textLength] = '\0';
	pos = textLength;
	slide(text, textLength);
}

void TextField::setTextPtr(char *textPtr){
	if(selfPtr){
		delete text;
		selfPtr = false;
	}
	TextParameters *params = label->getParams();
	params->text = textPtr;
	text = textPtr;
}

void TextField::slide(char *_text, int length){
	if(_text == NULL) return;
	if(length < 0) length = 0;
	char _text_[length+1];
	strncpy(_text_, _text, length);
	_text_[length] = '\0';
	cursor->slideHorizontal(Text::getTextWidthInPixels(_text_, GLUT_BITMAP_HELVETICA_12));
}

void TextField::setBorder(int _border){
	border = _border;
	if(border == BORDER_NEVER){
		box1->setVisible(false);
		box2->setVisible(false);
	}
	else if(border == BORDER_ON_SELECTED){
		box1->setVisible(isEditable());
		box2->setVisible(isEditable());
	}
}

void TextField::select(){
	onFocus = true;
	if(isEditable()) cursor->setVisible(true);
	if(border == BORDER_ON_SELECTED){
		box1->setVisible(true);
		box2->setVisible(true);
	}
}

void TextField::unselect(){
	onFocus = false;
	if(isEditable()) cursor->setVisible(false);
	if(border == BORDER_ON_SELECTED){
		box1->setVisible(false);
		box2->setVisible(false);
	}
}

int TextField::getWidthInPixels(){
	BoxParameters *params = box1->getParams();
	return params->coords[1].x - params->coords[0].x;
}

int TextField::getHeightInPixels(){
	BoxParameters *params = box1->getParams();
	return params->coords[1].y - params->coords[0].y;
}

bool TextField::isWithinBounds(int x, int y){
	return box1->isWithinBounds(x, y);
}

void TextField::setEditable(bool _editable){
	cursor->setVisible(_editable);
	Editable::setEditable(_editable);
}

void TextField::clearText(){
	setText((char*)"");
}

void TextField::requestFocus(){
	FocusManager::setFocus(getWindowDimensions()->getWindowHandler(), this);
}

void TextField::releaseFocus(){
	FocusManager::clearFocus(getWindowDimensions()->getWindowHandler());
}

void TextField::executeAction(){
	printf("Text entered: [%s]\n", text);
}

void TextField::getBounds(XYZ &vertice1, XYZ &vertice2){
	box1->getBounds(vertice1, vertice2);
}

int TextField::getSize(){
	return strlen(text);
}

void TextField::getText(char *_text){
	strcpy(_text, text);
}
