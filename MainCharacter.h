#include "Screen.h"
#include "GameObject.h"
#include "SkeletonBlast.h"

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
	bool isShooting;
	int blast_life;
	SkeletonBlast* blast;
	//std::vector<GraphicalGameObject*> objs;
public:
	MainCharacter(sf::Sprite s) : GraphicalGameObject(s)
	{
		textureSize = this->sprite()->getTexture()->getSize();
		textureSize.x /= 3;
		textureSize.y /= 4;
		imageCount.x = 0;
		this->sprite()->setTextureRect(sf::IntRect(imageCount.x * textureSize.x,
			imageCount.y * textureSize.y, textureSize.x, textureSize.y));
		blast_texture.loadFromFile("blast.png");
		isShooting = false;
		blast_life = 0;
	}
	void KeyPressed(sf::Event e)
	{
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
		switch (e.key.code)
		{
		case sf::Keyboard::W:
			this->W_KeyHeld = false;
			break;
		case sf::Keyboard::A:
			this->A_KeyHeld = false;
			break;
		case sf::Keyboard::S:
			this->S_KeyHeld = false;
			break;
		case sf::Keyboard::D:
			this->D_KeyHeld = false;
			break;
		default:
			break;
		}
	}
	void MouseButtonReleased(sf::Event e)
	{
		/*
		sf::Vector2f position = this->sprite()->getPosition();
		//commented out print statements for now, they hurt performance
		//std::cout << "Mouse clicked at (" << e.mouseButton.x << ", " << e.mouseButton.y << ")" << std::endl;
		//std::cout << "character is at (" << position.x << ", " << position.y << ")" << std::endl;
		int adjustedX = position.x + e.mouseButton.x - (startingScreen.windowWidth / 2);
		int adjustedY = position.y + e.mouseButton.y - (startingScreen.windowHeight / 2);
		*/
		sf::Vector2i mousePos = this->screen->getMousePosition();
		if (!isShooting)
		{
			sf::Vector2f distance = static_cast<sf::Vector2f>(mousePos) - this->sprite()->getPosition();
			blast = new SkeletonBlast(sf::Sprite(blast_texture), this->sprite()->getPosition(), distance);
			startingScreen.add(blast);
			isShooting = true;
		}

		//SampleSquare* s = new SampleSquare(); //use a heap allocated object which has to be cleaned up later.
		//objs.push_back(s);
		//s->square()->setPosition(mousePos.x, mousePos.y);
		//startingScreen.add(s);
	}
	void EveryFrame(uint64_t f)
	{
		sf::Sprite* s = this->sprite();
		if (f % 20 == 0)
		{
			if (imageCount.x == 2)
				imageCount.x = 0;
			else
				imageCount.x++;
		}
		if (this->W_KeyHeld)
		{
			imageCount.y = 3;
			this->sprite()->setTextureRect(sf::IntRect(imageCount.x * textureSize.x,
				imageCount.y * textureSize.y, textureSize.x, textureSize.y));
			s->move(0, -2);
		}
		if (this->A_KeyHeld)
		{
			imageCount.y = 1;
			this->sprite()->setTextureRect(sf::IntRect(imageCount.x * textureSize.x,
				imageCount.y * textureSize.y, textureSize.x, textureSize.y));
			s->move(-2, 0);
		}
		if (this->S_KeyHeld)
		{
			imageCount.y = 0;
			this->sprite()->setTextureRect(sf::IntRect(imageCount.x * textureSize.x,
				imageCount.y * textureSize.y, textureSize.x, textureSize.y));
			s->move(0, 2);
		}
		if (this->D_KeyHeld)
		{
			imageCount.y = 2;
			this->sprite()->setTextureRect(sf::IntRect(imageCount.x * textureSize.x,
				imageCount.y * textureSize.y, textureSize.x, textureSize.y));
			s->move(2, 0);
		}
		if (isShooting)
		{
			blast_life++;
			if (blast_life == 60)
			{
				startingScreen.remove(blast);
				delete blast;
				blast = nullptr;
				isShooting = false;
				blast_life = 0;
			}
		}
	}
	void Collision(GraphicalGameObject& other)
	{
		std::cout << "collision in the main character" << std::endl;
		std::cout << " " << std::endl;
		std::cout << " " << std::endl;
	}
	sf::Sprite* sprite()
	{
		return dynamic_cast<sf::Sprite*>(this->graphic);
	}
};
