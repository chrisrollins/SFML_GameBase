#ifndef ANTIMAGEPOTION_H
#define ANTIMAGEPOTION_H

#include "Screen.h"
#include "DifficultySettings.h"
#include "Score.h"
#include <vector>
#include <ctime>

using namespace Engine;

class AntiMagePotion : public GraphicalGameObject
{
private:
	int wiggleDirection = 1;
	float wiggleSpeed = 1.1f;
	float wiggleMagnitude = 18.f;
	float currentRotation = 0.f;
	sf::Vector2u textureSize;
	sf::Vector2u imageCount;
	sf::Vector2u currentImage;
public:
	AntiMagePotion() : GraphicalGameObject(sf::Sprite())
	{
		static sf::Texture potionTexture;
		if (potionTexture.getNativeHandle() == 0)
		{
			if (!potionTexture.loadFromFile("animated_potion.png")) { throw GameException::ImageFileLoadException("animated_potion.png"); }
		}
		this->spritePtr()->setTexture(potionTexture);
		this->textureSize = this->spritePtr()->getTexture()->getSize();
		this->textureSize.x /= 8U;
		this->imageCount.x = 0U;
		this->spritePtr()->setTextureRect(sf::IntRect(this->imageCount.x * this->textureSize.x,
			this->imageCount.y * this->textureSize.y, this->textureSize.x, this->textureSize.y));

		sf::IntRect size = this->spritePtr()->getTextureRect();
		this->spritePtr()->setOrigin(static_cast<float>(size.width / 2), static_cast<float>(size.height / 2));
	}

	void EveryFrame(uint64_t f)
	{
		if (f % 10 == 0)
		{
			if (this->imageCount.x == 7) { this->imageCount.x = 0; }
			else { this->imageCount.x++; }
		}
		this->spritePtr()->setTextureRect(sf::IntRect(this->imageCount.x * this->textureSize.x,
			this->imageCount.y * this->textureSize.y, this->textureSize.x, this->textureSize.y));

		if (this->currentRotation >= this->wiggleMagnitude || this->currentRotation <= (-1.f * this->wiggleMagnitude)) { this->wiggleDirection *= -1; }
		this->currentRotation += static_cast<float>(this->wiggleDirection) * this->wiggleSpeed;
		this->spritePtr()->setRotation(this->currentRotation);
	}

	void die()
	{
		this->screen->remove(this);
	}

	sf::Sprite* spritePtr()
	{
		return dynamic_cast<sf::Sprite*>(this->graphic);
	}
};

#endif
