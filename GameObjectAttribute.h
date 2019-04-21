#ifndef GAME_OBJECT_ATTRIBUTE
#define GAME_OBJECT_ATTRIBUTE

#include "GameObject.h"
#include "Screen.h"
#include <cmath>

namespace Engine
{
	class GameObjectAttribute
	{
	private:
		template<typename PtrType> class GraphicalGameObjectDrawablePointerAccess
		{
		public:
			PtrType* getDrawablePtr() const
			{
				if (this->drawablePtr == nullptr)
				{
					GraphicalGameObjectDrawablePointerAccess* _this = const_cast<GraphicalGameObjectDrawablePointerAccess*>(this);
					if (GraphicalGameObject* ggo = dynamic_cast<GraphicalGameObject*>(_this)) { _this->drawablePtr = dynamic_cast<PtrType*>(ggo->getGraphic()); }
				}
				return this->drawablePtr;
			}
		private:
			virtual void _() {}
			PtrType* drawablePtr = nullptr;
		};

		class GameObjectScreenAccess
		{
		public:
			Screen* getScreenPtr() const
			{
				if (this->screenPtr == nullptr)
				{
					GameObjectScreenAccess* _this = const_cast<GameObjectScreenAccess*>(this);
					if (const GameObject* go = dynamic_cast<const GameObject*>(this)) { _this->screenPtr = go->getScreenPtr(); }
				}
				return this->screenPtr;
			}
		private:
			virtual void _() {}
			Screen* screenPtr = nullptr;
		};

	public:
		GameObjectAttribute() = delete;

		//triggers the Collided event when another object with Collision is touched according to the CheckCollision method
		class Collision : public virtual GraphicalGameObjectDrawablePointerAccess<sf::Sprite>
		{
		protected:
			friend class Screen;
			virtual bool CheckCollision(Collision* other)
			{
				sf::FloatRect  thisRect = this->getDrawablePtr()->getGlobalBounds();
				sf::FloatRect otherRect = other->getDrawablePtr()->getGlobalBounds();
				return thisRect.intersects(otherRect);
			}
			virtual void Collided(Collision* other) { }
		};

		//prevents the object from leaving the map or moving through blocking tiles
		class TerrainCollision : public virtual GraphicalGameObjectDrawablePointerAccess<sf::Sprite>
		{
		public:
			sf::FloatRect getObstacleCollisionSize() const
			{
				if (this->customSize) { return this->obstacleCollisionSize; }
				else if (sf::Sprite* spr = this->getDrawablePtr())
				{
					sf::IntRect tr = spr->getTextureRect();
					return sf::FloatRect(0.f, 0.f, static_cast<float>(tr.width), static_cast<float>(tr.height));
				}
				return sf::FloatRect(0.f, 0.f, 0.f, 0.f);
			}

			void setObstacleCollisionSize(sf::FloatRect rect)
			{
				this->obstacleCollisionSize = rect;
				this->customSize = true;
			}
			
		private:
			sf::FloatRect obstacleCollisionSize;
			bool customSize = false;
		};

		//the object has a health total. it can be damaged or healed.
		class Health
		{
		public:
			Health(int maxHealth) : maxHealth(maxHealth), health(maxHealth) { }

			virtual int getHealth() const { return this->health; }

			virtual int getMaxHealth() const { return this->maxHealth; }

			virtual double getHealthPercent() const{ return static_cast<double>(this->getHealth()) / static_cast<double>(this->getMaxHealth()); }

			virtual void heal(int heal)
			{
				if (this->isAlive()) { this->changeHealth(this->Healed(heal)); }
			}

			virtual void damage(int damage)
			{
				if (this->isAlive()) { this->changeHealth(-1 * this->Damaged(damage)); }
			}

			virtual void die()
			{
				this->changeHealth(this->getHealth() * -1);
			}

			virtual void changeHealth(int change)
			{
				this->health += change;
				if (this->health > maxHealth) { this->health = maxHealth; }
				else if (!this->isAlive()) { this->Death(); }
			}

