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
 * Menu.cpp
 *
 * Created on: 07/Jan/2015
 * Author: Peter Frank Perroni (pfperroni@inf.ufpr.br)
 */

#include "../../include/gl/components/Menu.hpp"

void* Menu::FONT = GLUT_BITMAP_HELVETICA_12;
int Menu::MENU_HORIZONTAL = 0;
int Menu::MENU_VERTICAL = 1;
int Menu::MENU_ITEM = 0;
int Menu::MENU_SUBMENU= 1;

Menu::~Menu(){
	for(int i=0; i < (int)menuList.size(); i++){
		delete menuList[i];
	}
}

void Menu::addItem(MenuItem *menuItem){
	addDesignElement(menuItem);
//printf("added menu item UID %i\n", ((DOManagerGroup*)menuItem)->getUID());

	MenuType *type = new MenuType;
	type->type = MENU_ITEM; type->item = menuItem;
	menuList.push_back(type);

	pack();
}

void Menu::addSubMenu(Menu *subMenu){
	if(isExpandable() && subMenu->isExpandable() && menuList.size() == 0){
		printf("The first element of an expandable menu cannot be an expandable menu!");
		clFactory::shutdown();
		exit(1);
	}
	addDesignElement(subMenu);
//printf("added sub menu UID %i\n", ((DOManagerGroup*)subMenu)->getUID());
	MenuType *type = new MenuType;
	type->type = MENU_SUBMENU; type->item = subMenu;
	menuList.push_back(type);

	pack();
}

void Menu::pack(){
	pack(0);
	setExpanded(false);
}

void Menu::pack(int level){
	if(menuList.size() == 0) return;

	BoxParameters *boxParams = boxMenu->getParams();
	float x1 = boxParams->coords[0].x + 1;
	float y1 = boxParams->coords[0].y + 1;
	float _x2, x2, y2;
	int firstItem = (level > 0) ? 1 : 0;
	int i;
	Menu *menu;


	if(type == MENU_VERTICAL){
		_x2 = 0;
		y2 = y1;
		int sz;
		// Get the largest label size.
		for(i=((expandable)?1:0); i < (int)menuList.size(); i++){
			sz = getWidthFromMenuType(menuList[i]);
			if(_x2 < sz) _x2 = sz;
		}
		_x2 += x1 + 7;

		// Adjust sizes and colors.
		for(i=firstItem; i < (int)menuList.size(); i++, y1=y2){
			x2 = (level == 0 && expandable && i == 0) ? getWidthFromMenuType(menuList[i]) + 7: _x2;
			y2 += Text::getTextHeightInPixels(FONT) + 3;
			if(menuList[i]->type == MENU_ITEM){
				((MenuItem*)menuList[i]->item)->adjustContext(x1, y1, x2, y2, color.r, color.g, color.b);
			}
			else{
				// First item of a submenu.
				menu = (Menu*)menuList[i]->item;
				if(menu->menuList.size() > 0){
					((MenuItem*)menu->menuList[0]->item)->adjustContext(x1, y1, x2, y2, color.r, color.g, color.b);
//printf("[v] %s=[%f,%f,%f,%f]\n", ((MenuItem*)menu->menuList[0]->item)->getLabel(), x1, y1, x2, y2);
				}
				// Remaining items of a submenu.
				menu->adjustContext(level+1, x2, y1, color.r, color.g, color.b);
			}
		}
	}
	else{
		x2 = x1;
		y2 = Text::getTextHeightInPixels(FONT) + 3;
		for(i=firstItem; i < (int)menuList.size(); i++, x1=x2){
//printf("[h] %s=[%f,%f,%f,%f]\n", menuList[i]->item->getLabel(), x1, y1, x2, y2);
			x2 += getWidthFromMenuType(menuList[i]) + 7;
//printf("label found: %s - size:%i\n", getLabelFromMenuType(menuList[i]), Text::getTextWidthInPixels(getLabelFromMenuType(menuList[i]), FONT));
			if(menuList[i]->type == MENU_ITEM){
				((MenuItem*)menuList[i]->item)->adjustContext(x1, y1, x2, y2, color.r, color.g, color.b);
			}
			else{
				// First item of a submenu.
				menu = (Menu*)menuList[i]->item;
				if(menu->menuList.size() > 0){
					((MenuItem*)menu->menuList[0]->item)->adjustContext(x1, y1, x2, y2, color.r, color.g, color.b);
				}
				// Remaining items of a submenu.
//printf("adjusting submenu %s on [%f,%f]\n", x1, y2);
				menu->adjustContext(level+1, x1, y2, color.r, color.g, color.b);
			}
		}
	}

	boxParams->coords[1].x = x2 + 1;
	boxParams->coords[1].y = y2 + 1;
	if(level == 0){
		width = boxParams->coords[1].x - boxParams->coords[0].x;
		height = boxParams->coords[1].y - boxParams->coords[0].y;
	}
	boxMenu->enforceBounds();
//	printf("boxMenu=[%f,%f,%f,%f]\n", boxParams->coords[0].x, boxParams->coords[0].y, boxParams->coords[1].x, boxParams->coords[1].y);
}

