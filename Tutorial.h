#ifndef TUTORIAL_H
#define TUTORIAL_H

#include "GameObject.h"
#include "Screen.h"

using namespace Engine;

class Tutorial : public GraphicalGameObject
{
public:
	Tutorial() : GraphicalGameObject(sf::Sprite()) 
	{
		this->storyTexture.loadFromFile("story.png");
		this->story.setTexture(storyTexture);
		this->tutorialTexture.loadFromFile("tutorial.png");
		this->tutorial.setTexture(tutorialTexture);
		this->bookButtonTexture.loadFromFile("book_button.png");
		this->bookButton.setTexture(bookButtonTexture);
		this->bookButton.setPosition(Engine::Screen::windowWidth - 280.f, Engine::Screen::windowHeight - 260.f);
		this->xButtonTexture.loadFromFile("x_button.png");
		this->xButton.setTexture(this->xButtonTexture);
		this->xButton.setPosition(Engine::Screen::windowWidth - 55.f, 15.f);
		showStory = true;
	}
	void MouseButtonReleased(sf::Event e)
	{
		if (e.mouseButton.button != sf::Mouse::Button::Left) { return; }
		sf::Vector2i mouse = this->screen->getMousePosition();
		if (showStory && this->xButton.getGlobalBounds().contains(static_cast<float>(mouse.x), static_cast<float>(mouse.y)))
		{
			this->screen->remove(this);
		}
		else if (!showStory && this->xButton.getGlobalBounds().contains(static_cast<float>(mouse.x), static_cast<float>(mouse.y)))
		{
			showStory = true;
		}
		else if (showStory && this->bookButton.getGlobalBounds().contains(static_cast<float>(mouse.x), static_cast<float>(mouse.y)))
		{
			showStory = false;
		}
	}
	void draw(sf::RenderWindow& win)
	{
		win.clear();
		if (showStory)
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
private:
	sf::Texture storyTexture;
	sf::Sprite story;
	sf::Texture tutorialTexture;
	sf::Sprite tutorial;
	sf::Texture bookButtonTexture;
	sf::Sprite bookButton;
	sf::Texture xButtonTexture;
	sf::Sprite xButton;
	bool showStory;
};
#endif