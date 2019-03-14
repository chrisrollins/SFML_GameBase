#ifndef SCORE_H
#define SCORE_H

#include "Screen.h"
#include "DifficultySettings.h"
#include "FileLoadException.h"
#include <iostream>
#include <string>

namespace Engine
{
	class Score : public GraphicalGameObject
	{
	private:
		bool frozen;
		int number = 0;
		sf::Font font;
		sf::Text* text() { return dynamic_cast<sf::Text*>(this->getGraphic()); }
	public:
		Score(sf::Text t) : GraphicalGameObject(t)
		{
			if (this->font.loadFromFile("zombie.ttf"))
			{
				this->text()->setFont(font);
				this->text()->setStyle(sf::Text::Bold);
				this->text()->setOutlineColor({ 179, 45, 0 });
				this->text()->setOutlineThickness(2.f);
				this->text()->setFillColor(sf::Color::Black);
				this->text()->setPosition(830.f, 10.f);
				this->text()->setLetterSpacing(3.f);
				this->text()->setString("Score: 0");
			}
			else
			{
				throw GameException::FontFileLoadException("zombie.ttf");
			}
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
			if (this->frozen) { return; }
			this->number = n;
			this->text()->setString("Score: " + std::to_string(this->number));
		}

		int get() const
		{
			return this->number;
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
	};

	static Score* scorePtr;
}

#endif
