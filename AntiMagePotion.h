#ifndef ANTIMAGEPOTION_H
#define ANTIMAGEPOTION_H

#include "GameObject.h"
#include "RespawnManager.h"
#include "Screen.h"
#include "DifficultySettings.h"
#include "Score.h"

template<typename T> class RespawnManager;

class AntiMagePotion : public Engine::GraphicalGameObject
{
private:
	friend class RespawnManager<AntiMagePotion>;
	RespawnManager<AntiMagePotion>* respawnManager = nullptr;
public:

	AntiMagePotion(sf::Sprite s, RespawnManager<AntiMagePotion>* respawnManager) : AntiMagePotion(s)
	{
		this->respawnManager = respawnManager;
	}

	AntiMagePotion(sf::Sprite s) : Engine::GraphicalGameObject(s)
	{}

	void EveryFrame(uint64_t f)
	{}

	void Collision(Engine::GraphicalGameObject& other)
	{
		if (dynamic_cast<ZombieBlast*>(&other))
		{
			this->die();
		}
	}

	void die()
	{
		if (this->respawnManager) { this->respawnManager->died(this); }
		this->screen->remove(this);
	}

	sf::Sprite* spritePtr()
	{
		return dynamic_cast<sf::Sprite*>(this->graphic);
	}
};

#endif