			bool isAlive()
			{
				return this->getHealth() > 0;
			}
		protected:
			//event which triggers when the object dies
			virtual void Death() {}

			//these can be overridden to modify incoming damage or healing
			virtual int Damaged(int damage) { return damage; }

			virtual int Healed(int heal) { return heal; }
		private:
			int maxHealth;
			int health;			
		};

		//has the healTarget method which will attempt to heal a target. If the target does not have health, nothing happens.
		class Healer
		{
		public:
			virtual void healTarget(GameObject& target, int heal)
			{
				if (Health* targetHealth = dynamic_cast<Health*>(&target))
				{
					targetHealth->heal(heal);
				}
			}
		private:
			Health* lastTarget = nullptr;
		};

		//has the dealDamage method which will attempt to damage a target. If the target does not have health, nothing happens.
		class Attacker
		{
		public:
			virtual void dealDamage(GameObject& target, int damage)
			{
				if (Health* targetHealth = dynamic_cast<Health*>(&target)) { targetHealth->damage(damage); }
			}
		private:
			Health* lastTarget = nullptr;
		};

		//this class lets the player class know which objects are enemies
		class Enemy
		{

		};

		//this class gives the object the ability to move or be moved
		class Movement : public virtual GraphicalGameObjectDrawablePointerAccess<sf::Sprite>
		{
		#define DEG_2_RAD 0.017453292519943295
		#define RAD_2_DEG (1.0 / DEG_2_RAD)
		public:

			class Angle
			{
			public:
				Angle() = delete;
				double getRadians() const { return this->radians; }
				double getDegrees() const { return this->radians * RAD_2_DEG; }
				void setRadians(double radians) { this->radians = radians; }
				void setDegrees(double degrees) { this->radians = DEG_2_RAD * degrees; }
			protected:
				explicit Angle(double radians) : radians(radians) { }
			private:
				double radians;
			};

			class Degrees : public Angle
			{
			public:
				explicit Degrees(double degrees) : Angle(degrees * DEG_2_RAD) {}
				explicit Degrees(uint16_t degrees) : Degrees(static_cast<double>(degrees)) {}
				explicit Degrees(uint32_t degrees) : Degrees(static_cast<double>(degrees)) {}
				explicit Degrees(uint64_t degrees) : Degrees(static_cast<double>(degrees)) {}
				explicit Degrees(int16_t degrees) : Degrees(static_cast<double>(degrees)) {}
				explicit Degrees(int32_t degrees) : Degrees(static_cast<double>(degrees)) {}
				explicit Degrees(int64_t degrees) : Degrees(static_cast<double>(degrees)) {}
				explicit Degrees(float degrees) : Degrees(static_cast<double>(degrees)) {}
			};

			class Radians : public Angle
			{
			public:
				explicit Radians(double radians) : Angle(radians) {}
				explicit Radians(uint16_t radians) : Radians(static_cast<double>(radians)) {}
				explicit Radians(uint32_t radians) : Radians(static_cast<double>(radians)) {}
				explicit Radians(uint64_t radians) : Radians(static_cast<double>(radians)) {}
				explicit Radians(int16_t radians) : Radians(static_cast<double>(radians)) {}
				explicit Radians(int32_t radians) : Radians(static_cast<double>(radians)) {}
				explicit Radians(int64_t radians) : Radians(static_cast<double>(radians)) {}
				explicit Radians(float radians) : Radians(static_cast<double>(radians)) {}
			};
			#undef DEG_2_RAD
			#undef RAD_2_DEG

			virtual void move(sf::Vector2f distance)
			{
				this->xVelocity += static_cast<double>(distance.x);
				this->yVelocity += static_cast<double>(distance.y);
			}

			virtual void move(Angle angle, float distance = 1.0f)
			{
				double radians = angle.getRadians();
				double xDistance = cos(radians) * static_cast<double>(distance);
				double yDistance = sin(radians) * static_cast<double>(distance);
				this->xVelocity += xDistance;
				this->yVelocity += yDistance;
			}

