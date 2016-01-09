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
 * DesignObject.hpp
 *
 * Created on: 06/Jan/2015
 * Author: Peter Frank Perroni (pfperroni@inf.ufpr.br)
 */

#ifndef _DESIGNOBJECT_HPP_
#define _DESIGNOBJECT_HPP_

#include <CL/cl.h>
#include <GL/glew.h>
#include <GL/freeglut.h>
#include <stdio.h>
#include <cstdlib>
#include <omp.h>
#include <map>
#include <limits.h>
#include "WindowDimensions.hpp"
#include "Editable.hpp"
#include "glDrawable.hpp"
#include "../../UniqueUIDGenerator.hpp"
#include "../../cl/core/clFactory.hpp"
#include "../../cl/core/clObject.hpp"
#include "../../types.hpp"

using namespace std;

class DOManager;

/**
 * @brief Super struct representing a generic list of parameters to be passed to DesignObject's or DOManager's.
 *
 * All list of parameters must extend this struct.
 */
struct DOParameters {};

/**
 * @brief Class representing a directly drawable object.
 *
 * Any object that is directly drawable must extend this class.
 * A drawable cannot be a container of drawable objects, therefore, if it is required
 * to group multiple drawables, a DOManager must be used instead.
 */
class DesignObject : public glDrawable, public clObject, public Editable{
private:
	static omp_lock_t mutex;
	static map<int, DesignObject*> doMap;
	static bool lock_started;

	int UID;
	const char *componentName;
	bool reloadToDeviceNeeded;
	bool visible;
	bool contextualPosition;
	XYZ anchor, anchorScreenPos;
	DOParameters *params_internal;
	WindowDimensions* windowDimensions;

	void setup(XYZ _anchor, DOParameters *_params, const char* _componentName);

	/**
	 * @brief Get the state that indicates if the internal coordinates of this drawable must be adjusted.
	 *
	 * It uses the anchor as base to determine if any adjust is required.
	 *
	 * @return True if the adjust is required. No otherwise.
	 */
	bool isRefreshInternalDimensionsNeeded();

	/**
	 * @brief Abstract method that must be implemented by all design objects to adjust screen coordinates [0,1]
	 * based on its parameter coordinates [X,Y].
	 *
	 * For example:
	 * ...
	 * 	TextParameters *params = getParams();
	 * 	WindowDimensions *dim = getWindowDimensions();
	 * 	screenCoords.x = dim->getScreenCoordX(params->pos.x);
	 * 	screenCoords.y = dim->getScreenCoordY(params->pos.y + getTextHeightInPixels());
	 * 	screenCoords.z = dim->getScreenCoordZ(params->pos.z);
	 * 	...
	 *
	 */
	virtual void refreshInternalDimensions() = 0;

public:
	/**
	 * @brief Constructs the object without a parameter list.
	 *
	 * @param _anchor The screen anchor for the object.
	 * @param _componentName The component name.
	 */
	DesignObject(XYZ _anchor, const char* _componentName);

	/**
	 * @brief Constructs the object with a parameter list.
	 *
	 * @param _anchor The screen anchor for the object.
	 * @param _params The parameter list.
	 * @param _componentName The component name.
	 */
	DesignObject(XYZ _anchor, DOParameters *_params, const char* _componentName);

	virtual ~DesignObject();

	/**
	 * @brief Get the DesignObject with the UID provided.
	 *
	 * @param UID The UID.
	 * @return The DesignObject with the UID provided.
	 */
	static DesignObject* getDesignObject(int UID);

	/**
	 * @brief Change the color intensity based on a rate.
	 *
	 * @param r Red color.
	 * @param g Green color.
	 * @param b Blue color.
	 * @param rate The rate to change the color (<1 to reduce, >1 to increase).
	 */
	static void changeColorIntensity(float &r, float &g, float &b, float rate);

	/**
	 * @brief Get the parameter list for this DesignObject.
	 *
	 * @return The parameter list.
	 */
	virtual DOParameters* getParams();

