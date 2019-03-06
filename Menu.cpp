#include "Menu.h"
#include "TestLevel.h"
#include "DifficultySettings.h"

class EasyLevelButton : public UIButton
{
public:
	EasyLevelButton() : UIButton("Easy", { 412, 162}, { 200, 75 }) { }
	void MouseButtonReleased(sf::Event e)
	{
		if (e.mouseButton.button == sf::Mouse::Button::Left //if the left mouse button was clicked
			&& this->background.getGlobalBounds().contains(e.mouseButton.x, e.mouseButton.y)) //if the click was inside the button
		{
			DifficultySettings::setDifficulty(DifficultySettings::EASY);
			mainMenu.startTestLevel();
		}
	}
};

class NormalLevelButton : public UIButton{
public:
    NormalLevelButton() : UIButton("Normal", {412, 242}, {200, 75}) {}
    void MouseButtonReleased(sf::Event e)
    {
        if (e.mouseButton.button == sf::Mouse::Button::Left //if the left mouse button was clicked
            && this->background.getGlobalBounds().contains(e.mouseButton.x, e.mouseButton.y)) //if the click was inside the button
        {
			DifficultySettings::setDifficulty(DifficultySettings::NORMAL);
            mainMenu.startTestLevel();
        }
    }
};

class HardLevelButton : public UIButton{
public:
    HardLevelButton() : UIButton("Hard", {412, 322}, {200, 75}) {}
    void MouseButtonReleased(sf::Event e)
    {
        if (e.mouseButton.button == sf::Mouse::Button::Left //if the left mouse button was clicked
            && this->background.getGlobalBounds().contains(e.mouseButton.x, e.mouseButton.y)) //if the click was inside the button
        {
			DifficultySettings::setDifficulty(DifficultySettings::HARD);
            mainMenu.startTestLevel();
        }
    }
};

class Name : public SampleUIText{
public:
    Name() : SampleUIText("DoubleFeature.ttf", "Cursed Zombie", sf::Color::Red, 50, 340, 0){}
};



class QuitButton : public UIButton
{
public:
	QuitButton() : UIButton("Quit", { 412, 602}, { 200, 75 }) { }
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
            new Name(),
			new EasyLevelButton(),
            new NormalLevelButton(),
            new HardLevelButton,
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
