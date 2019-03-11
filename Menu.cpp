#include "Menu.h"
#include "TestLevel.h"
#include "DifficultySettings.h"

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
		this->font.loadFromFile("sansation.ttf");
		this->textPtr()->setFont(this->font);
		this->textPtr()->setPosition(400.f, 400.f);
		this->textPtr()->setFillColor({ 255, 255, 255, 255 });
		this->textPtr()->setString("Please enter your name:");
		this->background.setPosition(380.f, 380.f);
		this->background.setSize({ 420.f, 120.f });
		this->background.setFillColor({ 50, 70, 70, 255});
	}
	void draw(sf::RenderWindow& win)
	{
		win.draw(this->background);
		win.draw(*this->textPtr());
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
		}
		else if(key == static_cast<int>('\r') || key == static_cast<int>('\n'))
		{
			Music::ID music;
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
			
			mainMenu.startTestLevel();
			Engine::musicPlayer.stop();
			Engine::musicPlayer.play(music);
			Engine::musicPlayer.setVolume(20.f);
			this->screen->remove(this);
			return;
		}
		else
		{
			this->name += static_cast<char>(e.text.unicode);
		}

		this->textPtr()->setString("Please enter your name:\n" + this->name);		
	}
private:
	std::string name;
	sf::Font font;
	sf::Text* textPtr() { return dynamic_cast<sf::Text*>(this->graphic); }
	sf::RectangleShape background;
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

	void Menu::startTestLevel()
	{
		this->testLevel.start();
	}
}
