#ifndef MAGE_H
#define MAGE_H

#include "Screen.h"
#include "RespawnManager.h"
#include "ZombieBlast.h"
#include "MageBlast.h"
#include "DifficultySettings.h"
#include "Score.h"
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
		this->blockingCollision = false;
		this->ignoreObstacles = true;
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
		this->rectPtr()->setSize({37.f * health / this->maxHealth, 5.f});
	}
};

class Mage : public GraphicalGameObject
{
private:
	friend class RespawnManager<Mage>;
	bool W_KeyHeld = false;
	bool A_KeyHeld = false;
	bool S_KeyHeld = false;
	bool D_KeyHeld = false;
	bool alive = true;
	int deathCount = 0;
	int health = 3 + DifficultySettings::Mage::mageHealthModifier;
	std::unordered_set<GameObjectID> blastsHitBy;
	bool isShooting = false;;
	uint64_t internalClock = 0;
	int bullet_cooldown = 0;
	sf::Vector2u textureSize;
	sf::Vector2u imageCount;
	sf::Vector2u currentImage;
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

	Mage(sf::Sprite s) : GraphicalGameObject(s)
	{
		this->textureSize = this->spritePtr()->getTexture()->getSize();
		this->textureSize.x /= 4;
		this->textureSize.y /= 12;
		this->imageCount.x = 0;

		this->W_KeyHeld = false;
		this->A_KeyHeld = false;
		this->S_KeyHeld = false;
		this->D_KeyHeld = false;

		srand(static_cast<unsigned int>(time(0) * this->getID()));
		switch (rand() % 4)
		{
		case 0:
			this->W_KeyHeld = true;
			break;
		case 1:
			this->A_KeyHeld = true;
			break;
		case 2:
			this->S_KeyHeld = true;
			break;
		case 3:
			this->D_KeyHeld = true;
			break;
		default:
			break;
		}

		numMagesAlive++;
	}

	void AddedToScreen()
	{
		this->healthBar = new MageHealthBar();
		healthBar->setMaxHealth(this->health);
		sf::Vector2f pos = this->spritePtr()->getPosition();
		pos.y -= 10.f;
		pos.x += 5.f;
		this->healthBar->setCurrHealth(this->health);
		this->healthBar->setPosition(pos);
		this->screen->add(this->healthBar);
	}

	void EveryFrame(uint64_t f)
	{
		this->internalClock++;
		srand(static_cast<unsigned int>(time(0) * this->getID()));
		if (this->alive)
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
				//float a = (playerPosition.x - myPosition.x);
				//float b = (playerPosition.y - myPosition.y);

				this->W_KeyHeld = false;
				this->A_KeyHeld = false;
				this->S_KeyHeld = false;
				this->D_KeyHeld = false;

				int choice = rand() % 2;
				if (choice == 0)
				{
					if (xDirection == DIRECTION::RIGHT) { this->D_KeyHeld = true; }
					else { this->A_KeyHeld = true; }
				}
				else
				{
					if (yDirection == DIRECTION::DOWN) { this->S_KeyHeld = true; }
					else { this->W_KeyHeld = true; }
				}
			}

			if (this->W_KeyHeld)
			{
				if (!this->isShooting) { this->imageCount.y = 3; }
				this->spritePtr()->move(0, -0.5);
				if (this->internalClock % 100 == 0 && !this->isShooting)
				{
					this->imageCount.y = 7;
					this->isShooting = true;
					this->bullet_cooldown = 0;
				}
			}

			if (this->A_KeyHeld)
			{
				if (!this->isShooting) { this->imageCount.y = 1; }
				this->spritePtr()->move(-0.5, 0);
				if (this->internalClock % 100 == 0 && !this->isShooting)
				{
					this->imageCount.y = 5;
					this->isShooting = true;
					this->bullet_cooldown = 0;
				}
			}

			if (this->S_KeyHeld)
			{
				if (!this->isShooting) { this->imageCount.y = 0; }
				this->spritePtr()->move(0, 0.5);
				if (this->internalClock % 100 == 0 && !this->isShooting)
				{
					this->imageCount.y = 4;
					this->isShooting = true;
					this->bullet_cooldown = 0;
				}
			}

			if (this->D_KeyHeld)
			{
				if (!this->isShooting) { this->imageCount.y = 2; }
				this->spritePtr()->move(0.5, 0);
				if (this->internalClock % 100 == 0 && !this->isShooting)
				{
					this->imageCount.y = 6;
					this->isShooting = true;
					this->bullet_cooldown = 0;
				}
			}

			if (this->internalClock % 100 == 0)
			{
				sf::Vector2f pos = this->spritePtr()->getPosition();
				sf::Vector2f playerPos = dynamic_cast<sf::Transformable*>(dynamic_cast<GraphicalGameObject*>(this->screen->getMainCharacter())->getGraphic())->getPosition();
				MageBlast* blast = new MageBlast(pos, playerPos, 2.4 + static_cast<double>(DifficultySettings::Mage::blastSpeedModifier), 130);
				this->screen->add(blast);
			}

			// shooting delay
			this->bullet_cooldown++;
			if (this->bullet_cooldown == 50) { this->isShooting = false; }

			if (this->internalClock % 15 == 0)
			{
				if (this->imageCount.x == 3) { this->imageCount.x = 0; }
				else { this->imageCount.x++; }
			}
		}
		else
		{
			if (this->W_KeyHeld) { this->imageCount.y = 11; }
			if (this->A_KeyHeld) { this->imageCount.y = 9; }
			if (this->S_KeyHeld) { this->imageCount.y = 8; }
			if (this->D_KeyHeld) { this->imageCount.y = 10; }
			this->imageCount.x = this->deathCount;
			if (this->internalClock % 30 == 0) { this->deathCount++; }
			if (this->deathCount == 3) { this->screen->remove(this); }
		}
		this->spritePtr()->setTextureRect(sf::IntRect(this->imageCount.x * this->textureSize.x,
			this->imageCount.y * this->textureSize.y, this->textureSize.x, this->textureSize.y));
	}

	void Collision(GraphicalGameObject& other)
	{
		if (this->alive)
		{
			if (ZombieBlast* blast = dynamic_cast<ZombieBlast*>(&other))
			{
				if (this->blastsHitBy.find(blast->getID()) != this->blastsHitBy.end()) { return; }
				this->blastsHitBy.insert(blast->getID());
				this->health -= blast->getDamage();
				this->healthBar->setCurrHealth(this->health);
				if (this->health > 0) { return; }
				if (this->respawnManager) { this->respawnManager->died(this); }
				this->alive = false;
				numMagesAlive--;
				DifficultySettings::Score::cumulativeBonusMultiplierCurrent = fmin(DifficultySettings::Score::cumulativeBonusMultiplierMax, DifficultySettings::Score::cumulativeBonusMultiplierCurrent + DifficultySettings::Score::cumulativeBonusMultiplier);
				(*scorePtr) += DifficultySettings::Score::applyMultipliers(20);
				soundPlayer.play(SoundEffect::ID::MageDeath, 30.f);
				this->screen->remove(this->healthBar);
			}
		}
	}

	bool isAlive() const
	{
		return this->alive;
	}

	int getHealth() const
	{
		return this->health;
	}
};

#endif
