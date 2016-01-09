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
 * DOManager.hpp
 *
 * Created on: 06/Jan/2015
 * Author: Peter Frank Perroni (pfperroni@inf.ufpr.br)
 */

#ifndef _DOMANAGER_HPP_
#define _DOMANAGER_HPP_

#include "DesignObject.hpp"
#include "FocusManager.hpp"
#include "WindowDimensions.hpp"
#include "Editable.hpp"
#include "Movable.hpp"
#include "glDrawable.hpp"

#include <map>

/**
 * @brief Class representing a more complex and non directly drawable object.
 *
 * It is a container of directly drawable objects but it cannot draw itself.
 * If any draw is required, a DesignObject must be used instead.
 * It can also contain other DOManager's, which can be arranged in cascade.
 */
class DOManager : public glDrawable, public clObject, public Editable, public Movable{
private:
	static omp_lock_t mutex;
	static map<int, DOManager*> doGlobalManagerMap;
	static bool lock_started;

	omp_lock_t mutex_self1, mutex_self2;
	int UID;
	const char*componentName;
	DesignList designList; // Separate list holding the order that drawables should be drawn on screen for this DOManager.
	map<int, DesignList> pendingActions;
	WindowDimensions* windowDimensions;
	map<int, DesignObject*> doMap;
	map<int, DOManager*> doManagerMap;
	bool visible;
	bool reloadToDeviceNeeded;
	clQueue *cl_queue;

	/**
	 * @brief Schedule a design element to be added later to this container.
	 *
	 * Specially useful when avoiding blocking contexts.
	 *
	 * @param elem The design element to be added.
	 */
	virtual void scheduleAddDesignElement(DesignElement *elem);

	/**
	 * @brief Add a drawable to this container.
	 *
	 * @param _designList The design list where to add the drawable to.
	 * @param drawable The drawable to be added.
	 */
	void addDesignElementToList(DesignList *_designList, glDrawable *drawable);

	/**
	 * @brief Process all scheduled pending actions.
	 */
	void processPendingActions();

public:
	DOManager(WindowDimensions* _windowDimensions, const char* _componentName){
		// Generate the Unique Identifier for this object.
		UID = UniqueUIDGenerator::nextUniqueObjectUID();
		componentName = _componentName;

		setDrawableType(DESIGN_ELEMENT_DOMANAGER);
		designList.type = DESIGN_LIST_DRAWABLES;
		designList.first = designList.last = NULL;

		pendingActions[DESIGN_LIST_ADDS].type = DESIGN_LIST_ADDS;
		pendingActions[DESIGN_LIST_ADDS].first = pendingActions[DESIGN_LIST_ADDS].last = NULL;

		pendingActions[DESIGN_LIST_DELETES].type = DESIGN_LIST_DELETES;
		pendingActions[DESIGN_LIST_DELETES].first = pendingActions[DESIGN_LIST_DELETES].last = NULL;

		if(!lock_started){
			omp_init_lock(&mutex);
			lock_started = true;
		}

		omp_set_lock(&mutex);
		doGlobalManagerMap[UID] = this;
		omp_unset_lock(&mutex);

		omp_init_lock(&mutex_self1);
		omp_init_lock(&mutex_self2);

		windowDimensions = _windowDimensions;
		visible = true;
		setEditable(false);
		reloadToDeviceNeeded = true;
		cl_queue = NULL;
	}
	virtual ~DOManager();

	/**
	 * @brief Static method to obtain a specific DOManager from the list
	 * of all instances created by the application.
	 *
	 * @param _UID The DOManager UID.
	 * @return The DOManager.
	 */
	static DOManager* getGlobalDOManager(int _UID);

	/**
	 * @brief Repaint all components under this container.
	 */
	virtual void repaint();

