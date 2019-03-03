#ifndef RespawnManager_h
#define RespawnManager_h

#include "SFML\Graphics.hpp"
#include "GameObject.h"
#include "Screen.h"
#include "Tilemap.h"
#include <ctime>
#include <map>
#include <vector>

template <typename T>
class RespawnManager : public Engine::GameObject
{
public:

	RespawnManager(sf::Sprite& sprite, std::vector<sf::Vector2f> respawnPositions, int max, int respawnSpeed)
	{
		this->max = max;
		this->respawnSpeed = respawnSpeed;
		this->sprite = sprite;
		this->respawnPositions = respawnPositions;
	}

	void died(T* character)
	{
		this->characters.erase(character->getID());
	}

private:
	std::vector<sf::Vector2f> respawnPositions;
	std::map<GameObjectID, T*> characters;
	int max;
	int respawnSpeed;
	int cooldown = 0;
	sf::Sprite sprite;
	void EveryFrame(uint64_t frameNumber)
	{
		if (this->characters.size() >= this->max) { return; } //don't spawn if at max
		srand(time(0));
		if (cooldown == 0)
		{
			cooldown = respawnSpeed;
			const TileMap* map = this->screen->getMap();
			int randPosition = rand() % this->respawnPositions.size();
			sf::Vector2f position = this->respawnPositions[randPosition];
			std::cout << position.x << ", "<< position.y << std::endl;
			sprite.setPosition(position);
			this->add(sprite);
		}
		else
		{
			cooldown--;
		}
	}

	void add(sf::Sprite& sprite)
	{
		T* ptr = new T(sprite);
		this->screen->add(ptr);
		this->characters[ptr->getID()] = ptr;
	}
};
#endif

