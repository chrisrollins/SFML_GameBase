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
		font.loadFromFile("zombie.ttf");
		text.setFont(font);
        text.setStyle(sf::Text::Bold);
		text.setLetterSpacing(3.f);
		text.setOutlineColor(sf::Color(163, 19, 88));
		text.setOutlineThickness(2.f);
		text.setFillColor(sf::Color::Black);
	}

	void setCharacter(MainCharacter * mc) 
	{
		this->character = mc;
	}

	void EveryFrame(uint64_t f) 
	{
		string currNum = to_string(character->getPotionNum()) + "/" + to_string(character->getMaxPotionNum());
		text.setString(currNum);
	}

	void draw(sf::RenderWindow& win)
	{
		win.draw(*potion());
		text.setPosition(potion()->getPosition().x + 50, potion()->getPosition().y - 5);
		win.draw(text);
	}

private:
	sf::Sprite* potion() { return dynamic_cast<sf::Sprite*>(this->getGraphic()); }
	MainCharacter* character = nullptr;
	sf::Font font;
	sf::Text text;
};

#endif
