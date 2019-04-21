#ifndef MAGE_H
#define MAGE_H

#include "Screen.h"
#include "RespawnManager.h"
#include "ZombieBlast.h"
#include "MageBlast.h"
#include "DifficultySettings.h"
#include "Score.h"
#include "SoundPlayer.h"
#include <unordered_set>

using namespace Engine;

template<typename T> class RespawnManager;

static int numMagesAlive;

class MageHealthBar : public GraphicalGameObject
{
private:
	int maxHealth;
	sf::RectangleShape* rectPtr() { return dynamic_cast<sf::RectangleShape*>(this->graphic); }
public:
	MageHealthBar() : GraphicalGameObject(sf::RectangleShape())
	{
		sf::RectangleShape* rect = this->rectPtr();
		rect->setFillColor({ 50, 255, 50, 140 });
		rect->setSize({ 37.f, 5.f });
	}

	void setPosition(sf::Vector2f pos)
	{
		this->rectPtr()->setPosition(pos);
	}

	void setMaxHealth(int maxHealth)
	{
		this->maxHealth = maxHealth;
	}

	void setCurrHealth(int health)
	{
		if (health < 0) { health = 0; }
		this->rectPtr()->setSize({ 37.f * (static_cast<float>(health) / static_cast<float>(this->maxHealth)), 5.f });
	}
};

class Mage : StandardEnemy, SpriteSheet
{
private:
	friend class RespawnManager<Mage>;
	bool movingUp = false;
	bool movingLeft = false;
	bool movingDown = false;
	bool movingRight = false;
	//bool alive = true;
	int deathCount = 0;
	//int health = 3 + DifficultySettings::Mage::mageHealthModifier;
	std::unordered_set<GameObjectID> blastsHitBy;
	bool isShooting = false;;
	uint64_t internalClock = 0;
	int bulletCooldown = 0;
	//sf::Vector2u textureSize;
	//sf::Vector2u imageCount;
	//sf::Vector2u currentImage;
	MageHealthBar* healthBar = nullptr;
	RespawnManager<Mage>* respawnManager = nullptr;
	sf::Sprite* spritePtr()
	{
		return dynamic_cast<sf::Sprite*>(this->graphic);
	}
public:
	Mage(sf::Sprite s, RespawnManager<Mage>* respawnManager) : Mage(s)
	{
		this->respawnManager = respawnManager;
	}

	Mage(sf::Sprite s) :
		GraphicalGameObject(s),
		Health(3 + DifficultySettings::Mage::mageHealthModifier),
		SpriteSheet(4, 12)
	{
		/*this->textureSize = this->spritePtr()->getTexture()->getSize();
		this->textureSize.x /= 4;
		this->textureSize.y /= 12;
		this->imageCount.x = 0;
		this->spritePtr()->setTextureRect(sf::IntRect(this->imageCount.x * this->textureSize.x,
			this->imageCount.y * this->textureSize.y, this->textureSize.x, this->textureSize.y));*/

		this->resetSpriteSheet();
		this->movingUp = false;
		this->movingLeft = false;
		this->movingDown = false;
		this->movingRight = false;

		srand(static_cast<unsigned int>(time(0) * this->getID()));
		switch (rand() % 4)
		{
		case 0:
			this->movingUp = true;
			break;
		case 1:
			this->movingLeft = true;
			break;
		case 2:
			this->movingDown = true;
			break;
		case 3:
			this->movingRight = true;
			break;
		default:
			break;
		}

		numMagesAlive++;
	}

	void AddedToScreen()
	{
		this->healthBar = new MageHealthBar();
		healthBar->setMaxHealth(this->getHealth());
		sf::Vector2f pos = this->spritePtr()->getPosition();
		pos.y -= 10.f;
		pos.x += 5.f;
		this->healthBar->setCurrHealth(this->getHealth());
		this->healthBar->setPosition(pos);
		this->screen->add(this->healthBar);
	}

