#ifndef RESPAWNMANAGER_H
#define RESPAWNMANAGER_H

#include "Screen.h"
#include "ResourceManager.h"
#include "SpriteFactory.h"
#include <string>
#include <cstdlib>
#include <ctime>
#include <map>
#include <vector>

using namespace Engine;
using std::string;
using std::map;
using std::vector;

template <typename T>
class RespawnManager : public GameObject
{
private:
	map<GameObjectID, T*> characters;
	size_t max;
	uint32_t respawnSpeed;
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
			T* ptr = new T(this->sprite, this);
			this->screen->add(ptr);
			this->characters[ptr->getID()] = ptr;
		}
		else { this->cooldown--; }
	}
public:
	RespawnManager(Sprite::ID spriteID, int max, int respawnSpeed) : max(max), respawnSpeed(respawnSpeed), sprite(SpriteFactory::generateSprite(spriteID)) { }
	
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
