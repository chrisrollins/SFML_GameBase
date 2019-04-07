#ifndef CITIZEN_H
#define CITIZEN_H

#include "Screen.h"
#include "RespawnManager.h"
#include "ZombieBlast.h"
#include "DifficultySettings.h"
#include "Score.h"

using namespace Engine;

template<typename T> class RespawnManager;

class Citizen : StandardEnemy
{
private:
	friend class RespawnManager<Citizen>;
	bool movingUp = false;
	bool movingLeft = false;
	bool movingDown = false;
	bool movingRight = false;
	sf::Vector2u textureSize;
	sf::Vector2u imageCount;
	sf::Vector2u currentImage;
	RespawnManager<Citizen>* respawnManager = nullptr;
public:
	Citizen(sf::Sprite s, RespawnManager<Citizen>* respawnManager) : Citizen(s)
	{
		this->respawnManager = respawnManager;
	}

	Citizen(sf::Sprite s) :
		GraphicalGameObject(s),
		Health(1)
	{
		this->textureSize = this->spritePtr()->getTexture()->getSize();
		this->textureSize.x /= 3;
		this->textureSize.y /= 4;
		this->imageCount.x = 0;
		switch (rand() % 4)
		{
		case 0:
			this->movingUp = true;
			this->movingLeft = false;
			this->movingDown = false;
			this->movingRight = false;
			break;
		case 1:
			this->movingLeft = true;
			this->movingUp = false;
			this->movingDown = false;
			this->movingRight = false;
			break;
		case 2:
			this->movingDown = true;
			this->movingLeft = false;
			this->movingUp = false;
			this->movingRight = false;
			break;
		case 3:
			this->movingRight = true;
			this->movingLeft = false;
			this->movingDown = false;
			this->movingUp = false;
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
				this->movingUp = true;
				this->movingLeft = false;
				this->movingDown = false;
				this->movingRight = false;
				break;
			case 1:
				this->movingLeft = true;
				this->movingUp = false;
				this->movingDown = false;
				this->movingRight = false;
				break;
			case 2:
				this->movingDown = true;
				this->movingLeft = false;
				this->movingUp = false;
				this->movingRight = false;
				break;
			case 3:
				this->movingRight = true;
				this->movingLeft = false;
				this->movingDown = false;
				this->movingUp = false;
				break;
			default:
				break;
			}
		}

		float speed = 0.3f + DifficultySettings::Citizen::movementSpeedModifier;
		if (this->movingUp)
		{
			this->imageCount.y = 3;
			this->move(Degrees(270.f), speed);
		}
		else if (this->movingLeft)
		{
			this->imageCount.y = 1;
			this->move(Degrees(180.f), speed);
		}
		else if (this->movingDown)
		{
			this->imageCount.y = 0;
			this->move(Degrees(90.f), speed);
		}
		else if (this->movingRight)
		{
			this->imageCount.y = 2;
			this->move(Degrees(0.f), speed);
		}

		if (f % 20 == 0)
		{
			if (this->imageCount.x == 2) { this->imageCount.x = 0; }
			else { this->imageCount.x++; }
		}

		this->spritePtr()->setTextureRect(sf::IntRect(this->imageCount.x * this->textureSize.x,
			this->imageCount.y * this->textureSize.y, this->textureSize.x, this->textureSize.y));
	}

	void Collided(Collision* other)
	{
		if (dynamic_cast<ZombieBlast*>(other))
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
