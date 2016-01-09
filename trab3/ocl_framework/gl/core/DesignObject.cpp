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
 * DesignObject.cpp
 *
 * Created on: 07/Jan/2015
 * Author: Peter Frank Perroni (pfperroni@inf.ufpr.br)
 */

#include "../../include/gl/core/DesignObject.hpp"

omp_lock_t DesignObject::mutex;
bool DesignObject::lock_started = false;
map<int, DesignObject*> DesignObject::doMap;

DesignObject::DesignObject(XYZ _anchor, const char* _componentName){
	windowDimensions = NULL;
	setup(anchor, NULL, _componentName);
}

DesignObject::DesignObject(XYZ _anchor, DOParameters *_params, const char* _componentName){
	windowDimensions = NULL;
	setup(_anchor, _params, _componentName);
}

DesignObject::~DesignObject(){
	if(params_internal != NULL) delete params_internal;
//	printf("~DesignObject deleted\n");

	if(lock_started){
		omp_destroy_lock(&mutex);
		lock_started = false;
	}
}

void DesignObject::setup(XYZ _anchor, DOParameters *_params, const char* _componentName){
	// Generate the Unique Identifier for this object.
	UID = UniqueUIDGenerator::nextUniqueObjectUID();
	setDrawableType(DESIGN_ELEMENT_DO);
	componentName = _componentName;

	if(!lock_started){
		omp_init_lock(&mutex);
		lock_started = true;
	}

	omp_set_lock(&mutex);
	doMap[UID] = this;
	omp_unset_lock(&mutex);

	anchor = _anchor;
	anchor.z = 0;
	params_internal = _params;
	if(windowDimensions != NULL){
		refreshInternalDimensions();
		anchor.z = windowDimensions->getPixelDepth() / 2;
		anchorScreenPos.x = windowDimensions->getScreenCoordX(anchor.x);
		anchorScreenPos.y = windowDimensions->getScreenCoordY(anchor.y);
		anchorScreenPos.z = windowDimensions->getScreenCoordZ(anchor.z);
	}
	else{
		anchorScreenPos.x = anchorScreenPos.y = anchorScreenPos.z = 0.0f;
	}
	reloadToDeviceNeeded = true;
	visible = true;
	setEditable(false);
}

DesignObject* DesignObject::getDesignObject(int UID){
	return doMap[UID];
}

DOParameters* DesignObject::getParams(){
	return params_internal;
}

void DesignObject::loadToDevice(){
	reloadToDeviceNeeded = false;
}

int DesignObject::mousePassiveMove(int x, int y){
	return ACTION_IGNORED;
}

int DesignObject::mouseButton(int button, int state, int x, int y){
	return ACTION_IGNORED;
}

int DesignObject::mouseMove(int x, int y){
	return ACTION_IGNORED;
}

int DesignObject::mouseNoPassiveMove(int x, int y){
	return ACTION_IGNORED;
}

int DesignObject::mouseNoButton(int button, int state, int x, int y){
	return ACTION_IGNORED;
}

int DesignObject::keyboard(unsigned char key, int x, int y){
	return ACTION_IGNORED;
}

int DesignObject::keyboardSpecial(int key, int x, int y){
	return ACTION_IGNORED;
}

void DesignObject::setVisible(bool _visible){
	visible = _visible;
}

bool DesignObject::isVisible(){
	return visible;
}

bool DesignObject::isWithinBounds(int x, int y){
	return false;
}

void DesignObject::getBounds(XYZ &vertice1, XYZ &vertice2){
	XYZ vertice = {0, 0, 0};
	vertice1 = vertice2 = vertice;
}

bool DesignObject::isReloadToDeviceNeeded(){
	return reloadToDeviceNeeded;
}

void DesignObject::setReloadToDeviceNeeded(bool _reloadToDeviceNeeded){
	reloadToDeviceNeeded = _reloadToDeviceNeeded;
}

bool DesignObject::isContextualPosition(){
	return contextualPosition;
}

void DesignObject::setContextualPosition(bool _contextualPosition){
	contextualPosition = _contextualPosition;
}

