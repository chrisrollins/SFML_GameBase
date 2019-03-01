#include <SFML/Graphics.hpp>
#include <iostream>
#include <thread>
#include <vector>
#include <ctime>
#include "Screen.h"
#include "GameObject.h"
#include "Soldier.h"
#include "MainCharacter.h"
#include "SampleUIObject.h"
#include "SampleUIText.h"
#include "getMap.h"
#include "Score.h"

using namespace Engine;

int main(int argc, char** argv)
{
	//these are static members that should be set before rendering a screen. the window will be locked at these values after rendering has started.
	Screen::windowWidth = 1024;
	Screen::windowHeight = 768;
	Screen::windowTitle = "Game!";

	// declare a tilemap object
	TileMap map;
	// build map using different tiles

	int level[64*48];
	getMap("map.dat", 64, 48, level);
	// sf::Vector2u holds the size of each tile
	// the two variables after level stands for the num tiles per column/line
	map.load("map.png", sf::Vector2u(32, 32), level, 64, 48);
	//add map to the screen
	startingScreen.addMap(&map);

	sf::Sprite m_Sprite;
	m_Sprite.setPosition(Screen::windowWidth / 2, Screen::windowHeight / 2);
	sf::Texture m_Texture;
	m_Texture.loadFromFile("skeleton.png");
	// m_Texture.setSmooth(true);
	m_Sprite.setTexture(m_Texture);
	MainCharacter* char_ptr = new MainCharacter(m_Sprite);
	startingScreen.addMainCharacter(char_ptr);

	srand(time(0));
	sf::Texture soldier_texture;
	soldier_texture.loadFromFile("soldier.png");
	sf::Sprite* soldierSprite = new sf::Sprite[20];
	for (int i = 0; i < 20; i++)
	{
		soldierSprite[i].setTexture(soldier_texture);
		float randWidth = rand() % (map.width() * map.tileSize().x);
		float randHeight = rand() % (map.height() * map.tileSize().y);
		Soldier* soldier_ptr = new Soldier(soldierSprite[i], sf::Vector2f(randWidth, randHeight));
		startingScreen.add(soldier_ptr);
	}
    
    Score s(0, sf::Color::White, 32, 1000, 0);
	startingScreen.addUIObject(&s);

	//note: The lifetime of the objects added to the screen must be as long as the screen's lifetime. In this case it's ok to use these local variables because startingScreen lasts the duration of the program.
	//For most other cases we have to use heap allocated objects.

	//render the screen (replaces whichever screen is currently rendering)
	startingScreen.render();

	return 0;
}
