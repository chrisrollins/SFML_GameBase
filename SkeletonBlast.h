#ifndef SKELETONBLAST_HEADER
#define SKELETONBLAST_HEADER

#include "GameObject.h"
#include "Screen.h"
#include <cmath>

class SkeletonBlast : public Engine::GraphicalGameObject
{
private:
	sf::Vector2f distance;
	int blast_life;
public:
	SkeletonBlast(sf::Sprite r, sf::Vector2f pos, sf::Vector2f distance) : Engine::GraphicalGameObject(r)
	{
		this->spritePtr()->setPosition(pos);
		double radians = atan2(distance.y - pos.y, distance.x - pos.x);
		this->distance = sf::Vector2f(cos(radians), sin(radians));
		blast_life = 0;
	}
	void EveryFrame(uint64_t f)
	{
		this->spritePtr()->move(distance.x * 4, distance.y * 4);
		blast_life++;
		if (blast_life == 90)
		{
			Engine::startingScreen.remove(this);
			delete this;
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