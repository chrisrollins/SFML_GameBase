#ifndef SCREEN_H
#define SCREEN_H

#include "GameObject.h"
#include "GameObjectAttribute.h"
#include "TileMap.h"
#include "MusicPlayer.h"
#include "SoundPlayer.h"
#include <map>
#include <functional>
#include <queue>
#include <vector>
#include <utility>

using std::function;
using std::vector;
using std::unordered_map;
using std::map;

namespace Engine
{
	namespace TimeUnit
	{
		class Time
		{
		public:
			Time(uint64_t frames) : frames(frames) { }
			operator uint64_t() const { return this->frames; }
		private:
			uint64_t frames;
		};

		class Minutes : public Time
		{
		public:
			Minutes(uint64_t minutes) : Time(minutes * 60 * 60) {}
		};

		class Seconds : public Time
		{
		public:
			Seconds(uint64_t seconds) : Time(seconds * 60) {}
		};
		
		class Frames : public Time
		{
		public:
			Frames(uint64_t frames) : Time(frames) {}
		};
	}

	class Screen
	{
	public:
		Screen();
		~Screen();
		void addMap(TileMap* map);
		void addMainCharacter(GraphicalGameObject* mainCharacter);
		void add(GameObject* gameObject);
		void addUIObject(GameObject* uiObj);
		bool find(GameObject* gameObject);
		void remove(GameObject* gameObject, bool autoDelete = true);
		void schedule(function<void()> func, TimeUnit::Time delay, uint16_t repeatCount = 1);
		void render();
		void close();
		sf::Vector2i getMousePosition() const;
		GraphicalGameObject* getMainCharacter() const;
		const TileMap* getMap() const;
		unsigned static int windowWidth;
		unsigned static int windowHeight;
		static const char* windowTitle;
	private:
		unordered_map<GameObjectID, GameObject*> allObjects;
		unordered_map<GameObjectID, GraphicalGameObject*> renderObjects;
		unordered_map<GameObjectID, GraphicalGameObject*> uiObjects;
		unordered_map<GameObjectID, GameObjectAttribute::Collision*> collisionObjects;
		unordered_map<GameObjectID, GameObjectAttribute::Movement*> movingObjectsWithTerrainCollision;
		unordered_map<GameObjectID, GameObjectAttribute::Movement*> movingObjects;
		GraphicalGameObject* mainCharacter = nullptr;
		TileMap* tMap = nullptr;
	};
}
#endif