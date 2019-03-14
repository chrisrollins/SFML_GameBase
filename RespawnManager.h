#ifndef RESPAWNMANAGER_H
#define RESPAWNMANAGER_H

#include "Screen.h"
#include <ctime>
#include <map>
#include <vector>

using namespace Engine;

template <typename T>
class RespawnManager : public GameObject
{
private:
	std::map<GameObjectID, T*> characters;
	size_t max;
	int respawnSpeed;
	int cooldown = 0;
	sf::Sprite sprite;
	void EveryFrame(uint64_t frameNumber)
	{
		if (this->characters.size() >= this->max) { return; } //don't spawn if at max
		srand(static_cast<int>(frameNumber * this->getID())); // use frameNumber and ID as seed
		if (this->cooldown == 0)
		{
			this->cooldown = this->respawnSpeed + ((rand() % 120) - 60); //randomize respawn rate +/- 1 second
			if (this->cooldown <= 10) { this->cooldown = 10; }
			const TileMap* map = this->screen->getMap();
			std::vector<sf::Vector2f> spawnPositions = map->getSafeSpawnPositions();
			size_t randIndex = rand() % spawnPositions.size();
			sf::Vector2f position = spawnPositions[randIndex];
			this->sprite.setPosition(position);
			this->add(this->sprite);
		}
		else { this->cooldown--; }
	}

	void add(sf::Sprite& sprite)
	{
		T* ptr = new T(sprite, this);
		this->screen->add(ptr);
		this->characters[ptr->getID()] = ptr;
	}
public:

	RespawnManager(sf::Sprite& sprite, int max, int respawnSpeed)
	{
		this->max = max;
		this->respawnSpeed = respawnSpeed;
		this->sprite = sprite;
	}

	void died(T* character)
	{
		this->characters.erase(character->getID());
	}

	void clear()
	{
		for (auto ch : this->characters) { this->screen->remove(ch.second); }
		this->characters.clear();
	}
};

#endif