	/**
	 * @brief Forward mouse passive move event (simple movement without click) to drawables under this container.
	 *
	 * The methods isVisible() and isWithinBounds(int, int) are used to determine which drawables should receive this event.
	 *
	 * @param x The screen X coordinate where the mouse is.
	 * @param y The screen Y coordinate where the mouse is.
	 * @return Return one of the valid actions:
	 * ACTION_IGNORED (if the event was ignored by all components within this container),
	 * ACTION_CHECKED (if the event was checked by at least one component),
	 * ACTION_TAKEN (if some work was actually done by at least one component).
	 */
	virtual int mousePassiveMove(int x, int y);

	/**
	 * @brief Forward mouse click event to drawables under this container.
	 *
	 * The methods isVisible() and isWithinBounds(int, int) are used to determine which drawables should receive this event.
	 *
	 * @param button What button was clicked.
	 * @param state What is the state of the button (up/down).
	 * @param x The screen X coordinate where the mouse is.
	 * @param y The screen Y coordinate where the mouse is.
	 * @return Return one of the valid actions:
	 * ACTION_IGNORED (if the event was ignored by all components within this container),
	 * ACTION_CHECKED (if the event was checked by at least one component),
	 * ACTION_TAKEN (if some work was actually done by at least one component).
	 */
	virtual int mouseButton(int button, int state, int x, int y);

	/**
	 * @brief Forward mouse drag-and-drop event to drawables under this container.
	 *
	 * No bound area validation is done prior to forwarding this event, so all visible drawables will receive this event.
	 *
	 * @param x The screen X coordinate where the mouse is.
	 * @param y The screen Y coordinate where the mouse is.
	 * @return Return one of the valid actions:
	 * ACTION_IGNORED (if the event was ignored by all components within this container),
	 * ACTION_CHECKED (if the event was checked by at least one component),
	 * ACTION_TAKEN (if some work was actually done by at least one component).
	 */
	virtual int mouseMove(int x, int y);

	/**
	 * @brief Forward mouse out-of-focus event (mouse out of the bound area) to drawables under this container.
	 *
	 * The methods isVisible() and isWithinBounds(int, int) are used to determine which drawables should receive this event.
	 *
	 * @param x The screen X coordinate where the mouse is.
	 * @param y The screen Y coordinate where the mouse is.
	 * @return Return one of the valid actions:
	 * ACTION_IGNORED (if the event was ignored by all components within this container),
	 * ACTION_CHECKED (if the event was checked by at least one component),
	 * ACTION_TAKEN (if some work was actually done by at least one component).
	 */
	virtual int mouseNoPassiveMove(int x, int y);

	/**
	 * @brief Forward mouse click out-of-bounds event (mouse click out of the bound area) to drawables under this container.
	 *
	 * The methods isVisible() and isWithinBounds(int, int) are used to determine which drawables should receive this event.
	 *
	 * @param button What button was clicked.
	 * @param state What is the state of the button (up/down).
	 * @param x The screen X coordinate where the mouse is.
	 * @param y The screen Y coordinate where the mouse is.
	 * @return Return one of the valid actions:
	 * ACTION_IGNORED (if the event was ignored by all components within this container),
	 * ACTION_CHECKED (if the event was checked by at least one component),
	 * ACTION_TAKEN (if some work was actually done by at least one component).
	 */
	virtual int mouseNoButton(int button, int state, int x, int y);

	/**
	 * @brief Forward keyboard events for non-special keys to drawables under this container.
	 *
	 * The methods isVisible() and isWithinBounds(int, int) are used to determine which drawables should receive this event.
	 *
	 * @param key The key pressed.
	 * @param x The screen X coordinate where the mouse is.
	 * @param y The screen Y coordinate where the mouse is.
	 * @return Return one of the valid actions:
	 * ACTION_IGNORED (if the event was ignored by all components within this container),
	 * ACTION_CHECKED (if the event was checked by at least one component),
	 * ACTION_TAKEN (if some work was actually done by at least one component).
	 */
	virtual int keyboard(unsigned char key, int x, int y);

