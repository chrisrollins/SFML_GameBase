#include "Menu.h"
#include "TestLevel.h"
#include "DifficultySettings.h"

class Name : public GraphicalGameObject {
public:
	Name() : GraphicalGameObject(sf::Text())
	{
		font.loadFromFile("DoubleFeature.ttf");
		this->textPtr()->setFont(font);
		this->textPtr()->setString("Cursed Zombie");
		this->textPtr()->Bold;
		this->textPtr()->setFillColor(sf::Color(179, 45, 0));
		this->textPtr()->setCharacterSize(50);
		this->textPtr()->setPosition(340.f, 50.f);
	}
private:
	sf::Font font;
	sf::Text* textPtr() { return dynamic_cast<sf::Text*>(this->getGraphic()); }
};

class EasyLevelButton : public UIButton
{
public:
	EasyLevelButton() : UIButton("Easy", { 412.f, 162.f }, { 200.f, 75.f }) 
	{ 
		this->setFont("doublefeature.ttf");
		this->setTextSize(35.f);
		this->setTextColor(sf::Color(179, 45, 0));
		this->setBackgroundColor(sf::Color(0, 0, 0, 0));
	}
	void MouseButtonReleased(sf::Event e)
	{
		if (e.mouseButton.button == sf::Mouse::Button::Left //if the left mouse button was clicked
			&& this->background.getGlobalBounds().contains(static_cast<float>(e.mouseButton.x), static_cast<float>(e.mouseButton.y))) //if the click was inside the button
		{
			DifficultySettings::setDifficulty(DifficultySettings::EASY);
			mainMenu.startTestLevel();
			Engine::soundPlayer.play(SoundEffect::ID::MenuClick, 20.f);
			Engine::musicPlayer.stop();
			Engine::musicPlayer.play(Music::EasyGame);
		}
	}
};

class NormalLevelButton : public UIButton {
public:
	NormalLevelButton() : UIButton("Normal", { 412.f, 242.f }, { 200.f, 75.f }) 
	{
		this->setFont("doublefeature.ttf");
		this->setTextSize(35.f);
		this->setTextColor(sf::Color(179, 45, 0));
		this->setBackgroundColor(sf::Color(0, 0, 0, 0));
	}
	void MouseButtonReleased(sf::Event e)
	{
		if (e.mouseButton.button == sf::Mouse::Button::Left //if the left mouse button was clicked
			&& this->background.getGlobalBounds().contains(static_cast<float>(e.mouseButton.x), static_cast<float>(e.mouseButton.y))) //if the click was inside the button
		{
			DifficultySettings::setDifficulty(DifficultySettings::NORMAL);
			mainMenu.startTestLevel();
			Engine::soundPlayer.play(SoundEffect::MenuClick, 20.f);
			Engine::musicPlayer.stop();
			Engine::musicPlayer.play(Music::NormalGame);
			Engine::musicPlayer.setVolume(20.f);
		}
	}
};

class HardLevelButton : public UIButton {
public:
	HardLevelButton() : UIButton("Insane", { 412.f, 322.f }, { 200.f, 75.f }) 
	{
		this->setFont("doublefeature.ttf");
		this->setTextSize(35.f);
		this->setTextColor(sf::Color(179, 45, 0));
		this->setBackgroundColor(sf::Color(0, 0, 0, 0));
	}
	void MouseButtonReleased(sf::Event e)
	{
		if (e.mouseButton.button == sf::Mouse::Button::Left //if the left mouse button was clicked
			&& this->background.getGlobalBounds().contains(static_cast<float>(e.mouseButton.x), static_cast<float>(e.mouseButton.y))) //if the click was inside the button
		{
			DifficultySettings::setDifficulty(DifficultySettings::HARD);
			mainMenu.startTestLevel();
			Engine::soundPlayer.play(SoundEffect::MenuClick, 20.f);
			Engine::musicPlayer.stop();
			Engine::musicPlayer.play(Music::HardGame);
			Engine::musicPlayer.setVolume(20.f);
		}
	}
};

class TutorialButton : public UIButton {
public:
	TutorialButton() : UIButton("Tutorial", { 412.f, 402.f }, { 200.f, 75.f })
	{
		this->setFont("doublefeature.ttf");
		this->setTextSize(35.f);
		this->setTextColor(sf::Color(179, 45, 0));
		this->setBackgroundColor(sf::Color(0, 0, 0, 0));
	}
	void MouseButtonReleased(sf::Event e)
	{
		if (e.mouseButton.button == sf::Mouse::Button::Left //if the left mouse button was clicked
			&& this->background.getGlobalBounds().contains(static_cast<float>(e.mouseButton.x), static_cast<float>(e.mouseButton.y))) //if the click was inside the button
		{
		}
	}
};

class ScoreboardButton : public UIButton {
public:
	ScoreboardButton() : UIButton("Scoreboard", { 412.f, 482.f }, { 200.f, 75.f })
	{
		this->setFont("doublefeature.ttf");
		this->setTextSize(35.f);
		this->setTextColor(sf::Color(179, 45, 0));
		this->setBackgroundColor(sf::Color(0, 0, 0, 0));
	}
	void MouseButtonReleased(sf::Event e)
	{
		if (e.mouseButton.button == sf::Mouse::Button::Left //if the left mouse button was clicked
			&& this->background.getGlobalBounds().contains(static_cast<float>(e.mouseButton.x), static_cast<float>(e.mouseButton.y))) //if the click was inside the button
		{
		}
	}
};

class QuitButton : public UIButton
{
public:
	QuitButton() : UIButton("Escape", { 412.f, 562.f }, { 200.f, 75.f }) 
	{
		this->setFont("doublefeature.ttf");
		this->setTextSize(35.f);
		this->setTextColor(sf::Color(179, 45, 0));
		this->setBackgroundColor(sf::Color(0, 0, 0, 0));
	}
	void MouseButtonReleased(sf::Event e)
	{
		if (e.mouseButton.button == sf::Mouse::Button::Left //if the left mouse button was clicked
			&& this->background.getGlobalBounds().contains(static_cast<float>(e.mouseButton.x), static_cast<float>(e.mouseButton.y))) //if the click was inside the button
		{
			Engine::soundPlayer.play(SoundEffect::MenuClick, 20.f);
			Engine::musicPlayer.stop();
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
			new HardLevelButton(),
			new TutorialButton(),
			new ScoreboardButton(),
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
		Engine::musicPlayer.play(Music::Menu);
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