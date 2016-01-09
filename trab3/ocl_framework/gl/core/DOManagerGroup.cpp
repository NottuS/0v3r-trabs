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
 * DOManagerGroup.cpp
 *
 * Created on: 06/Jan/2015
 * Author: Peter Frank Perroni (pfperroni@inf.ufpr.br)
 */

#include "../../include/gl/core/DOManagerGroup.hpp"

DOManagerGroup::~DOManagerGroup(){
	processPendingActions();

	map<int, DOManagerGroup*>::iterator iter1;
	for(iter1=doManagerGroupMap.begin(); iter1 != doManagerGroupMap.end(); iter1++){
		delete iter1->second;
	}

	map<int, DOManager*>::iterator iter2;
//printf("doManagerMap.size() %i\n", (int)doManagerMap.size());
	for(iter2=doManagerMap.begin(); iter2 != doManagerMap.end(); iter2++){
//printf("~DOManagerGroup %p\n", iter2->second);
		delete iter2->second;
	}

//printf("disposing clqueue from DOManagerGroup\n");
	clFactory::disposeQueue(getCLQueue());
//printf("disposed\n");

	DesignElement *elemNext;
	for(DesignElement *elemCurr=getFirstDesignElement(); elemCurr != NULL; elemCurr=elemNext){
		elemNext = elemCurr->next;
		delete elemCurr;
	}

	omp_destroy_lock(&mutex_self1);
	omp_destroy_lock(&mutex_self2);
}

void DOManagerGroup::setCLQueue(clQueue *_cl_queue){
	DOManager::setCLQueue(_cl_queue);
	initializeCL();
	if(isReloadToDeviceNeeded()) loadToDevice();

	omp_set_lock(&mutex_self1);
	map<int, DOManagerGroup*>::iterator iter1;
	for(iter1=doManagerGroupMap.begin(); iter1 != doManagerGroupMap.end(); iter1++){
		iter1->second->setCLQueue(_cl_queue);
	}

	map<int, DOManager*>::iterator iter2;
	for(iter2=doManagerMap.begin(); iter2 != doManagerMap.end(); iter2++){
		iter2->second->setCLQueue(_cl_queue);
	}
	omp_unset_lock(&mutex_self1);
}

void DOManagerGroup::repaint(){
//printf("doManagerGroup UID %i begin repaint (visible=%i)\n", getUID(), isVisible());
	processPendingActions();
	if(!isVisible()) return;

	omp_set_lock(&mutex_self1);

	DOManager::repaint();

	glDrawable *drawable;
	int type;
	for(DesignElement *elem=getFirstDesignElement(); elem != NULL; elem=elem->next){
		drawable = elem->drawable;
		type = drawable->getDrawableType();
		if(type == DESIGN_ELEMENT_DOMANAGER){
			DOManager* doManager = (DOManager*)drawable;
			if(doManager->isReloadToDeviceNeeded()) doManager->loadToDevice();
			if(doManager->isVisible()) doManager->repaint();
		}
		else if(type == DESIGN_ELEMENT_DOMANAGERGROUP){
			DOManagerGroup* doManagerGroup = (DOManagerGroup*)drawable;
			if(doManagerGroup->isVisible()) doManagerGroup->repaint();
		}
	}

/*
	DOManager::repaint();

	omp_set_lock(&mutex_self1);
	map<int, DOManager*>::iterator iter2;
	DOManager* doManager;
	for(iter2=doManagerMap.begin(); iter2 != doManagerMap.end(); iter2++){
//printf("doManagerGroup UID %i repainting UID %i\n", getUID(), doManager->getUID());
		doManager = iter2->second;
		if(doManager->isVisible()) doManager->repaint();
	}

	map<int, DOManagerGroup*>::iterator iter1;
	DOManagerGroup* doManagerGroup;
	for(iter1=doManagerGroupMap.begin(); iter1 != doManagerGroupMap.end(); iter1++){
		doManagerGroup = iter1->second;
//printf("repaint doManagerGroup UID %i (visible=%i)\n", doManagerGroup->getUID(), doManagerGroup->isVisible());
		if(doManagerGroup->isVisible()) doManagerGroup->repaint();
	}
*/
	omp_unset_lock(&mutex_self1);
}

