#ifndef SCORE_H
#define SCORE_H

#include "Screen.h"
#include "DifficultySettings.h"
#include "FileLoadException.h"
#include "ResourceManager.h"
#include <iostream>
#include <string>

namespace Engine
{
	class Score : public GraphicalGameObject
	{
	private:
		bool frozen;
		int value = 0;
		sf::Text* text() { return dynamic_cast<sf::Text*>(this->getGraphic()); }
	public:
		Score() : GraphicalGameObject(sf::Text())
		{
			sf::Font* fontPtr = ResourceManager<sf::Font>::GetResource("zombie.ttf");
			this->text()->setFont(*fontPtr);
			this->text()->setStyle(sf::Text::Bold);
			this->text()->setOutlineColor({ 179, 45, 0 });
			this->text()->setOutlineThickness(2.f);
			this->text()->setFillColor(sf::Color::Black);
			this->text()->setPosition(830.f, 10.f);
			this->text()->setLetterSpacing(3.f);
			this->text()->setString("Score: 0");			
		}

		void operator++()
		{
			*this += 1;
		}

		void operator--()
		{
			*this -= 1;
		}

		int operator+=(const int& n)
		{
			this->set(this->value + n);
			return this->value;
		}

		int operator-=(const int& n)
		{
			this->set(this->value - n);
			return this->value;
		}

		int operator=(const int& n)
		{
			this->set(n);
			return this->value;
		}

		void set(int n)
		{
			if (this->frozen) { return; }
			this->value = n;
			this->text()->setString("Score: " + std::to_string(this->value));
		}

		int get() const
		{
			return this->value;
		}

		void freeze()
		{
			this->frozen = true;
		}

		void unfreeze()
		{
			this->frozen = false;
		}

		void EveryFrame(uint64_t f) {
			int n = value, count = 0;
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
	};

	static Score* scorePtr;
}

#endif
