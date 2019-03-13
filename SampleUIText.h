#ifndef SAMPLE_UI_TEXT_HEADER
#define SAMPLE_UI_TEXT_HEADER

#include "GameObject.h"

using namespace Engine;

class SampleUIText : public GraphicalGameObject
{
public:
	SampleUIText(std::string text) : GraphicalGameObject(sf::Text())
	{
		if (myFont.loadFromFile("zombie.ttf"))
		{
			this->textPtr()->setFont(myFont);
			this->textPtr()->setString(text);
			this->textPtr()->setPosition(0, 0);
		}
	}
	SampleUIText(std::string text, const sf::Color &C, unsigned int size,
		float position_x, float position_y) : GraphicalGameObject(sf::Text()) {
		if (myFont.loadFromFile("zombie.ttf"))
		{
			this->textPtr()->setFont(myFont);
			this->textPtr()->setOutlineThickness(2.f);
			this->textPtr()->Bold;
			this->textPtr()->setOutlineColor(sf::Color(179, 45, 0));
			this->textPtr()->setLetterSpacing(3.f);
			this->textPtr()->setString(text);
			this->textPtr()->setPosition(position_x, position_y);
			this->textPtr()->setFillColor(C);
			this->textPtr()->setCharacterSize(size);
		}

	}

	SampleUIText(std::string filename, std::string text, const sf::Color &C, unsigned int size,
		float position_x, float position_y) : GraphicalGameObject(sf::Text()) {
		if (myFont.loadFromFile(filename))
		{
			this->textPtr()->setFont(myFont);
			this->textPtr()->setString(text);
			this->textPtr()->setPosition(position_x, position_y);
			this->textPtr()->setFillColor(C);
			this->textPtr()->setCharacterSize(size);
		}

	}

	void setText(std::string text) {
		this->textPtr()->setString(text);
	}
	void setPosition(float position_x, float position_y) {
		this->textPtr()->setPosition(position_x, position_y);
	}
protected:
	sf::Font myFont;
	sf::Text* textPtr() { return dynamic_cast<sf::Text*>(this->getGraphic()); }
};

#endif
