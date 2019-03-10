#ifndef UI_BUTTON_HEADER
#define UI_BUTTON_HEADER

#include "GameObject.h"
#include <iostream>

using namespace Engine;

class UIButton : public GraphicalGameObject
{
public:
	UIButton(std::string text, sf::Vector2f position, sf::Vector2f size) : GraphicalGameObject(sf::Text())
	{
		if (myFont.loadFromFile("arial.ttf"))
		{
			this->textPtr()->setFont(myFont);
			this->textPtr()->setString(text);
		}
		this->background.setFillColor(sf::Color(70, 70, 70));
		this->background.setSize(size);
		this->setPosition(position);
	}
	void setFont(std::string font)
	{
		myFont.loadFromFile(font);
		this->textPtr()->setFont(myFont);
	}
	void setPosition(sf::Vector2f position)
	{
		this->position = position;
		this->background.setPosition(position);
		if (this->textPtr())
		{
			sf::Vector2f textPositionAdjust = this->background.getSize();
			sf::FloatRect textSize = this->textPtr()->getLocalBounds();
			textPositionAdjust.x = (textPositionAdjust.x / 2) - (textSize.width / 2);
			textPositionAdjust.y = (textPositionAdjust.y / 2) - (textSize.height);
			this->textPtr()->setPosition(position + textPositionAdjust);
		}
	}
	void setTextSize(float size)
	{
		this->textPtr()->setCharacterSize(static_cast<unsigned int>(size));
	}
	void setTextColor(sf::Color color)
	{
		this->textPtr()->setFillColor(color);
	}
	void setBackgroundColor(sf::Color color)
	{
		this->background.setFillColor(color);
	}
	void draw(sf::RenderWindow& win)
	{
		win.draw(this->background);
		win.draw(*this->graphic);
	}
protected:
	sf::Vector2f position;
	sf::Font myFont;
	sf::RectangleShape background;
	sf::Text* textPtr() { return dynamic_cast<sf::Text*>(this->getGraphic()); }
};

#endif