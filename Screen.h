#ifndef SCREEN_HEADER
#define SCREEN_HEADER

#include "SFML\Graphics.hpp"
#include "GameObject.h"
#include <map>
#include <thread>

class Screen
{
public:
	void add(GameObject* gameObject);
	void remove(GameObject* gameObject);
	void render(int fps = 60);
	unsigned static int windowWidth;
	unsigned static int windowHeight;
	static const char* windowTitle;
private:
	typedef std::map<GameObjectID, GameObject*> GameObjectMap;
	GameObjectMap objects;
	GameObjectMap g_objects; //GraphicalGameObjects go here so during rendering it doesn't have to check the other ones
};

#endif
