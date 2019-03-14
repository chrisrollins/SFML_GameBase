#ifndef CITIZEN_H
#define CITIZEN_H

#include "Screen.h"
#include "RespawnManager.h"
#include "ZombieBlast.h"
#include "DifficultySettings.h"
#include "Score.h"

using namespace Engine;

template<typename T> class RespawnManager;

class Citizen : public GraphicalGameObject
{
private:
	friend class RespawnManager<Citizen>;
	bool wKeyHeld = false;
	bool aKeyHeld = false;
	bool sKeyHeld = false;
	bool dKeyHeld = false;
	sf::Vector2u textureSize;
	sf::Vector2u imageCount;
	sf::Vector2u currentImage;
	RespawnManager<Citizen>* respawnManager = nullptr;
public:
	Citizen(sf::Sprite s, RespawnManager<Citizen>* respawnManager) : Citizen(s)
	{
		this->respawnManager = respawnManager;
	}

	Citizen(sf::Sprite s) : GraphicalGameObject(s)
	{
		this->textureSize = this->spritePtr()->getTexture()->getSize();
		this->textureSize.x /= 3;
		this->textureSize.y /= 4;
		this->imageCount.x = 0;
		switch (rand() % 4)
		{
		case 0:
			this->wKeyHeld = true;
			this->aKeyHeld = false;
			this->sKeyHeld = false;
			this->dKeyHeld = false;
			break;
		case 1:
			this->aKeyHeld = true;
			this->wKeyHeld = false;
			this->sKeyHeld = false;
			this->dKeyHeld = false;
			break;
		case 2:
			this->sKeyHeld = true;
			this->aKeyHeld = false;
			this->wKeyHeld = false;
			this->dKeyHeld = false;
			break;
		case 3:
			this->dKeyHeld = true;
			this->aKeyHeld = false;
			this->sKeyHeld = false;
			this->wKeyHeld = false;
			break;
		default:
			break;
		}
	}

	void EveryFrame(uint64_t f)
	{
		if (f % 120 == 0)
		{
			switch (rand() % 4)
			{
			case 0:
				this->wKeyHeld = true;
				this->aKeyHeld = false;
				this->sKeyHeld = false;
				this->dKeyHeld = false;
				break;
			case 1:
				this->aKeyHeld = true;
				this->wKeyHeld = false;
				this->sKeyHeld = false;
				this->dKeyHeld = false;
				break;
			case 2:
				this->sKeyHeld = true;
				this->aKeyHeld = false;
				this->wKeyHeld = false;
				this->dKeyHeld = false;
				break;
			case 3:
				this->dKeyHeld = true;
				this->aKeyHeld = false;
				this->sKeyHeld = false;
				this->wKeyHeld = false;
				break;
			default:
				break;
			}
		}

		if (this->wKeyHeld)
		{
			this->imageCount.y = 3;
			this->spritePtr()->move(0, -0.5);
		}
		if (this->aKeyHeld)
		{
			this->imageCount.y = 1;
			this->spritePtr()->move(-0.5, 0);
		}
		if (this->sKeyHeld)
		{
			this->imageCount.y = 0;
			this->spritePtr()->move(0, 0.5);
		}
		if (this->dKeyHeld)
		{
			this->imageCount.y = 2;
			this->spritePtr()->move(0.5, 0);
		}

		if (f % 20 == 0)
		{
			if (this->imageCount.x == 2) { this->imageCount.x = 0; }
			else { this->imageCount.x++; }
		}

		this->spritePtr()->setTextureRect(sf::IntRect(this->imageCount.x * this->textureSize.x,
			this->imageCount.y * this->textureSize.y, this->textureSize.x, this->textureSize.y));
	}

	void Collision(GraphicalGameObject& other)
	{
		if (dynamic_cast<ZombieBlast*>(&other))
		{
			(*scorePtr) += DifficultySettings::Score::applyMultipliers(1);
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