int DOManagerGroup::mousePassiveMove(int x, int y) {
	int ret = ACTION_IGNORED;

	omp_set_lock(&mutex_self1);
	map<int, DOManager*>::iterator iter2;
	DOManager* doManager;
	for(iter2=doManagerMap.begin(); iter2 != doManagerMap.end(); iter2++){
		doManager = iter2->second;
		if(doManager->isVisible() && doManager->isWithinBounds(x, y)) ret |= doManager->mousePassiveMove(x, y);
		else ret |= doManager->mouseNoPassiveMove(x, y);
	}

	map<int, DOManagerGroup*>::iterator iter1;
	DOManagerGroup* doManagerGroup;
	for(iter1=doManagerGroupMap.begin(); iter1 != doManagerGroupMap.end(); iter1++){
		doManagerGroup = iter1->second;
		if(doManagerGroup->isVisible() && doManagerGroup->isWithinBounds(x, y)) ret |= doManagerGroup->mousePassiveMove(x, y);
		else ret |= doManagerGroup->mouseNoPassiveMove(x, y);
	}
	omp_unset_lock(&mutex_self1);

	if(DOManager::isVisible() && DOManager::isWithinBounds(x, y)) ret |= DOManager::mousePassiveMove(x, y);
	else ret |= DOManager::mouseNoPassiveMove(x, y);

	return ret;
}

int DOManagerGroup::mouseButton(int button, int state, int x, int y) {
	int ret = ACTION_IGNORED;

	omp_set_lock(&mutex_self1);
	if(button == GLUT_LEFT_BUTTON && state == GLUT_UP){
		if(isOnMove()){
			setMoveEnabled(false);
//printf("move disabled for obj=%p->%i (%s)\n", this, isMoveEnabled(), getComponentName());
		}
	}

	map<int, DOManager*>::iterator iter2;
	DOManager* doManager;
	for(iter2=doManagerMap.begin(); iter2 != doManagerMap.end(); iter2++){
		doManager = iter2->second;
		if(doManager->isVisible() && doManager->isWithinBounds(x, y)) ret |= doManager->mouseButton(button, state, x, y);
		else ret |= doManager->mouseNoButton(button, state, x, y);
	}

	map<int, DOManagerGroup*>::iterator iter1;
	DOManagerGroup* doManagerGroup;
	for(iter1=doManagerGroupMap.begin(); iter1 != doManagerGroupMap.end(); iter1++){
		doManagerGroup = iter1->second;
		if(doManagerGroup->isVisible() && doManagerGroup->isWithinBounds(x, y)) ret |= doManagerGroup->mouseButton(button, state, x, y);
		else ret |= doManagerGroup->mouseNoButton(button, state, x, y);
	}
	omp_unset_lock(&mutex_self1);

	if(DOManager::isVisible() && DOManager::isWithinBounds(x, y)) ret |= DOManager::mouseButton(button, state, x, y);
	else ret |= DOManager::mouseNoButton(button, state, x, y);

	if(button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN){
		if(HAS_ONLY_IGNORED_ACTION(ret) && FocusManager::getFocus(glutGetWindow()) == NULL &&
				isContextualPosition() && (getDesignElementListSize() > 0)){
			int submenuPosition = glutCreateMenu(rightClick);
			glutAddMenuEntry("Move Component", 0);

			glutCreateMenu(DOManagerGroup::rightClick);
			glutAddSubMenu("General", submenuPosition);

			FocusManager::setFocus(getWindowDimensions()->getWindowHandler(), (Movable*)this);
//printf("set focus at window=%i, obj=%p (%s)\n", getWindowDimensions()->getWindowHandler(), this, getComponentName());
		}
	}

	return ret;
}

int DOManagerGroup::mouseMove(int x, int y) {
	int ret = ACTION_IGNORED;

	omp_set_lock(&mutex_self2);
	// Drag-and-drop event.
	if(isMoveEnabled()){
		setOnMove(true);
		XYZ moved = getLastMoveSize(x, y);
		if(moved.x != 0 || moved.y != 0){
			moveAnchorsRelative(moved.x, moved.y);
			ret |= ACTION_TAKEN;
		}
	}
	omp_unset_lock(&mutex_self2);

	omp_set_lock(&mutex_self1);
	map<int, DOManager*>::iterator iter2;
	DOManager* doManager;
	for(iter2=doManagerMap.begin(); iter2 != doManagerMap.end(); iter2++){
		doManager = iter2->second;
		if(doManager->isVisible()) ret |= doManager->mouseMove(x, y);
	}

	map<int, DOManagerGroup*>::iterator iter1;
	DOManagerGroup* doManagerGroup;
	for(iter1=doManagerGroupMap.begin(); iter1 != doManagerGroupMap.end(); iter1++){
		doManagerGroup = iter1->second;
		if(doManagerGroup->isVisible()) ret |= doManagerGroup->mouseMove(x, y);
	}
	omp_unset_lock(&mutex_self1);

	ret |= DOManager::mouseMove(x, y);

	return ret;
}

