#pragma once

#include "src/lib/leetlib.h"
//#include "src/lib/entities.h"
#include <vector>
#include <functional>
#include <string>


class Menu {
public:
	Menu() {}

	// Adds a menu item
	void AddItem(const char* text, std::function<int()> eventHandler);

	// Draws the menu
	void Draw(int x, int y, int size);

	// Handles input for the menu
	int HandleInput();
	int Loop();

private:
	struct MenuItem {
		const char* text;
		std::function<int()> eventHandler;
	};



	std::vector<MenuItem> m_items;
	int m_selectedIndex = 0;
};
