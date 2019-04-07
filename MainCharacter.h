#ifndef MAINCHARACTER_H
#define MAINCHARACTER_H

#include "GameOver.h"
#include "Score.h"
#include "Screen.h"
#include "ZombieBlast.h"
#include "Citizen.h"
#include "Mage.h"
#include "DifficultySettings.h"
#include "MageBlast.h"
#include "AntiMagePotion.h"
#include "FileLoadException.h"
#include "SpriteFactory.h"
#include "SoundPlayer.h"
#include "MusicPlayer.h"
#include "GameObjectAttribute.h"
#include <ctime>
#include <vector>
#include <stack>
#include <cmath>

using namespace Engine;

class MainCharacter :
	public GraphicalGameObject,
	public Health,
	public Movement,
	public Collision,
	public TerrainCollision
{
	bool upKeyHeld = false;
	bool leftKeyHeld = false;
	bool downKeyHeld = false;
	bool rightKeyHeld = false;
	DIRECTION direction = DIRECTION::DOWN;
	AntiMagePotion* potionPtr = nullptr;
	sf::Vector2u textureSize;
	sf::Vector2u imageCount;
	sf::Vector2u currentImage;
	sf::Clock aliveClock;
	sf::Clock hurtClock;
	sf::Clock trapClock;
	float totalAliveTime = 0.f;
	bool inTrap = false;
	bool isHurt = false;
	bool startDeath = false;
	bool finishedDying = false; // true when the zombie turns to invisible
	int deathCount = 0; // to control death animation
	int potionNum = 0;
	int maxPotionNum = 6;
	int healthDrain = 15;
	int additionalDrainPerMage = 1;
	int numCitizenEated = 0;
	int eatHeal = 7500;
	int eatDrainFreezeCountdown = 0;
	int attackHealthCost = 250;
	float baseSpeed = 3.5f;
	float currentSpeed = 3.5f;
	float maxSpeed = 4.5f;
	int speedDecayDelay = 0;
	int speedRestoreDelay = 0;
	int colorRestoreDelay = 0;
	std::vector<sf::Vector2f> spawnPositions;
	std::string name;
public:
	MainCharacter(std::string name) :
		GraphicalGameObject(SpriteFactory::generateSprite(Sprite::ID::Zombie)),
		Health(30 * 60 * 100 + DifficultySettings::Player::maxHealthModifier)
	{
		this->name = name;
		this->textureSize = this->sprite()->getTexture()->getSize();
		this->textureSize.x /= 4;
		this->textureSize.y /= 12;
		this->imageCount.x = 0;
		this->sprite()->setTextureRect(sf::IntRect(this->imageCount.x * this->textureSize.x,
			this->imageCount.y * this->textureSize.y, this->textureSize.x, this->textureSize.y));
		sf::IntRect size = this->sprite()->getTextureRect();
		sf::Vector2f collisionSizeRatio(0.4f, 0.3f); //these numbers shrink the collision size of the player, and the code below adjusts it to be positioned at the bottom of the sprite
		
		sf::FloatRect collisionSize;
		collisionSize.width = static_cast<float>(size.width) * collisionSizeRatio.x;
		collisionSize.height = static_cast<float>(size.height) * collisionSizeRatio.y;
		collisionSize.left = ((1.f - collisionSizeRatio.x) * static_cast<float>(size.width)) / 2.f;
		collisionSize.top = ((1.f - collisionSizeRatio.y) * static_cast<float>(size.height));
		this->setObstacleCollisionSize(collisionSize);
		this->aliveClock.restart();

		this->eatHeal += DifficultySettings::Player::eatHealModifier;
		this->healthDrain += DifficultySettings::Player::healthDrainModifier;
		this->attackHealthCost += DifficultySettings::Player::attackHealthCostModifier;
		this->maxPotionNum += DifficultySettings::Player::maxPotionNumModifier;
	}
	void KeyPressed(sf::Event e)
	{
		if (!this->startDeath)
		{
			switch (e.key.code)
			{
			case sf::Keyboard::W:
				this->upKeyHeld = true;
				break;
			case sf::Keyboard::A:
				this->leftKeyHeld = true;
				break;
			case sf::Keyboard::S:
				this->downKeyHeld = true;
				break;
			case sf::Keyboard::D:
				this->rightKeyHeld = true;
				break;
			case sf::Keyboard::Escape:
				if (DifficultySettings::currentDifficulty == DifficultySettings::DIFFICULTY::TEST)
				{
					this->healthDrain = 10000;
					DifficultySettings::Mage::healthDrainModifier = 0;
				}
			default:
				break;
			}
		}
	}

	void KeyReleased(sf::Event e)
	{
		if (!this->startDeath)
		{
			switch (e.key.code)
			{
			case sf::Keyboard::W:
				this->upKeyHeld = false;
				this->setDirection(DIRECTION::UP);
				break;
			case sf::Keyboard::A:
				this->leftKeyHeld = false;
				this->setDirection(DIRECTION::LEFT);
				break;
			case sf::Keyboard::S:
				this->downKeyHeld = false;
				this->setDirection(DIRECTION::DOWN);
				break;
			case sf::Keyboard::D:
				this->rightKeyHeld = false;
				this->setDirection(DIRECTION::RIGHT);
				break;
			default:
				break;
			}
		}
	}

	void MouseButtonReleased(sf::Event e)
	{
		if (this->getHealth() > 0)
		{
			if (e.mouseButton.button == sf::Mouse::Left)
			{
				SoundPlayer::play(SoundEffect::ID::ZombieAttack, 40.f);
				if (this->getHealthPercent() > 0.2) { this->changeHealth(-1 * this->attackHealthCost); } //health cost of ranged attack only applies if health is above 20%
				sf::Vector2i mousePos = this->screen->getMousePosition();
				sf::Vector2f shotOrigin = this->sprite()->getPosition();
				sf::IntRect size = this->sprite()->getTextureRect();
				shotOrigin.x += static_cast<float>(size.width / 2);
				shotOrigin.y += static_cast<float>(size.height / 4);
				ZombieBlast* blast = new ZombieBlast(Sprite::ID::Blast, shotOrigin, sf::Vector2f(static_cast<float>(mousePos.x), static_cast<float>(mousePos.y)), 3.5f, 140);
				this->screen->add(blast);
			}
			else if (this->potionNum > 0)
			{
				SoundPlayer::play(SoundEffect::ID::ZombieAttack, 40.f);
				sf::Vector2i mousePos = this->screen->getMousePosition();
				sf::Vector2f shotOrigin = this->sprite()->getPosition();
				sf::IntRect size = this->sprite()->getTextureRect();
				shotOrigin.x += static_cast<float>(size.width / 2);
				shotOrigin.y += static_cast<float>(size.height / 4);
				SuperZombieBlast* blast = new SuperZombieBlast(Sprite::ID::Brain, shotOrigin, sf::Vector2f(static_cast<float>(mousePos.x), static_cast<float>(mousePos.y)), 2.25f, 180, 1000, 0.1f, 0.2f);
				this->screen->add(blast);
				this->potionNum--;
			}
		}
	}

	void EveryFrame(uint64_t f)
	{
		sf::Sprite* s = this->sprite();
		sf::Vector2f adjustPos;
		adjustPos.x = s->getPosition().x + this->textureSize.x / 2;
		adjustPos.y = s->getPosition().y + this->textureSize.y;
		if (this->screen->getMap()->isTrap(adjustPos))
		{
			if (!this->inTrap)
			{
				this->inTrap = true;
				this->trapClock.restart();
				SoundPlayer::play(SoundEffect::ID::Trap, 10.f);
			}
			this->currentSpeed = 1.25f;
		}
		if (this->trapClock.getElapsedTime().asSeconds() > 2) { this->inTrap = false; }

		if (this->isAlive())
		{
			if (f % 20 == 0 && (this->upKeyHeld || this->leftKeyHeld || this->downKeyHeld || this->rightKeyHeld))
			{
				if (this->imageCount.x == 3) { this->imageCount.x = 0; }
				else { this->imageCount.x++; }
			}
			if (!this->upKeyHeld && !this->leftKeyHeld && !this->downKeyHeld && !this->rightKeyHeld) { this->imageCount.x = 0; }
			
			int xDirection = 0;
			int yDirection = 0;
			if (this->rightKeyHeld) { xDirection += 1; }
			if (this->leftKeyHeld) { xDirection -= 1; }
			if (this->downKeyHeld) { yDirection += 1; }
			if (this->upKeyHeld) { yDirection -= 1; }
		
			if (xDirection != 0 || yDirection != 0)
			{
				float angle = atan2(static_cast<float>(yDirection), static_cast<float>(xDirection));
				this->move(Radians(angle), currentSpeed);
			}

			if (this->upKeyHeld) { this->imageCount.y = 3; }
			else if (this->downKeyHeld) { this->imageCount.y = 0; }
			else if (this->leftKeyHeld) { this->imageCount.y = 1; }
			else if (this->rightKeyHeld) { this->imageCount.y = 2; }

			this->drain();
			if (f % 120 == 0) { *scorePtr += DifficultySettings::Score::applyMultipliers(1); }

			//speed goes back to base speed gradually
			if (f % 6 == 0)
			{
				if (this->currentSpeed > this->baseSpeed)
				{
					this->speedRestoreDelay = 0;
					if (this->speedDecayDelay > 0) { this->speedDecayDelay--; }
					else { this->changeSpeed(-0.75f); }
				}
				else if (this->currentSpeed < this->baseSpeed)
				{
					this->speedDecayDelay = 0;
					if (this->speedRestoreDelay > 0) { this->speedRestoreDelay--; }
					else { this->changeSpeed(0.75f); }
				}
			}

			//color goes back to base
			if (this->isAlive())
			{
				if (this->colorRestoreDelay > 0) { this->colorRestoreDelay--; }
				else { this->sprite()->setColor({ 255, 255, 255 }); }
			}
		}
		else
		{
			this->sprite()->setColor({ 255, 100, 100 });
			if (!this->startDeath)
			{
				this->totalAliveTime = this->aliveClock.getElapsedTime().asSeconds();
				this->startDeath = true;
				SoundPlayer::play(SoundEffect::ID::ZombieDeath, 60.f);
			}
			this->imageCount.x = this->deathCount;
			if (f % 50 == 0 && !this->finishedDying)
			{
				this->deathCount++;
				if (deathCount == 2)
				{
					MusicPlayer::play(Music::ID::GameOver);
				}
			}
			if (this->direction == DIRECTION::DOWN || this->downKeyHeld) { this->imageCount.y = 4; }
			if (this->direction == DIRECTION::LEFT || this->leftKeyHeld) { this->imageCount.y = 5; }
			if (this->direction == DIRECTION::RIGHT || this->rightKeyHeld) { this->imageCount.y = 6; }
			if (this->direction == DIRECTION::UP || this->upKeyHeld) { this->imageCount.y = 7; }
			if (this->imageCount.x == 3) { this->die(); }
		}

		if (this->isAlive())
		{
			this->sprite()->setTextureRect(sf::IntRect(this->imageCount.x * this->textureSize.x, this->imageCount.y * this->textureSize.y, this->textureSize.x, this->textureSize.y));
		}
	}

	void drain()
	{
		if (this->eatDrainFreezeCountdown > 0)
		{
			this->eatDrainFreezeCountdown--;
			return;
		}

		float time = this->aliveClock.getElapsedTime().asSeconds();
		float highHealthDrainPenalty = DifficultySettings::Player::highHealthDrainPenalty + (time * (0.01f + DifficultySettings::Player::highHealthDrainPenalty * 0.01f));
		float healthRatio = static_cast<float>(this->getHealthPercent());
		if (healthRatio > 1.f) { healthRatio = 1.f; }
		float drainPenaltyModifier = (healthRatio + (healthRatio * healthRatio)) * 0.5f;
		float drainPenalty = 1.f + (highHealthDrainPenalty * drainPenaltyModifier);
		int baseDrain = static_cast<int>(static_cast<float>(this->healthDrain) * drainPenalty) + static_cast<int>(time/15.f);
		int mageDrain = numMagesAlive * (this->additionalDrainPerMage + DifficultySettings::Mage::healthDrainModifier);
		int totalDrain = baseDrain + mageDrain;
		this->changeHealth(-1 * totalDrain);
	}

	void die()
	{
		if (!this->finishedDying)
		{
			this->sprite()->setColor({ 0, 0, 0, 0 });
			this->finishedDying = true;
			scorePtr->freeze();
			this->screen->addUIObject(new GameOver(scorePtr->get(), DifficultySettings::currentDifficulty));
		}
	}

	std::string getName() const
	{
		return this->name;
	}

	float getCurrAliveTime() const
	{
		return this->aliveClock.getElapsedTime().asSeconds();
	}

	float getTotalAliveTime() const
	{
		return this->totalAliveTime;
	}

	int getNumCitizenEated() const
	{
		return this->numCitizenEated;
	}
	
	void setDirection(DIRECTION direction)
	{
		this->direction = direction;
	}

	void damage(int damage)
	{
		this->changeHealth(-1 * damage);
		this->sprite()->setColor({ 255, 100, 100 });
		this->colorRestoreDelay = 2;
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

	int getMaxPotionNum() const
	{
		return this->maxPotionNum;
	}

	void changeSpeed(float change)
	{
		this->currentSpeed += change;
		if (this->currentSpeed > this->maxSpeed) { this->currentSpeed = this->maxSpeed; }
		else if (this->currentSpeed < 0.f) { this->currentSpeed = 0.f; }
	}

	void Collided(GameObjectAttribute::Collision* other)
	{
		if (this->isAlive())
		{
			if (MageBlast* blast = dynamic_cast<MageBlast*>(other))
			{
				if (!this->isHurt)
				{
					this->isHurt = true;
					this->hurtClock.restart();
					SoundPlayer::play(SoundEffect::ID::ZombieGroan, 15.f);
				}
				else if (this->hurtClock.getElapsedTime().asSeconds() > 0.5) { this->isHurt = false; }

				float time = this->aliveClock.getElapsedTime().asSeconds();
				float timeAmplifier = 1.f + time * 0.01f;
				sf::Vector2f myPos = this->sprite()->getPosition();
				sf::Vector2f blastPos = blast->spritePtr()->getPosition();
				sf::IntRect blastSize = blast->spritePtr()->getTextureRect();
				float dx = myPos.x - blastPos.x;
				float dy = myPos.y - blastPos.y;
				float distance = sqrt(dx * dx + dy * dy);
				float proximityMultiplier = 1.5f - distance / 100.f;
				float repeatDamageDampening = (1.f + 0.01f*(static_cast<float>(blast->getHits())));
				if (repeatDamageDampening > 3.0f) { repeatDamageDampening = 3.0f; }
				float baseDmg = (275.f + static_cast<float>(DifficultySettings::Mage::attackDamageModifier)) * proximityMultiplier * timeAmplifier;
				int damage = static_cast<int>((baseDmg) / repeatDamageDampening);
				this->damage(damage);
				blast->hitPlayer();
			}
			else if (Mage* mage = dynamic_cast<Mage*>(other))
			{
				if (!this->isHurt)
				{
					this->isHurt = true;
					this->hurtClock.restart();
					SoundPlayer::play(SoundEffect::ID::ZombieGroan, 10.f);
				}
				else if (this->hurtClock.getElapsedTime().asSeconds() > 0.5) { this->isHurt = false; }
				if (!mage->isAlive()) { return; }
				this->damage(100 + DifficultySettings::Mage::touchDamageModifier);
				this->currentSpeed = 1.5f;
			}
			else if (Citizen* citizen = dynamic_cast<Citizen*>(other))
			{
				srand(static_cast<int>(time(0) * this->getID()));
				int randSound = rand() % 3;
				switch (randSound)
				{
				case 0:
					SoundPlayer::play(SoundEffect::ID::ZombieEat1, 80.f);
					break;
				case 1:
					SoundPlayer::play(SoundEffect::ID::ZombieEat2, 80.f);
					break;
				case 2:
					SoundPlayer::play(SoundEffect::ID::ZombieEat3, 50.f);
					break;
				default:
					break;
				}
				citizen->die();
				this->numCitizenEated++;
				if (this->numCitizenEated % DifficultySettings::Player::potionMakingCitizenRequired == 0)
				{
					srand(static_cast<int>(time(0) << 7));
					switch (rand() % 4)
					{
					case 0:
						SoundPlayer::play(SoundEffect::ID::ZombieBurp1, 70.f);
						break;
					case 1:
						SoundPlayer::play(SoundEffect::ID::ZombieBurp2, 70.f);
						break;
					case 2:
						SoundPlayer::play(SoundEffect::ID::ZombieBurp3, 50.f);
						break;
					case 3:
						SoundPlayer::play(SoundEffect::ID::ZombieBurp4, 50.f);
						break;
					}
					this->spawnPositions = this->screen->getMap()->getSafeSpawnPositions();
					size_t randIndex = rand() % spawnPositions.size();
					sf::Vector2f position = spawnPositions[randIndex];
					potionPtr = new AntiMagePotion();
					potionPtr->spritePtr()->setPosition(position);
					this->screen->add(potionPtr);
				}
				float missingHealthBonus = DifficultySettings::Player::missingHealthHealBonus;
				float missingHealthMultiplier = (1.0f + missingHealthBonus) - (missingHealthBonus * (static_cast<float>(this->getHealthPercent())));
				this->changeHealth(static_cast<int>(static_cast<float>(this->eatHeal) * missingHealthMultiplier));
				this->changeSpeed(1.f);
				this->speedDecayDelay = 60;
				*scorePtr += DifficultySettings::Score::applyMultipliers(10);
				this->eatDrainFreezeCountdown = DifficultySettings::Player::eatDrainFreezeDuration;
			}
			else if (AntiMagePotion* potion = dynamic_cast<AntiMagePotion*>(other))
			{
				SoundPlayer::play(SoundEffect::ID::Potion, 40.f);
				this->addPotionNum();
				this->changeHealth(this->eatHeal / 2);
				this->eatDrainFreezeCountdown = DifficultySettings::Player::eatDrainFreezeDuration * 3;
				potion->die();
			}
		}
	}

	void changeScore(int change)
	{
		*scorePtr += change;
	}

	sf::Sprite* sprite()
	{
		return dynamic_cast<sf::Sprite*>(this->graphic);
	}
};
#endif