int DOManagerGroup::mouseNoButton(int button, int state, int x, int y) {
	int ret = ACTION_IGNORED;

	omp_set_lock(&mutex_self1);
	if(button == GLUT_LEFT_BUTTON && state == GLUT_UP){
		if(isOnMove()){
			setMoveEnabled(false);
//printf("move disabled for obj=%p->%i (%s)\n", this, isMoveEnabled(), getComponentName());
		}
	}

	map<int, DOManager*>::iterator iter2;
	DOManager* doManager;
	for(iter2=doManagerMap.begin(); iter2 != doManagerMap.end(); iter2++){
		doManager = iter2->second;
		ret |= doManager->mouseNoButton(button, state, x, y);
	}

	map<int, DOManagerGroup*>::iterator iter1;
	DOManagerGroup* doManagerGroup;
	for(iter1=doManagerGroupMap.begin(); iter1 != doManagerGroupMap.end(); iter1++){
		doManagerGroup = iter1->second;
		ret |= doManagerGroup->mouseNoButton(button, state, x, y);
	}
	omp_unset_lock(&mutex_self1);

	ret |= DOManager::mouseNoButton(button, state, x, y);

	return ret;
}

int DOManagerGroup::mouseNoPassiveMove(int x, int y) {
	int ret = ACTION_IGNORED;

	omp_set_lock(&mutex_self1);
	map<int, DOManager*>::iterator iter2;
	for(iter2=doManagerMap.begin(); iter2 != doManagerMap.end(); iter2++){
		ret |= iter2->second->mouseNoPassiveMove(x, y);
	}

	map<int, DOManagerGroup*>::iterator iter1;
	for(iter1=doManagerGroupMap.begin(); iter1 != doManagerGroupMap.end(); iter1++){
		ret |= iter1->second->mouseNoPassiveMove(x, y);
	}
	omp_unset_lock(&mutex_self1);

	ret |= DOManager::mouseNoPassiveMove(x, y);

	return ret;
}

int DOManagerGroup::keyboard(unsigned char key, int x, int y) {
	int ret = ACTION_IGNORED;

	omp_set_lock(&mutex_self1);
	map<int, DOManager*>::iterator iter2;
	DOManager* doManager;
	for(iter2=doManagerMap.begin(); iter2 != doManagerMap.end(); iter2++){
		doManager = iter2->second;
		if(doManager->isWithinBounds(x, y)) ret |= doManager->keyboard(key, x, y);
	}

	map<int, DOManagerGroup*>::iterator iter1;
	DOManagerGroup* doManagerGroup;
	for(iter1=doManagerGroupMap.begin(); iter1 != doManagerGroupMap.end(); iter1++){
		doManagerGroup = iter1->second;
		if(doManagerGroup->isWithinBounds(x, y)) ret |= doManagerGroup->keyboard(key, x, y);
	}
	omp_unset_lock(&mutex_self1);

	if(DOManager::isWithinBounds(x, y)) ret |= DOManager::keyboard(key, x, y);

	return ret;
}

int DOManagerGroup::keyboardSpecial(int key, int x, int y) {
	int ret = ACTION_IGNORED;

	omp_set_lock(&mutex_self1);
	map<int, DOManager*>::iterator iter2;
	DOManager* doManager;
	for(iter2=doManagerMap.begin(); iter2 != doManagerMap.end(); iter2++){
		doManager = iter2->second;
		if(doManager->isWithinBounds(x, y)) ret |= doManager->keyboardSpecial(key, x, y);
	}

	map<int, DOManagerGroup*>::iterator iter1;
	DOManagerGroup* doManagerGroup;
	for(iter1=doManagerGroupMap.begin(); iter1 != doManagerGroupMap.end(); iter1++){
		doManagerGroup = iter1->second;
		if(doManagerGroup->isWithinBounds(x, y)) ret |= doManagerGroup->keyboardSpecial(key, x, y);
	}
	omp_unset_lock(&mutex_self1);

	if(DOManager::isWithinBounds(x, y)) ret |= DOManager::keyboardSpecial(key, x, y);

	return ret;
}

