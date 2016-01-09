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
 * glWindow.hpp
 *
 * Created on: 06/Jan/2015
 * Author: Peter Frank Perroni (pfperroni@inf.ufpr.br)
 */

#ifndef _GLWINDOW_HPP_
#define _GLWINDOW_HPP_

#include "WindowDimensions.hpp"
#include "DOManagerGroup.hpp"
#include "FocusManager.hpp"

/**
 * Internal DOManagerGroup, to be used as primary container to add DOManager's and DesignObject's
 * when they are added directly to the window without a primary container.
 */
class InternalDOManagerGroup : public DOManagerGroup{
public:
	InternalDOManagerGroup(WindowDimensions* _windowDimensions, const char* _componentName) : DOManagerGroup(_windowDimensions, _componentName){}
	int getWidthInPixels(){return 0;}
	int getHeightInPixels(){return 0;}
	void runKernel(){}
	void initializeCL(){}
	void loadToDevice(){setReloadToDeviceNeeded(false);}
};


/**
 * @brief Class representing a physical window.
 */
class glWindow {
private:
	omp_lock_t mutex_self;
	WindowDimensions *dim;
	float originalWindowDepth;
	bool visible;
	map<int, DOManagerGroup*> doManagerGroupMap;

public:
	/**
	 * @brief Create a new window with the given configuration.
	 *
	 * @param windowWidth The window width, in pixels. This is a physical dimension.
	 * @param windowHeight The window height, in pixels. This is a physical dimension.
	 * @param windowDepth The window depth, in pixels (used to calculate the size of each layer). This is an abstract dimension.
	 * @param doManagerGroup The DOManagerGroup to be used as primary container (optional). If not provided, one is created internally.
	 */
	glWindow(int windowWidth, int windowHeight, int windowDepth, DOManagerGroup *doManagerGroup = NULL);

	~glWindow();

	/**
	 * @brief Repaint all containers under this window.
	 */
	void display();

	/**
	 * @brief Forward mouse passive move event (simple movement without click) to containers under this window.
	 *
	 * The methods isVisible() and isWithinBounds(int, int) are used to determine which containers should receive this event.
	 *
	 * @param x The screen X coordinate where the mouse is.
	 * @param y The screen Y coordinate where the mouse is.
	 * @return Return one of the valid actions:
	 * ACTION_IGNORED (if the event was ignored by all containers within this window),
	 * ACTION_CHECKED (if the event was checked by at least one container),
	 * ACTION_TAKEN (if some work was actually done by at least one container).
	 */
	int mousePassiveMove(int x, int y);

	/**
	 * @brief Forward mouse click event to containers under this window.
	 *
	 * The methods isVisible() and isWithinBounds(int, int) are used to determine which containers should receive this event.
	 *
	 * @param button What button was clicked.
	 * @param state What is the state of the button (up/down).
	 * @param x The screen X coordinate where the mouse is.
	 * @param y The screen Y coordinate where the mouse is.
	 * @return Return one of the valid actions:
	 * ACTION_IGNORED (if the event was ignored by all containers within this window),
	 * ACTION_CHECKED (if the event was checked by at least one container),
	 * ACTION_TAKEN (if some work was actually done by at least one container).
	 */
	int mouseButton(int button, int state, int x, int y);

	/**
	 * @brief Forward mouse drag-and-drop event to containers under this window.
	 *
	 * No bound area validation is done prior to forwarding this event, so all visible containers will receive this event.
	 *
	 * @param x The screen X coordinate where the mouse is.
	 * @param y The screen Y coordinate where the mouse is.
	 * @return Return one of the valid actions:
	 * ACTION_IGNORED (if the event was ignored by all containers within this window),
	 * ACTION_CHECKED (if the event was checked by at least one container),
	 * ACTION_TAKEN (if some work was actually done by at least one container).
	 */
	int mouseMove(int x, int y);

