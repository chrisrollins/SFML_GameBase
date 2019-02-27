#ifndef SAMPLE_UI_TEXT_HEADER
#define SAMPLE_UI_TEXT_HEADER

#include "GameObject.h"

using namespace Engine;

//this class doesn't work yet but this is the general idea

class SampleUIText : public GraphicalGameObject
{
public:
	SampleUIText(std::string text) : GraphicalGameObject(sf::Text())
	{
		if (myFont.loadFromFile("arial.ttf"))
		{			
			this->textPtr()->setFont(myFont);
			this->textPtr()->setString(text);
			this->textPtr()->setPosition(0, 0);
		}
	}
private:
	sf::Font myFont;
	sf::Text* textPtr() { return dynamic_cast<sf::Text*>(this->getGraphic()); }
};

#endif