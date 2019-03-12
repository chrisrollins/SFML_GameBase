#include "Menu.h"
#include "TestLevel.h"
#include "DifficultySettings.h"
#include "ScoreBoard.h"
#include <string>

static Menu* currentMenu = nullptr;

class GameTitle : public GraphicalGameObject {
public:
	GameTitle() : GraphicalGameObject(sf::Text())
	{
		font.loadFromFile("DoubleFeature.ttf");
		this->textPtr()->setFont(font);
		this->textPtr()->setString("Cursed Zombie");
		this->textPtr()->setStyle(sf::Text::Bold);
		this->textPtr()->setFillColor(sf::Color(179, 45, 0));
		this->textPtr()->setCharacterSize(50);
		this->textPtr()->setPosition(340.f, 50.f);
	}
private:
	sf::Font font;
	sf::Text* textPtr() { return dynamic_cast<sf::Text*>(this->getGraphic()); }
};

class PlayerNameEntry : public GraphicalGameObject
{
public:
	PlayerNameEntry() : GraphicalGameObject(sf::Text())
	{
		this->font.loadFromFile("Lycanthrope.ttf");
		this->textPtr()->setFont(this->font);
		this->textPtr()->setStyle(sf::Text::Bold);
		this->textPtr()->setFillColor(sf::Color(179, 45, 0));
		this->textPtr()->setCharacterSize(50U);
		this->textPtr()->setPosition(150.f, 150.f);
		this->textPtr()->setString("Enter your name while you can:");
		texture.loadFromFile("bloodyhands.png");
		background.setTexture(texture);
		background.setPosition(600.f, 400.f);
		ready = false;
	}
	void draw(sf::RenderWindow& win)
	{
		win.clear();
		win.draw(background);
		win.draw(*this->textPtr());
		if (ready)
		{
			win.display();
			do
			{
				;
			} while (clock.getElapsedTime().asSeconds() < 1.5f); // wait for a second for the good luck text to display before entering the game
			Music::ID music;
			{
				switch (DifficultySettings::currentDifficulty)
				{
				case DifficultySettings::EASY:
					music = Music::EasyGame;
					break;
				case DifficultySettings::NORMAL:
					music = Music::NormalGame;
					break;
				case DifficultySettings::HARD:
					music = Music::HardGame;
					break;
				default:
					music = Music::EasyGame;
					break;
				}
				Engine::musicPlayer.stop();
				Engine::musicPlayer.play(music);
				Engine::musicPlayer.setVolume(20.f);
				this->screen->remove(this);
				currentMenu->startTestLevel(this->name);
			}
		}
	}
	void TextEntered(sf::Event e)
	{
		int key = e.text.unicode;
		if (key == 8) //8 is delete
		{
			if (this->name.size() > 0)
			{
				this->name = this->name.substr(0, this->name.size() - 1);
			}
			this->textPtr()->setString("Enter your name while you can:\n" + this->name);
		}
		else if (key == static_cast<int>('\r') || key == static_cast<int>('\n'))
		{
			this->textPtr()->setString("Enter your name while you can:\n" + this->name + "\n\nGood luck, " + this->name + "!");
			ready = true;
			clock.restart();
		}
		else
		{
			this->name += static_cast<char>(e.text.unicode);
			this->textPtr()->setString("Enter your name while you can:\n" + this->name);
		}
	}
private:
	std::string name;
	sf::Font font;
	sf::Text* textPtr() { return dynamic_cast<sf::Text*>(this->graphic); }
	sf::Texture texture;
	sf::Sprite background;
	bool ready;
	sf::Clock clock;
};

class EasyLevelButton : public UIButton
{
public:
	EasyLevelButton() : UIButton("Easy", { 412.f, 162.f }, { 200.f, 75.f })
	{
		this->setFont("DoubleFeature.ttf");
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
			Engine::soundPlayer.play(SoundEffect::ID::MenuClick, 20.f);
			this->screen->addUIObject(new PlayerNameEntry());
		}
	}
};

class NormalLevelButton : public UIButton {
public:
	NormalLevelButton() : UIButton("Normal", { 412.f, 242.f }, { 200.f, 75.f })
	{
		this->setFont("DoubleFeature.ttf");
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
			Engine::soundPlayer.play(SoundEffect::ID::MenuClick, 20.f);
			this->screen->addUIObject(new PlayerNameEntry());
		}
	}
};

class HardLevelButton : public UIButton {
public:
	HardLevelButton() : UIButton("Insane", { 412.f, 322.f }, { 200.f, 75.f })
	{
		this->setFont("DoubleFeature.ttf");
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
			Engine::soundPlayer.play(SoundEffect::ID::MenuClick, 20.f);
			this->screen->addUIObject(new PlayerNameEntry());
		}
	}
};

class TutorialButton : public UIButton {
public:
	TutorialButton() : UIButton("Tutorial", { 412.f, 402.f }, { 200.f, 75.f })
	{
		this->setFont("DoubleFeature.ttf");
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
		this->setFont("DoubleFeature.ttf");
		this->setTextSize(35.f);
		this->setTextColor(sf::Color(179, 45, 0));
		this->setBackgroundColor(sf::Color(0, 0, 0, 0));
	}
	void MouseButtonReleased(sf::Event e)
	{
		if (e.mouseButton.button == sf::Mouse::Button::Left //if the left mouse button was clicked
			&& this->background.getGlobalBounds().contains(static_cast<float>(e.mouseButton.x), static_cast<float>(e.mouseButton.y))) //if the click was inside the button
		{
			ScoreBoard* scoreBoard = new ScoreBoard();
			this->screen->addUIObject(scoreBoard);
		}
	}
};

class QuitButton : public UIButton
{
public:
	QuitButton() : UIButton("Escape", { 412.f, 562.f }, { 200.f, 75.f })
	{
		this->setFont("DoubleFeature.ttf");
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
			new GameTitle(),
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

	void Menu::startTestLevel(std::string playerName)
	{
		this->testLevel.start(playerName);
	}
}
