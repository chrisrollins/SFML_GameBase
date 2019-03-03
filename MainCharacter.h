#ifndef MAIN_CHARACTER_HEADER
#define MAIN_CHARACTER_HEADER

#include <cmath>
#include "Screen.h"
#include "GameObject.h"
#include "SkeletonBlast.h"
#include "Citizen.h"
#include "Bullet.h"
#include "Soldier.h"

using namespace Engine;

//class SampleCircle : public GraphicalGameObject
//{
//public:
//	SampleCircle() : GraphicalGameObject(sf::CircleShape(50.0f))
//	{
//		this->circle()->setFillColor(sf::Color::Blue);
//	}
//	void MouseButtonReleased(sf::Event e)
//	{
//		this->circle()->setPosition(e.mouseButton.x, e.mouseButton.y);
//	}
//	sf::CircleShape* circle()
//	{
//		return dynamic_cast<sf::CircleShape*>(this->graphic);
//	}
//};

//class SampleSquare : public GraphicalGameObject
//{
//public:
//	SampleSquare() : GraphicalGameObject(sf::RectangleShape(sf::Vector2f(10, 10)))
//	{
//		this->square()->setFillColor(sf::Color::Green);
//	}
//	sf::RectangleShape* square()
//	{
//		return dynamic_cast<sf::RectangleShape*>(this->graphic);
//	}
//};


