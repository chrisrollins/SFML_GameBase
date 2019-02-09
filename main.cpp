#include <SFML/Graphics.hpp>
#include <iostream>
#include <thread>
#include <vector>
#include "Screen.h"
#include "GameObject.h"

Screen startingScreen;

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

int main(int argc, char** argv)
{
	//these are static members that should be set before rendering a screen. the window will be locked at these values after rendering has started.
	Screen::windowWidth = 1024;
	Screen::windowHeight = 768;
	Screen::windowTitle = "Game!";

	//screen object which holds game objects
	//Screen startingScreen;

	//a class which derives from GameObject and has an event handler
	ExampleClickHandler clicks;

	//a class which derives from GraphicalGameObject and puts a circle on the screen with an event to move it when the user clicks
	SampleCircle s;

	//add the objects to the screen
	startingScreen.add(&s);
	startingScreen.add(&clicks);

	//note: The lifetime of the objects added to the screen must be as long as the screen's lifetime. In this case it's ok to use these local variables because this function waits for the thread returned by the screen.
	//For most other cases we'll probably have to use heap allocated objects.

	//render the screen (replaces whichever screen is currently rendering)
	//also join the thread which was returned. this prevents the program from exiting.
	startingScreen.render();

	return 0;
}