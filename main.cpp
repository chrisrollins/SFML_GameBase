#include <SFML/Graphics.hpp>
#include <iostream>
#include <thread>
#include "Screen.h"
#include "GameObject.h"

class ExampleClickHandler : public GameObject
{
public:
	void MouseButtonReleased(sf::Event e)
	{
		std::cout << "Mouse clicked at (" << e.mouseButton.x << ", " << e.mouseButton.y << ")" << std::endl;
	}
};

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
private:
	sf::CircleShape* circle()
	{
		return reinterpret_cast<sf::CircleShape*>(this->graphic);
	}
};

int main(int argc, char** argv)
{
	//these are static members that should be set before rendering a screen. the window will be locked at these values after rendering has started.
	Screen::windowWidth = 1024;
	Screen::windowHeight = 768;
	Screen::windowTitle = "Game!";

	//screen object which holds game objects
	Screen startingScreen;

	//a class which derives from GameObject and has an event handler
	ExampleClickHandler clicks;

	//a class which derives from GraphicalGameObject and puts a circle on the screen with an event to move it when the user clicks
	SampleCircle s;

	//add the objects to the screen
	startingScreen.add(s);
	startingScreen.add(clicks);

	//render the screen (replaces whichever screen is currently rendering)
	//also join the thread which was returned. this prevents the program from exiting.
	startingScreen.render()->join();

	return 0;
}