void DOManagerGroup::rightClick(int op){
	if(op == 0){
		Movable *mov = (Movable*)FocusManager::getFocus(glutGetWindow());
		FocusManager::clearFocus(glutGetWindow());
		if(mov != NULL && mov->isContextualPosition()) mov->setMoveEnabled(true);
//printf("move enabled for window=%i, %p->%i (%s)\n", glutGetWindow(),mov, mov->isMoveEnabled(), mov->getComponentName());
	}
}

bool DOManagerGroup::isWithinBounds(int x, int y){
	omp_set_lock(&mutex_self1);
	map<int, DOManager*>::iterator iter2;
	for(iter2=doManagerMap.begin(); iter2 != doManagerMap.end(); iter2++){
		if(iter2->second->isWithinBounds(x, y)){
			omp_unset_lock(&mutex_self1);
			return true;
		}
	}

	map<int, DOManagerGroup*>::iterator iter1;
	for(iter1=doManagerGroupMap.begin(); iter1 != doManagerGroupMap.end(); iter1++){
		if(iter1->second->isWithinBounds(x, y)){
			omp_unset_lock(&mutex_self1);
			return true;
		}
	}
	omp_unset_lock(&mutex_self1);

	return DOManager::isWithinBounds(x, y);
}

bool DOManagerGroup::enforceBounds() {
	omp_set_lock(&mutex_self1);
	bool ret = false;
	map<int, DOManager*>::iterator iter2;
	for(iter2=doManagerMap.begin(); iter2 != doManagerMap.end(); iter2++){
		ret |= iter2->second->enforceBounds();
	}

	map<int, DOManagerGroup*>::iterator iter1;
	for(iter1=doManagerGroupMap.begin(); iter1 != doManagerGroupMap.end(); iter1++){
		ret |= iter1->second->enforceBounds();
	}
	omp_unset_lock(&mutex_self1);

	ret |= DOManager::enforceBounds();
	return ret;
}

void DOManagerGroup::scheduleAddDesignElement(DesignElement *elem){
	if(elem == NULL) return;
	addDesignElementToList(&pendingActions[DESIGN_LIST_ADDS], elem->drawable);
}

DOManager* DOManagerGroup::getDOManager(int _UID){
	omp_set_lock(&mutex_self1);
	map<int, DOManager*>::iterator iter = doManagerMap.find(_UID);
	omp_unset_lock(&mutex_self1);
	if(iter != doManagerMap.end()){
		return iter->second;
	}
	return NULL;
}

DOManagerGroup* DOManagerGroup::getDOManagerGroup(int _UID){
	omp_set_lock(&mutex_self1);
	map<int, DOManagerGroup*>::iterator iter = doManagerGroupMap.find(_UID);
	omp_unset_lock(&mutex_self1);
	if(iter != doManagerGroupMap.end()){
		return iter->second;
	}
	return NULL;
}

int DOManagerGroup::getDesignElementListSize(){
	int count = 0;
	omp_set_lock(&mutex_self2);
	for(DesignElement *elem=getFirstDesignElement(); elem != NULL; elem=elem->next, count++);
	omp_unset_lock(&mutex_self2);
	return count;
}

DesignElement* DOManagerGroup::getFirstDesignElement(){
	return designList.first;
}

void DOManagerGroup::addDesignElement(glDrawable *drawable){
	if(drawable == NULL) return;

	int _uid = drawable->getUID();
	int type = drawable->getDrawableType();
	if(type == DESIGN_ELEMENT_DO){
		DOManager::addDesignElement(drawable);
	}
	else if(type == DESIGN_ELEMENT_DOMANAGER){
		if(getDOManager(_uid) != NULL) return;
		omp_set_lock(&mutex_self1);
		DOManager* doManager = (DOManager*)drawable;
		doManagerMap[_uid] = doManager;
		doManager->setParent(getSelfDesignElement());
		doManager->setWindowDimensions(getWindowDimensions());
		doManager->setCLQueue(getCLQueue());
		doManager->layersUp(1);
		omp_unset_lock(&mutex_self1);
	}
	else if(type == DESIGN_ELEMENT_DOMANAGERGROUP){
		if(getDOManagerGroup(_uid) != NULL) return;
		omp_set_lock(&mutex_self1);
		DOManagerGroup* doManagerGroup = (DOManagerGroup*)drawable;
		doManagerGroupMap[_uid] = doManagerGroup;
		doManagerGroup->setParent(getSelfDesignElement());
		doManagerGroup->setWindowDimensions(getWindowDimensions());
		doManagerGroup->setCLQueue(getCLQueue());
		doManagerGroup->layersUp(1);
		omp_unset_lock(&mutex_self1);
	}

	omp_set_lock(&mutex_self1);
	addDesignElementToList(&designList, drawable);
	omp_unset_lock(&mutex_self1);
}

