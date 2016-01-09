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
 * DOManager.cpp
 *
 * Created on: 06/Jan/2015
 * Author: Peter Frank Perroni (pfperroni@inf.ufpr.br)
 */

#include "../../include/gl/core/DOManager.hpp"

omp_lock_t DOManager::mutex;
bool DOManager::lock_started = false;
map<int, DOManager*> DOManager::doGlobalManagerMap;
//bool DOManager::onMove = false;

DOManager::~DOManager(){
	processPendingActions();

	map<int, DesignObject*>::iterator iter2;
	for(iter2=doMap.begin(); iter2 != doMap.end(); iter2++){
		delete iter2->second;
	}

	map<int, DOManager*>::iterator iter1;
	for(iter1=doManagerMap.begin(); iter1 != doManagerMap.end(); iter1++){
		delete iter1->second;
	}

	omp_destroy_lock(&mutex_self1);
	omp_destroy_lock(&mutex_self2);
	if(lock_started){
		omp_destroy_lock(&mutex);
		lock_started = false;
	}

	DesignElement *elemNext;
	for(DesignElement *elemCurr=getFirstDesignElement(); elemCurr != NULL; elemCurr=elemNext){
		elemNext = elemCurr->next;
		delete elemCurr;
	}
}

void DOManager::setCLQueue(clQueue *_cl_queue){
	omp_set_lock(&mutex_self1);
	cl_queue = _cl_queue;
	initializeCL();
	if(isReloadToDeviceNeeded()) loadToDevice();

	map<int, DOManager*>::iterator iter1;
	for(iter1=doManagerMap.begin(); iter1 != doManagerMap.end(); iter1++){
		iter1->second->setCLQueue(_cl_queue);
	}

	map<int, DesignObject*>::iterator iter2;
	DesignObject* dObj;
	for(iter2=doMap.begin(); iter2 != doMap.end(); iter2++){
		dObj = iter2->second;
		dObj->setCLQueue(cl_queue);
		dObj->initializeCL();
		if(dObj->isReloadToDeviceNeeded()) loadToDevice();
	}
	omp_unset_lock(&mutex_self1);
}

void DOManager::repaint(){
//printf("DOManager UID %i trying repaint\n", getUID());
	processPendingActions();
	if(!visible) return;

	omp_set_lock(&mutex_self1);
	if(isReloadToDeviceNeeded()) loadToDevice();

	int type;
	DesignObject *dObj;
	DOManager* doManager;
	glDrawable *drawable;
	for(DesignElement *elem=getFirstDesignElement(); elem != NULL; elem=elem->next){
		drawable = elem->drawable;
		type = drawable->getDrawableType();
		if(type == DESIGN_ELEMENT_DO){
			dObj = (DesignObject*)drawable;
			if(!dObj->isVisible()) continue;
			if(dObj->isReloadToDeviceNeeded()) dObj->loadToDevice();
			dObj->draw();
		}
		else if(type == DESIGN_ELEMENT_DOMANAGER){
			doManager = (DOManager*)drawable;
			if(doManager->isReloadToDeviceNeeded()) doManager->loadToDevice();
			if(doManager->isVisible()) doManager->repaint();
		}
	}

/*
	map<int, DOManager*>::iterator iter1;
	DOManager* doManager;
	for(iter1=doManagerMap.begin(); iter1 != doManagerMap.end(); iter1++){
		doManager = iter1->second;
		if(doManager->isVisible()) doManager->repaint();
	}

	map<int, DesignObject*>::iterator iter2;
	DesignObject *dObj;
	for(iter2=doMap.begin(); iter2 != doMap.end(); iter2++){
		dObj = iter2->second;
//printf("DOManager UID %i repainting UID %i\n", getUID(), dObj->getUID());
		if(!dObj->isVisible()) continue;
		if(dObj->isReloadToDeviceNeeded()) dObj->loadToDevice();
		dObj->draw();
//printf("DOManager UID %i finish repaint on UID %i\n", getUID(), dObj->getUID());
	}
*/
	omp_unset_lock(&mutex_self1);
}

