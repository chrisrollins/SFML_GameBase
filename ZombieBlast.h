#ifndef ZOMBIEBLAST_HEADER
#define ZOMBIEBLAST_HEADER

#include "GameObject.h"
#include "Screen.h"
#include <cmath>

#define F(n) static_cast<float>(n)
#define D(n) static_cast<double>(n)

class ZombieBlast : public Engine::GraphicalGameObject
{
private:
	sf::Vector2f distance;
	int blast_life;
public:
	ZombieBlast(sf::Sprite r, sf::Vector2f pos, sf::Vector2f clickPos) : Engine::GraphicalGameObject(r)
	{
		this->ignoreObstacles = true;
		this->blockingCollision = false;
		double radians = atan2( D(clickPos.y - pos.y), D(clickPos.x - pos.x));
		this->distance = sf::Vector2f( F(cos(radians)), F(sin(radians)));
		this->spritePtr()->rotate( F(radians * (180 / 3.141592653589793) - 180.0) );
		this->spritePtr()->setPosition( pos.x + (this->distance.x * 15.f), pos.y + (this->distance.y * 15.f) );
		blast_life = 0;
	}
	void EveryFrame(uint64_t f)
	{
		this->spritePtr()->move(distance.x * 4, distance.y * 4);
		blast_life++;
		if (blast_life == 100)
		{
			this->screen->remove(this);
		}
	}
	void Collision(GraphicalGameObject& other)
	{
		//std::cout << "collision in the blast" << std::endl;
		//std::cout << " " << std::endl;
		//std::cout << " " << std::endl;
	}
	sf::Sprite* spritePtr()
	{
		return dynamic_cast<sf::Sprite*>(this->graphic);
	}
};

#undef F
#undef D

#endif