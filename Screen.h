#ifndef SCREEN_HEADER
#define SCREEN_HEADER

#include "SFML/Graphics.hpp"
#include "GameObject.h"
#include "TileMap.h"
#include "MusicPlayer.h"
#include "SoundPlayer.h"
#include <map>

namespace Engine
{
	class Screen
	{
	public:
		Screen();
		~Screen();
		void addMap(TileMap* map);
		void addMainCharacter(GameObject* mainCharacter);
		void addUIObject(GameObject* uiObj);
		void add(GameObject* gameObject);
		void remove(GameObject* gameObject);
		void render(int fps = 60);
		void close();
		sf::Vector2i getMousePosition() const;
		GameObject* getMainCharacter() const;
		const TileMap* getMap() const;
		unsigned static int windowWidth;
		unsigned static int windowHeight;
		static const char* windowTitle;
	private:
		typedef std::map<GameObjectID, GameObject*> GameObjectMap;
		GameObjectMap objects;
		GameObjectMap g_objects; //GraphicalGameObjects go here so during rendering it doesn't have to check the other ones
		GameObjectMap ui_objects; //UI objects have an absolute position on the screen so they follow the view. they have no collision either.
		GameObject* mainCharacter = nullptr;
		TileMap* map = nullptr;
	};

	static MusicPlayer musicPlayer;
	static SoundPlayer soundPlayer;
}
#endif