void DOManagerGroup::addDesignElementToList(DesignList *_designList, glDrawable *drawable){
	DesignElement *dElement = new DesignElement;
	dElement->drawable = drawable;
	if(_designList->first == NULL){
		dElement->next = dElement->prev = NULL;
		_designList->last = _designList->first = dElement;
	}
	else{
		dElement->next = NULL;
		dElement->prev = designList.last;
		dElement->prev->next = dElement;
		_designList->last = dElement;
	}
}

void DOManagerGroup::processPendingActions(){
	omp_set_lock(&mutex_self2);

	int _uid = -1;
	glDrawable *drawable;
	int type;
	//----------
	// Deletions
	//----------
	DesignList *deleteList = &pendingActions[DESIGN_LIST_DELETES];
	for(DesignElement *elemNext, *elemCurr=deleteList->first; elemCurr != NULL; elemCurr=elemNext){
		drawable = elemCurr->drawable;
		type = drawable->getDrawableType();
		if(type == DESIGN_ELEMENT_DO){
			DOManager::removeDesignElement(drawable->getSelfDesignElement());
			continue;
		}
		else if(type == DESIGN_ELEMENT_DOMANAGER){
			DOManager* doManager = (DOManager*)drawable;
			_uid = doManager->getUID();
			map<int, DOManager*>::iterator iter = doManagerMap.find(doManager->getUID());
			if(iter != doManagerMap.end()){
				doManagerMap.erase(iter);
			}
		}
		else if(type == DESIGN_ELEMENT_DOMANAGERGROUP){
			DOManagerGroup* doManagerGroup = (DOManagerGroup*)drawable;
			_uid = doManagerGroup->getUID();
			map<int, DOManagerGroup*>::iterator iter = doManagerGroupMap.find(doManagerGroup->getUID());
			if(iter != doManagerGroupMap.end()){
				doManagerGroupMap.erase(iter);
			}
		}

		DesignElement *curr;
		for(curr=getFirstDesignElement(); curr != NULL && curr->drawable->getUID() != _uid; curr=curr->next);
		if(curr != NULL){
			if(curr->prev != NULL) curr->prev->next = curr->next;
			if(curr->next != NULL) curr->next->prev = curr->prev;
			delete curr;
		}
		// Remove the references to the parent from the original drawable object.
		drawable->getSelfDesignElement()->next = drawable->getSelfDesignElement()->prev = NULL;

		elemNext = elemCurr->next;
		delete elemCurr;
	}
	deleteList->first = deleteList->last = NULL;

	//----------
	// Adds
	//----------
	DesignList *addList = &pendingActions[DESIGN_LIST_ADDS];
	for(DesignElement *elemNext, *elemCurr=addList->first; elemCurr != NULL; elemCurr=elemNext){
		addDesignElement(elemCurr->drawable);
		elemNext = elemCurr->next;
		delete elemCurr;
	}
	addList->first = addList->last = NULL;

	omp_unset_lock(&mutex_self2);
}

void DOManagerGroup::removeDesignElement(DesignElement *elem){
	if(elem == NULL) return;
	omp_set_lock(&mutex_self2);
	addDesignElementToList(&pendingActions[DESIGN_LIST_DELETES], elem->drawable);
	omp_unset_lock(&mutex_self2);
}

void DOManagerGroup::setVisible(bool _visible){
	omp_set_lock(&mutex_self1);
	DOManager::setVisible(_visible);
	omp_unset_lock(&mutex_self1);

	setVisibleChildren(_visible);
}

