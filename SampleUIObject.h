#ifndef SAMPLE_UI_OBJECT_HEADER
#define SAMPLE_UI_OBJECT_HEADER

#include "GameObject.h"

using namespace Engine;

class SampleUIObject : public GraphicalGameObject
{
public:
	SampleUIObject() : GraphicalGameObject(sf::RectangleShape(sf::Vector2f(100, 100)))
	{
		this->rPtr()->setPosition(100, 100);
		this->rPtr()->setFillColor(sf::Color(255, 255, 255, 140)); //the last number in the sf::Color constructor is alpha (transparency). if it is less than 255 the object will be transparent.
	}

private:
	sf::RectangleShape* rPtr() { return dynamic_cast<sf::RectangleShape*>(this->getGraphic()); }
};

#endif // !SAMPLE_UI_OBJECT_HEADER
