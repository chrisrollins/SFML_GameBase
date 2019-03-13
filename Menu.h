#ifndef MENU_H
#define MENU_H

#include "Screen.h"
#include "UIButton.h"
#include "TestLevel.h"
#include "DifficultySettings.h"
#include <vector>
#include <string>

using namespace Engine;

namespace Engine
{
	class Menu
	{
	private:
		std::vector<GameObject*> menuObjects;
		Screen menuScreen;
		TestLevel testLevel;
	public:
		Menu();
		~Menu();
		void start();
		void startTestLevel(std::string playerName);
		std::vector<GameObject*>& getMenuObjects() { return this->menuObjects; }
		static Menu* getCurrentMenu();
	};

	static Menu* currentMenu = nullptr;
}

#endif
