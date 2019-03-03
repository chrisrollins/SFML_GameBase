#ifndef CITIZEN_HEADER
#define CITIZEN_HEADER

#include "GameObject.h"
#include "SkeletonBlast.h"
#include "Screen.h"

class Citizen : public Engine::GraphicalGameObject
{
private:
	bool W_KeyHeld = false;
	bool A_KeyHeld = false;
	bool S_KeyHeld = false;
	bool D_KeyHeld = false;
	float health;
	bool alive;
	sf::Vector2u textureSize;
	sf::Vector2u imageCount;
	sf::Vector2u currentImage;
public:
	Citizen(sf::Sprite r) : Engine::GraphicalGameObject(r)
	{
		textureSize = this->spritePtr()->getTexture()->getSize();
		textureSize.x /= 3;
		textureSize.y /= 4;
		imageCount.x = 0;
		switch (rand() % 4)
		{
		case 0:
			W_KeyHeld = true;
			A_KeyHeld = false;
			S_KeyHeld = false;
			D_KeyHeld = false;
			break;
		case 1:
			A_KeyHeld = true;
			W_KeyHeld = false;
			S_KeyHeld = false;
			D_KeyHeld = false;
			break;
		case 2:
			S_KeyHeld = true;
			A_KeyHeld = false;
			W_KeyHeld = false;
			D_KeyHeld = false;
			break;
		case 3:
			D_KeyHeld = true;
			A_KeyHeld = false;
			S_KeyHeld = false;
			W_KeyHeld = false;
			break;
		}
	}
	void EveryFrame(uint64_t f)
	{
		if (f % 120 == 0)
		{
			switch (rand() % 4)
			{
			case 0:
				W_KeyHeld = true;
				A_KeyHeld = false;
				S_KeyHeld = false;
				D_KeyHeld = false;
				break;
			case 1:
				A_KeyHeld = true;
				W_KeyHeld = false;
				S_KeyHeld = false;
				D_KeyHeld = false;
				break;
			case 2:
				S_KeyHeld = true;
				A_KeyHeld = false;
				W_KeyHeld = false;
				D_KeyHeld = false;
				break;
			case 3:
				D_KeyHeld = true;
				A_KeyHeld = false;
				S_KeyHeld = false;
				W_KeyHeld = false;
				break;
			}
		}
		if (this->W_KeyHeld)
		{
			// row 4
			imageCount.y = 3;
			this->spritePtr()->move(0, -0.5);
			this->spritePtr()->setTextureRect(sf::IntRect(imageCount.x * textureSize.x,
				imageCount.y * textureSize.y, textureSize.x, textureSize.y));
		}
		if (this->A_KeyHeld)
		{
			// row 2
			imageCount.y = 1;
			this->spritePtr()->move(-0.5, 0);
			this->spritePtr()->setTextureRect(sf::IntRect(imageCount.x * textureSize.x,
				imageCount.y * textureSize.y, textureSize.x, textureSize.y));
		}
		if (this->S_KeyHeld)
		{
			// row 1
			imageCount.y = 0;
			this->spritePtr()->move(0, 0.5);
			this->spritePtr()->setTextureRect(sf::IntRect(imageCount.x * textureSize.x,
				imageCount.y * textureSize.y, textureSize.x, textureSize.y));
		}
		if (this->D_KeyHeld)
		{
			// row 3
			imageCount.y = 2;
			this->spritePtr()->move(0.5, 0);
			this->spritePtr()->setTextureRect(sf::IntRect(imageCount.x * textureSize.x,
				imageCount.y * textureSize.y, textureSize.x, textureSize.y));
		}
		// how often the sprite sheet is changing
		if (f % 20 == 0)
		{
			if (imageCount.x == 2)
				imageCount.x = 0;
			else
				imageCount.x++;
		}
	}
	void Collision(Engine::GraphicalGameObject& other)
	{
		if (dynamic_cast<SkeletonBlast*>(&other))
		{
			this->screen->remove(this);
		}
	}
	sf::Sprite* spritePtr()
	{
		return dynamic_cast<sf::Sprite*>(this->graphic);
	}
	bool isAlive() const;
	void setAlive(bool alive);
	void takeDamage(float amount);
	void setHealth(float health);
	float getHealth() const;
};

#endif