int DOManager::mousePassiveMove(int x, int y) {
	omp_set_lock(&mutex_self1);
	int ret = ACTION_IGNORED;

	map<int, DesignObject*>::iterator iter2;
	DesignObject *dObj;
	for(iter2=doMap.begin(); iter2 != doMap.end(); iter2++){
		dObj = iter2->second;
		if(dObj->isVisible() && dObj->isWithinBounds(x, y)) ret |= dObj->mousePassiveMove(x, y);
		else ret |= dObj->mouseNoPassiveMove(x, y);
	}

	map<int, DOManager*>::iterator iter1;
	DOManager *doManager;
	for(iter1=doManagerMap.begin(); iter1 != doManagerMap.end(); iter1++){
		doManager = iter1->second;
		if(doManager->isVisible() && doManager->isWithinBounds(x, y)) ret |= doManager->mousePassiveMove(x, y);
		else ret |= doManager->mouseNoPassiveMove(x, y);
	}
	omp_unset_lock(&mutex_self1);
//printf("doMap.size()=%i, doManagerMap.size(), ret=%i\n", doMap.size(), doManagerMap.size(), ret);

	return ret;
}

int DOManager::mouseButton(int button, int state, int x, int y) {
	omp_set_lock(&mutex_self1);
	int ret = ACTION_IGNORED;

//	if(button == GLUT_LEFT_BUTTON && state == GLUT_UP){
//		if(isOnMove()){
//			setMoveEnabled(false);
//printf("move disabled for obj=%p->%i (%s)\n", this, isMoveEnabled(), getComponentName());
//		}
//	}

	map<int, DesignObject*>::iterator iter2;
	DesignObject *dObj;
	for(iter2=doMap.begin(); iter2 != doMap.end(); iter2++){
		dObj = iter2->second;
		if(dObj->isVisible() && dObj->isWithinBounds(x, y)) ret |= dObj->mouseButton(button, state, x, y);
		else ret |= dObj->mouseNoButton(button, state, x, y);
	}

	map<int, DOManager*>::iterator iter1;
	DOManager *doManager;
	for(iter1=doManagerMap.begin(); iter1 != doManagerMap.end(); iter1++){
		doManager = iter1->second;
		if(doManager->isVisible() && doManager->isWithinBounds(x, y)) ret |= doManager->mouseButton(button, state, x, y);
		else ret |= doManager->mouseNoButton(button, state, x, y);
	}
//printf("ret=%i\n", ret);

	omp_unset_lock(&mutex_self1);
//printf("doMap.size()=%i, doManagerMap.size(), ret=%i\n", doMap.size(), doManagerMap.size(), ret);

	return ret;
}

int DOManager::mouseMove(int x, int y) {
	omp_set_lock(&mutex_self1);
	int ret = ACTION_IGNORED;

//printf("(%s)->doMap.size()=%i\n", getComponentName(), (int)doMap.size());
	map<int, DesignObject*>::iterator iter2;
	DesignObject *dObj;
	for(iter2=doMap.begin(); iter2 != doMap.end(); iter2++){
		dObj = iter2->second;
		if(dObj->isVisible()) ret |= dObj->mouseMove(x, y);
	}

//printf("(%s)->doManagerMap.size()=%i\n", getComponentName(), (int)doManagerMap.size());
	map<int, DOManager*>::iterator iter1;
	DOManager *doManager;
	for(iter1=doManagerMap.begin(); iter1 != doManagerMap.end(); iter1++){
		doManager = iter1->second;
		if(doManager->isVisible()) ret |= doManager->mouseMove(x, y);
	}
	omp_unset_lock(&mutex_self1);

	return ret;
}

int DOManager::mouseNoPassiveMove(int x, int y) {
	omp_set_lock(&mutex_self1);
	int ret = ACTION_IGNORED;

	map<int, DesignObject*>::iterator iter2;
	for(iter2=doMap.begin(); iter2 != doMap.end(); iter2++){
		ret |= iter2->second->mouseNoPassiveMove(x, y);
	}

	map<int, DOManager*>::iterator iter1;
	for(iter1=doManagerMap.begin(); iter1 != doManagerMap.end(); iter1++){
		ret |= iter1->second->mouseNoPassiveMove(x, y);
	}
	omp_unset_lock(&mutex_self1);

	return ret;
}

