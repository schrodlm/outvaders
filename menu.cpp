#include "menu.h"

void Menu::AddItem(const char* text, std::function<int()> eventHandler)
{
	m_items.push_back(MenuItem{ text, eventHandler });
	//setting up first item as selected
}

void Menu::Draw(int x, int y, int size)
{
	// Draw each menu item

	for (int i = 0; i < m_items.size(); ++i) {
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




int Menu::Loop()
{
	int eventTriggered = -1;
	while (1)
	{

		if (WantQuit()) return -1;
		if (IsKeyDown(VK_ESCAPE)) return -1;
		startFlip();
		Draw(400, 300, 40);
		eventTriggered = HandleInput();
		EndFlip();

		if (eventTriggered != -1) break;
	}

	return eventTriggered;
}