	/**
	 * @brief Forward keyboard events for special keys to drawables under this container.
	 *
	 * The methods isVisible() and isWithinBounds(int, int) are used to determine which drawables should receive this event.
	 *
	 * @param key The special key pressed.
	 * @param x The screen X coordinate where the mouse is.
	 * @param y The screen Y coordinate where the mouse is.
	 * @return Return one of the valid actions:
	 * ACTION_IGNORED (if the event was ignored by all components within this container),
	 * ACTION_CHECKED (if the event was checked by at least one component),
	 * ACTION_TAKEN (if some work was actually done by at least one component).
	 */
	virtual int keyboardSpecial(int key, int x, int y);

	/**
	 * @brief Move the anchor for all drawables under this container.
	 *
	 * It is usually called in a drag-and-drop event, but can be called every time the drawable
	 * must be moved to a different coordinate.
	 *
	 * @param x The absolute coordinate X to move this drawable to.
	 * @param y The absolute coordinate Y to move this drawable to.
	 */
	virtual void moveAnchors(float x, float y);

	/**
	 * @brief Move the anchor for all drawables under this container.
	 *
	 * It is usually called in a drag-and-drop event, but can be called every time the drawable
	 * must be moved to a different coordinate.
	 *
	 * @param x The shift on X position to move this drawable to.
	 * @param y The shift on Y position to move this drawable to.
	 */
	virtual void moveAnchorsRelative(float x, float y);

	/**
	 * @brief Check if any component inside this container has the specified [X,Y] coordinate within its bounds.
	 *
	 * @param x The X coordinate.
	 * @param y The Y coordinate.
	 * @return If the coordinate is within the bounds of any component inside this container.
	 */
	virtual bool isWithinBounds(int x, int y);

	/**
	 * @brief Get the top-left and bottom-right vertices for this container.
	 *
	 * @param vertice1 The top-left vertex.
	 * @param vertice2 The bottom-right vertex.
	 */
	virtual void getBounds(XYZ &vertice1, XYZ &vertice2);

	/**
	 * @brief Ensure that the all components within this container have their bounds valid.
	 *
	 * Must be called every time any change is made over the components coordinates.
	 *
	 * @return True if the bounds were adjusted. False otherwise.
	 */
	virtual bool enforceBounds();

	/**
	 * @brief Set the window dimensions for all drawables within this container.
	 *
	 * @param _windowDimensions The window dimensions.
	 */
	virtual void setWindowDimensions(WindowDimensions* _windowDimensions);

	/**
	 * @brief Abstract method that must be overridden by all classes, returning the screen width (in pixels) of this container.
	 *
	 * @return The screen width (in pixels) of this container.
	 */
	virtual int getWidthInPixels() = 0;

	/**
	 * @brief Abstract method that must be overridden by all classes, returning the screen height (in pixels) of this container.
	 *
	 * @return The screen height (in pixels) of this container.
	 */
	virtual int getHeightInPixels() = 0;

	/**
	 * @brief Empty method, just implementing the abstract method from clObject.
	 */
	virtual void initializeCL(){}

	/**
	 * @brief Set reloadToDeviceNeeded to false.
	 *
	 * Must be called by any method overriding it to ensure the flag will be disabled.
	 */
	virtual void loadToDevice();

	/**
	 * @brief Empty method, just implementing the abstract method from clObject.
	 */
	virtual void runKernel(){}

	/**
	 * @brief Schedule to remove the design element specified from this container.
	 *
	 * @param elem The design element to be removed.
	 */
	virtual void removeDesignElement(DesignElement *elem);

	/**
	 * @brief Get the DOManager under this container with the specified UID.
	 *
	 * @param _UID The DOManager UID.
	 * @return The DOManager found.
	 */
	DOManager* getDOManager(int _UID);

	/**
	 * @brief Get the design object under this container with the specified UID.
	 *
	 * @param _UID The design object UID.
	 * @return The design object found.
	 */
	DesignObject* getDesignObject(int _UID);