int DOManager::mouseNoButton(int button, int state, int x, int y) {
	omp_set_lock(&mutex_self1);
	int ret = ACTION_IGNORED;

	map<int, DesignObject*>::iterator iter2;
	for(iter2=doMap.begin(); iter2 != doMap.end(); iter2++){
		ret |= iter2->second->mouseNoButton(button, state, x, y);
	}

	map<int, DOManager*>::iterator iter1;
	for(iter1=doManagerMap.begin(); iter1 != doManagerMap.end(); iter1++){
		ret |= iter1->second->mouseNoButton(button, state, x, y);
	}

	omp_unset_lock(&mutex_self1);

	return ret;
}

int DOManager::keyboard(unsigned char key, int x, int y) {
	omp_set_lock(&mutex_self1);
	int ret = ACTION_IGNORED;

	map<int, DesignObject*>::iterator iter2;
	DesignObject *dObj;
	for(iter2=doMap.begin(); iter2 != doMap.end(); iter2++){
		dObj = iter2->second;
		if(dObj->isVisible() && dObj->isWithinBounds(x, y)) ret |= dObj->keyboard(key, x, y);
	}

	map<int, DOManager*>::iterator iter1;
	DOManager *doManager;
	for(iter1=doManagerMap.begin(); iter1 != doManagerMap.end(); iter1++){
		doManager = iter1->second;
		if(doManager->isVisible() && doManager->isWithinBounds(x, y)) ret |= doManager->keyboard(key, x, y);
	}
	omp_unset_lock(&mutex_self1);

	return ret;
}

int DOManager::keyboardSpecial(int key, int x, int y) {
	omp_set_lock(&mutex_self1);
	int ret = ACTION_IGNORED;

	map<int, DesignObject*>::iterator iter2;
	DesignObject *dObj;
	for(iter2=doMap.begin(); iter2 != doMap.end(); iter2++){
		dObj = iter2->second;
		if(dObj->isVisible() && dObj->isWithinBounds(x, y)) ret |= dObj->keyboardSpecial(key, x, y);
	}

	map<int, DOManager*>::iterator iter1;
	DOManager *doManager;
	for(iter1=doManagerMap.begin(); iter1 != doManagerMap.end(); iter1++){
		doManager = iter1->second;
		if(doManager->isVisible() && doManager->isWithinBounds(x, y)) ret |= doManager->keyboardSpecial(key, x, y);
	}
	omp_unset_lock(&mutex_self1);

	return ret;
}

bool DOManager::isWithinBounds(int x, int y){
	omp_set_lock(&mutex_self1);
	map<int, DesignObject*>::iterator iter2;
	for(iter2=doMap.begin(); iter2 != doMap.end(); iter2++){
		if(iter2->second->isWithinBounds(x, y)){
			omp_unset_lock(&mutex_self1);
			return true;
		}
	}

	map<int, DOManager*>::iterator iter1;
	for(iter1=doManagerMap.begin(); iter1 != doManagerMap.end(); iter1++){
		if(iter1->second->isWithinBounds(x, y)){
			omp_unset_lock(&mutex_self1);
			return true;
		}
	}
	omp_unset_lock(&mutex_self1);

	return false;
}

bool DOManager::enforceBounds(){
	omp_set_lock(&mutex_self1);
	bool ret = false;

	map<int, DesignObject*>::iterator iter2;
	for(iter2=doMap.begin(); iter2 != doMap.end(); iter2++){
		ret |= iter2->second->enforceBounds();
	}

	map<int, DOManager*>::iterator iter1;
	for(iter1=doManagerMap.begin(); iter1 != doManagerMap.end(); iter1++){
		ret |= iter1->second->enforceBounds();
	}
	omp_unset_lock(&mutex_self1);

	return ret;
}

