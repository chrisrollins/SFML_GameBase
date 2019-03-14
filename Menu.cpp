#include "Menu.h"
#include "ScoreBoard.h"
#include "Tutorial.h"
#include "FileLoadException.h"

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
		if (!this->font.loadFromFile("Lycanthrope.ttf")) { throw GameException::FontFileLoadException("Lycanthrope.ttf"); }
		this->textPtr()->setFont(this->font);
		this->textPtr()->setStyle(sf::Text::Bold);
		this->textPtr()->setFillColor({ 179, 45, 0 });
		this->textPtr()->setCharacterSize(50U);
		if (DifficultySettings::currentDifficulty != DifficultySettings::DIFFICULTY::TEST)
		{
			this->textPtr()->setPosition(150.f, 150.f);
			this->textPtr()->setString("Enter your name while you can:\n");
		}
		else
		{
			this->textPtr()->setPosition(100.f, 80.f);
			this->textPtr()->setString("Oops, you've entered a secret base\n created by your family, but the \nguardian requires a password:\n");
		}
		this->background.setPosition(600.f, 400.f);
		if (!this->texture.loadFromFile("bloodyhands.png")) { throw GameException::ImageFileLoadException("bloodyhands.png"); }
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
			} while (this->clock.getElapsedTime().asSeconds() < 1.5f); // pause the 'good luck' msg for some time before the real game start
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
			else { this->textPtr()->setString("Oops, you've entered a secret base\n created by your family, but the \nguardian requires a password:\n" + this->name); }
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
			else { this->textPtr()->setString("Oops, you've entered a secret base\n created by your family, but the \nguardian requires a password:\n" + this->name); }
		}
	}
};

class TestModeButton : public GraphicalGameObject
{
private:
	sf::Vector2u textureSize;
	sf::Vector2u imageCount;
	sf::Vector2u currentImage;
	bool activated = false;
	bool enabled = true;
	bool enterPressed = false;
	sf::Color color;
	sf::Texture guardianTexture;
	sf::Sprite* spritePtr() { return dynamic_cast<sf::Sprite*>(this->getGraphic()); }
public:
	TestModeButton() : GraphicalGameObject(sf::Sprite())
	{
		if (!this->guardianTexture.loadFromFile("guardian.png")) { throw GameException::ImageFileLoadException("guardian.png"); }
		this->spritePtr()->setTexture(this->guardianTexture);
		this->textureSize = this->spritePtr()->getTexture()->getSize();
		this->textureSize.x /= 3;
		this->imageCount.x = 0;
		this->spritePtr()->setPosition(320.f, 160.f);
		this->color = this->spritePtr()->getColor();
		this->spritePtr()->setColor({ 0, 0, 0, 0 });
	}

	void EveryFrame(uint64_t f)
	{
		if (f % 15 == 0)
		{
			if (this->imageCount.x == 2) { this->imageCount.x = 0; }
			else { this->imageCount.x++; }
		}

		this->spritePtr()->setTextureRect(sf::IntRect(this->imageCount.x * this->textureSize.x,
			this->imageCount.y * this->textureSize.y, this->textureSize.x, this->textureSize.y));
	}

	void KeyReleased(sf::Event e)
	{
		if (e.key.code == sf::Keyboard::Enter)
		{
			this->enterPressed = true;
			if (this->enterPressed && this->enabled)
			{
				this->activated = true;
				this->spritePtr()->setColor(this->color);
			}
		}
	}

	void MouseButtonReleased(sf::Event e)
	{
		if (!this->enabled || !this->activated) { return; }
		if (e.mouseButton.button == sf::Mouse::Button::Left
			&& this->spritePtr()->getGlobalBounds().contains(static_cast<float>(e.mouseButton.x), static_cast<float>(e.mouseButton.y)))
		{
			DifficultySettings::setDifficulty(DifficultySettings::DIFFICULTY::TEST);
			soundPlayer.play(SoundEffect::ID::MenuClick, 20.f);
			this->screen->addUIObject(new PlayerNameEntry());
		}
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
		if (!this->backgroundTexture.loadFromFile("menu_background.png")) { throw GameException::ImageFileLoadException("menu_background.png"); }
		this->spritePtr()->setTexture(this->backgroundTexture);
	}
};

class EasyLevelButton : public GraphicalGameObject
{
private:
	sf::Texture easyButtonTexture;
	sf::Sprite* spritePtr() { return dynamic_cast<sf::Sprite*>(this->getGraphic()); }
public:
	EasyLevelButton() : GraphicalGameObject(sf::Sprite())
	{
		if (!this->easyButtonTexture.loadFromFile("menu_easy.png")) { throw GameException::ImageFileLoadException("menu_easy.png"); }
		this->spritePtr()->setTexture(this->easyButtonTexture);
		this->spritePtr()->setPosition(430.f, 190.f);
	}

	void MouseButtonReleased(sf::Event e)
	{
		if (e.mouseButton.button == sf::Mouse::Button::Left
			&& this->spritePtr()->getGlobalBounds().contains(static_cast<float>(e.mouseButton.x), static_cast<float>(e.mouseButton.y)))
		{
			DifficultySettings::setDifficulty(DifficultySettings::DIFFICULTY::EASY);
			soundPlayer.play(SoundEffect::ID::MenuClick, 20.f);
			this->screen->addUIObject(new PlayerNameEntry());
		}
	}
};

