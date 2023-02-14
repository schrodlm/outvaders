/*****************************************************************//**
 * \file   menu.h
 * \brief  Manages menu class that is used to draw options for a player on the screen.
 *
 * \author schrodlm
 * \date   February 2023
 *********************************************************************/
#pragma once

#include "leetlib.h"

#include <vector>
#include <functional>
#include <string>

 /**
  * The Menu class provides functionality to create and interact with a menu.
	 A Menu object allows the user to add menu items and associated event handlers, draw the menu on the screen,
	 and handle user input for selecting a menu item.
  */
class Menu {
public:
	Menu() {}

	/**
	 * Adds a menu item.
	 *
	 * \param text Name of the item that will show on the screen
	 * \param eventHandler	Function that will be procced once that item is chosen by the player
	 */
	void AddItem(const char* text, std::function<int()> eventHandler);

	/**
	 * Draw the menu on the screen.
	 *
	 * \param x x_center
	 * \param y y_center
	 * \param size size of the text of menu items
	 */
	void Draw(int x, int y, int size);

	/**
	 * Checks for player input.
	 *
	 * \return event return value of the selected item, if no item was selected return -1.
	 */
	int HandleInput();

	/**
	 * Draw menu content on the screen and handles input using HandleInput() method.
	 *
	 * \return
	 */
	int Loop();

private:
	/**
	 * A struct to hold the text and event handler function pointer for a single menu item.
	 */
	struct MenuItem {
		const char* text;
		std::function<int()> eventHandler;
	};



	std::vector<MenuItem> m_items;
	int m_selectedIndex = 0;
};
