#ifndef POTIONUI_H
#define POTIONUI_H

#include "GameObject.h"
#include "MainCharacter.h"
#include <string>

using namespace Engine;
using namespace std;

class PotionUI : public GraphicalGameObject
{
public:
	PotionUI(sf::Sprite s) : GraphicalGameObject(s)
	{
		potion()->setPosition(50.f, 70.f);
		potion()->setColor(sf::Color(230, 230, 0));
		font.loadFromFile("zombie.ttf");
		text.setFont(font);
		text.Bold;
		text.setOutlineColor(sf::Color(179, 45, 0));
		text.setOutlineThickness(2.f);
		text.setFillColor(sf::Color::Black);
	}

	void setCharacter(MainCharacter * mc) 
	{
		this->character = mc;
	}

	void EveryFrame(uint64_t f) 
	{
		string currNum = to_string(character->getPotionNum());
		text.setString(currNum);
	}

	void draw(sf::RenderWindow& win)
	{
		win.draw(*potion());
		text.setPosition(potion()->getPosition().x + 50, potion()->getPosition().y);
		win.draw(text);
	}

private:
	sf::Sprite* potion() { return dynamic_cast<sf::Sprite*>(this->getGraphic()); }
	MainCharacter* character = nullptr;
	sf::Font font;
	sf::Text text;
};

#endif
