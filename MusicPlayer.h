#ifndef MusicPlayer_h
#define MusicPlayer_h

#include "SFML/Audio.hpp"
#include <map>
#include <stdexcept>

namespace Music
{
	enum ID {
		Menu,
		EasyGame,
		NormalGame,
		HardGame
	};
}

class MusicPlayer : private sf::NonCopyable
{
public:
	MusicPlayer();
	void play(Music::ID theme);
	void stop();
	void setPaused(bool paused);
	void setVolume(float volume);
private:
	sf::Music mMusic;
	std::map<Music::ID, std::string> mFilenames;
	float mVolume;

};
MusicPlayer::MusicPlayer(): mMusic(), mFilenames(), mVolume(100.f)
{
	mFilenames[Music::Menu] = "theme_menu.ogg";
	mFilenames[Music::EasyGame] = "theme_easy.ogg";
	mFilenames[Music::NormalGame] = "theme_normal.ogg";
	mFilenames[Music::HardGame] = "theme_hard.ogg";
}

void MusicPlayer::play(Music::ID theme)
{
	//mMusic.openFromFile("MenuTheme.ogg");
	std::string filename = mFilenames[theme];
	if (!mMusic.openFromFile(filename))
		throw std::runtime_error("Music " + filename + " could not be loaded.");
	mMusic.setVolume(mVolume);
	mMusic.setLoop(true);
	mMusic.play();
}

void MusicPlayer::setVolume(float volume)
{
	mMusic.setVolume(volume);
}

void MusicPlayer::stop()
{
	mMusic.stop();
}

void MusicPlayer::setPaused(bool paused)
{
	if (paused)
		mMusic.pause();
	else
		mMusic.play();
}

#endif