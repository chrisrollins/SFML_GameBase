#ifndef RespawnManager_h
#define RespawnManager_h

#include "SFML\Graphics.hpp"
#include "GameObject.h"
#include "Screen.h"
#include "Tilemap.h"
#include <ctime>

template <typename T>
class RespawnManager : public Engine::GameObject
{
public:
	RespawnManager(sf::Sprite sprite, T* ptr, int initialNum, int respawnSpeed, static Screen* screen, static TileMap* map)
	{
		this->respawnSpeed = respawnSpeed;
		this->sprite = sprite;
		this->screen_ptr = screen;
		this->map_ptr = map;
		srand(time(0));
		for (int i = 0; i < initialNum; i++)
		{
			float randWidth = rand() % (map->width() * map->tileSize().x);
			float randHeight = rand() % (map->height() * map->tileSize().y);
			sprite.setPosition(randWidth, randHeight);
			ptr = new T(sprite);
			screen->add(ptr);
		}
	}

private:
	int respawnSpeed;
	sf::Sprite sprite;
	Screen* screen_ptr;
	TileMap* map_ptr;
	void EveryFrame(uint64_t frameNumber)
	{
		srand(time(0));
		if (frameNumber % respawnSpeed == 0)
		{
			float randWidth = rand() % (map_ptr->width() * map_ptr->tileSize().x);
			float randHeight = rand() % (map_ptr->height() * map_ptr->tileSize().y);
			sprite.setPosition(randWidth, randHeight);
			T* ptr = new T(sprite);
			screen_ptr->add(ptr);
		}
	}
};
#endif

