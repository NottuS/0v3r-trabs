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
 * Movable.hpp
 *
 * Created on: 27/Jan/2015
 * Author: Peter Frank Perroni (pfperroni@inf.ufpr.br)
 */

#ifndef _MOVABLE_HPP_
#define _MOVABLE_HPP_

#include <omp.h>
#include <stdio.h>

#include "../../types.hpp"
#include "WindowDimensions.hpp"
#include "FocusManager.hpp"

using namespace std;

/**
 * @brief Class representing a movable design object.
 */
class Movable{
private:
	omp_lock_t mutex_self;
	bool contextualPosition;
	bool moveEnabled;
	bool onMove;
	XYZ curPos;

//	static void rightClick(int op);

public:
	Movable();
	virtual ~Movable(){}
//	void mouseButton(int button, int state, int x, int y);
//	void mouseMove(int x, int y);

	/**
	 * @brief Check the state that tells if the move is currently enabled for this design object.
	 * @return If the move is enabled.
	 */
	bool isMoveEnabled();
	void setMoveEnabled(bool _moveEnabled);

	/**
	 * @brief Check the state that tells if this design object is currently being moved.
	 *
	 * @return If the move is happening.
	 */
	bool isOnMove();
	void setOnMove(bool _onMove);
//	virtual WindowDimensions* getWindowDimensions() = 0;

	/**
	 * @brief Set the state that indicates if this design object will respect a contextual position.
	 *
	 * If the contextual position is set, this design object will be moved along with its parent on screen,
	 * in a drag-and-drop event.
	 *
	 * @param _contextualPosition If the position is contextual.
	 */
	bool isContextualPosition();
	void setContextualPosition(bool _contextualPosition);

	/**
	 * @brief Check the size of the last move for this design object, based on the last time this method was called.
	 *
	 * @param x The current X position.
	 * @param y The current Y position.
	 * @return The displacement in X and Y from the last time this method was called.
	 */
	XYZ getLastMoveSize(float x, float y);
	virtual const char* getComponentName() = 0;
};


#endif /* _MOVABLE_HPP_ */
