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
 * glWindow.cpp
 *
 * Created on: 06/Jan/2015
 * Author: Peter Frank Perroni (pfperroni@inf.ufpr.br)
 */

#include "../../include/gl/core/glWindow.hpp"

glWindow::glWindow(int windowWidth, int windowHeight, int windowDepth, DOManagerGroup *doManagerGroup){
	// Initialize GLUT.
    glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE);
    glutInitWindowPosition (glutGet(GLUT_SCREEN_WIDTH)/2 - windowWidth/2,
                            glutGet(GLUT_SCREEN_HEIGHT)/2 - windowHeight/2);
    glutInitWindowSize(windowWidth, windowHeight);

    int windowHandler = glutCreateWindow("");
    glutSetOption(GLUT_ACTION_ON_WINDOW_CLOSE, GLUT_ACTION_GLUTMAINLOOP_RETURNS);

	// Initialize necessary OpenGL extensions.
    glewInit();

    // Default initialization.
    glClearColor(1.0, 1.0, 1.0, 1.0);
    glDisable(GL_DEPTH_TEST);

    // Set view matrix.
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

	omp_init_lock(&mutex_self);

	dim = new WindowDimensions(windowHandler, windowWidth, windowHeight, windowDepth);
	originalWindowDepth = windowDepth;
	visible = true;

	if(doManagerGroup == NULL){
		doManagerGroup = new InternalDOManagerGroup(dim, "InternalDOManagerGroup");
	}
	else{
		doManagerGroup->setWindowDimensions(dim);
	}
	addDesignElement(doManagerGroup);
}

glWindow::~glWindow(){
	delete dim;

	map<int, DOManagerGroup*>::iterator iter;
//printf("doManagerGroupMap.size(): %i\n", (int)doManagerGroupMap.size());
	for(iter=doManagerGroupMap.begin(); iter != doManagerGroupMap.end(); iter++){
//printf("deleting doManagerGroupMap: %p\n", iter->second);
		delete iter->second;
//printf("deleted\n");
	}

	omp_destroy_lock(&mutex_self);

//printf("destroying window\n");
	// If glut context is still started, close the windows.
	if(glutGet(GLUT_ELAPSED_TIME) == 0){ // Trick to discover if the glut context is started.
		glutDestroyWindow(dim->getWindowHandler());
	}
}

void glWindow::addDesignElement(glDrawable *drawable){
	if(drawable == NULL) return;

	int _uid = drawable->getUID();
	int type = drawable->getDrawableType();
	if(type == glDrawable::DESIGN_ELEMENT_DO){
		doManagerGroupMap.begin()->second->addDesignElement(drawable);
	}
	else if(type == glDrawable::DESIGN_ELEMENT_DOMANAGER){
		doManagerGroupMap.begin()->second->addDesignElement(drawable);
	}
	else if(type == glDrawable::DESIGN_ELEMENT_DOMANAGERGROUP){
		DOManagerGroup* doManagerGroup = (DOManagerGroup*)drawable;
		omp_set_lock(&mutex_self);
		doManagerGroupMap[_uid] = doManagerGroup;
		omp_unset_lock(&mutex_self);
		doManagerGroup->setWindowDimensions(dim);
	}
}

void glWindow::display(){
	glutSetWindow(dim->getWindowHandler());
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	omp_set_lock(&mutex_self);
	map<int, DOManagerGroup*>::iterator iter;
	DOManagerGroup* doManagerGroup;
	for(iter=doManagerGroupMap.begin(); iter != doManagerGroupMap.end(); iter++){
		doManagerGroup = iter->second;
		if(doManagerGroup->isVisible()) doManagerGroup->repaint();
	}
	omp_unset_lock(&mutex_self);

	glutSwapBuffers();
}

void glWindow::forceRefresh(){
	glutSetWindow(getWindowHandler());
	glViewport(0, 0, glutGet(GLUT_WINDOW_WIDTH), glutGet(GLUT_WINDOW_HEIGHT));
	resetInternalViewPort();

	omp_set_lock(&mutex_self);
	map<int, DOManagerGroup*>::iterator iter;
	for(iter=doManagerGroupMap.begin(); iter != doManagerGroupMap.end(); iter++){
		iter->second->enforceBounds();
	}
	omp_unset_lock(&mutex_self);

	display();
}

int glWindow::mousePassiveMove(int x, int y) {
	int ret = ACTION_IGNORED;

	omp_set_lock(&mutex_self);
	map<int, DOManagerGroup*>::iterator iter;
	DOManagerGroup* doManagerGroup;
	for(iter=doManagerGroupMap.begin(); iter != doManagerGroupMap.end(); iter++){
		doManagerGroup = iter->second;
		if(doManagerGroup->isVisible() && doManagerGroup->isWithinBounds(x, y)){
			ret |= doManagerGroup->mousePassiveMove(x, y);
		}
		else ret |= doManagerGroup->mouseNoPassiveMove(x, y);
	}
	omp_unset_lock(&mutex_self);

	return ret;
}

