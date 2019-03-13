#include "Menu.h"
#include "ScoreBoard.h"
#include "Tutorial.h"

class PlayerNameEntry : public GraphicalGameObject
{
private:
	std::string name;
	sf::Font font;
	sf::Texture texture;
	sf::Sprite background;
	sf::Clock clock;
	bool ready = false;
	bool decline = false;
	sf::Text* textPtr() { return dynamic_cast<sf::Text*>(this->graphic); }
public:
	PlayerNameEntry() : GraphicalGameObject(sf::Text())
	{
		this->font.loadFromFile("Lycanthrope.ttf");
		this->textPtr()->setFont(this->font);
		this->textPtr()->setStyle(sf::Text::Bold);
		this->textPtr()->setFillColor(sf::Color(179, 45, 0));
		this->textPtr()->setCharacterSize(50U);
		if (DifficultySettings::currentDifficulty != DifficultySettings::DIFFICULTY::TEST)
		{
			this->textPtr()->setPosition(150.f, 150.f);
			this->textPtr()->setString("Enter your name while you can:\n");
		}
		else
		{
			this->textPtr()->setPosition(100.f, 80.f);
			this->textPtr()->setString("Oops, you've entered a secret base\n created by your family, but you need \na password to enter:\n");
		}
		this->background.setPosition(600.f, 400.f);
		this->texture.loadFromFile("bloodyhands.png");
		this->background.setTexture(texture);
		for (auto obj : Menu::getCurrentMenu()->getMenuObjects()) { if (obj != this) { obj->disableEvents(); } }
	}

	void RemovedFromScreen()
	{
		for (auto obj : Menu::getCurrentMenu()->getMenuObjects()) { if (obj != this) { obj->enableEvents(); } }
	}

	void draw(sf::RenderWindow& win)
	{
		win.clear();
		win.draw(this->background);
		win.draw(*this->textPtr());
		Music::ID music;
		if (this->ready && DifficultySettings::currentDifficulty != DifficultySettings::DIFFICULTY::TEST)
		{
			win.display();
			do
			{
				;
			} while (this->clock.getElapsedTime().asSeconds() < 1.5f); // pause the good luck msg for some time before the real game start
			{
				switch (DifficultySettings::currentDifficulty)
				{
				case DifficultySettings::DIFFICULTY::EASY:
					music = Music::ID::EasyGame;
					break;
				case DifficultySettings::DIFFICULTY::NORMAL:
					music = Music::ID::NormalGame;
					break;
				case DifficultySettings::DIFFICULTY::HARD:
					music = Music::ID::HardGame;
					break;
				default:
					music = Music::ID::TestMode;
					break;
				}
				musicPlayer.stop();
				musicPlayer.play(music);
				musicPlayer.setVolume(20.f);
				this->screen->remove(this);
				Menu::getCurrentMenu()->startTestLevel(this->name);
			}
		}
		if (this->ready && DifficultySettings::currentDifficulty == DifficultySettings::DIFFICULTY::TEST)
		{
			music = Music::ID::TestMode;
			win.display();
			do
			{
				;
			} while (this->clock.getElapsedTime().asSeconds() < 1.5f);
			musicPlayer.stop();
			musicPlayer.play(music);
			musicPlayer.setVolume(20.f);
			this->screen->remove(this);
			Menu::getCurrentMenu()->startTestLevel(this->name);
		}
		if (this->decline)
		{
			win.display();
			do
			{
				;
			} while (this->clock.getElapsedTime().asSeconds() < 1.5f);
			this->decline = false;
			this->screen->remove(this);
		}
	}

	void TextEntered(sf::Event e)
	{
		int key = e.text.unicode;
		if (key == 8) //8 is delete
		{
			if (this->name.size() > 0) { this->name = this->name.substr(0, this->name.size() - 1); }

			if (DifficultySettings::currentDifficulty != DifficultySettings::DIFFICULTY::TEST) { this->textPtr()->setString("Enter your name while you can:\n" + this->name); }
			else { this->textPtr()->setString("Oops, you've entered a secret base\n created by your family, but you need \na password to enter:\n" + this->name); }
		}
		else if (key == static_cast<int>('\r') || key == static_cast<int>('\n'))
		{
			if (DifficultySettings::currentDifficulty != DifficultySettings::DIFFICULTY::TEST)
			{
				this->textPtr()->setString("Enter your name while you can:\n" + this->name + "\n\nGood luck, " + this->name + "!");
				this->ready = true;
				this->clock.restart();
			}
			else
			{
				this->textPtr()->setPosition(290.f, 280.f);
				if (this->name != "our game is better than yours! :)")
				{
					this->textPtr()->setString("Sorry, get out!!!");
					this->decline = true;
					this->clock.restart();
				}
				else
				{
					this->textPtr()->setString("Welcome, my master...");
					this->ready = true;
					this->clock.restart();
				}
			}
		}
		else
		{
			this->name += static_cast<char>(e.text.unicode);
			if (DifficultySettings::currentDifficulty != DifficultySettings::DIFFICULTY::TEST) { this->textPtr()->setString("Enter your name while you can:\n" + this->name); }
			else { this->textPtr()->setString("Oops, you've entered a secret base\n created by your family, but you need \na password to enter:\n" + this->name); }
		}
	}
};

