#include <iostream>
#include "Driver.hpp"


using namespace Engine;

/*

class SampleCircle : public GraphicalGameObject
{
public:
	SampleCircle() : GraphicalGameObject(sf::CircleShape(50.0f))
	{
		this->circle()->setFillColor(sf::Color::Blue);
	}
	void MouseButtonReleased(sf::Event e)
	{
		this->circle()->setPosition(e.mouseButton.x, e.mouseButton.y);
	}
	sf::CircleShape* circle()
	{
		return dynamic_cast<sf::CircleShape*>(this->graphic);
	}
};

class SampleSquare : public GraphicalGameObject
{
public:
	SampleSquare() : GraphicalGameObject(sf::RectangleShape(sf::Vector2f(10, 10)))
	{
		this->square()->setFillColor(sf::Color::Green);
	}
	sf::RectangleShape* square()
	{
		return dynamic_cast<sf::RectangleShape*>(this->graphic);
	}
};




class ExampleClickHandler : public GameObject
{
public:
    void MouseButtonReleased(sf::Event e)
    {
        std::cout << "Mouse clicked at (" << e.mouseButton.x << ", " << e.mouseButton.y << ")" << std::endl;
        SampleSquare* s = new SampleSquare(); //use a heap allocated object which has to be cleaned up later.
        objs.push_back(s);
        s->square()->setPosition(e.mouseButton.x, e.mouseButton.y);
        startingScreen.add(s);
    }
private:
    std::vector<GraphicalGameObject*> objs;
};
*/

int main(int argc, char** argv)
{
    Driver d;
    
	return 0;
}