void DOManager::addDesignElement(glDrawable *drawable){
	if(drawable == NULL) return;

	int _uid = drawable->getUID();
	int type = drawable->getDrawableType();
	if(type == DESIGN_ELEMENT_DO){
		if(getDesignObject(_uid) != NULL) return;
		omp_set_lock(&mutex_self1);
		DesignObject *dObj = (DesignObject*)drawable;
		doMap[_uid] = dObj;
		dObj->setParent(getSelfDesignElement());
		dObj->setWindowDimensions(windowDimensions);
		dObj->setCLQueue(getCLQueue());
		dObj->initializeCL();
		dObj->loadToDevice();
		dObj->layerUp(1);
		omp_unset_lock(&mutex_self1);
	}
	else if(type == DESIGN_ELEMENT_DOMANAGER){
		if(getDOManager(_uid) != NULL) return;
		omp_set_lock(&mutex_self1);
		DOManager* doManager = (DOManager*)drawable;
		doManagerMap[_uid] = doManager;
		doManager->setParent(getSelfDesignElement());
		doManager->setWindowDimensions(windowDimensions);
		doManager->setCLQueue(getCLQueue());
		doManager->initializeCL();
		doManager->loadToDevice();
		doManager->layersUp(1);
		omp_unset_lock(&mutex_self1);
	}

	omp_set_lock(&mutex_self1);
	addDesignElementToList(&designList, drawable);
	omp_unset_lock(&mutex_self1);
}

