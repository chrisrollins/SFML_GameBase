#ifndef MAIN_CHARACTER_HEADER
#define MAIN_CHARACTER_HEADER

#include <cmath>
#include "Score.h"
#include "Screen.h"
#include "GameObject.h"
#include "ZombieBlast.h"
#include "Citizen.h"
#include "Bullet.h"
#include "Mage.h"
#include "DifficultySettings.h"

using namespace Engine;

class MainCharacter : public GraphicalGameObject
{
	bool W_KeyHeld = false;
	bool A_KeyHeld = false;
	bool S_KeyHeld = false;
	bool D_KeyHeld = false;
	sf::Texture blast_texture;
	sf::Vector2u textureSize;
	sf::Vector2u imageCount;
	sf::Vector2u currentImage;
	bool meleeAttack;
	int meleeAttackDuration = 20;
	int meleeAttackCounter = 0;
	int deathCount; // to control death animation
	bool isDead; // true when the zombie turns to invisible
	DIRECTION direction;
	int _health = 30 * 60 * 100;
	int maxHealth = 30 * 60 * 100;
	int healthDrain = 30;
	int eatHeal = 5000;
	int attackHealthCost = 0;
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
		sf::IntRect size = this->sprite()->getTextureRect();
		sf::Vector2f collisionSizeRatio(0.4, 0.3); //these numbers shrink the collision size of the player, and the code below adjusts it to be positioned at the bottom of the sprite
		this->obstacleCollisionSize.width = size.width * collisionSizeRatio.x;
		this->obstacleCollisionSize.height = size.height * collisionSizeRatio.y;
		this->obstacleCollisionSize.left = ((1 - collisionSizeRatio.x) * size.width) / 2;
		this->obstacleCollisionSize.top = ((1 - collisionSizeRatio.y) * size.height);
		meleeAttack = false;
		deathCount = 0;
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
			if (e.mouseButton.button == sf::Mouse::Left)
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
			}
			else
			{
				if (this->_health > 0.2 * this->maxHealth) { this->changeHealth(-1 * attackHealthCost); } //health cost of ranged attack only applies if health is above 20%
				sf::Vector2i mousePos = this->screen->getMousePosition();
				sf::Vector2f distance = static_cast<sf::Vector2f>(mousePos) - this->sprite()->getPosition();
				sf::Vector2f shotOrigin = this->sprite()->getPosition();
				sf::IntRect size = this->sprite()->getTextureRect();
				shotOrigin.x += size.width / 2;
				shotOrigin.y += size.height / 4;
				ZombieBlast* blast = new ZombieBlast(sf::Sprite(blast_texture), shotOrigin, sf::Vector2f(mousePos.x, mousePos.y));
				this->screen->add(blast);
			}
		}
	}
	void EveryFrame(uint64_t f)
	{
		sf::Sprite* s = this->sprite();
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
					s->move(0, -1 * this->speed);
				}
				if (this->A_KeyHeld)
				{
					imageCount.y = 1;
					s->move(-1 * this->speed, 0);
				}
				if (this->S_KeyHeld)
				{
					imageCount.y = 0;
					s->move(0, this->speed);
				}
				if (this->D_KeyHeld)
				{
					imageCount.y = 2;
					s->move(this->speed, 0);
				}
			}
			meleeAttackCounter++;
			this->drain(f);
			if (f % 120 == 0) { this->score += DifficultySettings::Score::applyMultipliers(1); }

			if (f % 300 == 0)
			{
				std::cout << "base: " << DifficultySettings::Score::baseMultiplier << std::endl;
				std::cout << "cumulative current: " << DifficultySettings::Score::cumulativeBonusMultiplierCurrent << std::endl;
				std::cout << "cumulative max: " << DifficultySettings::Score::cumulativeBonusMultiplierMax << std::endl;
				std::cout << "applyMultipliers(1): " << DifficultySettings::Score::applyMultipliers(1) << std::endl;
			}

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
	void drain(uint64_t f)
	{
		float highHealthDrainPenalty = DifficultySettings::Player::highHealthDrainPenalty;
		float drainPenalty = 1.0f + (highHealthDrainPenalty * (static_cast<float>(this->_health) / static_cast<float>(this->maxHealth)));
		int baseDrain = this->healthDrain * drainPenalty;
		int totalDrain = baseDrain + (numMagesAlive * (5 + DifficultySettings::Mage::healthDrainModifier));
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
	void changeSpeed(int change)
	{
		this->speed += change;
		if (this->speed > this->maxSpeed) { this->speed = this->maxSpeed; }
		else if (this->speed < 0) { this->speed = 0; }
	}
	void Collision(GraphicalGameObject& other)
	{
		if (_health > 0) {
			if (dynamic_cast<Bullet*>(&other))
			{
				this->takeDamage(500 + DifficultySettings::Mage::attackDamageModifier);
			}
			else if (Mage* mage = dynamic_cast<Mage*>(&other))
			{
				if (!mage->isAlive()) { return; }
				this->takeDamage(1000 + DifficultySettings::Mage::touchDamageModifier);
				this->speed = 1;
			}
			else if (Citizen* citizen = dynamic_cast<Citizen*>(&other))
			{
				citizen->die();
				float missingHealthBonus = DifficultySettings::Player::missingHealthHealBonus;
				float missingHealthMultiplier = (1.0f + missingHealthBonus) - (missingHealthBonus * (static_cast<float>(this->_health) / static_cast<float>(this->maxHealth)));
				this->changeHealth(this->eatHeal * missingHealthMultiplier);
				this->changeSpeed(1);
				this->speedDecayDelay = 60;
				this->score += DifficultySettings::Score::applyMultipliers(10);
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
