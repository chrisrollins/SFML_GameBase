#ifndef TUTORIAL_H
#define TUTORIAL_H

#include "GameObject.h"
#include "Screen.h"
#include "Menu.h"
#include "FileLoadException.h"

using namespace Engine;

class Tutorial : public GraphicalGameObject
{
private:
	sf::Texture storyTexture;
	sf::Sprite story;
	sf::Texture tutorialTexture;
	sf::Sprite tutorial;
	sf::Texture bookButtonTexture;
	sf::Sprite bookButton;
	sf::Texture xButtonTexture;
	sf::Sprite xButton;
	sf::Color currColor;
	int internalClock = 0;
	bool resetClock = false;
	bool showStory = true;
public:
	Tutorial() : GraphicalGameObject(sf::Sprite())
	{
		if (!this->storyTexture.loadFromFile("story.png")) { throw GameException::ImageFileLoadException("story.png"); }
		this->story.setTexture(storyTexture);
		if (!this->tutorialTexture.loadFromFile("tutorial.png")) { throw GameException::ImageFileLoadException("tutorial.png"); }
		this->tutorial.setTexture(tutorialTexture);
		if (!this->bookButtonTexture.loadFromFile("book_button.png")) { throw GameException::ImageFileLoadException("book_button.png"); }
		this->bookButton.setTexture(bookButtonTexture);
		this->bookButton.setPosition(Screen::windowWidth - 280.f, Screen::windowHeight - 260.f);
		if (!this->xButtonTexture.loadFromFile("x_button.png")) { throw GameException::ImageFileLoadException("x_button.png"); }
		this->xButton.setTexture(this->xButtonTexture);
		this->xButton.setPosition(Screen::windowWidth - 55.f, 15.f);
		for (auto obj : Menu::getCurrentMenu()->getMenuObjects()) { if (obj != this) { obj->disableEvents(); } }
	}

	void RemovedFromScreen()
	{
		for (auto obj : Menu::getCurrentMenu()->getMenuObjects()) { if (obj != this) { obj->enableEvents(); } }
	}

	void MouseButtonReleased(sf::Event e)
	{
		if (e.mouseButton.button != sf::Mouse::Button::Left) { return; }
		sf::Vector2i mouse = this->screen->getMousePosition();
		if (this->showStory && this->xButton.getGlobalBounds().contains(static_cast<float>(mouse.x), static_cast<float>(mouse.y))) { this->screen->remove(this); }
		else if (!this->showStory && this->xButton.getGlobalBounds().contains(static_cast<float>(mouse.x), static_cast<float>(mouse.y))) 
		{ 
			this->showStory = true; 
			this->resetClock = true;
		}
		else if (this->showStory && this->bookButton.getGlobalBounds().contains(static_cast<float>(mouse.x), static_cast<float>(mouse.y))) { this->showStory = false;  }
	}

	void EveryFrame(uint64_t f)
	{
		currColor = this->tutorial.getColor();
		if (this->internalClock++ < 255)
		{
			currColor.a++;
			this->tutorial.setColor(currColor);
		}

		if (this->resetClock) { internalClock = 0; }
	}

	void draw(sf::RenderWindow& win)
	{
		win.clear();
		if (this->showStory)
		{
			win.draw(this->story);
			win.draw(this->xButton);
			win.draw(this->bookButton);
		}
		else
		{
			win.draw(this->tutorial);
			win.draw(this->xButton);
		}
	}
};

#endif