#ifndef MENU_HEADER
#define MENU_HEADER

#include "SFML/Graphics.hpp"
#include "Screen.h"
#include "GameObject.h"
#include "UIButton.h"
#include "TestLevel.h"
#include <vector>
#include <string>

using namespace Engine;

namespace Engine
{
	class Menu
	{
	public:
		Menu();
		~Menu();
		void start();
		void startTestLevel(std::string playerName);
		std::vector<GameObject*>& getMenuObjects() { return this->menuObjects; }
		static Menu* getCurrentMenu();
	private:
		std::vector<GameObject*> menuObjects;
		Screen menuScreen;
		TestLevel testLevel;
	};
}

#endif