class NormalLevelButton : public GraphicalGameObject
{
private:
	sf::Texture normalButtonTexture;
	sf::Sprite* spritePtr() { return dynamic_cast<sf::Sprite*>(this->getGraphic()); }
public:
	NormalLevelButton() : GraphicalGameObject(sf::Sprite())
	{
		if (!this->normalButtonTexture.loadFromFile("menu_normal.png")) { throw GameException::ImageFileLoadException("menu_normal.png"); }
		this->spritePtr()->setTexture(this->normalButtonTexture);
		this->spritePtr()->setPosition(390.f, 265.f);
	}

	void MouseButtonReleased(sf::Event e)
	{
		if (e.mouseButton.button == sf::Mouse::Button::Left
			&& this->spritePtr()->getGlobalBounds().contains(static_cast<float>(e.mouseButton.x), static_cast<float>(e.mouseButton.y)))
		{
			DifficultySettings::setDifficulty(DifficultySettings::DIFFICULTY::NORMAL);
			soundPlayer.play(SoundEffect::ID::MenuClick, 20.f);
			this->screen->addUIObject(new PlayerNameEntry());
		}
	}
};

class HardLevelButton : public GraphicalGameObject
{
private:
	sf::Texture hardButtonTexture;
	sf::Sprite* spritePtr() { return dynamic_cast<sf::Sprite*>(this->getGraphic()); }
public:
	HardLevelButton() : GraphicalGameObject(sf::Sprite())
	{
		if (!this->hardButtonTexture.loadFromFile("menu_insane.png")) { throw GameException::ImageFileLoadException("menu_insane.png"); }
		this->spritePtr()->setTexture(this->hardButtonTexture);
		this->spritePtr()->setPosition(390.f, 360.f);
	}

	void MouseButtonReleased(sf::Event e)
	{
		if (e.mouseButton.button == sf::Mouse::Button::Left
			&& this->spritePtr()->getGlobalBounds().contains(static_cast<float>(e.mouseButton.x), static_cast<float>(e.mouseButton.y)))
		{
			DifficultySettings::setDifficulty(DifficultySettings::DIFFICULTY::HARD);
			soundPlayer.play(SoundEffect::ID::MenuClick, 20.f);
			this->screen->addUIObject(new PlayerNameEntry());
		}
	}
};

class TutorialButton : public GraphicalGameObject
{
private:
	sf::Texture tutorialButtonTexture;
	sf::Sprite* spritePtr() { return dynamic_cast<sf::Sprite*>(this->getGraphic()); }
public:
	TutorialButton() : GraphicalGameObject(sf::Sprite())
	{
		if (!this->tutorialButtonTexture.loadFromFile("menu_tutorial.png")) { throw GameException::ImageFileLoadException("menu_tutorial.png"); }
		this->spritePtr()->setTexture(this->tutorialButtonTexture);
		this->spritePtr()->setPosition(340.f, 450.f);
	}

	void MouseButtonReleased(sf::Event e)
	{
		if (e.mouseButton.button == sf::Mouse::Button::Left
			&& this->spritePtr()->getGlobalBounds().contains(static_cast<float>(e.mouseButton.x), static_cast<float>(e.mouseButton.y)))
		{
			Tutorial* tutorial = new Tutorial();
			this->screen->addUIObject(tutorial);
		}
	}
};

class ScoreboardButton : public GraphicalGameObject
{
private:
	sf::Texture scoreButtonTexture;
	sf::Sprite* spritePtr() { return dynamic_cast<sf::Sprite*>(this->getGraphic()); }
public:
	ScoreboardButton() : GraphicalGameObject(sf::Sprite())
	{
		if (!this->scoreButtonTexture.loadFromFile("menu_score.png")) { throw GameException::ImageFileLoadException("menu_score.png"); }
		this->spritePtr()->setTexture(this->scoreButtonTexture);
		this->spritePtr()->setPosition(410.f, 540.f);
	}

	void MouseButtonReleased(sf::Event e)
	{
		if (e.mouseButton.button == sf::Mouse::Button::Left
			&& this->spritePtr()->getGlobalBounds().contains(static_cast<float>(e.mouseButton.x), static_cast<float>(e.mouseButton.y)))
		{
			ScoreBoard* scoreBoard = new ScoreBoard();
			this->screen->addUIObject(scoreBoard);
		}
	}
};

class QuitButton : public GraphicalGameObject
{
private:
	sf::Texture quitButtonTexture;
	sf::Sprite* spritePtr() { return dynamic_cast<sf::Sprite*>(this->getGraphic()); }
public:
	QuitButton() : GraphicalGameObject(sf::Sprite())
	{
		if (!this->quitButtonTexture.loadFromFile("menu_escape.png")) { throw GameException::ImageFileLoadException("menu_escape.png"); }
		this->spritePtr()->setTexture(this->quitButtonTexture);
		this->spritePtr()->setPosition(390.f, 630.f);
	}

	void MouseButtonReleased(sf::Event e)
	{
		if (e.mouseButton.button == sf::Mouse::Button::Left
			&& this->spritePtr()->getGlobalBounds().contains(static_cast<float>(e.mouseButton.x), static_cast<float>(e.mouseButton.y)))
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