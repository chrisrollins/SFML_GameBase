#ifndef SCORE_H
#define SCORE_H

#include <iostream>
#include <string>
#include "Screen.h"
#include "GameObject.h"
#include "DifficultySettings.h"

namespace Engine
{
	class Score : public GraphicalGameObject 
	{
	public:
		Score(sf::Text t) : GraphicalGameObject(t) 
		{
			font.loadFromFile("zombie.ttf");
			this->text()->setFont(font);
			this->text()->Bold;
			this->text()->setOutlineColor(sf::Color(179, 45, 0));
			this->text()->setOutlineThickness(2.f);
			this->text()->setFillColor(sf::Color::Black);
			this->text()->setPosition(830.f, 10.f);
			this->text()->setLetterSpacing(3.f);
			this->text()->setString("Score: 0");
		}
		int operator++()
		{
			*this += 1;
		}
		int operator--()
		{
			*this -= 1;
		}
		int operator+=(const int& n)
		{
			this->set(this->number + n);
			return this->number;
		}
		int operator-=(const int& n)
		{
			this->set(this->number - n);
			return this->number;
		}
		int operator=(const int& n)
		{
			this->set(n);
			return this->number;
		}
		void set(int n)
		{
			this->number = n;
			this->text()->setString("Score: " + std::to_string(number));
		}
		void EveryFrame(uint64_t f) {
			int n = number, count = 0;
			while (n) {
				count++;
				n /= 10;
			}
			if (f % 60 == 0)
			{
				DifficultySettings::Score::baseMultiplier += DifficultySettings::Score::multiplierPerSecond;
				if (DifficultySettings::Score::baseMultiplier < 0.5f) { DifficultySettings::Score::baseMultiplier = 0.5f; }
			}
		}
	private:
		int number = 0;
		sf::Font font;
		sf::Text* text() { return dynamic_cast<sf::Text*>(this->getGraphic()); }
	};

	static Score* scorePtr;
}

#endif
