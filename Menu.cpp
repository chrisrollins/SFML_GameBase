#include "Menu.h"
#include "TestLevel.h"
#include "DifficultySettings.h"

class EasyLevelButton : public UIButton
{
public:
	EasyLevelButton() : UIButton("Easy", { 412.f, 162.f}, { 200.f, 75.f }) { }
	void MouseButtonReleased(sf::Event e)
	{
		if (e.mouseButton.button == sf::Mouse::Button::Left //if the left mouse button was clicked
			&& this->background.getGlobalBounds().contains(static_cast<float>(e.mouseButton.x), static_cast<float>(e.mouseButton.y))) //if the click was inside the button
		{
			DifficultySettings::setDifficulty(DifficultySettings::EASY);
			mainMenu.startTestLevel();
		}
	}
};

class NormalLevelButton : public UIButton{
public:
    NormalLevelButton() : UIButton("Normal", {412.f, 242.f}, {200.f, 75.f}) {}
    void MouseButtonReleased(sf::Event e)
    {
        if (e.mouseButton.button == sf::Mouse::Button::Left //if the left mouse button was clicked
            && this->background.getGlobalBounds().contains(static_cast<float>(e.mouseButton.x), static_cast<float>(e.mouseButton.y))) //if the click was inside the button
        {
			DifficultySettings::setDifficulty(DifficultySettings::NORMAL);
            mainMenu.startTestLevel();
        }
    }
};

class HardLevelButton : public UIButton{
public:
    HardLevelButton() : UIButton("Hard", {412.f, 322.f}, {200.f, 75.f}) {}
    void MouseButtonReleased(sf::Event e)
    {
        if (e.mouseButton.button == sf::Mouse::Button::Left //if the left mouse button was clicked
            && this->background.getGlobalBounds().contains(static_cast<float>(e.mouseButton.x), static_cast<float>(e.mouseButton.y))) //if the click was inside the button
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
	QuitButton() : UIButton("Quit", { 412.f, 602.f}, { 200.f, 75.f }) { }
	void MouseButtonReleased(sf::Event e)
	{
		if (e.mouseButton.button == sf::Mouse::Button::Left //if the left mouse button was clicked
			&& this->background.getGlobalBounds().contains(static_cast<float>(e.mouseButton.x), static_cast<float>(e.mouseButton.y))) //if the click was inside the button
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
