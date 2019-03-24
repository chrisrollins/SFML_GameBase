#ifndef TIMERUI_H
#define TIMERUI_H

#include "MainCharacter.h"
#include "FileLoadException.h"
#include "ResourceManager.h"
#include <string>
#include <sstream>
#include <iomanip>

using namespace Engine;

class TimerUI : public GraphicalGameObject
{
private:
	sf::Text* text() { return dynamic_cast<sf::Text*>(this->getGraphic()); }
	MainCharacter* character = nullptr;
	int minute;
	int second;
public:
	TimerUI(sf::Text t) : GraphicalGameObject(t)
	{
		sf::Font* fontPtr = ResourceManager<sf::Font>::GetResource("zombie.ttf");
		this->text()->setFont(*fontPtr);
		this->text()->setStyle(sf::Text::Bold);
		this->text()->setOutlineColor(sf::Color(179, 45, 0));
		this->text()->setOutlineThickness(2.f);
		this->text()->setFillColor(sf::Color::Black);
		this->text()->setPosition(830.f, 50.f);
		this->text()->setLetterSpacing(3.f);
		this->text()->setString("Time: 00:00");
		minute = 0;
		second = 0;
	}

	void setCharacter(MainCharacter * mc)
	{
		this->character = mc;
	}

	void EveryFrame(uint64_t f)
	{
		std::ostringstream sout;
		sout << "Time: ";
		if (character->isAlive())
		{
			minute = static_cast<int>(character->getCurrAliveTime() / 60);
			second = static_cast<int>(character->getCurrAliveTime() - minute * 60);
			sout << std::setw(2) << std::setfill('0') << std::right << minute << ":" << std::setw(2) <<
				std::setprecision(2) << std::setfill('0') << second;
			this->text()->setString(sout.str());
		}
	}
};

#endif
