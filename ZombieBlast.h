#ifndef ZOMBIEBLAST_H
#define ZOMBIEBLAST_H

#include "Screen.h"
#include "SpriteFactory.h"
#include "GameObjectAttribute.h"
#include <cmath>
#include <string>

using namespace Engine;
using std::string;

#define F(n) static_cast<float>(n)
#define D(n) static_cast<double>(n)

class ZombieBlast :
	public GraphicalGameObject,
	public Collision,
	public Attacker,
	public Movement
{
protected:
	sf::Vector2f distance;
	int blastLife;
	int damage = 1;
	float growRate;
	float growth = 1.f;
public:
	ZombieBlast(Sprite::ID spriteID, sf::Vector2f pos, sf::Vector2f clickPos, float speed = 1.f, int duration = 100, int damage = 1, float startingSize = 1.f, float growRate = 0.05f) : GraphicalGameObject(SpriteFactory::generateSprite(spriteID))
	{
		const double pi = 3.14159265358979323846;
		double radians = atan2(D(clickPos.y - pos.y), D(clickPos.x - pos.x));
		this->distance = sf::Vector2f(F(cos(radians)) * speed, F(sin(radians)) * speed);
		sf::Vector2u size = this->spritePtr()->getTexture()->getSize();
		this->spritePtr()->setOrigin(static_cast<float>(size.x) / 2.f, static_cast<float>(size.y) / 2.f);
		this->spritePtr()->setRotation(F(radians * (180.0 / pi) - 180.0));
		this->spritePtr()->setPosition(pos.x + (this->distance.x * 5.f), pos.y + (this->distance.y * 5.f));
		this->spritePtr()->setScale(startingSize, startingSize);
		this->blastLife = duration;
		this->growRate = growRate;
		this->damage = damage;
	}

	virtual void EveryFrame(uint64_t f)
	{
		this->move(this->distance);
		this->growth += growRate / 60.0f;
		this->spritePtr()->setScale(this->growth, this->growth);
		this->blastLife--;
		if (this->blastLife <= 0) { this->screen->remove(this); }
	}

	int getDamage() const
	{
		return this->damage;
	}

	sf::Sprite* spritePtr()
	{
		return dynamic_cast<sf::Sprite*>(this->graphic);
	}
};

class SuperZombieBlast : public ZombieBlast
{
private:
	float rotationRate = -3.f;
public:
	SuperZombieBlast(Sprite::ID spriteID, sf::Vector2f pos, sf::Vector2f clickPos, float speed = 1.f, int duration = 100, int damage = 1, float startingSize = 1.f, float growRate = 0.05f)
		: ZombieBlast(spriteID, pos, clickPos, speed, duration, damage, startingSize, growRate)
	{
		this->spritePtr()->setColor(sf::Color(255, 255, 255, 150));
	}

	void EveryFrame(uint64_t f)
	{
		sf::Sprite* spr = this->spritePtr();
		this->move(this->distance);
		this->growth += growRate / 60.f;
		spr->setScale(this->growth, this->growth);
		spr->rotate(this->rotationRate);
		if (this->blastLife <= 20)
		{
			this->rotationRate -= 3.f;
			spr->scale(1.02f, 1.02f);
			sf::Color currentColor = spr->getColor();
			currentColor.a -= (currentColor.a > 13) ? 13 : currentColor.a;
			spr->setColor(currentColor);
		}
		this->blastLife--;
		if (this->blastLife <= 0) { this->screen->remove(this); }
	}
};

#undef F
#undef D

#endif