#ifndef MAIN_CHARACTER_HEADER
#define MAIN_CHARACTER_HEADER

#include <cmath>
#include "Score.h"
#include "Screen.h"
#include "GameObject.h"
#include "ZombieBlast.h"
#include "Citizen.h"
#include "Mage.h"
#include "DifficultySettings.h"
#include "MageBlast.h"
#include "AntiMagePotion.h"

using namespace Engine;

class MainCharacter : public GraphicalGameObject
{
	bool W_KeyHeld = false;
	bool A_KeyHeld = false;
	bool S_KeyHeld = false;
	bool D_KeyHeld = false;
	sf::Texture blast_texture;
	sf::Texture super_blast_texture;
	sf::Vector2u textureSize;
	sf::Vector2u imageCount;
	sf::Vector2u currentImage;
	bool meleeAttack;
	int meleeAttackDuration = 20;
	int meleeAttackCounter = 0;
	bool inTrap;
	bool isHurt;
	sf::Clock hurtClock;
	sf::Clock trapClock;
	int deathCount; // to control death animation
	bool startDeath;
	bool isDead; // true when the zombie turns to invisible
	DIRECTION direction;
	int potionNum;
	int maxPotionNum = 5;
	int _health = 30 * 60 * 100;
	int maxHealth = 30 * 60 * 100;
	int healthDrain = 13;
	int additionalDrainPerMage = 1;
	int eatHeal = 5500;
	int eatDrainFreezeCountdown = 0;
	int attackHealthCost = 500;
	int baseSpeed = 3;
	int speed = 3;
	int maxSpeed = 4;
	int speedDecayDelay = 0;
	int speedRestoreDelay = 0;
	int colorRestoreDelay = 0;
	Score score;
public:
	MainCharacter(sf::Sprite s) : GraphicalGameObject(s)
	{
		textureSize = this->sprite()->getTexture()->getSize();
		textureSize.x /= 4;
		textureSize.y /= 12;
		imageCount.x = 0;
		this->sprite()->setTextureRect(sf::IntRect(imageCount.x * textureSize.x,
			imageCount.y * textureSize.y, textureSize.x, textureSize.y));
		blast_texture.loadFromFile("blast.png");
		super_blast_texture.loadFromFile("super_zombie_blast.png");
		sf::IntRect size = this->sprite()->getTextureRect();
		sf::Vector2f collisionSizeRatio(0.4f, 0.3f); //these numbers shrink the collision size of the player, and the code below adjusts it to be positioned at the bottom of the sprite
		this->obstacleCollisionSize.width = static_cast<float>(size.width) * collisionSizeRatio.x;
		this->obstacleCollisionSize.height = static_cast<float>(size.height) * collisionSizeRatio.y;
		this->obstacleCollisionSize.left = ((1.f - collisionSizeRatio.x) * static_cast<float>(size.width)) / 2.f;
		this->obstacleCollisionSize.top = ((1.f - collisionSizeRatio.y) * static_cast<float>(size.height));
		potionNum = 0;
		meleeAttack = false;
		startDeath = false;
		deathCount = 0;
		inTrap = false;
		isHurt = false;
		isDead = false;

		//set up the score object
		this->score = 0;
		Engine::scorePtr = &this->score;

		//difficulty adjustments
		this->maxHealth += DifficultySettings::Player::maxHealthModifier;
		this->eatHeal += DifficultySettings::Player::eatHealModifier;
		this->healthDrain += DifficultySettings::Player::healthDrainModifier;
		this->attackHealthCost += DifficultySettings::Player::attackHealthCostModifier;
	}
	void AddedToScreen()
	{
		this->screen->addUIObject(&this->score);
	}
	void KeyPressed(sf::Event e)
	{
		switch (e.key.code)
		{
		case sf::Keyboard::W:
			this->W_KeyHeld = true;
			break;
		case sf::Keyboard::A:
			this->A_KeyHeld = true;
			break;
		case sf::Keyboard::S:
			this->S_KeyHeld = true;
			break;
		case sf::Keyboard::D:
			this->D_KeyHeld = true;
			break;
		default:
			break;
		}
	}
	void KeyReleased(sf::Event e)
	{
		switch (e.key.code)
		{
		case sf::Keyboard::W:
			this->W_KeyHeld = false;
			this->setDirection(DIRECTION::UP);
			break;
		case sf::Keyboard::A:
			this->A_KeyHeld = false;
			this->setDirection(DIRECTION::LEFT);
			break;
		case sf::Keyboard::S:
			this->S_KeyHeld = false;
			this->setDirection(DIRECTION::DOWN);
			break;
		case sf::Keyboard::D:
			this->D_KeyHeld = false;
			this->setDirection(DIRECTION::RIGHT);
			break;
		default:
			break;
		}
	}
	void MouseButtonReleased(sf::Event e)
	{
		if (_health > 0)
		{
			/*if (e.mouseButton.button == sf::Mouse::Left)
			{
				this->speed = 0;
				this->speedRestoreDelay = 6;
				meleeAttack = true;
				if (direction == DIRECTION::UP)
				{
					imageCount.y = 8;
					imageCount.x = 2;
				}
				if (direction == DIRECTION::DOWN)
				{
					imageCount.y = 8;
					imageCount.x = 0;
				}
				if (direction == DIRECTION::LEFT)
				{
					imageCount.y = 9;
					imageCount.x = 2;
				}
				if (direction == DIRECTION::RIGHT)
				{
					imageCount.y = 9;
					imageCount.x = 0;
				}
				this->sprite()->setTextureRect(sf::IntRect(imageCount.x * textureSize.x,
					imageCount.y * textureSize.y, textureSize.x, textureSize.y));
			}*/
			if (e.mouseButton.button == sf::Mouse::Left)
			{
				this->screen->getSoundPlayer()->play(SoundEffect::ID::ZombieAttack, 40.f);
				if (this->_health > 0.2 * this->maxHealth) { this->changeHealth(-1 * attackHealthCost); } //health cost of ranged attack only applies if health is above 20%
				sf::Vector2i mousePos = this->screen->getMousePosition();
				sf::Vector2f distance = static_cast<sf::Vector2f>(mousePos) - this->sprite()->getPosition();
				sf::Vector2f shotOrigin = this->sprite()->getPosition();
				sf::IntRect size = this->sprite()->getTextureRect();
				shotOrigin.x += static_cast<float>(size.width / 2);
				shotOrigin.y += static_cast<float>(size.height / 4);
				ZombieBlast* blast = new ZombieBlast(sf::Sprite(blast_texture), shotOrigin, sf::Vector2f(static_cast<float>(mousePos.x), static_cast<float>(mousePos.y)), 3.5f, 140);
				this->screen->add(blast);
			}
			else if (potionNum > 0)
			{
				this->screen->getSoundPlayer()->play(SoundEffect::ID::ZombieAttack, 40.f);
				sf::Vector2i mousePos = this->screen->getMousePosition();
				sf::Vector2f distance = static_cast<sf::Vector2f>(mousePos) - this->sprite()->getPosition();
				sf::Vector2f shotOrigin = this->sprite()->getPosition();
				sf::IntRect size = this->sprite()->getTextureRect();
				shotOrigin.x += static_cast<float>(size.width / 2);
				shotOrigin.y += static_cast<float>(size.height / 4);
				SuperZombieBlast* blast = new SuperZombieBlast(sf::Sprite(super_blast_texture), shotOrigin, sf::Vector2f(static_cast<float>(mousePos.x), static_cast<float>(mousePos.y)), 2.25f, 180, 1000, 0.1f, 0.2f);
				this->screen->add(blast);
				potionNum--;
			}
		}
	}
	void EveryFrame(uint64_t f)
	{
		sf::Sprite* s = this->sprite();
		sf::Vector2f adjustPos;
		adjustPos.x = s->getPosition().x + textureSize.x / 2;
		adjustPos.y = s->getPosition().y + textureSize.y;
		if (this->screen->getMap()->isTrap(adjustPos))
		{
			if (!inTrap)
			{
				inTrap = true;
				trapClock.restart();
				this->screen->getSoundPlayer()->play(SoundEffect::ID::Trap, 20.f);
			}
			speed = 1.f;
		}
		if (trapClock.getElapsedTime().asSeconds() > 2)
		{
			inTrap = false;
		}
		if (_health > 0)
		{
			if (meleeAttackCounter >= meleeAttackDuration && meleeAttack)
			{
				meleeAttack = false;
				if (direction == DIRECTION::DOWN || direction == DIRECTION::RIGHT)
				{
					if (imageCount.x == 1)
					{
						if (direction == DIRECTION::DOWN)
							imageCount.y = 0;
						else
							imageCount.y = 2;
					}
					else
						imageCount.x++;
				}
				else //if (direction == DIRECTION::UP || direction == DIRECTION::LEFT)
				{
					if (imageCount.x == 3)
					{
						if (direction == DIRECTION::UP)
							imageCount.y = 3;
						else
							imageCount.y = 1;
					}
					else
						imageCount.x++;
				}
			}
			if (!meleeAttack)
			{
				float fSpeed = static_cast<float>(this->speed);
				if (f % 20 == 0)
				{
					if (imageCount.x == 3)
						imageCount.x = 0;
					else
						imageCount.x++;
				}
				if (this->W_KeyHeld)
				{
					imageCount.y = 3;
					s->move(0.f, -1.f * fSpeed);
				}
				if (this->A_KeyHeld)
				{
					imageCount.y = 1;
					s->move(-1.f * fSpeed, 0.f);
				}
				if (this->S_KeyHeld)
				{
					imageCount.y = 0;
					s->move(0.f, fSpeed);
				}
				if (this->D_KeyHeld)
				{
					imageCount.y = 2;
					s->move(fSpeed, 0.f);
				}
			}
			meleeAttackCounter++;
			this->drain();
			if (f % 120 == 0) { this->score += DifficultySettings::Score::applyMultipliers(1); }

			//speed goes back to base speed gradually
			if (f % 6 == 0)
			{
				if (this->speed > this->baseSpeed)
				{
					speedRestoreDelay = 0;
					if (this->speedDecayDelay > 0) { this->speedDecayDelay--; }
					else { this->changeSpeed(-1); }
				}
				else if (this->speed < this->baseSpeed)
				{
					speedDecayDelay = 0;
					if (this->speedRestoreDelay > 0) { this->speedRestoreDelay--; }
					else { this->changeSpeed(1); }
				}
			}

			//color goes back to base
			if (!this->isDead)
			{
				if (colorRestoreDelay > 0) { colorRestoreDelay--; }
				else { this->sprite()->setColor(sf::Color(255, 255, 255)); }
			}
		}
		else
		{
			this->sprite()->setColor(sf::Color(255, 100, 100));
			if (!startDeath)
			{
				startDeath = true;
				this->screen->getSoundPlayer()->play(SoundEffect::ID::ZombieDeath, 60.f);
			}
			imageCount.x = deathCount;
			if (f % 50 == 0 && !this->isDead)
			{
				deathCount++;
			}
			if (direction == DIRECTION::DOWN || S_KeyHeld)
			{
				imageCount.y = 4;
			}
			if (direction == DIRECTION::LEFT || A_KeyHeld)
			{
				imageCount.y = 5;
			}
			if (direction == DIRECTION::RIGHT || D_KeyHeld)
			{
				imageCount.y = 6;
			}
			if (direction == DIRECTION::UP || W_KeyHeld)
			{
				imageCount.y = 7;
			}
			if (imageCount.x == 3)
			{
				this->sprite()->setColor(sf::Color(0, 0, 0, 0));
				isDead = true;
			}
		}
		if (!isDead)
		{
			this->sprite()->setTextureRect(sf::IntRect(imageCount.x * textureSize.x,
				imageCount.y * textureSize.y, textureSize.x, textureSize.y));
		}
	}
	void drain()
	{
		if (this->eatDrainFreezeCountdown > 0)
		{
			this->eatDrainFreezeCountdown--;
			return;
		}
		float highHealthDrainPenalty = DifficultySettings::Player::highHealthDrainPenalty;
		float drainPenalty = 1.0f + (highHealthDrainPenalty * (static_cast<float>(this->_health) / static_cast<float>(this->maxHealth)));
		int baseDrain = static_cast<int>(static_cast<float>(this->healthDrain) * drainPenalty);
		int mageDrain = numMagesAlive * (this->additionalDrainPerMage + DifficultySettings::Mage::healthDrainModifier);
		int totalDrain = baseDrain + mageDrain;
		this->changeHealth(-1 * totalDrain);
	}
	int getHealth()
	{
		return _health;
	}
	int getMaxHealth()
	{
		return maxHealth;
	}
	void setDirection(DIRECTION direction)
	{
		if (this->meleeAttack) { return; }
		this->direction = direction;
	}
	void takeDamage(int damage)
	{
		this->changeHealth(-1 * damage);
		this->sprite()->setColor(sf::Color(255, 100, 100));
		this->colorRestoreDelay = 2;
	}
	void changeHealth(int change)
	{
		this->_health += change;
		if (this->_health > this->maxHealth) { this->_health = this->maxHealth; }
	}
	void addPotionNum()
	{
		this->potionNum++;
		if (this->potionNum > this->maxPotionNum) { this->potionNum = this->maxPotionNum; }
	}
	int getPotionNum() const
	{
		return this->potionNum;
	}
	void changeSpeed(int change)
	{
		this->speed += change;
		if (this->speed > this->maxSpeed) { this->speed = this->maxSpeed; }
		else if (this->speed < 0) { this->speed = 0; }
	}
	void Collision(GraphicalGameObject& other)
	{
		if (_health > 0) {
			if (dynamic_cast<MageBlast*>(&other))
			{
				if (!isHurt)
				{
					isHurt = true;
					hurtClock.restart();
					this->screen->getSoundPlayer()->play(SoundEffect::ID::ZombieGroan, 15.f);
				}
				else if (hurtClock.getElapsedTime().asSeconds() > 0.5)
				{
					isHurt = false;
				}
				this->takeDamage(750 + DifficultySettings::Mage::attackDamageModifier);
			}
			else if (Mage* mage = dynamic_cast<Mage*>(&other))
			{
				if (!isHurt)
				{
					isHurt = true;
					hurtClock.restart();
					this->screen->getSoundPlayer()->play(SoundEffect::ZombieGroan, 10.f);
				}
				else if (hurtClock.getElapsedTime().asSeconds() > 0.5)
				{
					isHurt = false;
				}
				if (!mage->isAlive()) { return; }
				this->takeDamage(1000 + DifficultySettings::Mage::touchDamageModifier);
				this->speed = 1;
			}
			else if (Citizen* citizen = dynamic_cast<Citizen*>(&other))
			{
				srand(static_cast<int>(time(0) * other.getID()));
				int randSound = rand() % 3;
				switch (randSound)
				{
				case 0:
					this->screen->getSoundPlayer()->play(SoundEffect::ID::ZombieEat1, 80.f);
					break;
				case 1:
					this->screen->getSoundPlayer()->play(SoundEffect::ID::ZombieEat2, 80.f);
					break;
				case 2:
					this->screen->getSoundPlayer()->play(SoundEffect::ID::ZombieEat3, 50.f);
					break;
				default:
					break;
				}
				citizen->die();
				float missingHealthBonus = DifficultySettings::Player::missingHealthHealBonus;
				float missingHealthMultiplier = (1.0f + missingHealthBonus) - (missingHealthBonus * (static_cast<float>(this->_health) / static_cast<float>(this->maxHealth)));
				this->changeHealth(static_cast<int>(static_cast<float>(this->eatHeal) * missingHealthMultiplier));
				this->changeSpeed(1);
				this->speedDecayDelay = 60;
				this->score += DifficultySettings::Score::applyMultipliers(10);
				this->eatDrainFreezeCountdown = DifficultySettings::Player::eatDrainFreezeDuration;
			}
			else if (AntiMagePotion* potion = dynamic_cast<AntiMagePotion*>(&other))
			{
				this->screen->getSoundPlayer()->play(SoundEffect::ID::Potion, 40.f);
				this->addPotionNum();
				potion->die();
			}
		}
	}
	void changeScore(int change)
	{
		this->score += change;
	}
	sf::Sprite* sprite()
	{
		return dynamic_cast<sf::Sprite*>(this->graphic);
	}
};

#endif
