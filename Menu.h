//
//  Menu.h
//  group PROJECT
//
//  Created by Jakin Wang on 2/26/19.
//  Copyright © 2019 Jakin Wang. All rights reserved.
//

#include <SFML/Graphics.hpp>
#include "Screen.h"
#include "GameObject.h"
#include "UIButton.h"
#include <vector>
#ifndef MENU_HEADER
#define MENU_HEADER

using namespace Engine;

namespace Engine
{
	class Menu
	{
	public:
		Menu();
		~Menu();
		void start();
	private:
		std::vector<GameObject*> menuObjects;
		Screen menuScreen;
	};
}

#endif
