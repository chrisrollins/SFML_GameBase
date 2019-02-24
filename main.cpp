#include <SFML/Graphics.hpp>
#include <iostream>
#include <thread>
#include <vector>
#include "Screen.h"
#include "GameObject.h"
#include "Soldier.h"

using namespace Engine;

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

class MainCharacter : public GraphicalGameObject
{
	bool W_KeyHeld = false;
	bool A_KeyHeld = false;
	bool S_KeyHeld = false;
	bool D_KeyHeld = false;
	std::vector<GraphicalGameObject*> objs;
public:
	MainCharacter(sf::Sprite s) : GraphicalGameObject(s) { }
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
		sf::Vector2f position = this->sprite()->getPosition();
		//commented out print statements for now, they hurt performance
		//std::cout << "Mouse clicked at (" << e.mouseButton.x << ", " << e.mouseButton.y << ")" << std::endl;
		//std::cout << "character is at (" << position.x << ", " << position.y << ")" << std::endl;
		int adjustedX = position.x + e.mouseButton.x - (startingScreen.windowWidth / 2);
		int adjustedY = position.y + e.mouseButton.y - (startingScreen.windowHeight / 2);
		SampleSquare* s = new SampleSquare(); //use a heap allocated object which has to be cleaned up later.
		objs.push_back(s);
		s->square()->setPosition(adjustedX, adjustedY);
		startingScreen.add(s);
	}
	void EveryFrame(uint64_t f)
	{
		sf::Sprite* s = this->sprite();
		if (this->W_KeyHeld) 
		{ 
			s->move(0, -10);
		}
		if (this->A_KeyHeld) 
		{ 
			s->move(-10, 0);
		}
		if (this->S_KeyHeld) 
		{ 
			s->move(0, 10);
		}
		if (this->D_KeyHeld) 
		{ 
			s->move(10, 0);
		}
	}
	void Collision(GraphicalGameObject& other)
	{
		std::cout << "collision" << std::endl;
		std::cout << " " << std::endl;
		std::cout << " " << std::endl;
	}
	sf::Sprite* sprite()
	{
		return dynamic_cast<sf::Sprite*>(this->graphic);
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


	// This is for quick generation of random tiles
	/*const int ROW = 64;
	const int COLUMN = 48;
	int test[ROW][COLUMN];
	for (int i = 0; i < 64; i++)
	{
		for (int j = 0; j < 48; j++)
		{
			test[i][j] = rand() % 4;
			std::cout << test[i][j] << ", ";
		}
		std::cout << std::endl;
	}*/

	const int level[] =
	{
		1, 3, 2, 0, 1, 0, 2, 2, 2, 0, 1, 1, 1, 3, 1, 3, 3, 2, 3, 0, 3, 0, 2, 1, 0, 2, 1, 0, 2, 3, 3, 2, 3, 2, 1, 0, 3, 3, 3, 2, 3, 3, 2, 1, 1, 0, 1, 3,
		1, 0, 3, 0, 2, 1, 1, 3, 3, 1, 1, 2, 0, 3, 2, 2, 0, 2, 0, 2, 0, 0, 2, 1, 2, 1, 2, 2, 2, 1, 1, 0, 1, 3, 0, 2, 0, 0, 2, 0, 3, 0, 0, 3, 2, 3, 1, 2,
		2, 2, 1, 1, 1, 3, 3, 2, 0, 2, 1, 2, 1, 2, 1, 1, 0, 0, 2, 1, 1, 1, 1, 0, 2, 2, 1, 0, 3, 3, 3, 2, 3, 0, 2, 2, 1, 0, 2, 2, 3, 3, 3, 1, 1, 3, 1, 3,
		1, 1, 1, 2, 1, 0, 2, 2, 2, 2, 1, 0, 0, 3, 2, 3, 2, 3, 0, 1, 0, 3, 3, 1, 2, 2, 3, 0, 2, 0, 0, 1, 0, 3, 0, 0, 1, 2, 1, 3, 2, 2, 0, 0, 3, 3, 0, 1,
		0, 3, 3, 1, 2, 1, 1, 2, 1, 0, 3, 3, 0, 1, 3, 0, 2, 3, 0, 0, 0, 1, 1, 2, 3, 1, 1, 3, 3, 3, 2, 3, 0, 0, 2, 2, 0, 0, 2, 1, 1, 2, 1, 1, 0, 2, 2, 0,
		2, 3, 2, 1, 2, 3, 3, 0, 0, 3, 2, 2, 0, 3, 0, 2, 2, 2, 3, 1, 1, 2, 0, 0, 1, 2, 2, 3, 1, 3, 1, 0, 0, 1, 3, 0, 0, 2, 3, 1, 1, 0, 1, 1, 3, 3, 2, 1,
		2, 1, 0, 2, 1, 1, 0, 0, 1, 2, 1, 1, 0, 1, 0, 0, 1, 3, 2, 1, 1, 0, 1, 0, 2, 1, 3, 3, 3, 0, 3, 1, 1, 1, 2, 1, 1, 3, 1, 0, 2, 1, 0, 2, 3, 2, 2, 1,
		0, 3, 2, 3, 1, 3, 1, 1, 0, 0, 0, 1, 1, 0, 1, 2, 2, 2, 1, 0, 0, 2, 1, 1, 3, 2, 1, 2, 0, 3, 0, 3, 1, 0, 2, 2, 1, 1, 2, 1, 2, 2, 0, 2, 3, 1, 2, 3,
		2, 1, 1, 2, 2, 0, 2, 2, 0, 3, 1, 2, 3, 1, 2, 1, 1, 2, 3, 0, 0, 1, 2, 0, 1, 3, 2, 3, 3, 1, 3, 0, 3, 0, 1, 1, 3, 0, 1, 1, 1, 3, 0, 2, 2, 2, 0, 0,
		3, 0, 1, 2, 2, 2, 3, 1, 3, 0, 0, 3, 1, 2, 0, 2, 3, 0, 3, 1, 0, 2, 2, 2, 3, 2, 2, 1, 3, 0, 2, 0, 3, 0, 0, 3, 0, 2, 2, 1, 2, 3, 0, 3, 3, 2, 0, 1,
		2, 2, 1, 3, 1, 2, 1, 3, 1, 0, 3, 0, 2, 2, 3, 0, 1, 1, 3, 1, 0, 0, 2, 0, 2, 2, 3, 3, 3, 3, 1, 0, 2, 0, 0, 3, 0, 3, 2, 1, 0, 1, 2, 2, 1, 3, 1, 0,
		2, 3, 3, 0, 1, 0, 1, 0, 1, 3, 1, 2, 0, 0, 1, 0, 1, 0, 3, 0, 3, 0, 0, 2, 3, 1, 0, 0, 2, 1, 2, 1, 1, 3, 1, 1, 0, 2, 1, 0, 1, 0, 1, 1, 3, 1, 1, 1,
		3, 0, 0, 3, 1, 2, 0, 2, 2, 0, 1, 1, 0, 3, 3, 3, 3, 3, 3, 0, 3, 2, 3, 2, 2, 3, 1, 3, 3, 1, 1, 2, 2, 2, 1, 3, 2, 2, 3, 1, 1, 2, 1, 2, 0, 1, 0, 1,
		2, 0, 1, 0, 1, 1, 3, 3, 2, 2, 3, 1, 0, 2, 0, 3, 0, 1, 0, 1, 3, 0, 2, 2, 1, 0, 0, 3, 1, 3, 0, 0, 1, 2, 1, 1, 2, 1, 1, 2, 3, 0, 0, 2, 3, 3, 0, 1,
		0, 1, 3, 1, 3, 0, 0, 1, 1, 0, 1, 3, 3, 1, 1, 0, 1, 2, 1, 1, 3, 3, 1, 0, 0, 1, 0, 3, 2, 0, 3, 0, 3, 2, 2, 2, 3, 3, 0, 3, 1, 3, 2, 3, 2, 3, 1, 0,
		0, 2, 3, 1, 3, 0, 2, 0, 2, 1, 3, 3, 3, 2, 2, 3, 0, 0, 3, 0, 1, 3, 1, 0, 2, 2, 1, 2, 3, 1, 1, 0, 1, 0, 1, 0, 2, 2, 1, 3, 0, 0, 0, 3, 2, 3, 1, 3,
		3, 3, 0, 1, 3, 1, 0, 3, 3, 0, 3, 1, 1, 3, 1, 1, 0, 2, 0, 0, 1, 2, 2, 0, 2, 2, 1, 3, 2, 0, 0, 3, 0, 3, 3, 2, 1, 2, 1, 1, 0, 0, 3, 3, 1, 2, 0, 3,
		2, 2, 3, 1, 3, 1, 1, 0, 0, 3, 0, 2, 3, 1, 2, 0, 0, 0, 2, 2, 1, 1, 0, 1, 2, 0, 3, 1, 1, 0, 0, 1, 2, 2, 0, 0, 0, 2, 2, 3, 3, 3, 2, 1, 1, 3, 1, 2,
		2, 2, 2, 2, 1, 1, 0, 0, 0, 2, 1, 2, 3, 2, 3, 0, 0, 0, 1, 1, 1, 0, 0, 0, 2, 2, 2, 3, 1, 0, 3, 0, 3, 1, 2, 1, 1, 0, 2, 1, 2, 0, 2, 3, 3, 1, 2, 3,
		0, 3, 3, 3, 1, 1, 0, 0, 1, 1, 2, 1, 2, 2, 1, 2, 1, 2, 0, 1, 3, 1, 3, 1, 1, 1, 2, 0, 3, 0, 1, 3, 3, 3, 2, 3, 0, 0, 2, 2, 1, 2, 0, 2, 0, 3, 3, 1,
		3, 3, 2, 2, 1, 0, 1, 0, 0, 1, 1, 0, 1, 2, 3, 3, 0, 0, 2, 0, 3, 1, 2, 3, 1, 3, 1, 3, 0, 1, 0, 1, 2, 2, 1, 1, 2, 0, 0, 1, 3, 0, 3, 0, 1, 1, 2, 0,
		0, 2, 2, 3, 1, 0, 0, 2, 1, 2, 1, 0, 1, 3, 2, 1, 0, 1, 1, 2, 3, 3, 2, 0, 2, 2, 3, 3, 1, 0, 0, 2, 2, 1, 0, 0, 2, 1, 0, 0, 2, 1, 0, 0, 3, 0, 3, 2,
		3, 2, 2, 0, 3, 0, 1, 3, 2, 3, 2, 2, 3, 2, 2, 0, 2, 1, 2, 2, 2, 0, 1, 1, 3, 2, 2, 3, 2, 2, 0, 0, 2, 3, 0, 0, 2, 0, 3, 3, 2, 3, 0, 1, 2, 1, 0, 3,
		0, 2, 1, 1, 2, 1, 0, 0, 0, 3, 2, 3, 3, 0, 1, 2, 2, 2, 2, 2, 0, 1, 2, 3, 3, 3, 1, 3, 3, 0, 2, 2, 1, 0, 1, 2, 1, 0, 1, 1, 2, 3, 2, 1, 1, 2, 1, 0,
		1, 2, 1, 0, 2, 0, 3, 1, 3, 1, 1, 0, 1, 3, 0, 2, 0, 0, 1, 1, 0, 1, 0, 2, 0, 2, 0, 0, 1, 0, 2, 3, 3, 3, 1, 1, 3, 1, 1, 1, 0, 2, 3, 1, 0, 2, 1, 2,
		1, 0, 3, 1, 1, 1, 0, 0, 2, 3, 0, 2, 2, 2, 2, 3, 2, 0, 2, 3, 3, 0, 2, 0, 2, 3, 0, 2, 0, 1, 2, 3, 1, 1, 1, 3, 2, 2, 3, 2, 1, 0, 0, 3, 0, 0, 2, 3,
		0, 3, 0, 3, 3, 2, 0, 2, 3, 2, 1, 2, 2, 0, 3, 0, 3, 0, 0, 2, 3, 1, 3, 2, 3, 0, 3, 0, 3, 2, 2, 0, 0, 3, 2, 0, 2, 1, 0, 2, 1, 1, 3, 2, 1, 0, 0, 3,
		0, 3, 0, 3, 2, 3, 1, 0, 3, 3, 0, 1, 0, 1, 2, 1, 3, 1, 1, 2, 0, 3, 2, 1, 0, 2, 1, 1, 1, 3, 1, 1, 2, 0, 0, 2, 1, 2, 1, 1, 2, 2, 0, 1, 2, 3, 1, 3,
		3, 3, 1, 3, 2, 2, 2, 2, 2, 3, 2, 0, 3, 2, 1, 0, 3, 2, 2, 2, 0, 0, 2, 1, 2, 3, 3, 1, 2, 1, 1, 2, 2, 2, 2, 1, 3, 0, 3, 2, 1, 3, 1, 3, 2, 2, 1, 0,
		2, 0, 0, 0, 1, 0, 2, 3, 2, 3, 1, 0, 2, 0, 0, 1, 0, 2, 0, 0, 3, 0, 3, 0, 0, 1, 3, 1, 3, 1, 1, 0, 0, 0, 3, 3, 2, 1, 3, 0, 2, 2, 1, 3, 0, 2, 3, 0,
		0, 3, 3, 1, 3, 2, 0, 1, 0, 3, 0, 2, 0, 1, 1, 1, 0, 1, 3, 0, 2, 1, 2, 3, 3, 1, 1, 1, 0, 1, 2, 0, 2, 2, 2, 3, 2, 1, 3, 1, 0, 0, 0, 0, 3, 0, 1, 3,
		2, 3, 0, 2, 2, 3, 3, 1, 0, 2, 3, 3, 0, 0, 0, 1, 2, 1, 2, 0, 0, 3, 0, 3, 0, 0, 2, 0, 3, 0, 0, 1, 2, 2, 1, 2, 1, 3, 2, 1, 1, 3, 0, 0, 2, 2, 3, 2,
		1, 2, 3, 1, 1, 3, 2, 0, 1, 3, 0, 1, 1, 0, 0, 3, 1, 2, 0, 3, 3, 1, 2, 0, 3, 2, 0, 2, 1, 1, 1, 0, 2, 3, 1, 0, 2, 1, 3, 3, 1, 1, 0, 0, 0, 3, 3, 2,
		2, 3, 3, 2, 0, 1, 1, 1, 1, 1, 2, 2, 2, 0, 3, 3, 1, 2, 3, 1, 2, 1, 1, 1, 2, 2, 3, 0, 0, 3, 2, 2, 2, 1, 3, 3, 1, 3, 1, 2, 3, 3, 0, 2, 0, 3, 1, 2,
		2, 2, 3, 0, 2, 0, 1, 1, 0, 3, 1, 3, 2, 3, 1, 2, 0, 1, 0, 1, 1, 3, 3, 0, 2, 3, 0, 2, 3, 2, 2, 1, 2, 3, 3, 0, 1, 0, 3, 0, 2, 2, 2, 1, 1, 3, 3, 3,
		0, 1, 1, 1, 1, 2, 2, 1, 1, 3, 3, 2, 3, 1, 0, 1, 1, 0, 0, 1, 3, 2, 3, 1, 0, 0, 1, 0, 1, 3, 2, 1, 2, 2, 0, 2, 3, 2, 0, 1, 0, 2, 1, 0, 1, 0, 1, 2,
		1, 3, 2, 0, 0, 2, 0, 1, 2, 2, 3, 1, 2, 3, 0, 2, 3, 1, 3, 1, 2, 0, 3, 3, 3, 0, 2, 3, 0, 3, 2, 3, 0, 3, 0, 1, 1, 1, 3, 0, 1, 2, 0, 2, 1, 0, 2, 1,
		2, 0, 3, 1, 3, 1, 1, 0, 3, 1, 1, 1, 1, 2, 3, 1, 1, 0, 0, 3, 1, 1, 0, 1, 0, 2, 0, 3, 1, 0, 0, 0, 3, 1, 0, 3, 0, 3, 0, 0, 3, 1, 3, 3, 3, 1, 0, 3,
		1, 2, 0, 3, 0, 0, 3, 1, 3, 0, 3, 0, 2, 0, 1, 0, 3, 2, 1, 2, 0, 0, 1, 2, 0, 0, 0, 0, 2, 1, 0, 2, 2, 1, 1, 0, 3, 0, 0, 3, 3, 2, 1, 0, 1, 0, 3, 0,
		1, 1, 0, 0, 3, 2, 3, 2, 1, 2, 0, 0, 2, 0, 1, 2, 2, 2, 3, 3, 1, 3, 3, 0, 3, 3, 1, 3, 1, 1, 0, 1, 1, 1, 3, 2, 1, 3, 0, 2, 2, 0, 3, 1, 0, 0, 1, 2,
		1, 3, 2, 3, 2, 0, 3, 0, 3, 2, 0, 2, 3, 1, 1, 2, 3, 1, 3, 2, 3, 2, 2, 3, 3, 0, 1, 0, 3, 3, 1, 2, 2, 1, 1, 1, 3, 3, 1, 2, 3, 1, 2, 1, 1, 2, 2, 0,
		0, 3, 0, 0, 1, 3, 2, 0, 1, 3, 1, 0, 3, 3, 1, 3, 1, 1, 1, 1, 2, 3, 1, 2, 0, 3, 3, 3, 1, 2, 3, 2, 0, 2, 2, 3, 1, 2, 0, 2, 3, 2, 0, 3, 1, 2, 0, 1,
		2, 1, 3, 2, 2, 2, 3, 1, 1, 0, 2, 1, 2, 3, 3, 1, 1, 2, 0, 0, 1, 2, 0, 0, 1, 3, 2, 1, 0, 2, 0, 3, 2, 3, 1, 0, 0, 3, 3, 3, 3, 0, 1, 2, 1, 0, 3, 1,
		1, 3, 0, 3, 0, 1, 1, 0, 0, 0, 3, 3, 0, 2, 3, 2, 0, 1, 3, 1, 2, 2, 0, 3, 1, 3, 2, 3, 1, 1, 3, 1, 3, 1, 0, 3, 3, 3, 0, 2, 3, 3, 3, 1, 1, 2, 2, 2,
		0, 1, 3, 2, 3, 3, 3, 2, 3, 1, 2, 3, 2, 1, 3, 2, 3, 1, 1, 0, 0, 2, 2, 3, 2, 2, 1, 2, 0, 1, 1, 2, 3, 2, 3, 2, 0, 2, 0, 3, 1, 3, 1, 3, 1, 2, 3, 3,
		2, 1, 3, 3, 2, 3, 0, 3, 2, 0, 2, 3, 0, 1, 3, 2, 3, 3, 0, 3, 3, 0, 3, 3, 0, 3, 2, 3, 0, 3, 3, 1, 3, 2, 1, 0, 2, 2, 1, 1, 1, 1, 2, 3, 2, 1, 2, 3,
		3, 2, 2, 1, 2, 3, 0, 2, 3, 2, 3, 2, 2, 0, 2, 3, 3, 0, 1, 2, 2, 3, 0, 2, 2, 1, 1, 3, 1, 1, 0, 1, 0, 2, 1, 1, 1, 0, 2, 3, 2, 1, 2, 1, 0, 2, 1, 2,
		0, 0, 1, 2, 1, 3, 0, 1, 2, 3, 3, 3, 1, 2, 2, 3, 0, 3, 1, 3, 2, 0, 3, 2, 2, 2, 0, 3, 0, 3, 0, 1, 0, 0, 2, 0, 3, 0, 2, 3, 3, 1, 3, 2, 2, 2, 0, 2,
		2, 1, 1, 2, 0, 1, 1, 0, 2, 3, 1, 1, 1, 0, 3, 1, 3, 2, 0, 3, 1, 0, 2, 0, 0, 3, 3, 0, 1, 2, 2, 1, 3, 0, 3, 3, 0, 1, 1, 1, 2, 2, 1, 2, 3, 1, 2, 2,
		3, 1, 3, 3, 2, 1, 0, 3, 2, 3, 1, 3, 1, 2, 3, 1, 1, 0, 1, 1, 3, 2, 2, 1, 3, 1, 3, 3, 3, 3, 0, 3, 2, 0, 2, 1, 2, 1, 0, 0, 3, 2, 2, 0, 2, 3, 1, 2,
		3, 1, 0, 1, 2, 2, 2, 2, 0, 3, 1, 3, 0, 0, 2, 1, 3, 1, 2, 1, 2, 3, 1, 0, 1, 0, 1, 1, 1, 3, 2, 3, 2, 0, 3, 2, 1, 0, 3, 3, 1, 3, 1, 3, 2, 1, 0, 3,
		3, 3, 3, 2, 1, 2, 1, 3, 0, 1, 0, 2, 1, 0, 3, 0, 0, 3, 0, 1, 2, 3, 2, 2, 3, 3, 0, 3, 2, 3, 2, 2, 1, 2, 1, 1, 3, 0, 2, 0, 3, 2, 1, 3, 2, 0, 0, 2,
		0, 3, 1, 2, 3, 1, 3, 3, 1, 2, 2, 2, 3, 1, 0, 2, 1, 2, 0, 1, 0, 0, 1, 0, 2, 1, 2, 3, 1, 1, 0, 1, 2, 1, 3, 0, 3, 1, 2, 3, 3, 0, 2, 1, 1, 2, 2, 2,
		0, 3, 0, 3, 1, 2, 1, 2, 1, 0, 1, 2, 1, 2, 1, 0, 3, 3, 2, 3, 3, 3, 3, 2, 1, 1, 2, 3, 1, 0, 3, 2, 0, 1, 0, 2, 1, 2, 0, 3, 1, 1, 2, 1, 2, 2, 1, 0,
		2, 3, 2, 1, 2, 3, 1, 3, 0, 1, 0, 1, 2, 2, 0, 3, 0, 0, 1, 3, 1, 0, 0, 3, 1, 2, 3, 0, 1, 3, 1, 1, 3, 0, 3, 3, 0, 1, 1, 3, 3, 0, 1, 3, 0, 0, 1, 1,
		1, 0, 3, 2, 0, 0, 2, 1, 1, 2, 2, 2, 1, 0, 1, 2, 3, 1, 0, 0, 1, 1, 0, 1, 3, 3, 1, 0, 0, 2, 2, 3, 1, 1, 2, 2, 2, 2, 2, 2, 1, 0, 1, 0, 2, 1, 0, 3,
		1, 0, 2, 2, 2, 0, 3, 2, 2, 0, 3, 0, 0, 0, 2, 2, 3, 3, 2, 0, 3, 2, 0, 1, 2, 2, 2, 1, 0, 2, 0, 1, 1, 0, 1, 1, 2, 0, 1, 2, 2, 0, 1, 0, 3, 2, 3, 1,
		3, 3, 2, 3, 0, 1, 0, 3, 3, 3, 2, 2, 0, 3, 3, 0, 3, 1, 1, 2, 1, 2, 1, 0, 3, 0, 1, 0, 2, 0, 3, 1, 2, 3, 3, 0, 0, 1, 1, 3, 1, 3, 0, 3, 1, 0, 2, 3,
		0, 3, 2, 1, 0, 3, 2, 1, 3, 2, 0, 3, 3, 2, 3, 2, 3, 1, 1, 2, 3, 0, 0, 2, 3, 2, 2, 3, 0, 1, 1, 3, 3, 2, 3, 1, 2, 1, 1, 0, 0, 0, 2, 2, 1, 0, 3, 3,
		2, 3, 0, 2, 2, 0, 1, 3, 1, 0, 2, 2, 3, 3, 0, 3, 1, 2, 1, 3, 0, 2, 2, 1, 2, 0, 3, 0, 0, 2, 2, 1, 0, 3, 0, 0, 0, 3, 0, 3, 3, 1, 1, 1, 1, 0, 2, 0,
		1, 3, 0, 2, 0, 0, 1, 2, 3, 0, 2, 2, 1, 1, 2, 3, 0, 3, 0, 3, 1, 3, 1, 2, 2, 1, 3, 0, 3, 1, 1, 2, 3, 1, 0, 3, 0, 1, 1, 1, 1, 3, 1, 0, 3, 3, 1, 2,
		2, 1, 1, 3, 2, 2, 3, 3, 1, 1, 0, 2, 3, 3, 3, 0, 3, 1, 1, 1, 3, 0, 0, 2, 3, 0, 2, 1, 2, 2, 0, 1, 3, 0, 0, 2, 2, 3, 3, 3, 1, 3, 3, 0, 0, 0, 1, 2,
		2, 1, 2, 1, 2, 0, 0, 2, 2, 1, 2, 1, 3, 0, 3, 1, 1, 0, 2, 1, 1, 2, 1, 1, 0, 0, 2, 2, 2, 2, 3, 1, 3, 2, 1, 3, 0, 3, 2, 1, 1, 1, 1, 2, 1, 3, 2, 1,
		3, 1, 2, 3, 0, 2, 1, 2, 0, 1, 3, 3, 1, 0, 0, 1, 2, 1, 0, 2, 1, 0, 1, 2, 0, 0, 2, 1, 0, 1, 0, 2, 2, 3, 3, 2, 1, 1, 3, 1, 1, 0, 3, 3, 2, 1, 2, 1,
	};
	// sf::Vector2u holds the size of each tile
	// the two variables after level stands for the num tiles per column/line
	map.load("map.png", sf::Vector2u(32, 32), level, 64, 48);
	//add map to the screen
	startingScreen.addMap(&map);

	sf::Sprite m_Sprite;
	m_Sprite.setPosition(Screen::windowWidth /2, Screen::windowHeight /2);
	sf::Texture m_Texture;
	m_Texture.loadFromFile("zombie.png");
	// m_Texture.setSmooth(true);
	m_Sprite.setTexture(m_Texture);
	MainCharacter* char_ptr = new MainCharacter(m_Sprite);
	startingScreen.addMainCharacter(char_ptr);

	sf::Sprite soldier_1;
	sf::Texture solder_texture;
	solder_texture.loadFromFile("soldier.png");
	//solder_texture.setSmooth(true);
	soldier_1.setTexture(solder_texture);
	Soldier* soldier_ptr = new Soldier(soldier_1);
	startingScreen.add(soldier_ptr);

	//note: The lifetime of the objects added to the screen must be as long as the screen's lifetime. In this case it's ok to use these local variables because this function waits for the thread returned by the screen.
	//For most other cases we'll probably have to use heap allocated objects.

	//render the screen (replaces whichever screen is currently rendering)
	//also join the thread which was returned. this prevents the program from exiting.
	startingScreen.render();

	return 0;
}
