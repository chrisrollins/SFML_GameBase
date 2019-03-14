#ifndef MAGEBLAST_H
#define MAGEBLAST_H

#include "Screen.h"
#include "ZombieBlast.h"
#include "FileLoadException.h"

using namespace Engine;

enum class DIRECTION
{
	UP, DOWN, LEFT, RIGHT
};

class MageBlast : public GraphicalGameObject
{
private:
	sf::Vector2f movePerFrame;
	sf::Texture texture;
	int life;
	int hitsAgainstPlayer = 0;
public:
	MageBlast(const sf::Vector2f& pos, const sf::Vector2f& destination, double speed, int duration) : GraphicalGameObject(sf::Sprite())
	{
		this->ignoreObstacles = true;
		this->blockingCollision = false;
		if (!this->texture.loadFromFile("mageblast.png")) { throw GameException::ImageFileLoadException("mageblast.png"); }
		this->spritePtr()->setTexture(this->texture);
		this->spritePtr()->setPosition(pos);
		sf::Vector2u size = this->texture.getSize();
		this->spritePtr()->setOrigin(static_cast<float>(size.x) / 2.f, static_cast<float>(size.y) / 2.f);
		double radians = atan2(static_cast<double>(destination.y - pos.y), static_cast<double>(destination.x - pos.x));
		this->movePerFrame = { static_cast<float>(speed * cos(radians)), static_cast<float>(speed * sin(radians)) };
		this->life = duration;
	}

	void EveryFrame(uint64_t f)
	{
		sf::Sprite* spr = this->spritePtr();
		spr->move(this->movePerFrame);
		spr->rotate(3.f);
		spr->setScale(cos(static_cast<float>(this->life)), sin(static_cast<float>(this->life)));
		this->life--;
		if (this->life <= 20)
		{
			sf::Color currentColor = spr->getColor();
			currentColor.a -= (currentColor.a > 13) ? 13 : currentColor.a;
			spr->setColor(currentColor);
		}
		if (this->life <= 0) { this->screen->remove(this); }
	}

	void Collision(GraphicalGameObject& other)
	{
		if (dynamic_cast<SuperZombieBlast*>(&other) && this->life > 20)
		{
			this->life = 20;
			this->movePerFrame.x /= 4.f;
			this->movePerFrame.y /= 4.f;
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