WindowDimensions* DesignObject::getWindowDimensions(){
	return windowDimensions;
}

int DesignObject::getUID(){
	return UID;
}

void DesignObject::moveAnchor(XYZ _anchor){
//printf("moving anchor [%f,%f] to [%f,%f]\n", anchor.x, anchor.y, _anchor.x, _anchor.y);
	anchor.x = _anchor.x; anchor.y = _anchor.y;
	moveTo(anchor.x, anchor.y);
	enforceBounds();
}

void DesignObject::moveAnchorRelative(float x, float y){
	XYZ _anchor;
	_anchor.x = anchor.x + x;
	_anchor.y = anchor.y + y;
	moveAnchor(_anchor);
}

XYZ DesignObject::getAnchor(){
	return anchor;
}

void DesignObject::layerUp(int nLayers){
	anchor.z -= nLayers * (getWindowDimensions()->getHalfPixelDepth() / 100.0f);
	anchorScreenPos.z = windowDimensions->getScreenCoordZ(anchor.z);
}

void DesignObject::layerDown(int nLayers){
	anchor.z += nLayers * (getWindowDimensions()->getHalfPixelDepth() / 100.0f);
	anchorScreenPos.z = windowDimensions->getScreenCoordZ(anchor.z);
}

void DesignObject::setParent(DesignElement* parent){
	if(parent != NULL){
		getSelfDesignElement()->prev = parent;
	}
}

DesignElement* DesignObject::getParent(){
	return getSelfDesignElement()->prev;
}

DesignElement* DesignObject::getTopParent(){
	DesignElement *curr, *next;
	for(curr=getSelfDesignElement()->prev; curr != NULL && (next=curr->next) != NULL; curr=next);
	return curr;
}

void DesignObject::removeMe(){
	DesignElement *_parent = getParent();
	if(_parent == NULL) return;

	_parent->drawable->removeDesignElement(getSelfDesignElement());
}

const char* DesignObject::getComponentName(){
	return componentName;
}

void DesignObject::refresh(DOParameters *_params){
	if(params_internal != NULL) delete params_internal;
	params_internal = _params;
	reloadToDeviceNeeded = true;
	refreshInternalDimensions();
}

void DesignObject::setWindowDimensions(WindowDimensions* _windowDimensions){
	if(_windowDimensions == NULL) return;
	windowDimensions = _windowDimensions;
	anchor.z = windowDimensions->getPixelDepth() / 2;
	refreshInternalDimensions();
	anchorScreenPos.x = windowDimensions->getScreenCoordX(anchor.x);
	anchorScreenPos.y = windowDimensions->getScreenCoordY(anchor.y);
	anchorScreenPos.z = windowDimensions->getScreenCoordZ(anchor.z);
}

void DesignObject::changeColorIntensity(float &r, float &g, float &b, float rate){
	if((r = r * rate) > 1.0f) r = 1.0f; else if(r < 0.0f) r = 0.0f;
	if((g = g * rate) > 1.0f) g = 1.0f; else if(g < 0.0f) g = 0.0f;
	if((b = b * rate) > 1.0f) b = 1.0f; else if(b < 0.0f) b = 0.0f;
}

bool DesignObject::isRefreshInternalDimensionsNeeded(){
//printf("=>[%f->%f %f->%f]\n", screenPos.x, getWindowDimension()->getScreenCoordX(anchor.x), screenPos.y, getWindowDimension()->getScreenCoordY(anchor.y));
	return anchorScreenPos.x != windowDimensions->getScreenCoordX(anchor.x) ||
			anchorScreenPos.y != windowDimensions->getScreenCoordY(anchor.y);
}

bool DesignObject::enforceBounds(){
	if(windowDimensions != NULL && isRefreshInternalDimensionsNeeded()){
		refreshInternalDimensions();
		anchorScreenPos.x = windowDimensions->getScreenCoordX(anchor.x);
		anchorScreenPos.y = windowDimensions->getScreenCoordY(anchor.y);
		anchorScreenPos.z = windowDimensions->getScreenCoordZ(anchor.z);
		return true;
	}
	return false;
}
