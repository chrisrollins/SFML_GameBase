#include "Menu.h"
#include "TestLevel.h"
#include "DifficultySettings.h"

class EasyLevelButton : public UIButton
{
public:
	EasyLevelButton() : UIButton("Easy", { 412.f, 162.f }, { 200.f, 75.f }) { }
	void MouseButtonReleased(sf::Event e)
	{
		if (e.mouseButton.button == sf::Mouse::Button::Left //if the left mouse button was clicked
			&& this->background.getGlobalBounds().contains(static_cast<float>(e.mouseButton.x), static_cast<float>(e.mouseButton.y))) //if the click was inside the button
		{
			DifficultySettings::setDifficulty(DifficultySettings::EASY);
			mainMenu.startTestLevel();
			this->screen->getSoundPlayer()->play(SoundEffect::ID::MenuClick, 20.f);
			this->screen->getMusicPlayer()->stop();
			this->screen->getMusicPlayer()->play(Music::EasyGame);
		}
	}
};

class NormalLevelButton : public UIButton {
public:
	NormalLevelButton() : UIButton("Normal", { 412.f, 242.f }, { 200.f, 75.f }) {}
	void MouseButtonReleased(sf::Event e)
	{
		if (e.mouseButton.button == sf::Mouse::Button::Left //if the left mouse button was clicked
			&& this->background.getGlobalBounds().contains(static_cast<float>(e.mouseButton.x), static_cast<float>(e.mouseButton.y))) //if the click was inside the button
		{
			DifficultySettings::setDifficulty(DifficultySettings::NORMAL);
			mainMenu.startTestLevel();
			this->screen->getSoundPlayer()->play(SoundEffect::MenuClick, 20.f);
			this->screen->getMusicPlayer()->stop();
			this->screen->getMusicPlayer()->play(Music::NormalGame);
			this->screen->getMusicPlayer()->setVolume(20.f);
		}
	}
};

class HardLevelButton : public UIButton {
public:
	HardLevelButton() : UIButton("Insane", { 412.f, 322.f }, { 200.f, 75.f }) {}
	void MouseButtonReleased(sf::Event e)
	{
		if (e.mouseButton.button == sf::Mouse::Button::Left //if the left mouse button was clicked
			&& this->background.getGlobalBounds().contains(static_cast<float>(e.mouseButton.x), static_cast<float>(e.mouseButton.y))) //if the click was inside the button
		{
			DifficultySettings::setDifficulty(DifficultySettings::HARD);
			mainMenu.startTestLevel();
			this->screen->getSoundPlayer()->play(SoundEffect::MenuClick, 20.f);
			this->screen->getMusicPlayer()->stop();
			this->screen->getMusicPlayer()->play(Music::HardGame);
			this->screen->getMusicPlayer()->setVolume(20.f);
		}
	}
};

class Name : public SampleUIText {
public:
	Name() : SampleUIText("DoubleFeature.ttf", "Cursed Zombie", sf::Color::Red, 50, 340, 0) {}
};



class QuitButton : public UIButton
{
public:
	QuitButton() : UIButton("Quit", { 412.f, 602.f }, { 200.f, 75.f }) { }
	void MouseButtonReleased(sf::Event e)
	{
		if (e.mouseButton.button == sf::Mouse::Button::Left //if the left mouse button was clicked
			&& this->background.getGlobalBounds().contains(static_cast<float>(e.mouseButton.x), static_cast<float>(e.mouseButton.y))) //if the click was inside the button
		{
			this->screen->getSoundPlayer()->play(SoundEffect::MenuClick, 20.f);
			this->screen->close();
			this->screen->getMusicPlayer()->stop();
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
			obj = nullptr;
		}
	}

	void Menu::start()
	{
		menuScreen.getMusicPlayer()->play(Music::Menu);
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