	/**
	 * @brief Set the clQueue for all components within this container and then initialize them.
	 *
	 * The method initializeCL() is called to start up the CL objects.
	 *
	 * @param _cl_queue The clQueue.
	 */
	void setCLQueue(clQueue *_cl_queue);

	/**
	 * @brief Get the clQueue used by this container and its components.
	 *
	 * @return The clQueue.
	 */
	clQueue* getCLQueue();

	/**
	 * @brief Set the parent container for this container.
	 *
	 * @param parent The parent container.
	 */
	void setParent(DesignElement* parent);

	/**
	 * @brief Get the parent container.
	 *
	 * @return The parent container.
	 */
	DesignElement* getParent();

	/**
	 * @brief Get the root container in the parenthood chain.
	 *
	 * @return The top parent.
	 */
	DesignElement* getTopParent();

	/**
	 * @brief Get this component's name.
	 *
	 * @return The component name.
	 */
	const char* getComponentName();

	/**
	 * @brief Get the window dimension used by this container.
	 *
	 * @return The window dimension.
	 */
	WindowDimensions* getWindowDimensions();

	/**
	 * @brief Check the state that tells if the data and configuration must be reloaded to GPU.
	 *
	 * @return If a reload to GPU is required.
	 */
	bool isReloadToDeviceNeeded();

	/**
	 * @brief Set the state that tells if the data and configuration must be reloaded to GPU.
	 *
	 * @param _reloadToDeviceNeeded If a reload to GPU is required.
	 */
	void setReloadToDeviceNeeded(bool _reloadToDeviceNeeded);

	/**
	 * @brief Set the state that indicates if the components under this container will respect a contextual position.
	 *
	 * If the contextual position is set, the components will be moved along with this container on screen,
	 * in a drag-and-drop event.
	 *
	 * @param _contextualPosition If the position is contextual.
	 */
	void setContextualPosition(bool _contextualPosition);

	/**
	 * @brief Add a drawable to the list of components of this container.
	 *
	 * It can be a DesignObject or a DOManager.
	 *
	 * @param drawable The drawable to be added.
	 */
	void addDesignElement(glDrawable *drawable);

	/**
	 * @brief Schedule to add this container to a parent container later.
	 *
	 * @param parent The parent to be added to.
	 */
	void addMe(DesignElement *parent);

	/**
	 * @brief Remove this container from its parent container.
	 */
	void removeMe();

	/**
	 * @brief Get the list of drawables added to this container.
	 *
	 * It does not count the own container as one item of the list,
	 * although a DOManager is also a component itself.
	 *
	 * @return The count of drawables under this container.
	 */
	int getDesignElementListSize();

	/**
	 * @brief Get the first drawable component from the list
	 *
	 * @return The first drawable.
	 */
	DesignElement* getFirstDesignElement();

	/**
	 * @brief Get the UID for this container.
	 *
	 * @return The UID.
	 */
	int getUID();

	/**
	 * @brief Set the visibility state of this container and its components.
	 *
	 * @param _visible If visible or not.
	 */
	void setVisible(bool _visible);

	/**
	 * @brief Get the visibility state of this container.
	 *
	 * @return If visible or not.
	 */
	bool isVisible();

	/**
	 * @brief Move the layer for this container and all its components up.
	 *
	 * Move the Z coordinate the specified number of layers up.
	 * The Z coordinate is internally divided in 100 layers.
	 *
	 * @param nLayers The number of layers to move this container and all its components up.
	 */
	void layersDown(int nLayers);

	/**
	 * @brief Move the layer for this container and all its components down.
	 *
	 * Move the Z coordinate the specified number of layers down.
	 * The Z coordinate is internally divided in 100 layers.
	 *
	 * @param nLayers The number of layers to move this container and all its components down.
	 */
	void layersUp(int nLayers);
};

#endif /* _DOMANAGER_HPP_ */