void DOManagerGroup::setVisibleChildren(bool _visible){
	omp_set_lock(&mutex_self1);

	map<int, DOManager*>::iterator iter2;
	for(iter2=doManagerMap.begin(); iter2 != doManagerMap.end(); iter2++){
		iter2->second->setVisible(_visible);
	}

	map<int, DOManagerGroup*>::iterator iter1;
	for(iter1=doManagerGroupMap.begin(); iter1 != doManagerGroupMap.end(); iter1++){
		iter1->second->setVisible(_visible);
	}

	visibleChildren = _visible;
	omp_unset_lock(&mutex_self1);
}

void DOManagerGroup::setContextualPosition(bool _contextualPosition){
	omp_set_lock(&mutex_self1);
	map<int, DOManager*>::iterator iter2;
	for(iter2=doManagerMap.begin(); iter2 != doManagerMap.end(); iter2++){
		iter2->second->setContextualPosition(_contextualPosition);
	}

	map<int, DOManagerGroup*>::iterator iter1;
	for(iter1=doManagerGroupMap.begin(); iter1 != doManagerGroupMap.end(); iter1++){
		iter1->second->setContextualPosition(_contextualPosition);
	}
	omp_unset_lock(&mutex_self1);

	DOManager::setContextualPosition(_contextualPosition);
}

void DOManagerGroup::setWindowDimensions(WindowDimensions* _windowDimensions){
	omp_set_lock(&mutex_self1);
	map<int, DOManager*>::iterator iter2;
	for(iter2=doManagerMap.begin(); iter2 != doManagerMap.end(); iter2++){
		iter2->second->setWindowDimensions(_windowDimensions);
	}

	map<int, DOManagerGroup*>::iterator iter1;
	for(iter1=doManagerGroupMap.begin(); iter1 != doManagerGroupMap.end(); iter1++){
		iter1->second->setWindowDimensions(_windowDimensions);
	}
	omp_unset_lock(&mutex_self1);

	DOManager::setWindowDimensions(_windowDimensions);
}

void DOManagerGroup::moveAnchors(float x, float y){
	omp_set_lock(&mutex_self1);
	map<int, DOManager*>::iterator iter2;
	for(iter2=doManagerMap.begin(); iter2 != doManagerMap.end(); iter2++){
		iter2->second->moveAnchors(x, y);
	}

	map<int, DOManagerGroup*>::iterator iter1;
	for(iter1=doManagerGroupMap.begin(); iter1 != doManagerGroupMap.end(); iter1++){
		iter1->second->moveAnchors(x, y);
	}
	omp_unset_lock(&mutex_self1);

	DOManager::moveAnchors(x, y);
}

void DOManagerGroup::moveAnchorsRelative(float x, float y){
	omp_set_lock(&mutex_self1);
	map<int, DOManager*>::iterator iter2;
	for(iter2=doManagerMap.begin(); iter2 != doManagerMap.end(); iter2++){
		iter2->second->moveAnchorsRelative(x, y);
	}

	map<int, DOManagerGroup*>::iterator iter1;
	for(iter1=doManagerGroupMap.begin(); iter1 != doManagerGroupMap.end(); iter1++){
		iter1->second->moveAnchorsRelative(x, y);
	}
	omp_unset_lock(&mutex_self1);

	DOManager::moveAnchorsRelative(x, y);
}

void DOManagerGroup::layersDown(int nLayers){
	omp_set_lock(&mutex_self1);
	map<int, DOManager*>::iterator iter2;
	for(iter2=doManagerMap.begin(); iter2 != doManagerMap.end(); iter2++){
		iter2->second->layersDown(nLayers);
	}

	map<int, DOManagerGroup*>::iterator iter1;
	for(iter1=doManagerGroupMap.begin(); iter1 != doManagerGroupMap.end(); iter1++){
		iter1->second->layersDown(nLayers);
	}
	omp_unset_lock(&mutex_self1);

	DOManager::layersDown(nLayers);
}

void DOManagerGroup::layersUp(int nLayers){
	omp_set_lock(&mutex_self1);
	map<int, DOManager*>::iterator iter2;
	for(iter2=doManagerMap.begin(); iter2 != doManagerMap.end(); iter2++){
		iter2->second->layersUp(nLayers);
	}
	map<int, DOManagerGroup*>::iterator iter1;
	for(iter1=doManagerGroupMap.begin(); iter1 != doManagerGroupMap.end(); iter1++){
		iter1->second->layersUp(nLayers);
	}
	omp_unset_lock(&mutex_self1);

	DOManager::layersUp(nLayers);
}