void DOManager::addDesignElementToList(DesignList *_designList, glDrawable *drawable){
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

void DOManager::processPendingActions(){
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
		_uid = drawable->getUID();
		if(type == DESIGN_ELEMENT_DO){
			map<int, DesignObject*>::iterator iter = doMap.find(_uid);
			if(iter != doMap.end()){
				doMap.erase(iter);
			}
		}
		else if(type == DESIGN_ELEMENT_DOMANAGER){
			map<int, DOManager*>::iterator iter = doManagerMap.find(_uid);
			if(iter != doManagerMap.end()){
				doManagerMap.erase(iter);
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

void DOManager::removeDesignElement(DesignElement *elem){
	if(elem == NULL) return;
	omp_set_lock(&mutex_self2);
	addDesignElementToList(&pendingActions[DESIGN_LIST_DELETES], elem->drawable);
	omp_unset_lock(&mutex_self2);
}

void DOManager::removeMe(){
	DesignElement *_parent = getParent();
	if(_parent == NULL) return;

	_parent->drawable->removeDesignElement(getSelfDesignElement());
}

void DOManager::addMe(DesignElement *parent){
	if(parent != NULL){
		parent->drawable->scheduleAddDesignElement(getSelfDesignElement());
	}
}

int DOManager::getDesignElementListSize(){
	int count = 0;
	omp_set_lock(&mutex_self2);
	for(DesignElement *elem=getFirstDesignElement(); elem != NULL; elem=elem->next, count++);
	omp_unset_lock(&mutex_self2);
	return count;
}

DesignElement* DOManager::getFirstDesignElement(){
	return designList.first;
}

/*void DOManager::addDO(DesignObject *dObj){
	if(dObj == NULL) return;
	if(getDesignObject(dObj->getUID()) != NULL) return;

	omp_set_lock(&mutex_self1);
	doMap[dObj->getUID()] = dObj;
	dObj->setParent(getSelfDesignElement());
	dObj->setWindowDimensions(windowDimensions);
	dObj->setCLQueue(getCLQueue());
	dObj->initializeCL();
	dObj->loadToDevice();
	dObj->layerUp(1);
	omp_unset_lock(&mutex_self1);

	addDesignElement(dObj);
}

void DOManager::addDOManager(DOManager *doManager){
	if(doManager == NULL) return;
	if(getDOManager(doManager->getUID()) != NULL) return;

	omp_set_lock(&mutex_self1);
	doManagerMap[doManager->getUID()] = doManager;
	doManager->setParent(getSelfDesignElement());
	doManager->setWindowDimensions(windowDimensions);
	doManager->setCLQueue(getCLQueue());
	doManager->initializeCL();
	doManager->loadToDevice();
	doManager->layersUp(1);
	omp_unset_lock(&mutex_self1);

	addDesignElement(doManager);
}
*/
void DOManager::scheduleAddDesignElement(DesignElement *elem){
	if(elem == NULL) return;
	addDesignElementToList(&pendingActions[DESIGN_LIST_ADDS], elem->drawable);
//	if(doManager == NULL) return;
//	if(getDOManager(doManager->getUID()) != NULL) return;
//
//	doManagerMap[doManager->getUID()] = doManager;
//	doManager->setParent(getSelfDesignElement());
//	doManager->setWindowDimensions(windowDimensions);
//	doManager->setCLQueue(getCLQueue());
//	doManager->initializeCL();
//	doManager->loadToDevice();
//	doManager->layersUp(1);
//	addDesignElement(doManager);
}

/*DesignObject* DOManager::removeDO(int doUID){
	omp_set_lock(&mutex_self1);

	map<int, DesignObject*>::iterator iter = doMap.find(doUID);
	if(iter == doMap.end()){
		return NULL;
	}
	else{
		doMap.erase(iter);
	}
	omp_unset_lock(&mutex_self1);

	return iter->second;
}*/

void DOManager::setVisible(bool _visible){
	omp_set_lock(&mutex_self1);

	map<int, DesignObject*>::iterator iter2;
	for(iter2=doMap.begin(); iter2 != doMap.end(); iter2++){
		iter2->second->setVisible(_visible);
	}

	map<int, DOManager*>::iterator iter1;
	for(iter1=doManagerMap.begin(); iter1 != doManagerMap.end(); iter1++){
		iter1->second->setVisible(_visible);
	}

	visible = _visible;
	omp_unset_lock(&mutex_self1);
}

void DOManager::setContextualPosition(bool _contextualPosition){
	omp_set_lock(&mutex_self1);

	map<int, DesignObject*>::iterator iter2;
	for(iter2=doMap.begin(); iter2 != doMap.end(); iter2++){
		iter2->second->setContextualPosition(_contextualPosition);
	}

	map<int, DOManager*>::iterator iter1;
	for(iter1=doManagerMap.begin(); iter1 != doManagerMap.end(); iter1++){
		iter1->second->setContextualPosition(_contextualPosition);
	}

	Movable::setContextualPosition(_contextualPosition);
	omp_unset_lock(&mutex_self1);
}

void DOManager::setWindowDimensions(WindowDimensions* _windowDimensions){
	omp_set_lock(&mutex_self1);

	map<int, DesignObject*>::iterator iter2;
	for(iter2=doMap.begin(); iter2 != doMap.end(); iter2++){
		iter2->second->setWindowDimensions(_windowDimensions);
	}

	map<int, DOManager*>::iterator iter1;
	for(iter1=doManagerMap.begin(); iter1 != doManagerMap.end(); iter1++){
		iter1->second->setWindowDimensions(_windowDimensions);
	}

	windowDimensions = _windowDimensions;
	omp_unset_lock(&mutex_self1);
}

void DOManager::moveAnchors(float x, float y){
	omp_set_lock(&mutex_self1);
	XYZ anchor = {x, y, 0.0};

	map<int, DesignObject*>::iterator iter2;
	for(iter2=doMap.begin(); iter2 != doMap.end(); iter2++){
		iter2->second->moveAnchor(anchor);
	}

	map<int, DOManager*>::iterator iter1;
	for(iter1=doManagerMap.begin(); iter1 != doManagerMap.end(); iter1++){
		iter1->second->moveAnchors(x, y);
	}

	omp_unset_lock(&mutex_self1);
}

void DOManager::moveAnchorsRelative(float x, float y){
	omp_set_lock(&mutex_self1);
	map<int, DesignObject*>::iterator iter2;
	for(iter2=doMap.begin(); iter2 != doMap.end(); iter2++){
		iter2->second->moveAnchorRelative(x, y);
	}

	map<int, DOManager*>::iterator iter1;
	for(iter1=doManagerMap.begin(); iter1 != doManagerMap.end(); iter1++){
		iter1->second->moveAnchorsRelative(x, y);
	}
	omp_unset_lock(&mutex_self1);
}

void DOManager::layersDown(int nLayers){
	omp_set_lock(&mutex_self1);
	map<int, DesignObject*>::iterator iter2;
	for(iter2=doMap.begin(); iter2 != doMap.end(); iter2++){
		iter2->second->layerDown(nLayers);
	}

	map<int, DOManager*>::iterator iter1;
	for(iter1=doManagerMap.begin(); iter1 != doManagerMap.end(); iter1++){
		iter1->second->layersDown(nLayers);
	}
	omp_unset_lock(&mutex_self1);
}

void DOManager::layersUp(int nLayers){
	omp_set_lock(&mutex_self1);
	map<int, DesignObject*>::iterator iter2;
	for(iter2=doMap.begin(); iter2 != doMap.end(); iter2++){
		iter2->second->layerUp(nLayers);
	}

	map<int, DOManager*>::iterator iter1;
	for(iter1=doManagerMap.begin(); iter1 != doManagerMap.end(); iter1++){
		iter1->second->layersUp(nLayers);
	}
	omp_unset_lock(&mutex_self1);
}

void DOManager::setParent(DesignElement* parent){
	if(parent != NULL){
		getSelfDesignElement()->prev = parent;
	}
}

DesignElement* DOManager::getParent(){
	return getSelfDesignElement()->prev;
}

DesignElement* DOManager::getTopParent(){
	DesignElement *curr, *next;
//	printf("curr(%s vs %s)->", getComponentName(), ((DOManager*)getSelfDesignElement()->item)->getComponentName());
	omp_set_lock(&mutex_self2);
	for(curr=getSelfDesignElement()->prev; curr != NULL && (next=curr->prev) != NULL; curr=next);//{printf("curr(%s)->", ((DOManager*)curr->item)->getComponentName());}
	omp_unset_lock(&mutex_self2);
//	printf("curr(%s)\n", ((DOManager*)curr->item)->getComponentName());
	return curr;
}

const char* DOManager::getComponentName(){
	return componentName;
}

DOManager* DOManager::getGlobalDOManager(int _UID){
	omp_set_lock(&mutex);
	map<int, DOManager*>::iterator iter = doGlobalManagerMap.find(_UID);
	omp_unset_lock(&mutex);
	if(iter != doGlobalManagerMap.end()){
		return iter->second;
	}
	return NULL;
}

DOManager* DOManager::getDOManager(int _UID){
	omp_set_lock(&mutex_self1);
	map<int, DOManager*>::iterator iter = doManagerMap.find(_UID);
	omp_unset_lock(&mutex_self1);
	if(iter != doManagerMap.end()){
		return iter->second;
	}
	return NULL;
}

DesignObject* DOManager::getDesignObject(int _UID){
	omp_set_lock(&mutex_self1);
	map<int, DesignObject*>::iterator iter = doMap.find(_UID);
	omp_unset_lock(&mutex_self1);
	if(iter != doMap.end()){
		return iter->second;
	}
	return NULL;
}

void DOManager::loadToDevice(){
	reloadToDeviceNeeded = false;
}

WindowDimensions* DOManager::getWindowDimensions(){
	return windowDimensions;
}

bool DOManager::isReloadToDeviceNeeded(){
	return reloadToDeviceNeeded;
}

void DOManager::setReloadToDeviceNeeded(bool _reloadToDeviceNeeded){
	reloadToDeviceNeeded = _reloadToDeviceNeeded;
}

clQueue* DOManager::getCLQueue(){
	return cl_queue;
}

int DOManager::getUID(){
	return UID;
}

bool DOManager::isVisible(){
	return visible;
}

void DOManager::getBounds(XYZ &vertice1, XYZ &vertice2){
	XYZ vertice = {0, 0, 0};
	vertice1 = vertice2 = vertice;
}
