#ifndef GAMEOVER_HEADER
#define GAMEOVER_HEADER

#include "GameObject.h"
#include "Screen.h"

namespace Engine
{
	class GameOver : public GraphicalGameObject
	{
	public:
		GameOver() : GraphicalGameObject(sf::Sprite())
		{
			this->texture.loadFromFile("gameover.png");
			this->spritePtr()->setTexture(this->texture);
		}
		void AddedToScreen()
		{
			sf::IntRect size = this->spritePtr()->getTextureRect();
			sf::Vector2f pos;
			pos.x = static_cast<float>(Screen::windowWidth / 2 - size.width / 2);
			pos.y = static_cast<float>(Screen::windowHeight / 2 - size.height / 2);
			this->spritePtr()->setPosition(pos);
		}
		void EveryFrame(uint64_t f)
		{
			if (this->internalClock++ < 255)
			{
				sf::Color color = this->spritePtr()->getColor();
				color.a++;
				this->spritePtr()->setColor(color);
			}
		}
	private:
		int internalClock = 0;
		sf::Texture texture;
		sf::Sprite* spritePtr() { return dynamic_cast<sf::Sprite*>(this->graphic); }
	};
}

#endif // !GAMEOVER_HEADER
