#ifndef BULLET_HEADER
#define BULLET_HEADER

#include "GameObject.h"
#include "Screen.h"

enum class DIRECTION
{
	UP, DOWN, LEFT, RIGHT
};

class Bullet : public Engine::GraphicalGameObject
{
private:
	DIRECTION direction;
	sf::Vector2u textureSize;
public:
	Bullet(sf::Sprite r, sf::Vector2f pos, DIRECTION direction) : Engine::GraphicalGameObject(r) 
	{
		this->spritePtr()->setPosition(pos);
		this->direction = direction;
		textureSize = this->spritePtr()->getTexture()->getSize();
	}
	void EveryFrame(uint64_t f)
	{
		if (direction == DIRECTION::UP)
		{
			this->spritePtr()->setTextureRect(sf::IntRect(0, 0, textureSize.x / 4, textureSize.y));
			this->spritePtr()->move(0, -2);
		}
		else if (direction == DIRECTION::DOWN)
		{
			this->spritePtr()->setTextureRect(sf::IntRect(textureSize.x / 4, 0, textureSize.x / 4, textureSize.y));
			this->spritePtr()->move(0, 2);
		}
		else if (direction == DIRECTION::LEFT)
		{
			this->spritePtr()->setTextureRect(sf::IntRect(textureSize.x / 2, 0, textureSize.x / 4, textureSize.y));
			this->spritePtr()->move(-2, 0);
		}
		else // (direction == DIRECTION::RIGHT)
		{
			this->spritePtr()->setTextureRect(sf::IntRect(textureSize.x / 4 * 3, 0, textureSize.x / 4, textureSize.y));
			this->spritePtr()->move(2, 0);
		}
	}
	sf::Sprite* spritePtr()
	{
		return dynamic_cast<sf::Sprite*>(this->graphic);
	}
};

#endif