	/**
	 * @brief Abstract method that must be implemented by all drawables.
	 *
	 * Here is where all the design work must go.
	 */
	virtual void draw() = 0;

	/**
	 * @brief Virtual method that must be overridden by any class that requires some control over the mouse move.
	 *
	 * Passive move event is mouse movement without clicks.
	 *
	 * @param x The screen X coordinate where the mouse is.
	 * @param y The screen Y coordinate where the mouse is.
	 * @return Must return one of the valid actions:
	 * ACTION_IGNORED (if the event is not meant for this design object),
	 * ACTION_CHECKED (if the event is due but no action was needed),
	 * ACTION_TAKEN (if some work was actually done).
	 */
	virtual int mousePassiveMove(int x, int y);

	/**
	 * @brief Virtual method that must be overridden by any class that requires some control over the mouse move.
	 *
	 * Button event is mouse click happening.
	 *
	 * @param button What button was clicked.
	 * @param state What is the state of the button (up/down).
	 * @param x The screen X coordinate where the mouse is.
	 * @param y The screen Y coordinate where the mouse is.
	 * @return Must return one of the valid actions:
	 * ACTION_IGNORED (if the event is not meant for this design object),
	 * ACTION_CHECKED (if the event is due but no action was needed),
	 * ACTION_TAKEN (if some work was actually done).
	 */
	virtual int mouseButton(int button, int state, int x, int y);

	/**
	 * @brief Virtual method that must be overridden by any class that requires some control over the mouse move.
	 *
	 * Mouse Move event is drag-and-drop happening.
	 *
	 * @param x The screen X coordinate where the mouse is.
	 * @param y The screen Y coordinate where the mouse is.
	 * @return Must return one of the valid actions:
	 * ACTION_IGNORED (if the event is not meant for this design object),
	 * ACTION_CHECKED (if the event is due but no action was needed),
	 * ACTION_TAKEN (if some work was actually done).
	 */
	virtual int mouseMove(int x, int y);

	/**
	 * @brief Virtual method that must be overridden by any class that requires some control over the mouse move.
	 *
	 * No Passive move event is when the mouse move is not meant for this object.
	 * Specially useful when doing some in/out focus-like control.
	 *
	 * @param x The screen X coordinate where the mouse is.
	 * @param y The screen Y coordinate where the mouse is.
	 * @return Must return one of the valid actions:
	 * ACTION_IGNORED (if the event is not meant for this design object),
	 * ACTION_CHECKED (if the event is due but no action was needed),
	 * ACTION_TAKEN (if some work was actually done).
	 */
	virtual int mouseNoPassiveMove(int x, int y);

	/**
	 * @brief Virtual method that must be overridden by any class that requires some control over the mouse move.
	 *
	 * No Button event is when the mouse click is not meant for this object.
	 * Specially useful when doing some in/out focus-like control.
	 *
	 * @param button What button was clicked.
	 * @param state What is the state of the button (up/down).
	 * @param x The screen X coordinate where the mouse is.
	 * @param y The screen Y coordinate where the mouse is.
	 * @return Must return one of the valid actions:
	 * ACTION_IGNORED (if the event is not meant for this design object),
	 * ACTION_CHECKED (if the event is due but no action was needed),
	 * ACTION_TAKEN (if some work was actually done).
	 */
	virtual int mouseNoButton(int button, int state, int x, int y);

	/**
	 * @brief Virtual method that must be overridden by any class that requires some control over the keyboard events.
	 *
	 * The keyboard event captures all non-special keys from the keyboard.
	 *
	 * @param key The key pressed.
	 * @param x The screen X coordinate where the mouse is.
	 * @param y The screen Y coordinate where the mouse is.
	 * @return Must return one of the valid actions:
	 * ACTION_IGNORED (if the event is not meant for this design object),
	 * ACTION_CHECKED (if the event is due but no action was needed),
	 * ACTION_TAKEN (if some work was actually done).
	 */
	virtual int keyboard(unsigned char key, int x, int y);
	/**
	 * @brief Virtual method that must be overridden by any class that requires some control over the keyboard events.
	 *
	 * The keyboard Special event captures all special keys from the keyboard (like SHIFT, CTRL and ALT).
	 *
	 * @param key The special key pressed.
	 * @param x The screen X coordinate where the mouse is.
	 * @param y The screen Y coordinate where the mouse is.
	 * @return Must return one of the valid actions:
	 * ACTION_IGNORED (if the event is not meant for this design object),
	 * ACTION_CHECKED (if the event is due but no action was needed),
	 * ACTION_TAKEN (if some work was actually done).
	 */
	virtual int keyboardSpecial(int key, int x, int y);