	void EveryFrame(uint64_t f)
	{
		this->internalClock++;
		srand(static_cast<unsigned int>(time(0) * this->getID()));
		if (this->isAlive())
		{
			sf::Vector2f healthBarPos = this->spritePtr()->getPosition();
			healthBarPos.y -= 10.f;
			healthBarPos.x += 5.f;
			this->healthBar->setPosition(healthBarPos);

			if (this->internalClock % 120 == 0)
			{
				GraphicalGameObject* player = dynamic_cast<GraphicalGameObject*>(this->screen->getMainCharacter());
				sf::Vector2f playerPosition = dynamic_cast<sf::Transformable*>(player->getGraphic())->getPosition();
				sf::Vector2f myPosition = this->spritePtr()->getPosition();
				DIRECTION xDirection = (playerPosition.x > myPosition.x) ? DIRECTION::RIGHT : DIRECTION::LEFT;
				DIRECTION yDirection = (playerPosition.y > myPosition.y) ? DIRECTION::DOWN : DIRECTION::UP;

				this->movingUp = false;
				this->movingLeft = false;
				this->movingDown = false;
				this->movingRight = false;

				int choice = rand() % 2;
				if (choice == 0)
				{
					if (xDirection == DIRECTION::RIGHT) { this->movingRight = true; }
					else { this->movingLeft = true; }
				}
				else
				{
					if (yDirection == DIRECTION::DOWN) { this->movingDown = true; }
					else { this->movingUp = true; }
				}
			}

			float speed = 0.5f + DifficultySettings::Mage::movementSpeedModifier;
			if (this->movingUp)
			{
				//if (!this->isShooting) { this->imageCount.y = 3; }
				if (!this->isShooting) { this->spriteSheetColumn = 3; }
				this->move(Degrees(270.f), speed);
				if (this->internalClock % 100 == 0 && !this->isShooting)
				{
					//this->imageCount.y = 7;
					this->spriteSheetColumn = 7;
					this->isShooting = true;
					this->bulletCooldown = 0;
				}
			}
			else if (this->movingLeft)
			{
				//if (!this->isShooting) { this->imageCount.y = 1; }
				if (!this->isShooting) { this->spriteSheetColumn= 1; }
				this->move(Degrees(180.f), speed);
				if (this->internalClock % 100 == 0 && !this->isShooting)
				{
					//this->imageCount.y = 5;
					this->spriteSheetColumn = 5;
					this->isShooting = true;
					this->bulletCooldown = 0;
				}
			}
			else if (this->movingDown)
			{
				//if (!this->isShooting) { this->imageCount.y = 0; }
				if (!this->isShooting) { this->spriteSheetColumn = 0; }
				this->move(Degrees(90.f), speed);
				if (this->internalClock % 100 == 0 && !this->isShooting)
				{
					//this->imageCount.y = 4;
					this->spriteSheetColumn = 4;
					this->isShooting = true;
					this->bulletCooldown = 0;
				}
			}
			else if (this->movingRight)
			{
				//if (!this->isShooting) { this->imageCount.y = 2; }
				if (!this->isShooting) { this->spriteSheetColumn = 2; }
				this->move(Degrees(0.f), speed);
				if (this->internalClock % 100 == 0 && !this->isShooting)
				{
					//this->imageCount.y = 6;
					this->spriteSheetColumn = 6;
					this->isShooting = true;
					this->bulletCooldown = 0;
				}
			}

			if (this->internalClock % 100 == 0)
			{
				sf::Vector2f pos = this->spritePtr()->getPosition();
				sf::Vector2f playerPos = dynamic_cast<sf::Transformable*>(dynamic_cast<GraphicalGameObject*>(this->screen->getMainCharacter())->getGraphic())->getPosition();
				MageBlast* blast = new MageBlast(pos, playerPos, 1.5 + static_cast<double>(DifficultySettings::Mage::blastSpeedModifier), 135);
				this->screen->add(blast);
			}

			// shooting delay
			this->bulletCooldown++;
			if (this->bulletCooldown == 50) { this->isShooting = false; }

			if (this->internalClock % 15 == 0)
			{
				//if (this->imageCount.x == 3) { this->imageCount.x = 0; }
				//else { this->imageCount.x++; }
				this->spriteSheetRow++;
			}
		}
		else
		{
			/*if (this->movingUp) { this->imageCount.y = 11; }
			if (this->movingLeft) { this->imageCount.y = 9; }
			if (this->movingDown) { this->imageCount.y = 8; }
			if (this->movingRight) { this->imageCount.y = 10; }
			this->imageCount.x = this->deathCount;*/

			if (this->movingUp) { this->spriteSheetColumn = 11; }
			if (this->movingLeft) { this->spriteSheetColumn = 9; }
			if (this->movingDown) { this->spriteSheetColumn = 8; }
			if (this->movingRight) { this->spriteSheetColumn = 10; }
			this->spriteSheetRow = this->deathCount;
			if (this->internalClock % 30 == 0) { this->deathCount++; }
			if (this->deathCount == 3) { this->screen->remove(this); }
		}
		//this->spritePtr()->setTextureRect(sf::IntRect(this->imageCount.x * this->textureSize.x, this->imageCount.y * this->textureSize.y, this->textureSize.x, this->textureSize.y));
	}

	void Collided(Collision* other)
	{
		if (this->isAlive())
		{
			if (ZombieBlast* blast = dynamic_cast<ZombieBlast*>(other))
			{
				if (this->blastsHitBy.find(blast->getID()) != this->blastsHitBy.end()) { return; }
				this->blastsHitBy.insert(blast->getID());
				this->damage(blast->getDamage());
				this->healthBar->setCurrHealth(this->getHealth());
			}
		}
	}

	void Death()
	{
		DifficultySettings::Score::cumulativeBonusMultiplierCurrent = fmin(DifficultySettings::Score::cumulativeBonusMultiplierMax, DifficultySettings::Score::cumulativeBonusMultiplierCurrent + DifficultySettings::Score::cumulativeBonusMultiplier);
		(*scorePtr) += DifficultySettings::Score::applyMultipliers(20);
		SoundPlayer::play(SoundEffect::ID::MageDeath, 30.f);
		this->screen->remove(this->healthBar);
		numMagesAlive--;
		if (this->respawnManager) { this->respawnManager->died(this); }
	}
};

#endif
