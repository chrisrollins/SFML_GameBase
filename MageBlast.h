#ifndef MAGEBLAST_H
#define MAGEBLAST_H

#include "Screen.h"
#include "ZombieBlast.h"
#include "FileLoadException.h"
#include "SpriteFactory.h"
#include "GameObjectAttribute.h"
#include "DebugManager.h"

using namespace Engine;

enum class DIRECTION
{
	UP, DOWN, LEFT, RIGHT
};

class MageBlast :
	public GraphicalGameObject,
	public Collision,
	public Movement,
	public Attacker
{
private:
	sf::Vector2f baseMovePerFrame;
	sf::Vector2f movePerFrame;
	uint64_t remainingFrames;
	int hitsAgainstPlayer = 0;
	double _speed = 1.0;
	Radians angle = Radians(0.0);
	sf::Vector2f _destination;
	float _rotationRate;
	int _damage = 1;
	float scaleModifier = 0.f;
	bool initializationOver = false;

	void initSpeed()
	{
		double radians = this->angle.getRadians();
		this->baseMovePerFrame = { static_cast<float>(_speed * cos(radians)), static_cast<float>(_speed * sin(radians)) };
		this->movePerFrame = this->baseMovePerFrame;
	}

	void AddedToScreen()
	{
		this->initializationOver = true;
	}

	void EveryFrame(uint64_t f)
	{
		sf::Sprite* spr = this->getDrawablePtr();
		this->move(this->movePerFrame);
		spr->rotate(this->_rotationRate);
		spr->setScale(scaleModifier + cos(static_cast<float>(this->remainingFrames)), scaleModifier + sin(static_cast<float>(this->remainingFrames)));
		this->movePerFrame.x += DifficultySettings::Mage::blastSpeedAccel * this->baseMovePerFrame.x;
		this->movePerFrame.y += DifficultySettings::Mage::blastSpeedAccel * this->baseMovePerFrame.y;
		this->remainingFrames--;
		if (this->remainingFrames <= 20)
		{
			sf::Color currentColor = spr->getColor();
			currentColor.a -= (currentColor.a > 13) ? 13 : currentColor.a;
			spr->setColor(currentColor);
		}
		if (this->remainingFrames <= 0) { this->screen->remove(this); }
	}

	void Collided(Collision* other)
	{
		if (dynamic_cast<SuperZombieBlast*>(other) && this->remainingFrames > 20)
		{
			this->remainingFrames = 20;
			this->baseMovePerFrame.x = 0.f;
			this->baseMovePerFrame.y = 0.f;
			this->movePerFrame.x /= 4.5f;
			this->movePerFrame.y /= 4.5f;
		}
	}

public:
	MageBlast(sf::Sprite sprite) : GraphicalGameObject(sprite)
	{
		sf::Vector2u size = this->getDrawablePtr()->getTexture()->getSize();
		this->getDrawablePtr()->setOrigin(static_cast<float>(size.x) / 2.f, static_cast<float>(size.y) / 2.f);
		srand(static_cast<unsigned int>(this->getID()));
		this->_rotationRate = (rand() % 2 == 0) ? 3.5f : -3.5f;
	}

	#define INIT_FUNC\
	if (initializationOver)\
	{\
		DebugManager::PrintMessage(DebugManager::MessageType::ERROR_REPORTING, "Warning: Initialization function called on MageBlast after it was added to screen. No effect.");\
		return this;\
	}
	
	MageBlast* position(sf::Vector2f pos)
	{
		INIT_FUNC
		this->getDrawablePtr()->setPosition(pos);
		return this;
	}

	MageBlast* destination(sf::Vector2f dest)
	{
		INIT_FUNC
		this->_destination = dest;
		sf::Vector2f pos = this->getDrawablePtr()->getPosition();
		this->angle = Radians(atan2(static_cast<double>(dest.y - pos.y), static_cast<double>(dest.x - pos.x)));
		this->initSpeed();
		return this;
	}

	MageBlast* speed(double speed)
	{
		INIT_FUNC
		this->_speed = speed * (1.0 + static_cast<double>(DifficultySettings::Mage::blastSpeedModifier));
		this->initSpeed();
		return this;
	}

	MageBlast* duration(TimeUnit::Time duration)
	{
		INIT_FUNC
		this->remainingFrames = duration;
		return this;
	}

	MageBlast* damage(int damage)
	{
		INIT_FUNC
		this->_damage = damage;
		return this;
	}
	
	MageBlast* scale(float scale)
	{
		INIT_FUNC
		this->scaleModifier = scale;
		return this;
	}
	
	MageBlast* rotationRate(float rotationRate)
	{
		INIT_FUNC
		this->_rotationRate = rotationRate;
		return this;
	}

	#undef INIT_FUNC

	int getDamage() const { return this->_damage; }

	void hitPlayer() { this->hitsAgainstPlayer++; }

	int getHits() const { return this->hitsAgainstPlayer; }
};

#endif