int glWindow::mouseButton(int button, int state, int x, int y) {
	int ret = ACTION_IGNORED;

	omp_set_lock(&mutex_self);
	map<int, DOManagerGroup*>::iterator iter;
	DOManagerGroup* doManagerGroup;
	for(iter=doManagerGroupMap.begin(); iter != doManagerGroupMap.end(); iter++){
		doManagerGroup = iter->second;
		if(doManagerGroup->isVisible() && doManagerGroup->isWithinBounds(x, y)){
			ret |= doManagerGroup->mouseButton(button, state, x, y);
		}
		else ret |= doManagerGroup->mouseNoButton(button, state, x, y);
	}
	omp_unset_lock(&mutex_self);

	return ret;
}

int glWindow::mouseMove(int x, int y) {
	int ret = ACTION_IGNORED;

	omp_set_lock(&mutex_self);
	map<int, DOManagerGroup*>::iterator iter;
	DOManagerGroup* doManagerGroup;
	for(iter=doManagerGroupMap.begin(); iter != doManagerGroupMap.end(); iter++){
		doManagerGroup = iter->second;
		if(doManagerGroup->isVisible()) ret |= doManagerGroup->mouseMove(x, y);
	}
	omp_unset_lock(&mutex_self);

	return ret;
}

int glWindow::keyboard(unsigned char key, int x, int y) {
	int ret = ACTION_IGNORED;

	omp_set_lock(&mutex_self);
	map<int, DOManagerGroup*>::iterator iter;
	DOManagerGroup* doManagerGroup;
	for(iter=doManagerGroupMap.begin(); iter != doManagerGroupMap.end(); iter++){
		doManagerGroup = iter->second;
		if(doManagerGroup->isVisible() && doManagerGroup->isWithinBounds(x, y)){
			ret |= doManagerGroup->keyboard(key, x, y);
		}
	}
	omp_unset_lock(&mutex_self);

	return ret;
}

int glWindow::keyboardSpecial(int key, int x, int y) {
	int ret = ACTION_IGNORED;

	omp_set_lock(&mutex_self);
	map<int, DOManagerGroup*>::iterator iter;
	DOManagerGroup* doManagerGroup;
	for(iter=doManagerGroupMap.begin(); iter != doManagerGroupMap.end(); iter++){
		doManagerGroup = iter->second;
		if(doManagerGroup->isVisible() && doManagerGroup->isWithinBounds(x, y)){
			ret |= doManagerGroup->keyboardSpecial(key, x, y);
		}
	}
	omp_unset_lock(&mutex_self);

	return ret;
}

void glWindow::resizeInternalViewPort(float zoom, int windowWidth, int windowHeight, int windowDepth){
	omp_set_lock(&mutex_self);
	WindowDimensions *_dim = new WindowDimensions(dim->getWindowHandler(), windowWidth, windowHeight, windowDepth, zoom);

	map<int, DOManagerGroup*>::iterator iter;
	for(iter=doManagerGroupMap.begin(); iter != doManagerGroupMap.end(); iter++){
		iter->second->setWindowDimensions(_dim);
	}
	delete dim;
	dim = _dim;

	omp_unset_lock(&mutex_self);
}

void glWindow::resetInternalViewPort(){
	resizeInternalViewPort(1.0f, glutGet(GLUT_WINDOW_WIDTH), glutGet(GLUT_WINDOW_HEIGHT), originalWindowDepth);
}

void glWindow::setFocus(void *focus){
	FocusManager::setFocus(getWindowHandler(), focus);
}

void glWindow::clearFocus(){
	FocusManager::clearFocus(getWindowHandler());
}

void* glWindow::getFocus(){
	return FocusManager::getFocus(getWindowHandler());
}

void glWindow::setVisible(bool _visible){
	glutSetWindow(dim->getWindowHandler());
	if(visible && !_visible) glutHideWindow();
	else if(!visible && _visible) glutShowWindow();
	visible = _visible;
}

bool glWindow::isVisible(){
	return visible;
}

void glWindow::setWindowTitle(char *title){
	glutSetWindow(dim->getWindowHandler()); glutSetWindowTitle(title);
}

int glWindow::getWindowHandler(){
	return dim->getWindowHandler();
}

WindowDimensions* glWindow::getWindowDimensions(){
	return dim;
}

void glWindow::resizeInternalViewPort(int windowWidth, int windowHeight, int windowDepth){
	resizeInternalViewPort(dim->getZoom(), windowWidth, windowHeight, windowDepth);
}

void glWindow::zoom(float zoom){
	resizeInternalViewPort(zoom, dim->getWindowWidth(), dim->getWindowHeight(), dim->getWindowDepth());
}

void glWindow::rotate(float angle, float x, float y, float z){
	glutSetWindow(dim->getWindowHandler());
	glRotatef(angle, x, y, z);
}
