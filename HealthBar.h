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
		isAlarming = false;
	}

	void setCharacter(MainCharacter * mc) {
		this->character = mc;
		float fHealth = static_cast<float>(this->character->getHealth());
		this->maxPtr()->setSize(sf::Vector2f(fHealth / 600.f, 10.f));
		this->currPtr()->setSize(sf::Vector2f(fHealth / 600, 10.f));
		this->currPtr()->setPosition(50.f, 50.f);
		this->currPtr()->setFillColor(sf::Color(sf::Color::Green));
	}

	void EveryFrame(uint64_t f) {
		float fMaxHealth = static_cast<float>(this->character->getMaxHealth());
		float fCurrHealth = static_cast<float>(this->character->getHealth());
		if (this->character->getHealth() > this->character->getMaxHealth())
		{
			this->currPtr()->setSize(sf::Vector2f(fMaxHealth / 600.f, 10.f));
		}
		else
		{
			this->currPtr()->setSize(sf::Vector2f(fCurrHealth / 600.f, 10.f));
		}

		if (this->character->getHealth() > (this->character->getMaxHealth() / 2))
		{
			this->currPtr()->setFillColor(sf::Color(sf::Color::Green));
		}
		else if (this->character->getHealth() > (this->character->getMaxHealth() / 4))
		{
			this->currPtr()->setFillColor(sf::Color(sf::Color::Yellow));
		}
		else
		{
			if (character->getHealth() > 0)
			{
				if (!isAlarming)
				{
					isAlarming = true;
					alarmClock.restart();
					Engine::soundPlayer.play(SoundEffect::ID::Alarm, 20.f);
				}
				else if (alarmClock.getElapsedTime().asSeconds() > 3.5)
				{
					isAlarming = false;
				}
			}
			this->currPtr()->setFillColor(sf::Color(sf::Color::Red));
		}
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
	sf::Clock alarmClock;
	bool isAlarming;
};

#endif /* HealthBar_h */
