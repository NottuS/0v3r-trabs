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
 * Movable.cpp
 *
 * Created on: 27/Jan/2015
 * Author: Peter Frank Perroni (pfperroni@inf.ufpr.br)
 */

#include "../../include/gl/core/Movable.hpp"

Movable::Movable(){
	moveEnabled = false;
	onMove = false;
	contextualPosition = false;
}

bool Movable::isMoveEnabled(){
	return moveEnabled;
}

void Movable::setMoveEnabled(bool _moveEnabled){
//	if(moveEnabled && !_moveEnabled)printf("turning off onMove for obj=%p\n", this);
//	if(!moveEnabled && _moveEnabled)printf("turning on onMove for obj=%p\n", this);
	moveEnabled = _moveEnabled;
	curPos.x = curPos.y = curPos.z = 0;
	if(!moveEnabled) onMove = false;
}
bool Movable::isOnMove(){
	return onMove;
}

void Movable::setOnMove(bool _onMove){
	onMove = _onMove;
}
bool Movable::isContextualPosition(){
	return contextualPosition;
}

void Movable::setContextualPosition(bool _contextualPosition){
	contextualPosition = _contextualPosition;
}

XYZ Movable::getLastMoveSize(float x, float y){
	XYZ sz = {0,0};
	if(curPos.x != 0 || curPos.y != 0){
		sz.x = x - curPos.x; sz.y = y - curPos.y;
	}
	curPos.x = x, curPos.y = y;
	return sz;
}


/*void Movable::mouseButton(int button, int state, int x, int y) {
	omp_set_lock(&mutex_self);

	if(button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN){
		if(isContextualPosition()){
			int submenuPosition = glutCreateMenu(rightClick);
			glutAddMenuEntry("Move", 0);

			glutCreateMenu(Movable::rightClick);
			glutAddSubMenu("Position", submenuPosition);

			FocusManager::setFocus(getWindowDimensions()->getWindowHandler(), this);
		}
	}
	else if(button == GLUT_LEFT_BUTTON && state == GLUT_UP){
		if(isContextualPosition()){
printf("move disabled\n");
			onMove = false;
			curPos.x = curPos.y = curPos.z = 0;
		}
	}
	omp_unset_lock(&mutex_self);
}

void Movable::mouseMove(int x, int y) {
	omp_set_lock(&mutex_self);

	if(isOnMove()){
		XYZ moved = getLastMoveSize(x, y);
printf("1current pos [%f, %f]\n", moved.x, moved.y);
		if(moved.x != 0 || moved.y != 0){
printf("move size [%f, %f] / position [%f, %f]\n", moved.x, moved.y, x, y);
			for(iter=doMap.begin(); iter != doMap.end(); iter++){
				iter->second->moveAnchorRelative(moved.x, moved.y);
			}
		}
printf("2current pos [%f, %f]\n", moved.x, moved.y);
	}

	omp_unset_lock(&mutex_self);
}

void Movable::rightClick(int op){
	if(op == 0){
		Movable *mov;
		mov = (Movable*)FocusManager::getFocus(glutGetWindow());
		FocusManager::clearFocus(glutGetWindow());
		mov->onMove = true;
printf("move enabled\n");
	}
}
*/