class TestModeButton : public UIButton
{
private:
	bool activated = false;
	bool enabled = true;
	bool enterPressed = false;
public:
	TestModeButton() : UIButton("?", { 200.f, 25.f }, { 100.f, 100.f })
	{
		this->myFont.loadFromFile("harting.ttf");
		this->textPtr()->setFont(myFont);
		this->textPtr()->setCharacterSize(50);
		this->textPtr()->setFillColor({ 179, 45, 0 });
	}

	void KeyReleased(sf::Event e)
	{
		if (e.key.code == sf::Keyboard::Enter)
		{
			this->enterPressed = true;
			if (this->enterPressed && this->enabled) { this->activated = true; }
		}
	}

	void MouseButtonReleased(sf::Event e)
	{
		if (!this->enabled || !this->activated) { return; }
		if (e.mouseButton.button == sf::Mouse::Button::Left
			&& this->background.getGlobalBounds().contains(static_cast<float>(e.mouseButton.x), static_cast<float>(e.mouseButton.y))) //if the click was inside the button
		{
			DifficultySettings::setDifficulty(DifficultySettings::DIFFICULTY::TEST);
			soundPlayer.play(SoundEffect::ID::MenuClick, 20.f);
			this->screen->addUIObject(new PlayerNameEntry());
		}
	}

	void draw(sf::RenderWindow& win)
	{
		if (this->activated && this->enabled) { win.draw(*this->graphic); }
	}

	void enable()
	{
		this->enabled = true;
	}

	void disable()
	{
		this->enabled = false;
	}
};

class MenuBackground : public GraphicalGameObject
{
private:
	sf::Texture backgroundTexture;
	sf::Sprite* spritePtr() { return dynamic_cast<sf::Sprite*>(this->getGraphic()); }
public:
	MenuBackground() : GraphicalGameObject(sf::Sprite())
	{
		this->backgroundTexture.loadFromFile("menu_background.png");
		this->spritePtr()->setTexture(backgroundTexture);
	}
};

class EasyLevelButton : public UIButton
{
public:
	EasyLevelButton() : UIButton("Easy", { 372.f, 162.f }, { 250.f, 75.f })
	{
		this->setFont("Lycanthrope.ttf");
		this->setTextSize(50.f);
		this->setTextColor({ 179, 45, 0 });
		this->setBackgroundColor(sf::Color::White);
		//this->setPosition({ 372.f, 162.f });
		//this->setBackgroundColor({ 0, 0, 0, 0 });
	}

	void MouseButtonReleased(sf::Event e)
	{
		if (e.mouseButton.button == sf::Mouse::Button::Left
			&& this->background.getGlobalBounds().contains(static_cast<float>(e.mouseButton.x), static_cast<float>(e.mouseButton.y))) //if the click was inside the button
		{
			DifficultySettings::setDifficulty(DifficultySettings::DIFFICULTY::EASY);
			soundPlayer.play(SoundEffect::ID::MenuClick, 20.f);
			this->screen->addUIObject(new PlayerNameEntry());
		}
	}
};

class NormalLevelButton : public UIButton {
public:
	NormalLevelButton() : UIButton("Normal", { 372.f, 242.f }, { 250.f, 75.f })
	{
		this->setFont("Lycanthrope.ttf");
		this->setTextSize(50.f);
		this->setTextColor({ 179, 45, 0 });
		this->setBackgroundColor(sf::Color::White);
		//this->setBackgroundColor({ 0, 0, 0, 0 });
	}

