/*****************************************************************//**
 * \file   menu.cpp
 * \brief  Defines menu methods
 *
 * \author schrodlm
 * \date   February 2023
 *********************************************************************/

#include "menu.h"

void Menu::AddItem(const char* text, std::function<int()> eventHandler)
{
	m_items.push_back(MenuItem{ text, eventHandler });
	//setting up first item as selected
}

void Menu::Draw(int x, int y, int size)
{
	// Draw each menu item

	for (int i = 0; i < (int)m_items.size(); ++i) {
		// Highlight the selected item
		if (m_selectedIndex == i) {
			DrawText(x, y + i * size, size + 5, CYAN, true, m_items[i].text);
			continue;

			/* your highlight drawing code here */
		}
		DrawText(x, y + i * size, size, WHITE, true, m_items[i].text);

		/* your text drawing code here */


	}
}

int Menu::HandleInput()
{
	// Check for up/down arrow key presses to change the selected item
	if (KeyPressed(VK_UP)) {
		m_selectedIndex = max(0, m_selectedIndex - 1);
	}
	else if (KeyPressed(VK_DOWN)) {
		m_selectedIndex = min((int)m_items.size() - 1, m_selectedIndex + 1);
	}
	else if (KeyPressed(VK_RETURN)) {
		// Trigger the event handler for the selected item
		return m_items[m_selectedIndex].eventHandler();
	}

	return -1;
}

int Menu::Loop(int x, int y, int size)
{
	int eventTriggered = -1;
	while (1)
	{

		if (!StartFlip()) return 2;
		if (IsKeyDown(VK_ESCAPE)) return 2;
		Draw(x, y, size);
		eventTriggered = HandleInput();
		EndFlip();

		if (eventTriggered != -1) break;
	}

	return eventTriggered;
}