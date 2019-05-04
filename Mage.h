#ifndef MAGE_H
#define MAGE_H

#include "Screen.h"
#include "RespawnManager.h"
#include "ZombieBlast.h"
#include "MageBlast.h"
#include "DifficultySettings.h"
#include "Score.h"
#include "SoundPlayer.h"
#include "VisualEffect.h"
#include <unordered_set>
#include <functional>

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
	int deathCount = 0;
	std::unordered_set<GameObjectID> blastsHitBy;
	bool isShooting = false;;
	uint64_t internalClock = 0;
	int bulletCooldown = 0;
	MageHealthBar* healthBar = nullptr;
	RespawnManager<Mage>* respawnManager = nullptr;
	int attackCooldown;
protected:
	sf::Vector2f calculateSoundPosition()
	{
		sf::Sprite* playerSprite = dynamic_cast<sf::Sprite*>(this->screen->getMainCharacter()->getGraphic());
		return SoundPlayer::calculateSoundPosition(playerSprite->getPosition(), this->getDrawablePtr()->getPosition());
	}
public:
	Mage(sf::Sprite s, RespawnManager<Mage>* respawnManager) : Mage(s)
	{
		this->respawnManager = respawnManager;
	}

	Mage(sf::Sprite s, TimeUnit::Time attackCooldown = TimeUnit::Frames(100u)) :
		GraphicalGameObject(s),
		Health(3 + DifficultySettings::Mage::mageHealthModifier),
		SpriteSheet(4, 12),
		attackCooldown(static_cast<int>(attackCooldown))
	{
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

	virtual void Attack()
	{
		sf::Vector2f pos = this->getDrawablePtr()->getPosition();
		sf::Vector2f playerPos = dynamic_cast<sf::Transformable*>(dynamic_cast<GraphicalGameObject*>(this->screen->getMainCharacter())->getGraphic())->getPosition();
		MageBlast* blast = (new MageBlast(SpriteFactory::generateSprite(Sprite::ID::Mageblast)))
			->position(pos)
			->destination(playerPos)
			->speed(1.6)
			->duration(TimeUnit::Frames(135))
			->damage(275 + DifficultySettings::Mage::attackDamageModifier);
		this->screen->add(blast);
		SoundPlayer::play(SoundEffect::ID::MageAttack2, 10.f, SoundPlayer::calculateSoundPosition(playerPos, pos));
	}

	void AddedToScreen()
	{
		this->healthBar = new MageHealthBar();
		healthBar->setMaxHealth(this->getHealth());
		sf::Vector2f pos = this->getDrawablePtr()->getPosition();
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
			sf::Vector2f healthBarPos = this->getDrawablePtr()->getPosition();
			healthBarPos.y -= 10.f;
			healthBarPos.x += 5.f;
			this->healthBar->setPosition(healthBarPos);

			if (this->internalClock % 120 == 0)
			{
				GraphicalGameObject* player = dynamic_cast<GraphicalGameObject*>(this->screen->getMainCharacter());
				sf::Vector2f playerPosition = dynamic_cast<sf::Transformable*>(player->getGraphic())->getPosition();
				sf::Vector2f myPosition = this->getDrawablePtr()->getPosition();
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
				if (!this->isShooting) { this->spriteSheetColumn = 3; }
				this->move(Degrees(270.f), speed);
				if (this->internalClock % this->attackCooldown == 0 && !this->isShooting)
				{
					this->spriteSheetColumn = 7;
					this->isShooting = true;
					this->bulletCooldown = 0;
				}
			}
			else if (this->movingLeft)
			{
				if (!this->isShooting) { this->spriteSheetColumn= 1; }
				this->move(Degrees(180.f), speed);
				if (this->internalClock % this->attackCooldown == 0 && !this->isShooting)
				{
					this->spriteSheetColumn = 5;
					this->isShooting = true;
					this->bulletCooldown = 0;
				}
			}
			else if (this->movingDown)
			{
				if (!this->isShooting) { this->spriteSheetColumn = 0; }
				this->move(Degrees(90.f), speed);
				if (this->internalClock % this->attackCooldown == 0 && !this->isShooting)
				{
					this->spriteSheetColumn = 4;
					this->isShooting = true;
					this->bulletCooldown = 0;
				}
			}
			else if (this->movingRight)
			{
				if (!this->isShooting) { this->spriteSheetColumn = 2; }
				this->move(Degrees(0.f), speed);
				if (this->internalClock % this->attackCooldown == 0 && !this->isShooting)
				{
					this->spriteSheetColumn = 6;
					this->isShooting = true;
					this->bulletCooldown = 0;
				}
			}

			if (this->internalClock % this->attackCooldown == 0) { this->Attack(); }

			// shooting delay
			this->bulletCooldown++;
			if (this->bulletCooldown == 50) { this->isShooting = false; }
			if (this->internalClock % 15 == 0) { this->spriteSheetRow++; }
		}
		else
		{
			if (this->movingUp) { this->spriteSheetColumn = 11; }
			if (this->movingLeft) { this->spriteSheetColumn = 9; }
			if (this->movingDown) { this->spriteSheetColumn = 8; }
			if (this->movingRight) { this->spriteSheetColumn = 10; }
			this->spriteSheetRow = this->deathCount;
			if (this->internalClock % 30 == 0) { this->deathCount++; }
			if (this->deathCount == 3) { this->screen->remove(this); }
		}
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
		SoundPlayer::play(SoundEffect::ID::MageDeath, 30.f, this->calculateSoundPosition());
		this->screen->remove(this->healthBar);
		numMagesAlive--;
		if (this->respawnManager) { this->respawnManager->died(this); }
	}
};

