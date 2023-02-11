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
	void AddItem(const char* text, std::function<void()> eventHandler) {
		m_items.push_back(MenuItem{ text, eventHandler });
		//setting up first item as selected
	}

	// Draws the menu
	void Draw(int x, int y, int size) {
		// Draw each menu item

		for (int i = 0; i < m_items.size(); ++i) {
			// Highlight the selected item
			if (m_selectedIndex == i) {
				DrawText(x, y + i * size, size + 5, 0xff5d8aa8, true, m_items[i].text);
				continue;

				/* your highlight drawing code here */
			}
			DrawText(x, y + i * size, size, 0xffffffff, true, m_items[i].text);

			/* your text drawing code here */


		}
	}

	// Handles input for the menu
	void HandleInput(/* your input code here */) {
		// Check for up/down arrow key presses to change the selected item
		if (KeyPressed(VK_UP)) {
			m_selectedIndex = max(0, m_selectedIndex - 1);
		}
		else if (KeyPressed(VK_DOWN)) {
			m_selectedIndex = min((int)m_items.size() - 1, m_selectedIndex + 1);
		}
		else if (KeyPressed(VK_RETURN) && m_selectedIndex != -1) {
			// Trigger the event handler for the selected item
			m_items[m_selectedIndex].eventHandler();
		}
	}

private:
	struct MenuItem {
		const char* text;
		std::function<void()> eventHandler;
	};



	std::vector<MenuItem> m_items;
	int m_selectedIndex = 0;
};
