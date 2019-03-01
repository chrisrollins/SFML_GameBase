#include "Menu.h"
#include "TestLevel.h"


class StartButton : public UIButton
{
public:
	StartButton() : UIButton("Start", { 20, 20 }, { 200, 75 }) { }
	void MouseButtonReleased(sf::Event e)
	{
		if (e.mouseButton.button == sf::Mouse::Button::Left //if the left mouse button was clicked
			&& this->background.getGlobalBounds().contains(e.mouseButton.x, e.mouseButton.y)) //if the click was inside the button
		{
			mainMenu.startTestLevel();
		}
	}
};

class QuitButton : public UIButton
{
public:
	QuitButton() : UIButton("Quit", { 20, 105 }, { 200, 75 }) { }
	void MouseButtonReleased(sf::Event e)
	{
		if (e.mouseButton.button == sf::Mouse::Button::Left //if the left mouse button was clicked
			&& this->background.getGlobalBounds().contains(e.mouseButton.x, e.mouseButton.y)) //if the click was inside the button
		{
			this->screen->close();
		}
	}
};

namespace Engine
{
	Menu::Menu()
	{
		this->menuObjects = {
			new StartButton(),
			new QuitButton()
		};
	}

	Menu::~Menu()
	{
		for (auto obj : this->menuObjects)
		{
			delete obj;
		}
	}

	void Menu::start()
	{
		for (auto obj : this->menuObjects)
		{
			menuScreen.addUIObject(obj);
		}
		this->menuScreen.render();
	}

	void Menu::startTestLevel()
	{
		this->testLevel.start();
	}
}