			sf::Vector2f getVelocity() const
			{
				return sf::Vector2f(static_cast<float>(this->xVelocity), static_cast<float>(this->yVelocity));
			}
		private:
			friend class Screen;
			double xVelocity;
			double yVelocity;
		};

		//provides the sprite sheet functionality
		class SpriteSheet : public virtual GraphicalGameObjectDrawablePointerAccess<sf::Sprite>
		{
		public:
			SpriteSheet(int rows) : SpriteSheet(rows, 1) { }
			SpriteSheet(int rows, int columns) : spriteSheetRow(rows, this), spriteSheetColumn(columns, this){ }

			//call this from the inheriting class if you need to set up the texture rect and the sheet hasn't set it up yet
			void resetSpriteSheet()
			{
				this->spriteSheetRow.set(0);
				this->spriteSheetColumn.set(0);
			}

			class SpriteSheetDimension
			{
			public:
				SpriteSheetDimension() = delete;
				SpriteSheetDimension operator ++(int)
				{
					this->add(1);
					return *this;
				}
				void operator +=(int n) { this->add(n); }
				void operator -=(int n) { this->add(-1 * n); }
				void operator =(int n) { this->set(n); }
				void operator *=(int n) { this->multiply(n); }
				void operator /=(int n) { this->divide(n); }
				bool operator ==(int n) const { return this->position == n; }
				operator int() const { return this->position; }
				int size() const { return this->max; }
			private:
				friend class SpriteSheet;
				void add(int n) { this->set(this->position + n); }
				void multiply(int n) { this->set(this->position * n); }
				void divide(int n) { this->set(this->position / n); }
				void set(int n)
				{
					n = n % this->max;
					if (n < 0) { n = this->max + n; }
					if (sf::Sprite* s = this->sheet->getDrawablePtr())
					{
						this->sheet->init();
						s->setTextureRect({
							this->sheet->spriteSheetRow * static_cast<int>(this->sheet->textureSize.x),
							this->sheet->spriteSheetColumn * static_cast<int>(this->sheet->textureSize.y),
							static_cast<int>(this->sheet->textureSize.x),
							static_cast<int>(this->sheet->textureSize.y)
						});
					}
					this->position = static_cast<uint32_t>(n);
				}
				friend SpriteSheet::SpriteSheet(int, int);
				SpriteSheetDimension(int max, SpriteSheet* sheet) : max(max), sheet(sheet), position(0) { }
				int position;
				int max;
				SpriteSheet* sheet;
			};
			
			SpriteSheetDimension spriteSheetRow;
			SpriteSheetDimension spriteSheetColumn;

		private:
			friend void SpriteSheetDimension::set(int);

			void init()
			{
				if (this->initialized) { return; }
				this->initialized = true;
				sf::Sprite* s = this->getDrawablePtr();
				const sf::Texture* t = s->getTexture();
				this->textureSize = t->getSize();
				this->textureSize.x /= (this->spriteSheetRow.max > 0) ? this->spriteSheetRow.max : 1;
				this->textureSize.y /= (this->spriteSheetColumn.max > 0) ? this->spriteSheetColumn.max : 1 ;
			}
			bool initialized = false;
			sf::Vector2u textureSize;
		};
	};

	
	#define TYPE_SHORTCUT(type) typedef Engine::GameObjectAttribute::type type
	TYPE_SHORTCUT(Health);
	TYPE_SHORTCUT(Movement);
	TYPE_SHORTCUT(Collision);
	TYPE_SHORTCUT(TerrainCollision);
	TYPE_SHORTCUT(Attacker);
	TYPE_SHORTCUT(Healer);
	TYPE_SHORTCUT(Enemy);
	TYPE_SHORTCUT(SpriteSheet);
	#undef TYPE_SHORTCUT

	#define StandardEnemy public GraphicalGameObject, public Health, public Movement, public Collision, public TerrainCollision, public Enemy
}
#endif