	/**
	 * @brief Forward keyboard events for non-special keys to containers under this window.
	 *
	 * The methods isVisible() and isWithinBounds(int, int) are used to determine which containers should receive this event.
	 *
	 * @param key The key pressed.
	 * @param x The screen X coordinate where the mouse is.
	 * @param y The screen Y coordinate where the mouse is.
	 * @return Return one of the valid actions:
	 * ACTION_IGNORED (if the event was ignored by all containers within this window),
	 * ACTION_CHECKED (if the event was checked by at least one container),
	 * ACTION_TAKEN (if some work was actually done by at least one container).
	 */
	int keyboard(unsigned char key, int x, int y);

	/**
	 * @brief Forward keyboard events for special keys to containers under this window.
	 *
	 * The methods isVisible() and isWithinBounds(int, int) are used to determine which containers should receive this event.
	 *
	 * @param key The special key pressed.
	 * @param x The screen X coordinate where the mouse is.
	 * @param y The screen Y coordinate where the mouse is.
	 * @return Return one of the valid actions:
	 * ACTION_IGNORED (if the event was ignored by all containers within this window),
	 * ACTION_CHECKED (if the event was checked by at least one container),
	 * ACTION_TAKEN (if some work was actually done by at least one container).
	 */
	int keyboardSpecial(int key, int x, int y);

	/**
	 * @brief Set a method as focus for this window.
	 *
	 * The focus method must be static.
	 *
	 * @param focus The static method to be set as focus.
	 */
	void setFocus(void *focus);

	/**
	 * @brief Clear the focus method for this window.
	 */
	void clearFocus();

	/**
	 * @brief Get the method that was set as focus for this window.
	 */
	void* getFocus();

	/**
	 * @brief Show or hide this window.
	 *
	 * @param _visible If false, will hide this window.
	 */
	void setVisible(bool _visible);

	/**
	 * @brief Get the visibility state for this window.
	 *
	 * @return If the window is visible.
	 */
	bool isVisible();

	/**
	 * @brief Add a component or container to this window.
	 *
	 * It can be a DesignObject, a DOManager or a DOManagerGroup.
	 *
	 * @param drawable The drawable to be added.
	 */
	void addDesignElement(glDrawable *drawable);

	/**
	 * @brief Force refresh for all containers within this window.
	 *
	 * It calls the methods resetInternalViewPort(), enforceBounds() and display();
	 */
	void forceRefresh();

	/**
	 * @brief Set a title for this window.
	 *
	 * @param title The window title.
	 */
	void setWindowTitle(char *title);

	/**
	 * Get the glut handler for this window.
	 *
	 * @return The window handler.
	 */
	int getWindowHandler();

	/**
	 * @brief Get the window dimension configured for this window.
	 *
	 * @return The window dimension.
	 */
	WindowDimensions* getWindowDimensions();

	/**
	 * @brief Resize the internal view port based on window dimension's zoom configured.
	 *
	 * @param windowWidth The internal view port width.
	 * @param windowHeight The internal view port height.
	 * @param windowDepth The internal view port depth.
	 */
	void resizeInternalViewPort(int windowWidth, int windowHeight, int windowDepth);

	/**
	 * @brief Resize the internal view port based on a zoom specified.
	 *
	 * @param zoom The zoom to be used (<1 is zoom out, >1 is zoom in).
	 * @param windowWidth The internal view port width.
	 * @param windowHeight The internal view port height.
	 * @param windowDepth The internal view port depth.
	 */
	void resizeInternalViewPort(float zoom, int windowWidth, int windowHeight, int windowDepth);

	/**
	 * @brief Reset the internal view port to standard zoom and depth, and glut window dimensions.
	 */
	void resetInternalViewPort();

	/**
	 * @brief Zoom in or zoom out.
	 *
	 * @param zoom The zoom to be used (<1 is zoom out, >1 is zoom in).
	 */
	void zoom(float zoom);

	/**
	 * @brief Rotate the window's contents.
	 *
	 * @param angle The rotation angle.
	 * @param x Set 1 to rotate X axis, 0 otherwise.
	 * @param y Set 1 to rotate Y axis, 0 otherwise.
	 * @param z Set 1 to rotate Z axis, 0 otherwise.
	 */
	void rotate(float angle, float x, float y, float z);
};


#endif /* _GLWINDOW_HPP_ */
