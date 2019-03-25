#ifndef MUSICPLAYER_H
#define MUSICPLAYER_H

#include "SFML/Audio.hpp"
#include "FileLoadException.h"
#include "ResourceManager.h"
#include <map>
#include <string>

using std::map;
using std::string;
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
	sf::Music* currentMusic = nullptr;
	std::map<Music::ID, std::string> mFilenames;
	float mVolume;
public:
	MusicPlayer() : currentMusic(), mFilenames(), mVolume(100.f)
	{
		this->mFilenames[Music::ID::Menu] = "theme_menu.ogg";
		this->mFilenames[Music::ID::TestMode] = "theme_test.ogg";
		this->mFilenames[Music::ID::EasyGame] = "theme_easy.ogg";
		this->mFilenames[Music::ID::NormalGame] = "theme_normal.ogg";
		this->mFilenames[Music::ID::HardGame] = "theme_hard.ogg";
		this->mFilenames[Music::ID::GameOver] = "theme_gameover.ogg";
	}

	void play(Music::ID theme)
	{
		this->stop();
		std::string filename = this->mFilenames[theme];
		MusicWrapper* wrapper = ResourceManager<MusicWrapper>::GetResource(filename);
		this->currentMusic = &wrapper->music;
		wrapper->music.setVolume(mVolume);
		wrapper->music.setLoop(true);
		wrapper->music.play();
	}

	void stop()
	{
		if (this->currentMusic) { this->currentMusic->stop(); }
	}

	void setPaused(bool paused)
	{
		if (paused) { this->currentMusic->pause(); }
		else { this->currentMusic->play(); }
	}

	void setVolume(float volume)
	{
		this->currentMusic->setVolume(volume);
	}
};

#endif