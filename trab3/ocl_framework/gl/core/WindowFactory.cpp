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
 * WindowFactory.cpp
 *
 * Created on: 06/Jan/2015
 * Author: Peter Frank Perroni (pfperroni@inf.ufpr.br)
 */

#include "../../include/gl/core/WindowFactory.hpp"

omp_lock_t WindowFactory::mutex;
map<int, glWindow*> WindowFactory::windowMap;
bool WindowFactory::started = false;

void WindowFactory::startup(){
	if(started) return;
	omp_init_lock(&mutex);

	omp_set_lock(&mutex);
	XInitThreads();
	int argc[1] = {0}; glutInit(argc, NULL); // GL context Must be initialized Before CL context.
	clFactory::startup();
	FocusManager::startup();
	started = true;
	omp_unset_lock(&mutex);
}

void WindowFactory::shutdown(){
	if(!started) return;

	omp_set_lock(&mutex);
	map<int, glWindow*>::iterator iter;
	for(iter=windowMap.begin(); iter != windowMap.end(); iter++){
//printf("deleting window: %i\n", iter->second->getWindowHandler());
		delete iter->second;
		windowMap.erase(iter);
	}
	FocusManager::shutdown();
	clFactory::shutdown();
	started = false;

	omp_unset_lock(&mutex);
	omp_destroy_lock(&mutex);
}

void WindowFactory::mainGlLoop(){
	startup();
	forceRefresh();
	while(glutGet(GLUT_ELAPSED_TIME) > 0 && WindowFactory::countWindows() > 0){
		display();
		glutMainLoopEvent();
		usleep(10000);
	}
	shutdown();
}

void WindowFactory::addWindow(glWindow *window){
	omp_set_lock(&mutex);
	// Each doManagerGroup can have its own WindowDimensions configuration (internal viewport),
	// so doesn't enforce one single dimension at this point.
	windowMap[window->getWindowHandler()] = window;
	glutSetWindow(window->getWindowHandler());

	glutReshapeFunc(WindowFactory::windowReshape);
	glutCloseFunc(WindowFactory::windowClose);
	glutDisplayFunc(WindowFactory::forceRefresh);

	glutPassiveMotionFunc(WindowFactory::mousePassiveMove);
    glutMouseFunc(WindowFactory::mouseButton);
   	glutMotionFunc(WindowFactory::mouseMove);

   	glutKeyboardFunc(WindowFactory::keyboard);
   	glutSpecialFunc(WindowFactory::keyboardSpecial);

	omp_unset_lock(&mutex);
}

void WindowFactory::display(){
	omp_set_lock(&mutex);
	map<int, glWindow*>::iterator iter = windowMap.find(glutGetWindow());
	glWindow *window;
	if(iter != windowMap.end()){
		window = iter->second;
		if(window->isVisible()) window->display();
	}
	omp_unset_lock(&mutex);
}

void WindowFactory::forceRefresh(){
	omp_set_lock(&mutex);
	map<int, glWindow*>::iterator iter;
	glWindow *window;
	for(iter=windowMap.begin(); iter != windowMap.end(); iter++){
		window = iter->second;
		if(window->isVisible()) window->forceRefresh();
	}
	omp_unset_lock(&mutex);
}

void WindowFactory::windowReshape(int w, int h){
	omp_set_lock(&mutex);
	windowMap[glutGetWindow()]->forceRefresh();
	omp_unset_lock(&mutex);
}

void WindowFactory::windowClose(){
	glutTimerFunc(10000, WindowFactory::timerEvent, 0);
	dispose(glutGetWindow());
}

void WindowFactory::mousePassiveMove(int x, int y) {
	glutDetachMenu(GLUT_RIGHT_BUTTON);

	omp_set_lock(&mutex);
	map<int, glWindow*>::iterator iter = windowMap.find(glutGetWindow());
	glWindow *window;
	if(iter != windowMap.end()){
		window = iter->second;
		if(window->isVisible()) window->mousePassiveMove(x, y);
	}
	omp_unset_lock(&mutex);
}

void WindowFactory::mouseButton(int button, int state, int x, int y) {
	if(button == GLUT_LEFT_BUTTON && state == GLUT_UP){
		glutDetachMenu(GLUT_RIGHT_BUTTON);
	}
	else if(button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN){
		glutCreateMenu(WindowFactory::rightClick);
	}

	omp_set_lock(&mutex);
	map<int, glWindow*>::iterator iter = windowMap.find(glutGetWindow());
	glWindow *window;
	if(iter != windowMap.end()){
		window = iter->second;
		if(window->isVisible()) window->mouseButton(button, state, x, y);
	}
	omp_unset_lock(&mutex);

	if(button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN){
		glutAttachMenu(GLUT_RIGHT_BUTTON);
	}
}

void WindowFactory::mouseMove(int x, int y) {
	omp_set_lock(&mutex);
	map<int, glWindow*>::iterator iter = windowMap.find(glutGetWindow());
	glWindow *window;
	if(iter != windowMap.end()){
		window = iter->second;
		if(window->isVisible()) window->mouseMove(x, y);
	}
	omp_unset_lock(&mutex);
}

void WindowFactory::keyboard(unsigned char key, int x, int y) {
	omp_set_lock(&mutex);
	map<int, glWindow*>::iterator iter;
	glWindow *window;
	for(iter=windowMap.begin(); iter != windowMap.end(); iter++){
		window = iter->second;
		if(window->isVisible()) window->keyboard(key, x, y);
	}
	omp_unset_lock(&mutex);
}

void WindowFactory::keyboardSpecial(int key, int x, int y) {
	omp_set_lock(&mutex);
	map<int, glWindow*>::iterator iter;
	glWindow *window;
	for(iter=windowMap.begin(); iter != windowMap.end(); iter++){
		window = iter->second;
		if(window->isVisible()) window->keyboardSpecial(key, x, y);
	}
	omp_unset_lock(&mutex);
}

glWindow* WindowFactory::createWindow(int windowWidth, int windowHeight, int windowDepth, DOManagerGroup *doManagerGroup)
{
    startup();
	glWindow *window = new glWindow(windowWidth, windowHeight, windowDepth, doManagerGroup);
	addWindow(window);
    return window;
}

int WindowFactory::countWindows(){
	omp_set_lock(&mutex);
	int count = windowMap.size();
	omp_unset_lock(&mutex);
	return count;
}

void WindowFactory::dispose(int windowHandler){
	omp_set_lock(&mutex);
	map<int, glWindow*>::iterator iter = windowMap.find(windowHandler);
	if(iter != windowMap.end()){
		delete iter->second;
		windowMap.erase(iter);
	}
	omp_unset_lock(&mutex);
}

void WindowFactory::handOverControlToGlut(){
	glutMainLoop();
}

glWindow* WindowFactory::getWindow(int windowHandler){
	return windowMap[windowHandler];
}
