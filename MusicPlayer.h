#ifndef MUSICPLAYER_H
#define MUSICPLAYER_H

#include "SFML/Audio.hpp"
#include <map>
#include <stdexcept>

namespace Music
{
	enum ID {
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
public:
	MusicPlayer() : mMusic(), mFilenames(), mVolume(100.f)
	{
		mFilenames[Music::Menu] = "theme_menu.ogg";
		mFilenames[Music::TestMode] = "theme_test.ogg";
		mFilenames[Music::EasyGame] = "theme_easy.ogg";
		mFilenames[Music::NormalGame] = "theme_normal.ogg";
		mFilenames[Music::HardGame] = "theme_hard.ogg";
		mFilenames[Music::GameOver] = "theme_gameover.ogg";
	}
	~MusicPlayer()
	{
		delete this->mMusic;
	}
	void play(Music::ID theme)
	{		
		if (!this->mMusic) { this->mMusic = new sf::Music(); }
		std::string filename = mFilenames[theme];
		if (!mMusic->openFromFile(filename)) { throw std::runtime_error("Music " + filename + " could not be loaded."); }
		mMusic->setVolume(mVolume);
		mMusic->setLoop(true);
		mMusic->play();
	}
	void stop()
	{
		mMusic->stop();
	}
	void setPaused(bool paused)
	{
		if (paused) { mMusic->pause(); }
		else { mMusic->play(); }
	}
	void setVolume(float volume)
	{
		mMusic->setVolume(volume);
	}
private:
	sf::Music* mMusic = nullptr;
	std::map<Music::ID, std::string> mFilenames;
	float mVolume;
};

#endif