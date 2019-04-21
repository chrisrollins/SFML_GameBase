#include "Menu.h"
#include "ScoreBoard.h"
#include "Tutorial.h"
#include "FileLoadException.h"
#include "ResourceManager.h"
#include "SpriteFactory.h"
#include <functional>
#include <cstdint>

class PlayerNameEntry : public GraphicalGameObject
{
private:
	std::string name;
	sf::Sprite background;
	sf::Clock clock;
	bool ready = false;
	bool decline = false;
	sf::Text* textPtr() { return dynamic_cast<sf::Text*>(this->graphic); }
public:
	PlayerNameEntry() : GraphicalGameObject(sf::Text()), background(SpriteFactory::generateSprite(Sprite::ID::Bloodyhands))
	{
		sf::Font* fontPtr = ResourceManager<sf::Font>::GetResource("Lycanthrope.ttf");
		this->textPtr()->setFont(*fontPtr);
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
				MusicPlayer::play(music);
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
			MusicPlayer::play(music);
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

class MenuButton : public GraphicalGameObject
{
private:
	sf::Sprite* spritePtr() { return dynamic_cast<sf::Sprite*>(this->getGraphic()); }
	std::function<void()> clickFunction;
public:
	MenuButton(Sprite::ID spriteID, sf::Vector2f position, std::function<void()> clickFunction) : GraphicalGameObject(SpriteFactory::generateSprite(spriteID))
	{
		this->spritePtr()->setColor({ 255, 255, 255, 0 });
		this->spritePtr()->setPosition(position);
		this->clickFunction = clickFunction;
	}

	void MouseButtonReleased(sf::Event e)
	{
		if (e.mouseButton.button == sf::Mouse::Button::Left
			&& this->spritePtr()->getGlobalBounds().contains(static_cast<float>(e.mouseButton.x), static_cast<float>(e.mouseButton.y)))
		{
			SoundPlayer::play(SoundEffect::ID::MenuClick, 20.f);
			this->clickFunction();
		}
	}
};

class TestModeButton : public MenuButton, SpriteSheet
{
private:
	//sf::Vector2u textureSize;
	//sf::Vector2u imageCount;
	//sf::Vector2u currentImage;
	bool activated = false;
	bool enabled = true;
	bool enterPressed = false;
	sf::Color color;
	sf::Sprite* spritePtr() { return dynamic_cast<sf::Sprite*>(this->getGraphic()); }
public:
	TestModeButton() : MenuButton(Sprite::ID::Guardian, { 320.f, 160.f }, [&]()
		{
			if (!this->enabled || !this->activated) { return; }
			DifficultySettings::setDifficulty(DifficultySettings::DIFFICULTY::TEST);
			this->screen->addUIObject(new PlayerNameEntry());
		}),
		SpriteSheet(3)
	{
		/*
		this->textureSize = this->spritePtr()->getTexture()->getSize();
		this->textureSize.x /= 3;
		this->imageCount.x = 0;
		this->color = this->spritePtr()->getColor();
		this->spritePtr()->setColor({ 0, 0, 0, 0 });*/
	}

	void EveryFrame(uint64_t f)
	{
		if (f % 15 == 0)
		{
			//if (this->imageCount.x == 2) { this->imageCount.x = 0; }
			//else { this->imageCount.x++; }
			this->spriteSheetRow++;
		}

		//this->spritePtr()->setTextureRect(sf::IntRect(this->imageCount.x * this->textureSize.x, this->imageCount.y * this->textureSize.y, this->textureSize.x, this->textureSize.y));
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
			SoundPlayer::play(SoundEffect::ID::MenuClick, 20.f);
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
	sf::Sprite* spritePtr() { return dynamic_cast<sf::Sprite*>(this->getGraphic()); }
public:
	MenuBackground() : GraphicalGameObject(SpriteFactory::generateSprite(Sprite::ID::MenuBackground))
	{
		this->spritePtr()->setColor({ 255, 255, 255, 0 });
	}

	void AddedToScreen()
	{
		this->screen->schedule([&]() {
			sf::Color color = this->spritePtr()->getColor();
			color.a += 3;
			this->spritePtr()->setColor(color);
		}, TimeUnit::Frames(1), 84);
	}
};

class EasyLevelButton : public MenuButton
{
private:
	sf::Sprite* spritePtr() { return dynamic_cast<sf::Sprite*>(this->getGraphic()); }
public:
	EasyLevelButton() : MenuButton(Sprite::ID::MenuEasy, { 430.f, 190.f }, [&]()
	{
		DifficultySettings::setDifficulty(DifficultySettings::DIFFICULTY::EASY);
		this->screen->addUIObject(new PlayerNameEntry());
	}) {}

	void AddedToScreen()
	{
		this->screen->schedule([&]() {
			sf::Color color = this->spritePtr()->getColor();
			color.a += 3;
			this->spritePtr()->setColor(color);
		}, TimeUnit::Frames(1), 84);
	}
};

class NormalLevelButton : public MenuButton
{
private:
	sf::Sprite* spritePtr() { return dynamic_cast<sf::Sprite*>(this->getGraphic()); }
public:
	NormalLevelButton() : MenuButton(Sprite::ID::MenuNormal, { 390.f, 265.f }, [&]()
	{
		DifficultySettings::setDifficulty(DifficultySettings::DIFFICULTY::NORMAL);
		this->screen->addUIObject(new PlayerNameEntry());
	}) {}

	void AddedToScreen()
	{
		this->screen->schedule([&]() {
			sf::Color color = this->spritePtr()->getColor();
			color.a += 3;
			this->spritePtr()->setColor(color);
		}, TimeUnit::Frames(1), 84);
	}
};

class HardLevelButton : public MenuButton
{
private:
	sf::Sprite* spritePtr() { return dynamic_cast<sf::Sprite*>(this->getGraphic()); }
public:
	HardLevelButton() : MenuButton(Sprite::ID::MenuInsane, { 390.f, 360.f }, [&]()
	{
		DifficultySettings::setDifficulty(DifficultySettings::DIFFICULTY::HARD);
		this->screen->addUIObject(new PlayerNameEntry());
	}) {}

	void AddedToScreen()
	{
		this->screen->schedule([&]() {
			sf::Color color = this->spritePtr()->getColor();
			color.a += 3;
			this->spritePtr()->setColor(color);
		}, TimeUnit::Frames(1), 84);
	}
};

class TutorialButton : public MenuButton
{
private:
	sf::Sprite* spritePtr() { return dynamic_cast<sf::Sprite*>(this->getGraphic()); }
public:
	TutorialButton() : MenuButton(Sprite::ID::MenuTutorial, { 340.f, 450.f }, [&]()
	{
		Tutorial* tutorial = new Tutorial();
		this->screen->addUIObject(tutorial);
	}) {}

	void AddedToScreen()
	{
		this->screen->schedule([&]() {
			sf::Color color = this->spritePtr()->getColor();
			color.a += 3;
			this->spritePtr()->setColor(color);
		}, TimeUnit::Frames(1), 84);
	}
};

class ScoreboardButton : public MenuButton
{
private:
	sf::Sprite* spritePtr() { return dynamic_cast<sf::Sprite*>(this->getGraphic()); }
public:
	ScoreboardButton() : MenuButton(Sprite::ID::MenuScore, { 410.f, 540.f }, [&]()
	{
		ScoreBoard* scoreBoard = new ScoreBoard();
		this->screen->addUIObject(scoreBoard);
	}) {}

	void AddedToScreen()
	{
		this->screen->schedule([&]() {
			sf::Color color = this->spritePtr()->getColor();
			color.a += 3;
			this->spritePtr()->setColor(color);
		}, TimeUnit::Frames(1), 84);
	}
};

class QuitButton : public MenuButton
{
private:
	sf::Sprite* spritePtr() { return dynamic_cast<sf::Sprite*>(this->getGraphic()); }
public:
	QuitButton() : MenuButton(Sprite::ID::MenuEscape, { 390.f, 630.f }, [&]()
	{
		MusicPlayer::stop();
		this->screen->close();
	}) {}

	void AddedToScreen()
	{
		this->screen->schedule([&]() {
			sf::Color color = this->spritePtr()->getColor();
			color.a += 3;
			this->spritePtr()->setColor(color);
		}, TimeUnit::Frames(1), 84);
	}
};

class Loader : public GraphicalGameObject
{
public:
	Loader(bool show, std::vector<function<void()>> loadingActions) : GraphicalGameObject(sf::RectangleShape())
	{
		sf::RectangleShape* rect = this->rectPtr();
		rect->setPosition(static_cast<float>(Screen::windowWidth) * 0.05f, static_cast<float>(Screen::windowHeight) * 0.5f);
		rect->setSize({ 0.f , 10.f });
		rect->setFillColor({ 9, 67, 8, (show ? static_cast<uint8_t>(255) : static_cast<uint8_t>(0)) });
		size_t numActions = loadingActions.size();
		this->barIncrement = (static_cast<float>(Screen::windowWidth) * 0.9f) / static_cast<float>(numActions);
		this->loadingActions = loadingActions;
		this->finishedLoading = false;
	}

	void draw(sf::RenderWindow& win)
	{
		if (!this->finishedLoading)
		{
			for (auto action : this->loadingActions)
			{
				action();
				sf::Vector2f size = this->rectPtr()->getSize();
				size.x += this->barIncrement;
				this->rectPtr()->setSize(size);
				win.clear();
				win.draw(*this->graphic);
				win.display();
			}
			this->finishedLoading = true;
			this->screen->schedule([&]() {
				sf::Color color = this->rectPtr()->getFillColor();
				color.a -= (color.a > 8) ? 8 : color.a;;
				this->rectPtr()->setFillColor(color);
			}, TimeUnit::Frames(1), 31);
		}
		else
		{
			win.draw(*this->graphic);
		}
	}

private:
	std::vector<function<void()>> loadingActions;
	sf::RectangleShape* rectPtr() { return dynamic_cast<sf::RectangleShape*>(this->graphic); }
	float barIncrement;
	bool finishedLoading;
};

namespace Engine
{
	Menu::Menu(bool showLoadingBar)
	{
		currentMenu = this;
		Loader* loader = new Loader(showLoadingBar, {
			[]() { MusicPlayer::play(Music::ID::Menu); },
			[]() { SoundPlayer::preloadSounds(); },
			[&]() { this->menuObjects.push_back(new MenuBackground()); },
			[&]() { this->menuObjects.push_back(new EasyLevelButton()); },
			[&]() { this->menuObjects.push_back(new NormalLevelButton()); },
			[&]() { this->menuObjects.push_back(new HardLevelButton()); },
			[&]() { this->menuObjects.push_back(new TutorialButton()); },
			[&]() { this->menuObjects.push_back(new ScoreboardButton()); },
			[&]() { this->menuObjects.push_back(new QuitButton()); },
			[&]() { this->menuObjects.push_back(new TestModeButton()); },
			[&]() {	for (auto obj : this->menuObjects) { this->menuScreen.addUIObject(obj); } }
			});
		this->menuScreen.addUIObject(loader);
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