int Menu::getWidthFromMenuType(MenuType *menuType){
	int ret = 0;
	if(menuType->type == MENU_ITEM){
		ret = ((MenuItem*)menuType->item)->getWidthInPixels();
	}
	else{
		Menu *menu = (Menu*)menuType->item;
		if(menu->menuList.size() > 0){
			ret = ((MenuItem*)(menu->menuList[0]->item))->getWidthInPixels();
		}
	}
	return ret;
}

/*bool Menu::isWithinBounds(int x, int y) {
	for(int i=0; i < (int)menuList.size(); i++){
		if(menuList[i]->type == MENU_ITEM){
			menuList[i]->item->setVisible(expanded);
		}
		else{
			menu = (Menu*)menuList[i]->item;
			menu->setVisible(expanded);
			if(expanded && menu->menuList.size() > 1){
				menu->setExpanded(level+1, false);
//				((MenuItem*)(menu->menuList[0]->item))->setVisible(expanded);
			}
		}
	}

	if(expandable){
		for(int i=0; i < (int)menuList.size(); i++){
			if(((Menu*)menuList[i]->item)->isWithinBounds(x, y)) return true;
		}
	}
	return boxMenu->isWithinBounds(x, y);
}*/

int Menu::mouseButton(int button, int state, int x, int y){
	int ret = ACTION_IGNORED;
	if(button == GLUT_LEFT_BUTTON && state == GLUT_DOWN){
//		printf("checking click: menu option [%s - %i - %i - % i] => ", getLabelFromMenuType(menuList[0]),
//				expandable, (int)menuList.size(), menuList[0]->item->isWithinBounds(x, y));
		if(expandable && menuList.size() > 0 && menuList[0]->item->isVisible() && menuList[0]->item->isWithinBounds(x, y)){
//			printf("expandable !\n");
			setExpanded(!isExpanded());
			ret |= ACTION_CHECKED;
		}
		else if(DOManagerGroup::isWithinBounds(x, y)){
//for(int i=0; i < (int)menuList.size(); i++){
//	XYZ pos1, pos2;
//	menuList[i]->item->getBounds(pos1, pos2);
//	printf("(mouse[%i,%i] vs ", x, y);
//	printf("bounds[%f,%f,%f,%f]) ", pos1.x, pos1.y, pos2.x, pos2.y);
//	printf("menuItem[%i]: isWithinBounds=%i ", i, menuList[2]->item->isWithinBounds(x, y));
//	printf("isVisible=%i\n", menuList[2]->item->isVisible());
//}
//			printf("non expandable !\n");
			ret |= DOManagerGroup::mouseButton(button, state, x, y);
			if(HAS_ONLY_IGNORED_ACTION(ret) || HAS_TOOK_ACTION(ret)) setExpanded(false);
		}
	}
//	if(!alwaysVisible){
//		DOManagerGroup::setVisible(false);
//	}
	return ret;
}

int Menu::mouseNoButton(int button, int state, int x, int y){
	if(expandable && menuList.size() > 0 && menuList[0]->item->isVisible()){
		setExpanded(false);
	}
	return ACTION_CHECKED;
}

void Menu::adjustContext(int level, float x, float y, float r, float g, float b){
	color.r = r; color.g = g; color.b = b;

	BoxParameters *boxParams = boxMenu->getParams();
	boxParams->setCoords(x, y, x, y);
	DesignObject::changeColorIntensity(r, g, b, 1.2f);
	boxParams->setRgbFill(r, g, b, r, g, b);
	DesignObject::changeColorIntensity(r, g, b, 1.2f);
	boxParams->setRgbBorder(r, g, b);
	boxParams->setSmooth(false);

	pack(level);
}

void Menu::adjustContext(float x, float y, float r, float g, float b){
	adjustContext(0, x, y, r, g, b);
}

void Menu::setExpanded(bool _expanded){
	setExpanded(0, _expanded);
}
void Menu::setExpanded(int level, bool _expanded){
	if(level == 0 && !isExpandable()) return;

	expanded = _expanded;
	setVisible(true);
	boxMenu->setVisible(expanded);

	if(menuList.size() == 0) return;

	Menu *menu;
	// First item of an expandable menu must remain visible.
	for(int i=1; i < (int)menuList.size(); i++){
		if(menuList[i]->type == MENU_ITEM){
			menuList[i]->item->setVisible(expanded);
		}
		else{
			menu = (Menu*)menuList[i]->item;
			menu->setVisible(expanded);
			if(expanded && menu->menuList.size() > 1){
				menu->setExpanded(level+1, false);
//				((MenuItem*)(menu->menuList[0]->item))->setVisible(expanded);
			}
		}
	}
}

bool Menu::isExpandable(){
	return expandable;
}

bool Menu::isExpanded(){
	return expanded;
}

int Menu::getWidthInPixels(){
	return width;
}

int Menu::getHeightInPixels(){
	return height;
}

void Menu::getBounds(XYZ &vertice1, XYZ &vertice2){
	boxMenu->getBounds(vertice1, vertice2);
}