class StrongMage : public Mage
{
public:
	StrongMage(sf::Sprite s) : Mage(s, TimeUnit::Seconds(5))
	{
		this->setMaxHealth(10 + this->getMaxHealth() * 3);
		this->heal(this->getMaxHealth());
	}

	void Attack()
	{
		const TimeUnit::Seconds chargeTime(3.2);
		SoundPlayer::play(SoundEffect::ID::StrongMageCharge, 20.f, this->calculateSoundPosition());
		VisualEffect* chargingEffect = new VisualEffect(SpriteFactory::generateSprite(Sprite::ID::MageSuperAttackCharge), chargeTime);
		sf::IntRect size = this->getDrawablePtr()->getTextureRect();
		chargingEffect->setOffset({ static_cast<float>(size.width) * 0.5f, static_cast<float>(size.height) / -2.f });
		const float startingScale = 0.4f;
		chargingEffect->getSprite()->setColor({ 255, 255, 255, 127 });
		chargingEffect->getSprite()->setScale(startingScale, startingScale);
		chargingEffect->attachTo(this);
		chargingEffect->setEveryFrameAdditionalAction([=](VisualEffect* self, uint64_t remainingFrames)
		{
			sf::Sprite* spr = self->getSprite();
			const float growth = (startingScale * -0.7f) / static_cast<float>(chargeTime);
			spr->setScale(spr->getScale() + sf::Vector2f(growth, growth));
			sf::Uint8 alpha = spr->getColor().a;
			if (remainingFrames > 25)
			{
				uint16_t newAlpha = static_cast<uint16_t>(alpha) + 2u;
				spr->setColor({ 255, 255, 255, static_cast<sf::Uint8>( newAlpha <= 255 ? (newAlpha) : 255 ) });
			}
			else { spr->setColor({ 255, 255, 255, static_cast<sf::Uint8>(remainingFrames * 10) }); }
			if (remainingFrames == 0 && this->isAlive())
			{
				sf::Vector2f pos = this->getDrawablePtr()->getPosition();
				sf::Vector2f playerPos = dynamic_cast<sf::Transformable*>(dynamic_cast<GraphicalGameObject*>(this->screen->getMainCharacter())->getGraphic())->getPosition();
				SoundPlayer::play(SoundEffect::ID::StrongMageAttack, 20.f, SoundPlayer::calculateSoundPosition(playerPos, pos));
				MageBlast* blast = (new MageBlast(SpriteFactory::generateSprite(Sprite::ID::Mageblast)))
					->position(pos)
					->destination(playerPos)
					->speed(8.0)
					->duration(TimeUnit::Frames(135))
					->damage(600 + DifficultySettings::Mage::attackDamageModifier * 3)
					->scale(1.9f);
				this->screen->add(blast);
			}
		});
		this->screen->add(chargingEffect);
	}

	int Damaged(int damage)
	{
		if (damage > this->getMaxHealth() / 5) { damage = this->getMaxHealth() / 5; }
		const int shieldDuration = 25;
		VisualEffect* shield = new VisualEffect(SpriteFactory::generateSprite(Sprite::ID::MageShield), TimeUnit::Frames(shieldDuration));
		sf::IntRect size = this->getDrawablePtr()->getTextureRect();
		shield->setOffset({ static_cast<float>(size.width) * 0.75f, static_cast<float>(size.height) / 2.f });
		shield->setPosition(this->getDrawablePtr()->getPosition());
		shield->setEveryFrameAdditionalAction([=](VisualEffect* self, uint64_t remainingFrames)
		{
			sf::Sprite* spr = self->getSprite();
			spr->setColor({255, 255, 255, static_cast<sf::Uint8>(remainingFrames * (255 / shieldDuration)) });
			spr->scale(1.02f, 1.02f);
		});
		this->screen->add(shield);		
		return damage;
	}
};

#endif