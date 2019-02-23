#include <SFML/Graphics.hpp>
#include <iostream>
#include <thread>
#include <vector>
#include <cassert>
#include "Screen.h"
#include "GameObject.h"

using namespace Engine;

//screen object which holds game objects
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

class SampleMainCharacter : public GraphicalGameObject
{
	bool W_KeyHeld = false;
	bool A_KeyHeld = false;
	bool S_KeyHeld = false;
	bool D_KeyHeld = false;
	std::vector<GraphicalGameObject*> objs;
public:
	SampleMainCharacter(sf::RectangleShape r) : GraphicalGameObject(r) { }
	void KeyPressed(sf::Event e)
	{
		switch (e.key.code)
		{
		case sf::Keyboard::W:
			this->W_KeyHeld = true;
			break;
		case sf::Keyboard::A:
			this->A_KeyHeld = true;
			break;
		case sf::Keyboard::S:
			this->S_KeyHeld = true;
			break;
		case sf::Keyboard::D:
			this->D_KeyHeld = true;
			break;
		default:
                	break;
		}
	}
	void KeyReleased(sf::Event e)
	{
		switch (e.key.code)
		{
		case sf::Keyboard::W:
			this->W_KeyHeld = false;
			break;
		case sf::Keyboard::A:
			this->A_KeyHeld = false;
			break;
		case sf::Keyboard::S:
			this->S_KeyHeld = false;
			break;
		case sf::Keyboard::D:
			this->D_KeyHeld = false;
			break;
		default:
			break;
		}
	}
	void MouseButtonReleased(sf::Event e)
	{
		sf::Vector2f position = this->placeholder()->getPosition();
		std::cout << "Mouse clicked at (" << e.mouseButton.x << ", " << e.mouseButton.y << ")" << std::endl;
		std::cout << "character is at (" << position.x << ", " << position.y << ")" << std::endl;
		int adjustedX = position.x + e.mouseButton.x - (startingScreen.windowWidth / 2);
		int adjustedY = position.y + e.mouseButton.y - (startingScreen.windowHeight / 2);
		SampleSquare* s = new SampleSquare(); //use a heap allocated object which has to be cleaned up later.
		objs.push_back(s);
		s->square()->setPosition(adjustedX, adjustedY);
		startingScreen.add(s);
	}
	void EveryFrame(uint64_t f)
	{
		sf::RectangleShape* r = this->placeholder();
		if (this->W_KeyHeld) { r->move(0, -10); }
		if (this->A_KeyHeld) { r->move(-10, 0); }
		if (this->S_KeyHeld) { r->move(0, 10); }
		if (this->D_KeyHeld) { r->move(10, 0); }
	}
	sf::RectangleShape* placeholder()
	{
		return dynamic_cast<sf::RectangleShape*>(this->graphic);
	}
};

int main(int argc, char** argv)
{
	//these are static members that should be set before rendering a screen. the window will be locked at these values after rendering has started.
	Screen::windowWidth = 1024;
	Screen::windowHeight = 768;
	Screen::windowTitle = "Game!";

	// declare a tilemap object
	TileMap map;
	// build map using different tiles
	const int level[] =
	{
		0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
		0, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 2, 0, 0, 0, 0,
		1, 1, 0, 0, 0, 0, 0, 0, 3, 3, 3, 3, 3, 3, 3, 3,
		0, 1, 0, 0, 2, 0, 3, 3, 3, 0, 1, 1, 1, 0, 0, 0,
		0, 1, 1, 0, 3, 3, 3, 0, 0, 0, 1, 1, 1, 2, 0, 0,
		0, 0, 1, 0, 3, 0, 2, 2, 0, 0, 1, 1, 1, 1, 2, 0,
		2, 0, 1, 0, 3, 0, 2, 2, 2, 0, 1, 1, 1, 1, 1, 1,
		0, 0, 1, 0, 3, 2, 2, 2, 0, 0, 0, 0, 1, 1, 1, 1,
	};
	// 32,32 means size of each tile
	// 16 means how many tiles per line
	// 8 means how many tiles per column
	// 16 & 8 are related to the level
	assert(map.load("map.png", sf::Vector2u(32, 32), level, 16, 8));

	//a class which derives from GraphicalGameObject and puts a circle on the screen with an event to move it when the user clicks
	SampleCircle s;

	//main character sample
	SampleMainCharacter c(sf::RectangleShape(sf::Vector2f(100, 100)));

	//add map to the screen
	startingScreen.addMap(&map);
	//add the objects to the screen
	startingScreen.add(&s);
	startingScreen.addMainCharacter(&c);

	//note: The lifetime of the objects added to the screen must be as long as the screen's lifetime. In this case it's ok to use these local variables because this function waits for the thread returned by the screen.
	//For most other cases we'll probably have to use heap allocated objects.

	//render the screen (replaces whichever screen is currently rendering)
	//also join the thread which was returned. this prevents the program from exiting.
	startingScreen.render();

	return 0;
}
