#ifndef ZOMBIEBLAST_HEADER
#define ZOMBIEBLAST_HEADER

#include "GameObject.h"
#include "Screen.h"
#include <cmath>

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
		double radians = atan2(clickPos.y - pos.y, clickPos.x - pos.x);
		this->distance = sf::Vector2f(cos(radians), sin(radians));
		this->spritePtr()->rotate(radians * (180 / 3.141592653589793) - 180);
		this->spritePtr()->setPosition(pos.x + (this->distance.x * 15), pos.y + (this->distance.y * 15));
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

#endif