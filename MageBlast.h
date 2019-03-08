#ifndef MAGE_BLAST_HEADER
#define MAGE_BLAST_HEADER

#include "GameObject.h"
#include "Screen.h"
#include <cmath>

class MageBlast : public Engine::GraphicalGameObject
{
private:
	sf::Vector2f movePerFrame;
	sf::Texture texture;
	int life;
public:
	MageBlast(const sf::Vector2f& pos, const sf::Vector2f& destination, double speed, int duration) : Engine::GraphicalGameObject(sf::Sprite())
	{
		this->ignoreObstacles = true;
		this->blockingCollision = false;
		this->texture.loadFromFile("mageblast.png");
		this->spritePtr()->setTexture(this->texture);
		this->spritePtr()->setPosition(pos);
		sf::Vector2u size = this->texture.getSize();
		this->spritePtr()->setOrigin( static_cast<float>(size.x) / 2.f, static_cast<float>(size.y) / 2.f);
		double radians = atan2(static_cast<double>(destination.y - pos.y), static_cast<double>(destination.x - pos.x));
		this->movePerFrame = { static_cast<float>(speed * cos(radians)), static_cast<float>(speed * sin(radians)) };
		this->life = duration;		
	}
	void EveryFrame(uint64_t f)
	{
		this->spritePtr()->move(this->movePerFrame);
		this->life--;
		this->spritePtr()->rotate(3.f);
		this->spritePtr()->setScale(cos(static_cast<float>(this->life)), sin(static_cast<float>(this->life)));
		if (this->life <= 0)
		{
			this->screen->remove(this);
		}
	}
	void Collision(GraphicalGameObject& other)
	{

	}
	sf::Sprite* spritePtr()
	{
		return dynamic_cast<sf::Sprite*>(this->graphic);
	}
};

#endif