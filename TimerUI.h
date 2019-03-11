#ifndef TIMERUI_H
#define TIMERUI_H

#include "GameObject.h"
#include "MainCharacter.h"
#include <string>
#include <sstream>
#include <iomanip>

using namespace Engine;
using namespace std;

class TimerUI : public GraphicalGameObject
{
public:
	TimerUI(sf::Text t): GraphicalGameObject(t)
	{
		font.loadFromFile("zombie.ttf");
		this->text()->setFont(font);
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
		ostringstream sout;
		sout << "Time: ";
		if (character->isAlive())
		{
			minute = static_cast<int>(character->getCurrAliveTime() / 60);
			second = static_cast<int>(character->getCurrAliveTime() - minute * 60);
			sout << setw(2) << setfill('0') << right << minute << ":" << setw(2) <<
				setprecision(2) << setfill('0') << second;
			this->text()->setString(sout.str());
		}
	}

private:
	sf::Text* text() { return dynamic_cast<sf::Text*>(this->getGraphic()); }
	MainCharacter* character = nullptr;
	sf::Font font;
	int minute;
	int second;
};

#endif
