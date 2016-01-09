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
 * Menu.hpp
 *
 * Created on: 07/Jan/2015
 * Author: Peter Frank Perroni (pfperroni@inf.ufpr.br)
 */

#ifndef _MENU_HPP_
#define _MENU_HPP_

#include "../core/WindowDimensions.hpp"
#include "../core/DOManager.hpp"
#include "Box.hpp"
#include "Text.hpp"
#include "MenuItem.hpp"

struct MenuType{
	int type;
	DOManager* item;
};

class Menu : public DOManagerGroup {
private:
	static void* FONT;
	static int MENU_ITEM;
	static int MENU_SUBMENU;

	int type, width, height;
	Box *boxMenu;
	vector<MenuType*> menuList;
//	bool alwaysVisible;
	bool expanded;
	bool expandable;
	RGB color;
//	XYZ anchor;

	void adjustContext(int level, float x, float y, float r, float g, float b);
	void pack(int level);
	void pack();
	void setExpanded(int level, bool _expanded);

public:
	static int MENU_HORIZONTAL;
	static int MENU_VERTICAL;

	Menu(WindowDimensions* _windowDimensions, float x, float y, float r, float g, float b, int _type, bool _expandable = false) :
			DOManagerGroup(_windowDimensions, "Menu"){
		type = _type;
		width = height = 0;
//		alwaysVisible = false;

		boxMenu = new Box(new BoxParameters());

		DOManagerGroup::addDesignElement(boxMenu);
//		boxMenu->layerDown(10);
		expandable = _expandable;
		setExpanded(false);
		adjustContext(x, y, r, g, b);

		DOManagerGroup::setContextualPosition(true);
	}
	~Menu();
	void adjustContext(float x, float y, float r, float g, float b);
	static int getWidthFromMenuType(MenuType *menuType);
	void setExpanded(bool _expanded);
	bool isExpandable();
//	bool isAlwaysVisible(){return alwaysVisible;}
//	void setAlwaysVisible(bool _alwaysVisible){alwaysVisible = _alwaysVisible;}
	bool isExpanded();
//	bool isWithinBounds(int x, int y);
	void addItem(MenuItem *menuItem);
	void addSubMenu(Menu *_subMenu);
	int mouseButton(int button, int state, int x, int y);
	int mouseNoButton(int button, int state, int x, int y);
	int getWidthInPixels();
	int getHeightInPixels();
	void getBounds(XYZ &vertice1, XYZ &vertice2);
};

#endif /* _MENU_HPP_ */
