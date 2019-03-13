#ifndef MUSICPLAYER_H
#define MUSICPLAYER_H

#include <map>
#include <stdexcept>
#include "SFML/Audio.hpp"
#include "FileLoadException.h"

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

class MusicPlayer : private sf::NonCopyable
{
private:
	sf::Music* mMusic = nullptr;
	std::map<Music::ID, std::string> mFilenames;
	float mVolume;
public:
	MusicPlayer() : mMusic(), mFilenames(), mVolume(100.f)
	{
		this->mFilenames[Music::ID::Menu] = "theme_menu.ogg";
		this->mFilenames[Music::ID::TestMode] = "theme_test.ogg";
		this->mFilenames[Music::ID::EasyGame] = "theme_easy.ogg";
		this->mFilenames[Music::ID::NormalGame] = "theme_normal.ogg";
		this->mFilenames[Music::ID::HardGame] = "theme_hard.ogg";
		this->mFilenames[Music::ID::GameOver] = "theme_gameover.ogg";
	}

	~MusicPlayer()
	{
		delete this->mMusic;
	}

	void play(Music::ID theme)
	{
		if (!this->mMusic) { this->mMusic = new sf::Music(); }
		std::string filename = this->mFilenames[theme];
		if (!this->mMusic->openFromFile(filename)) { throw GameException::SoundFileLoadException(filename); }
		this->mMusic->setVolume(mVolume);
		this->mMusic->setLoop(true);
		this->mMusic->play();
	}

	void stop()
	{
		this->mMusic->stop();
	}

	void setPaused(bool paused)
	{
		if (paused) { this->mMusic->pause(); }
		else { this->mMusic->play(); }
	}

	void setVolume(float volume)
	{
		this->mMusic->setVolume(volume);
	}
};

#endif