	void MouseButtonReleased(sf::Event e)
	{
		if (e.mouseButton.button == sf::Mouse::Button::Left
			&& this->background.getGlobalBounds().contains(static_cast<float>(e.mouseButton.x), static_cast<float>(e.mouseButton.y))) //if the click was inside the button
		{
			DifficultySettings::setDifficulty(DifficultySettings::DIFFICULTY::NORMAL);
			soundPlayer.play(SoundEffect::ID::MenuClick, 20.f);
			this->screen->addUIObject(new PlayerNameEntry());
		}
	}
};

class HardLevelButton : public UIButton {
public:
	HardLevelButton() : UIButton("Insane", { 372.f, 322.f }, { 250.f, 75.f })
	{
		this->setFont("Lycanthrope.ttf");
		this->setTextSize(50.f);
		this->setTextColor({ 179, 45, 0 });
		this->setBackgroundColor(sf::Color::White);
		//this->setBackgroundColor({ 0, 0, 0, 0 });
	}

	void MouseButtonReleased(sf::Event e)
	{
		if (e.mouseButton.button == sf::Mouse::Button::Left
			&& this->background.getGlobalBounds().contains(static_cast<float>(e.mouseButton.x), static_cast<float>(e.mouseButton.y))) //if the click was inside the button
		{
			DifficultySettings::setDifficulty(DifficultySettings::DIFFICULTY::HARD);
			soundPlayer.play(SoundEffect::ID::MenuClick, 20.f);
			this->screen->addUIObject(new PlayerNameEntry());
		}
	}
};

class TutorialButton : public UIButton {
public:
	TutorialButton() : UIButton("Tutorial", { 372.f, 402.f }, { 250.f, 75.f })
	{
		this->setFont("Lycanthrope.ttf");
		this->setTextSize(50.f);
		this->setTextColor({ 179, 45, 0 });
		this->setBackgroundColor(sf::Color::White);
		//this->setBackgroundColor({ 0, 0, 0, 0 });
	}

	void MouseButtonReleased(sf::Event e)
	{
		if (e.mouseButton.button == sf::Mouse::Button::Left
			&& this->background.getGlobalBounds().contains(static_cast<float>(e.mouseButton.x), static_cast<float>(e.mouseButton.y))) //if the click was inside the button
		{
			Tutorial* tutorial = new Tutorial();
			this->screen->addUIObject(tutorial);
		}
	}
};

class ScoreboardButton : public UIButton {
public:
	ScoreboardButton() : UIButton("Scoreboard", { 372.f, 482.f }, { 250.f, 75.f })
	{
		this->setFont("Lycanthrope.ttf");
		this->setTextSize(50.f);
		this->setTextColor({ 179, 45, 0 });
		this->setBackgroundColor(sf::Color::White);
		//this->setBackgroundColor({ 0, 0, 0, 0 });
	}

	void MouseButtonReleased(sf::Event e)
	{
		if (e.mouseButton.button == sf::Mouse::Button::Left
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
	QuitButton() : UIButton("Escape", { 372.f, 562.f }, { 250.f, 75.f })
	{
		this->setFont("Lycanthrope.ttf");
		this->setTextSize(50.f);
		this->setTextColor({ 179, 45, 0 });
		this->setBackgroundColor(sf::Color::White);
		//this->setBackgroundColor({ 0, 0, 0, 0 });
	}

	void MouseButtonReleased(sf::Event e)
	{
		if (e.mouseButton.button == sf::Mouse::Button::Left
			&& this->background.getGlobalBounds().contains(static_cast<float>(e.mouseButton.x), static_cast<float>(e.mouseButton.y))) //if the click was inside the button
		{
			soundPlayer.play(SoundEffect::ID::MenuClick, 20.f);
			musicPlayer.stop();
			this->screen->close();
		}
	}
};

namespace Engine
{
	Menu::Menu()
	{
		currentMenu = this;
		this->menuObjects = {
			new MenuBackground(),
			new EasyLevelButton(),
			new NormalLevelButton(),
			new HardLevelButton(),
			new TutorialButton(),
			new ScoreboardButton(),
			new QuitButton(),
			new TestModeButton(),
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
		musicPlayer.play(Music::ID::Menu);
		for (auto obj : this->menuObjects) { this->menuScreen.addUIObject(obj); }
		this->menuScreen.render();
	}

	void Menu::startTestLevel(std::string playerName)
	{
		this->testLevel.start(playerName);
	}

	Menu* Menu::getCurrentMenu()
	{
		return currentMenu;
	}
}