	/**
	 * @brief Virtual method that must be overridden by any class that intends to receive any kind of mouse or keyboard event.
	 *
	 * The class must check if the [X,Y] coordinates are within its area of interest.
	 *
	 * @param x The screen X coordinate where the mouse is.
	 * @param y The screen Y coordinate where the mouse is.
	 * @return Must return one of the valid actions:
	 * ACTION_IGNORED (if the event is not meant for this design object),
	 * ACTION_CHECKED (if the event is due but no action was needed),
	 * ACTION_TAKEN (if some work was actually done).
	 */
	virtual bool isWithinBounds(int x, int y);

	/**
	 * @brief Virtual method that must be overridden by any class that intends to receive any kind of mouse or keyboard event.
	 *
	 * The class must set the vertice1 as the top-left and the vertice2 as the bottom-right corners.
	 * If the object does not have a continuous area, the vertice1 must inform
	 * the first vertex of the first screen object and the vertice2
	 * the last vertex of the last screen object.
	 *
	 * @param vertice1 The top-left coordinate of this drawable.
	 * @param vertice2 The bottom-right coordinate of this drawable.
	 */
	virtual void getBounds(XYZ &vertice1, XYZ &vertice2);

	/**
	 * @brief Set the visibility state of this drawable.
	 *
	 * @param _visible If visible or not.
	 */
	virtual void setVisible(bool _visible);

	/**
	 * @brief Get the visibility state of this drawable.
	 *
	 * @return If visible or not.
	 */
	virtual bool isVisible();

	/**
	 * @brief Virtual method that must be overridden by any class that intends to receive cascading drag-and-drop events.
	 *
	 * This method must contain all code to adjust the drawable's internal parameters and coordinates (except the anchor).
	 *
	 * @param x The screen X coordinate where to move the drawable.
	 * @param y The screen Y coordinate where to move the drawable.
	 */
	virtual void moveTo(float x, float y){}

	/**
	 * @brief Set reloadToDeviceNeeded to false.
	 *
	 * Must be called by any method overriding it to ensure the flag will be disabled.
	 */
	virtual void loadToDevice();

	/**
	 * @brief Get the state of the drawable that indicates if the data must be reloaded to GPU.
	 *
	 * @return if the data must be reloaded to GPU.
	 */
	bool isReloadToDeviceNeeded();

	/**
	 * @brief Set the state of the drawable that indicates if the data must be reloaded to GPU.
	 *
	 * @param _reloadToDeviceNeeded Set if the data must be reloaded to GPU.
	 */
	void setReloadToDeviceNeeded(bool _reloadToDeviceNeeded);

	/**
	 * @brief Empty method, just implementing the abstract method from clObject.
	 */
	virtual void runKernel(){}

	/**
	 * @brief Empty method, just implementing the abstract method from clObject.
	 */
	virtual void initializeCL(){}

	/**
	 * @brief Abstract method that must be overridden by all classes, returning the screen width (in pixels) of this drawable.
	 *
	 * @return The screen width (in pixels) of this drawable.
	 */
	virtual int getWidthInPixels() = 0;

	/**
	 * @brief Abstract method that must be overridden by all classes, returning the screen height (in pixels) of this drawable.
	 *
	 * @return The screen height (in pixels) of this drawable.
	 */
	virtual int getHeightInPixels() = 0;

	/**
	 * @brief Set the parent object for this drawable.
	 *
	 * The parent is the object that has added this drawable as one of its design components.
	 *
	 * @param parent The parent object.
	 */
	void setParent(DesignElement *parent);

