#ifndef HEALTHBAR_H
#define HEALTHBAR_H

#include "MainCharacter.h"

class HealthBar : public GraphicalGameObject
{
private:
	sf::RectangleShape* maxPtr() { return &this->maxHealthBar; } //max health
	sf::RectangleShape* currPtr() { return dynamic_cast<sf::RectangleShape*>(this->getGraphic()); }	//current health
	MainCharacter* character = nullptr;
	sf::RectangleShape maxHealthBar;
	sf::Clock alarmClock;
	bool isAlarming = false;
public:
	HealthBar() : GraphicalGameObject(sf::RectangleShape({ 360.f, 10.f }))
	{
		this->maxPtr()->setSize({ 360.f, 15.f });
		this->maxPtr()->setPosition(50.f, 50.f);
		this->maxPtr()->setFillColor({ 255, 255, 255, 140 });
		this->maxPtr()->setOutlineColor({ 0, 0, 0, 100 });
		this->maxPtr()->setOutlineThickness(3);
		this->currPtr()->setOutlineThickness(3);
		this->currPtr()->setOutlineColor({ 0, 0, 0, 100 });
	}

	void setCharacter(MainCharacter* mc)
	{
		this->character = mc;
		float fHealth = static_cast<float>(this->character->getHealth());
		this->maxPtr()->setSize(sf::Vector2f(fHealth / 600.f, 10.f));
		this->currPtr()->setSize(sf::Vector2f(fHealth / 600, 10.f));
		this->currPtr()->setPosition(50.f, 50.f);
		this->currPtr()->setFillColor(sf::Color(sf::Color::Green));
	}

	void EveryFrame(uint64_t f)
	{
		float fMaxHealth = static_cast<float>(this->character->getMaxHealth());
		float fCurrHealth = static_cast<float>(this->character->getHealth());

		if (this->character->getHealth() > this->character->getMaxHealth()) { this->currPtr()->setSize({ fMaxHealth / 600.f, 10.f }); }
		else { this->currPtr()->setSize({ fCurrHealth / 600.f, 10.f }); }

		if (this->character->getHealth() > (this->character->getMaxHealth() / 2)) { this->currPtr()->setFillColor(sf::Color::Green); }
		else if (this->character->getHealth() > (this->character->getMaxHealth() / 4)) { this->currPtr()->setFillColor(sf::Color::Yellow); }
		else
		{
			if (this->character->getHealth() > 0)
			{
				if (!this->isAlarming)
				{
					this->isAlarming = true;
					this->alarmClock.restart();
					soundPlayer.play(SoundEffect::ID::Alarm, 20.f);
				}
				else if (this->alarmClock.getElapsedTime().asSeconds() > 3.5) { this->isAlarming = false; }
			}
			this->currPtr()->setFillColor(sf::Color::Red);
		}
	}

	void draw(sf::RenderWindow& win)
	{
		this->maxPtr()->setPosition(this->currPtr()->getPosition());
		win.draw(*this->maxPtr());
		win.draw(*this->currPtr());
	}
};

#endif
