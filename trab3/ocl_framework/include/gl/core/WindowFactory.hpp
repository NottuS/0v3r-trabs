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
 * WindowFactory.hpp
 *
 * Created on: 06/Jan/2015
 * Author: Peter Frank Perroni (pfperroni@inf.ufpr.br)
 */

#ifndef _WINDOWFACTORY_HPP_
#define _WINDOWFACTORY_HPP_

#include <X11/Xlib.h>

#include "glWindow.hpp"
#include <unistd.h>

/**
 * @brief Factory to create and manage all physical windows.
 */
class WindowFactory{
private:
	static omp_lock_t mutex;
	static map<int, glWindow*> windowMap;
	static bool started;

public:
	/**
	 * @brief Create a new window and store it the factory's controlled list.
	 *
	 * @param windowWidth The window width.
	 * @param windowHeight The window height.
	 * @param windowDepth The window depth.
	 * @param doManagerGroup The DOManagerGroup to be used (optional). If not provided, one will be created.
	 * @return The new window created.
	 */
	static glWindow* createWindow(int windowWidth, int windowHeight, int windowDepth, DOManagerGroup *doManagerGroup = NULL);

	/**
	 * @brief Startup all contexts required to create and work with windows.
	 *
	 * It includes the X environment, the glut context and the CL context.
	 */
	static void startup();

	/**
	 * @brief Shutdown the contexts and the windows created.
	 */
	static void shutdown();

	/**
	 * @brief Enter a loop to capture all GL events.
	 *
	 * This method starts up the contexts and enter a blocking loop.
	 * The method will only be left when all windows were closed.
	 * Once the loop is left, the contexts are shut down.
	 */
	static void mainGlLoop();

	/**
	 * @brief Close, remove and delete one window from the list.
	 *
	 * @param windowHandler The glut window handler for the window to be disposed.
	 */
	static void dispose(int windowHandler);

	/**
	 * @brief Repaint all containers in the current window.
	 */
	static void display();

	/**
	 * @brief Force the refresh for all windows.
	 *
	 * Called by the glut event glutDisplayFunc.
	 */
	static void forceRefresh();

	/**
	 * @brief Force the refresh for the current window.
	 *
	 * Called by the glut event glutReshapeFunc, triggered when the window is resized.
	 *
	 * @param w The new window width.
	 * @param h The new window height.
	 */
	static void windowReshape(int w, int h);

	/**
	 * @brief Dispose the current window, removing it from this factory.
	 *
	 * Called by the glut event glutCloseFunc, triggered when the window is closed.
	 */
	static void windowClose();

	static void timerEvent(int value){}
	static void rightClick(int op){}

	/**
	 * @brief Forward mouse passive move event (simple movement without click) to the current window.
	 *
	 * The method isVisible() is used to determine if the window should receive this event.
	 *
	 * @param x The screen X coordinate where the mouse is.
	 * @param y The screen Y coordinate where the mouse is.
	 * @return Return one of the valid actions:
	 * ACTION_IGNORED (if the event was ignored by the window),
	 * ACTION_CHECKED (if the event was checked by the window),
	 * ACTION_TAKEN (if some work was actually done by the window).
	 */
	static void mousePassiveMove(int x, int y);

	/**
	 * @brief Forward mouse click event to current window.
	 *
	 * The method isVisible() is used to determine if the window should receive this event.
	 *
	 * @param button What button was clicked.
	 * @param state What is the state of the button (up/down).
	 * @param x The screen X coordinate where the mouse is.
	 * @param y The screen Y coordinate where the mouse is.
	 * @return Return one of the valid actions:
	 * ACTION_IGNORED (if the event was ignored by the window),
	 * ACTION_CHECKED (if the event was checked by the window),
	 * ACTION_TAKEN (if some work was actually done by the window).
	 */
	static void mouseButton(int button, int state, int x, int y);

	/**
	 * @brief Forward mouse drag-and-drop event to current window.
	 *
	 * The method isVisible() is used to determine if the window should receive this event.
	 *
	 * @param x The screen X coordinate where the mouse is.
	 * @param y The screen Y coordinate where the mouse is.
	 * @return Return one of the valid actions:
	 * ACTION_IGNORED (if the event was ignored by the window),
	 * ACTION_CHECKED (if the event was checked by the window),
	 * ACTION_TAKEN (if some work was actually done by the window).
	 */
	static void mouseMove(int x, int y);

	/**
	 * @brief Forward keyboard events for non-special keys to current window.
	 *
	 * The method isVisible() is used to determine if the window should receive this event.
	 *
	 * @param key The key pressed.
	 * @param x The screen X coordinate where the mouse is.
	 * @param y The screen Y coordinate where the mouse is.
	 * @return Return one of the valid actions:
	 * ACTION_IGNORED (if the event was ignored by the window),
	 * ACTION_CHECKED (if the event was checked by the window),
	 * ACTION_TAKEN (if some work was actually done by the window).
	 */
	static void keyboard(unsigned char key, int x, int y);

	/**
	 * @brief Forward keyboard events for special keys to current window.
	 *
	 * The method isVisible() is used to determine if the window should receive this event.
	 *
	 * @param key The special key pressed.
	 * @param x The screen X coordinate where the mouse is.
	 * @param y The screen Y coordinate where the mouse is.
	 * @return Return one of the valid actions:
	 * ACTION_IGNORED (if the event was ignored by the window),
	 * ACTION_CHECKED (if the event was checked by the window),
	 * ACTION_TAKEN (if some work was actually done by the window).
	 */
	static void keyboardSpecial(int key, int x, int y);

	/**
	 * @brief Hand over the control of events to GLUT.
	 *
	 * Once handed over, the application will lose the fine control over the events.
	 * The main program Thread will be under GLUT control from this point on,
	 * until the user closes all windows opened.
	 */
	static void handOverControlToGlut();

	/**
	 * @brief Add an existent window to the factory.
	 *
	 * Add the window and set the events to be sent to this factory.
	 *
	 * @param window The window.
	 */
	static void addWindow(glWindow *window);

	/**
	 * @brief Get a window based on the glut window handler provided.
	 *
	 * @param windowHandler The glut window handler.
	 * @return
	 */
	static glWindow* getWindow(int windowHandler);

	/**
	 * @brief Count the number of windows managed by this factory.
	 *
	 * @return The number of windows.
	 */
	static int countWindows();
};


#endif /* _WINDOWFACTORY_HPP_ */