	/**
	 * @brief Get the parent for this drawable.
	 *
	 * @return The parent.
	 */
	DesignElement* getParent();

	/**
	 * @brief Walk in the parenthood chain and get the top parent.
	 *
	 * @return The first parent in the parenthood chain.
	 */
	DesignElement* getTopParent();

	/**
	 * @brief Empty method, just implementing the abstract method from glDrawable.
	 */
	void scheduleAddDesignElement(DesignElement *elem){}

	/**
	 * @brief Empty method, just implementing the abstract method from glDrawable.
	 */
	void removeDesignElement(DesignElement *elem){}

	/**
	 * @brief Remove this drawable from the parent.
	 */
	void removeMe();

	/**
	 * @brief Get this component name.
	 *
	 * @return This component name.
	 */
	const char* getComponentName();

	/**
	 * @brief Replace the parameter list.
	 *
	 * @param _params The new parameter list.
	 */
	void refresh(DOParameters *_params);

	/**
	 * @brief Ensure that the component bounds are valid.
	 *
	 * Must be called every time any change is made over the component coordinates.
	 * If the bounds are not valid anymore, this method will call refreshInternalDimensions(),
	 * which must be implemented by all drawables to adjust the bounds.
	 *
	 * @return True if the bounds were adjusted. False otherwise.
	 */
	bool enforceBounds();

	/**
	 * @brief Get the state that indicates if this drawable will respect a contextual position.
	 *
	 * If the contextual position is set, the drawable will be moved along with its parent on screen,
	 * in a drag-and-drop event.
	 *
	 * @return If this drawable will respect a contextual position.
	 */
	bool isContextualPosition();

	/**
	 * @brief Set the state that indicates if this drawable will respect a contextual position.
	 *
	 * If the contextual position is set, the drawable will be moved along with its parent on screen,
	 * in a drag-and-drop event.
	 *
	 * @param _contextualPosition If the position is contextual.
	 */
	void setContextualPosition(bool _contextualPosition);

	/**
	 * @brief Get the UID for this drawable.
	 *
	 * @return The UID.
	 */
	int getUID();

	/**
	 * @brief Set the window dimensions for this drawable.
	 *
	 * @param _windowDimensions The window dimensions.
	 */
	void setWindowDimensions(WindowDimensions* _windowDimensions);

	/**
	 * @brief Get the window dimensions for this drawable.
	 *
	 * @return The window dimensions.
	 */
	WindowDimensions* getWindowDimensions();

	/**
	 * @brief Move the anchor for this drawable.
	 *
	 * It is usually called in a drag-and-drop event, but can be called every time the drawable
	 * must be moved to a different coordinate.
	 * It will call the methods moveTo() and enforceBounds().
	 *
	 * @param _anchor The absolute coordinate to move this drawable to.
	 */
	void moveAnchor(XYZ _anchor);

	/**
	 * @brief Move the anchor for this drawable.
	 *
	 * It is usually called in a drag-and-drop event, but can be called every time the drawable
	 * must be moved to a different coordinate.
	 * It will call the method moveAnchor(XYZ).
	 *
	 * @param x The shift on X position to move this drawable to.
	 * @param y The shift on Y position to move this drawable to.
	 */
	void moveAnchorRelative(float x, float y);

	/**
	 * @brief Get the anchor for this drawable.
	 *
	 * @return The anchor.
	 */
	XYZ getAnchor();

	/**
	 * @brief Move the layer for this drawable up.
	 *
	 * Move the Z coordinate the specified number of layers up.
	 * The Z coordinate is internally divided in 100 layers.
	 *
	 * @param nLayers The number of layers to move this drawable up.
	 */
	void layerUp(int nLayers);

	/**
	 * @brief Move the layer for this drawable down.
	 *
	 * Move the Z coordinate the specified number of layers down.
	 * The Z coordinate is internally divided in 100 layers.
	 *
	 * @param nLayers The number of layers to move this drawable down.
	 */
	void layerDown(int nLayers);
};


#endif /* _DESIGNOBJECT_HPP_ */
