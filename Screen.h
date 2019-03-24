#ifndef SCREEN_H
#define SCREEN_H

#include "GameObject.h"
#include "TileMap.h"
#include "MusicPlayer.h"
#include "SoundPlayer.h"
#include <map>
#include <functional>
#include <queue>

using std::map;
using std::function;

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
		void addMainCharacter(GameObject* mainCharacter);
		void addUIObject(GameObject* uiObj);
		void add(GameObject* gameObject);
		void remove(GameObject* gameObject, bool autoDelete = true);
		void schedule(function<void()> func, TimeUnit::Time delay, uint16_t repeatCount = 1);
		void render(int fps = 60);
		void close();
		sf::Vector2i getMousePosition() const;
		GameObject* getMainCharacter() const;
		const TileMap* getMap() const;
		unsigned static int windowWidth;
		unsigned static int windowHeight;
		static const char* windowTitle;
	private:
		typedef map<GameObjectID, GameObject*> GameObjectMap;
		GameObjectMap objects;
		GameObjectMap gObjects; //GraphicalGameObjects go here so during rendering it doesn't have to check the other ones
		GameObjectMap uiObjects; //UI objects have an absolute position on the screen so they follow the view. they have no collision either.
		GameObject* mainCharacter = nullptr;
		TileMap* map = nullptr;
	};

	static MusicPlayer musicPlayer;
	static SoundPlayer soundPlayer;
}
#endif