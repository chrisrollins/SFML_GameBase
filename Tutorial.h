#ifndef TUTORIAL_H
#define TUTORIAL_H

#include "GameObject.h"
#include "Screen.h"
#include "Menu.h"
#include "FileLoadException.h"
#include "SpriteFactory.h"

using namespace Engine;

class Tutorial : public GraphicalGameObject
{
private:
	sf::Sprite story;
	sf::Sprite tutorial;
	sf::Sprite bookButton;
	sf::Sprite xButton;
	sf::Color currColor;
	int internalClock = 0;
	bool resetClock = false;
	bool showStory = true;
public:
	Tutorial()
		: GraphicalGameObject(sf::Sprite()),
		story(SpriteFactory::generateSprite(Sprite::ID::Story)),
		tutorial(SpriteFactory::generateSprite(Sprite::ID::Tutorial)),
		bookButton(SpriteFactory::generateSprite(Sprite::ID::BookButton)),
		xButton(SpriteFactory::generateSprite(Sprite::ID::XButton))
	{
		this->bookButton.setPosition(Screen::windowWidth - 280.f, Screen::windowHeight - 260.f);
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
		else if (this->showStory && this->bookButton.getGlobalBounds().contains(static_cast<float>(mouse.x), static_cast<float>(mouse.y))) { this->showStory = false; }
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