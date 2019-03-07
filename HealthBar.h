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
		this->currPtr()->setOutlineThickness(3);
		this->currPtr()->setOutlineColor(sf::Color(0, 0, 0, 100));

	}

	void setCharacter(MainCharacter * mc) {
		this->character = mc;

		this->maxPtr()->setSize(sf::Vector2f(this->character->getHealth() / 600, 10));
		this->currPtr()->setSize(sf::Vector2f(this->character->getHealth() / 600, 10));
		this->currPtr()->setPosition(50, 50);
		this->currPtr()->setFillColor(sf::Color(sf::Color::Green));


	}
	void EveryFrame(uint64_t f) {
		if (this->character->getHealth() > this->character->getMaxHealth())
			this->currPtr()->setSize(sf::Vector2f(this->character->getMaxHealth() / 600, 10));
		else
			this->currPtr()->setSize(sf::Vector2f(this->character->getHealth() / 600, 10));

		if (this->character->getHealth() > 15 * 60)
			this->currPtr()->setFillColor(sf::Color(sf::Color::Green));
		else
			this->currPtr()->setFillColor(sf::Color(sf::Color::Red));
	}
	void draw(sf::RenderWindow& win)
	{
		this->maxPtr()->setPosition(this->currPtr()->getPosition());
		win.draw(*this->maxPtr());
		win.draw(*this->currPtr());
	}

private:
	//max health
	sf::RectangleShape* maxPtr() { return &this->maxHealthBar; }
	//current health
	sf::RectangleShape* currPtr() { return dynamic_cast<sf::RectangleShape*>(this->getGraphic()); }
	MainCharacter * character = nullptr;
	sf::RectangleShape maxHealthBar;
};

#endif /* HealthBar_h */
