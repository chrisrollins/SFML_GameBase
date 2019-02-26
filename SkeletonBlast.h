#ifndef SKELETONBLAST_HEADER
#define SKELETONBLAST_HEADER

#include "GameObject.h"
#include "Screen.h"

class SkeletonBlast : public Engine::GraphicalGameObject
{
private:
	sf::Vector2f distance;
	//int blast_life;
public:
	SkeletonBlast(sf::Sprite r, sf::Vector2f pos, sf::Vector2f distance) : Engine::GraphicalGameObject(r)
	{
		this->spritePtr()->setPosition(pos);
		this->distance = distance;
		//blast_life = 0;
	}
	void EveryFrame(uint64_t f)
	{
		this->spritePtr()->move(distance.x / 80, distance.y / 80);
		// I was trying to do this, but it will give me an error
		/*blast_life++;
		if (blast_life == 60)
		{
			Engine::startingScreen.remove(this);
		}*/
	}
	void Collision(GraphicalGameObject& other)
	{
		std::cout << "collision in the blast" << std::endl;
		std::cout << " " << std::endl;
		std::cout << " " << std::endl;
	}
	sf::Sprite* spritePtr()
	{
		return dynamic_cast<sf::Sprite*>(this->graphic);
	}
};

#endif