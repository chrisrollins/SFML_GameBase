#ifndef HealthBar_h
#define HealthBar_h

#include "MainCharacter.h"

class HealthBar : public GraphicalGameObject
{
public:
	HealthBar() : GraphicalGameObject(sf::RectangleShape(sf::Vector2f(360, 10)))
	{
		this->maxPtr()->setSize(sf::Vector2f(360, 15));
		this->maxPtr()->setPosition(50, 50);
		this->maxPtr()->setFillColor(sf::Color(255, 255, 255, 140)); //the last number in the sf::Color constructor is alpha (transparency). if it is less than 255 the object will be transparent.
		this->maxPtr()->setOutlineThickness(3);
		this->maxPtr()->setOutlineColor(sf::Color(0, 0, 0, 100));

	}

	void setCharacter(MainCharacter * mc) {
		this->character = mc;

		this->currPtr()->setSize(sf::Vector2f(this->character->getHealth() / 600, 10));
		this->currPtr()->setPosition(50, 50);
		this->currPtr()->setFillColor(sf::Color(sf::Color::Green)); //the last number in the sf::Color constructor is alpha (transparency). if it is less than 255 the object will be transparent.


	}
	void EveryFrame(uint64_t f) {
		if (this->character->getHealth() > this->character->getMaxHealth())
			this->currPtr()->setSize(sf::Vector2f(this->character->getMaxHealth() / 600, 10));
		else
			this->currPtr()->setSize(sf::Vector2f(this->character->getHealth() / 600, 10));

		if (this->character->getHealth() > 15 * 60)
			this->currPtr()->setFillColor(sf::Color(sf::Color::Green)); //the last number in the sf::Color constructor is alpha (transparency). if it is less than 255 the object will be transparent.
		else
			this->currPtr()->setFillColor(sf::Color(sf::Color::Red));
	}


private:
	//max health
	sf::RectangleShape* maxPtr() { return dynamic_cast<sf::RectangleShape*>(this->getGraphic()); }
	//current health
	sf::RectangleShape* currPtr() { return dynamic_cast<sf::RectangleShape*>(this->getGraphic()); }
	MainCharacter * character = nullptr;
};

#endif /* HealthBar_h */