class MainCharacter : public GraphicalGameObject
{
	bool W_KeyHeld = false;
	bool A_KeyHeld = false;
	bool S_KeyHeld = false;
	bool D_KeyHeld = false;
	sf::Texture blast_texture;
	sf::Vector2u textureSize;
	sf::Vector2u imageCount;
	sf::Vector2u currentImage;
	bool meleeAttack;
	bool isWalking; // used so that the zombie stands still when the key is not pressed
	int deathCount; // to control death animation
	bool isDead; // true when the zombie turns to invisible
	DIRECTION direction;
	int _health = 30 * 60; // 60 frames per second
	int maxHealth = 30 * 60;
	int baseSpeed = 2;
	int speed = 2;
	int maxSpeed = 5;
	int speedDecayDelay = 0;
	int speedRestoreDelay = 0;
public:
	MainCharacter(sf::Sprite s) : GraphicalGameObject(s)
	{
		textureSize = this->sprite()->getTexture()->getSize();
		textureSize.x /= 4;
		textureSize.y /= 12;
		imageCount.x = 0;
		this->sprite()->setTextureRect(sf::IntRect(imageCount.x * textureSize.x,
			imageCount.y * textureSize.y, textureSize.x, textureSize.y));
		blast_texture.loadFromFile("blast.png");
		sf::IntRect size = this->sprite()->getTextureRect();
		sf::Vector2f collisionSizeRatio(0.5, 0.3); //these numbers shrink the collision size of the player, and the code below adjusts it to be positioned at the bottom of the sprite
		this->obstacleCollisionSize.width = size.width * collisionSizeRatio.x;
		this->obstacleCollisionSize.height = size.height * collisionSizeRatio.y;
		this->obstacleCollisionSize.left = ((1 - collisionSizeRatio.x) * size.width) / 2;
		this->obstacleCollisionSize.top = ((1 - collisionSizeRatio.y) * size.height);
		meleeAttack = false;
		isWalking = false;
		deathCount = 0;
		isDead = false;
	}
	void KeyPressed(sf::Event e)
	{
		isWalking = true;
		switch (e.key.code)
		{
		case sf::Keyboard::W:
			this->W_KeyHeld = true;
			break;
		case sf::Keyboard::A:
			this->A_KeyHeld = true;
			break;
		case sf::Keyboard::S:
			this->S_KeyHeld = true;
			break;
		case sf::Keyboard::D:
			this->D_KeyHeld = true;
			break;
		default:
			break;
		}
	}
	void KeyReleased(sf::Event e)
	{
		isWalking = false;
		switch (e.key.code)
		{
		case sf::Keyboard::W:
			this->W_KeyHeld = false;
			direction = DIRECTION::UP;
			break;
		case sf::Keyboard::A:
			this->A_KeyHeld = false;
			direction = DIRECTION::LEFT;
			break;
		case sf::Keyboard::S:
			this->S_KeyHeld = false;
			direction = DIRECTION::DOWN;
			break;
		case sf::Keyboard::D:
			this->D_KeyHeld = false;
			direction = DIRECTION::RIGHT;
			break;
		default:
			break;
		}
	}
	void MouseButtonReleased(sf::Event e)
	{
		if (_health > 0)
		{
			if (e.mouseButton.button == sf::Mouse::Left)
			{
				meleeAttack = true;
				isWalking = false;
				if (direction == DIRECTION::UP)
				{
					imageCount.y = 8;
					imageCount.x = 2;
				}
				if (direction == DIRECTION::DOWN)
				{
					imageCount.y = 8;
					imageCount.x = 0;
				}
				if (direction == DIRECTION::LEFT)
				{
					imageCount.y = 9;
					imageCount.x = 2;
				}
				if (direction == DIRECTION::RIGHT)
				{
					imageCount.y = 9;
					imageCount.x = 0;
				}
				this->sprite()->setTextureRect(sf::IntRect(imageCount.x * textureSize.x,
					imageCount.y * textureSize.y, textureSize.x, textureSize.y));
			}
			else
			{
				isWalking = false;
				sf::Vector2i mousePos = this->screen->getMousePosition();
				sf::Vector2f distance = static_cast<sf::Vector2f>(mousePos) - this->sprite()->getPosition();
				sf::Vector2f shotOrigin = this->sprite()->getPosition();
				sf::IntRect size = this->sprite()->getTextureRect();
				shotOrigin.x += size.width / 2;
				shotOrigin.y += size.height / 4;
				SkeletonBlast* blast = new SkeletonBlast(sf::Sprite(blast_texture), shotOrigin, sf::Vector2f(mousePos.x, mousePos.y));
				this->screen->add(blast);
			}
		}
	}
	void EveryFrame(uint64_t f)
	{
		sf::Sprite* s = this->sprite();
		if (_health > 0)
		{
			if (f % 30 == 0 && meleeAttack)
			{
				if (direction == DIRECTION::DOWN || direction == DIRECTION::RIGHT)
				{
					if (imageCount.x == 1)
					{
						meleeAttack = false;
						if (direction == DIRECTION::DOWN)
							imageCount.y = 0;
						else
							imageCount.y = 2;
					}
					else
						imageCount.x++;
				}
				else //if (direction == DIRECTION::UP || direction == DIRECTION::LEFT)
				{
					if (imageCount.x == 3)
					{
						meleeAttack = false;
						if (direction == DIRECTION::UP)
							imageCount.y = 3;
						else
							imageCount.y = 1;
					}
					else
						imageCount.x++;
				}
			}
			if (!meleeAttack && isWalking)
			{
				if (f % 20 == 0)
				{
					if (imageCount.x == 3)
						imageCount.x = 0;
					else
						imageCount.x++;
				}
				if (this->W_KeyHeld)
				{
					imageCount.y = 3;
					s->move(0, -1 * this->speed);
				}
				if (this->A_KeyHeld)
				{
					imageCount.y = 1;
					s->move(-1 * this->speed, 0);
				}
				if (this->S_KeyHeld)
				{
					imageCount.y = 0;
					s->move(0, this->speed);
				}
				if (this->D_KeyHeld)
				{
					imageCount.y = 2;
					s->move(this->speed, 0);
				}
			}
			_health--;
			//speed goes back to base speed gradually
			if (f % 6 == 0)
			{
				if (this->speed > this->baseSpeed)
				{
					speedRestoreDelay = 0;
					if (this->speedDecayDelay > 0) { this->speedDecayDelay--; }
					else { this->changeSpeed(-1); }
				}
				else if (this->speed < this->baseSpeed)
				{
					speedDecayDelay = 0;
					if (this->speedRestoreDelay > 0) { this->speedRestoreDelay--; }
					else { this->changeSpeed(1); }
				}
			}
		}
		else
		{
			imageCount.x = deathCount;
			if (f % 20 == 0 && !isDead)
			{
				deathCount++;
			}
			if (direction == DIRECTION::DOWN || S_KeyHeld)
			{
				imageCount.y = 4;
			}
			if (direction == DIRECTION::LEFT || A_KeyHeld)
			{
				imageCount.y = 5;
			}
			if (direction == DIRECTION::RIGHT || D_KeyHeld)
			{
				imageCount.y = 6;
			}
			if (direction == DIRECTION::UP || W_KeyHeld)
			{
				imageCount.y = 7;
			}
			if (imageCount.x == 3)
			{
				this->sprite()->setColor(sf::Color(0, 0, 0, 0));
				isDead = true;
			}
		}
		if (!isDead)
		{
			this->sprite()->setTextureRect(sf::IntRect(imageCount.x * textureSize.x,
				imageCount.y * textureSize.y, textureSize.x, textureSize.y));
		}
	}
	int getHealth()
	{
		return _health;
	}
	int getMaxHealth()
	{
		return maxHealth;
	}
	void changeHealth(int change)
	{

		this->_health += change;
		if (this->_health > this->maxHealth) { this->_health = this->maxHealth; }
	}
	void changeSpeed(int change)
	{
		this->speed += change;
		if (this->speed > this->maxSpeed) { this->speed = this->maxSpeed; }
		else if (this->speed < 1) { this->speed = 1; }
	}
	void Collision(GraphicalGameObject& other)
	{
		if (_health > 0) {
			if (dynamic_cast<Bullet*>(&other))
			{
				this->changeHealth(-5);
			}
			else if (dynamic_cast<Soldier*>(&other))
			{
				this->changeHealth(-10);
				this->changeSpeed(-1);
			}
			else if (dynamic_cast<Citizen*>(&other))
			{
				this->screen->remove(&other);
				float missingHealthMultiplier = 2.0f - (this->_health / this->maxHealth);
				this->changeHealth(60 * missingHealthMultiplier);
				this->changeSpeed(1);
				this->speedDecayDelay = 60;

			}
		}
	}
	sf::Sprite* sprite()
	{
		return dynamic_cast<sf::Sprite*>(this->graphic);
	}
};

#endif
