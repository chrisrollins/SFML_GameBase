//
//  Score.hpp
//  Project_XCODE
//
//  Created by Jakin Wang on 2/27/19.
//  Copyright © 2019 Jakin Wang. All rights reserved.
//

#ifndef Score_hpp
#define Score_hpp

#include <iostream>
#include <string>
#include "Screen.h"
#include "SampleUIText.h"
#include "DifficultySettings.h"

namespace Engine
{
	class Score : public SampleUIText {

	public:
		Score(std::string text) : SampleUIText(text), number(0) {}
		Score(int s) : number(s), SampleUIText(std::to_string(s)) {}
		Score(std::string text, const sf::Color &C, unsigned int size,
			float position_x, float position_y) : SampleUIText(text, C, size, position_x, position_y) {}
		Score(int s, const sf::Color &C, unsigned int size,
			float position_x, float position_y) : number(s), SampleUIText("Your Score: " + std::to_string(s), C, size, position_x, position_y) {}
		Score() : Score(0, sf::Color::Cyan, 32, 1000, 0) {}
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
			this->setText("Your Score: " + std::to_string(number));
		}
		void EveryFrame(uint64_t f) {
			int n = number, count = 0;
			while (n) {
				count++;
				n /= 10;
			}
			this->setPosition(1000 - (count + 11) * 16, 0);
		}
	private:
		int number = 0;
	};

	static Score* scorePtr;
}

#endif
