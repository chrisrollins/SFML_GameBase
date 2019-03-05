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
	sf::Texture texture;
	sf::Vector2u textureSize;
	int life = 0;
public:
	Bullet(sf::Vector2f pos, DIRECTION direction) : Engine::GraphicalGameObject(sf::Sprite())
	{
		this->ignoreObstacles = true;
		this->blockingCollision = false;
		this->texture.loadFromFile("bullet.png");
		this->spritePtr()->setTexture(this->texture);
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
			this->spritePtr()->setTextureRect(sf::IntRect(textureSize.x / 4 * 3, 0, textureSize.x / 4, textureSize.y));
			this->spritePtr()->move(0, 2);
		}
		else if (direction == DIRECTION::LEFT)
		{
			this->spritePtr()->setTextureRect(sf::IntRect(textureSize.x / 2, 0, textureSize.x / 4, textureSize.y));
			this->spritePtr()->move(-2, 0);
		}
		else // (direction == DIRECTION::RIGHT)
		{
			this->spritePtr()->setTextureRect(sf::IntRect(textureSize.x / 4, 0, textureSize.x / 4, textureSize.y));
			this->spritePtr()->move(2, 0);
		}
		this->life++;
		if (this->life == 60)
		{
			this->screen->remove(this);
		}
	}
	void Collision(GraphicalGameObject& other)
	{
		//std::cout << "collision in the bullet" << std::endl;
		//std::cout << " " << std::endl;
		//std::cout << " " << std::endl;
	}
	sf::Sprite* spritePtr()
	{
		return dynamic_cast<sf::Sprite*>(this->graphic);
	}
};

#endif