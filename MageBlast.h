#ifndef MAGEBLAST_H
#define MAGEBLAST_H

#include "Screen.h"
#include "ZombieBlast.h"
#include "FileLoadException.h"
#include "SpriteFactory.h"
#include "GameObjectAttribute.h"

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
	sf::Vector2f movePerFrame;
	sf::Vector2f baseSpeed;
	int life;
	int hitsAgainstPlayer = 0;
	float rotationRate;
public:
	MageBlast(const sf::Vector2f& pos, const sf::Vector2f& destination, double speed, int duration) :
		GraphicalGameObject(SpriteFactory::generateSprite(Sprite::ID::Mageblast))
	{
		this->spritePtr()->setPosition(pos);
		sf::Vector2u size = this->spritePtr()->getTexture()->getSize();
		this->spritePtr()->setOrigin(static_cast<float>(size.x) / 2.f, static_cast<float>(size.y) / 2.f);
		double radians = atan2(static_cast<double>(destination.y - pos.y), static_cast<double>(destination.x - pos.x));
		this->baseSpeed = { static_cast<float>(speed * cos(radians)), static_cast<float>(speed * sin(radians)) };
		this->movePerFrame = this->baseSpeed;
		this->life = duration;
		srand(static_cast<unsigned int>(this->getID()));
		this->rotationRate = (rand() % 2 == 0) ? 3.5f : -3.5f;
	}

	void EveryFrame(uint64_t f)
	{
		sf::Sprite* spr = this->spritePtr();
		this->move(this->movePerFrame);
		spr->rotate(this->rotationRate);
		spr->setScale(cos(static_cast<float>(this->life)), sin(static_cast<float>(this->life)));
		this->movePerFrame.x += DifficultySettings::Mage::blastSpeedAccel * this->baseSpeed.x;
		this->movePerFrame.y += DifficultySettings::Mage::blastSpeedAccel * this->baseSpeed.y;
		this->life--;
		if (this->life <= 20)
		{
			sf::Color currentColor = spr->getColor();
			currentColor.a -= (currentColor.a > 13) ? 13 : currentColor.a;
			spr->setColor(currentColor);
		}
		if (this->life <= 0) { this->screen->remove(this); }
	}

	void Collided(GraphicalGameObject* other)
	{
		if (dynamic_cast<SuperZombieBlast*>(other) && this->life > 20)
		{
			this->life = 20;
			this->baseSpeed.x = 0.f;
			this->baseSpeed.y = 0.f;
			this->movePerFrame.x /= 4.5f;
			this->movePerFrame.y /= 4.5f;
		}
	}

	void hitPlayer()
	{
		this->hitsAgainstPlayer++;
	}

	int getHits() const
	{
		return this->hitsAgainstPlayer;
	}

	sf::Sprite* spritePtr()
	{
		return dynamic_cast<sf::Sprite*>(this->graphic);
	}
};

#endif