#ifndef MUSICPLAYER_H
#define MUSICPLAYER_H

#include "SFML/Audio.hpp"
#include "FileLoadException.h"
#include "ResourceManager.h"
#include <map>
#include <string>
#include <functional>

using std::map;
using std::string;
using std::function;
using Engine::ResourceManager;

namespace Music
{
	enum class ID {
		Menu,
		TestMode,
		EasyGame,
		NormalGame,
		HardGame,
		GameOver
	};
}

class MusicWrapper
{
public:
	sf::Music music;
	bool loadFromFile(string filename)
	{
		return music.openFromFile(filename);
	}
};

class MusicPlayer : private sf::NonCopyable
{
private:

	static map<Music::ID, std::string>& getMusicIDMap()
	{
		static map<Music::ID, std::string> musicIDMap;
		return musicIDMap;
	}

	static void InitializeIDMap()
	{
		static bool initialized = false;
		if (initialized) { return; }
		map<Music::ID, std::string>& musicIDMap = getMusicIDMap();
		musicIDMap[Music::ID::Menu] = "theme_menu.ogg";
		musicIDMap[Music::ID::TestMode] = "theme_test.ogg";
		musicIDMap[Music::ID::EasyGame] = "theme_easy.ogg";
		musicIDMap[Music::ID::NormalGame] = "theme_normal.ogg";
		musicIDMap[Music::ID::HardGame] = "theme_hard.ogg";
		musicIDMap[Music::ID::GameOver] = "theme_gameover.ogg";
		initialized = true;
	}

public:	
	static void play(string musicFileName, float volume = 20.f);
	static void play(Music::ID theme, float volume = 20.f);
	static void stop();	
	static void setPaused(bool paused);
	static void setVolume(float volume);
};

#endif