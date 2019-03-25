#include "MusicPlayer.h"
#include "SFML/Audio.hpp"
#include "FileLoadException.h"
#include "ResourceManager.h"
#include <map>
#include <string>

using std::map;
using std::string;
using std::function;

static sf::Music* musicPtr = nullptr;

void MusicPlayer::play(string musicFileName, float volume)
{
	stop();
	MusicWrapper* wrapper = ResourceManager<MusicWrapper>::GetResource(musicFileName);
	musicPtr = &(wrapper->music);
	musicPtr->setVolume(volume);
	musicPtr->setLoop(true);
	musicPtr->play();		
}

void MusicPlayer::play(Music::ID theme, float volume)
{
	InitializeIDMap();
	map<Music::ID, std::string>& musicIDMap = getMusicIDMap();
	auto iter = musicIDMap.find(theme);
	if (iter == musicIDMap.end()) { return; }
	play((*iter).second, volume);
}

void MusicPlayer::stop()
{
	if (!musicPtr) { return; }
	musicPtr->stop();
}

void MusicPlayer::setPaused(bool paused)
{
	if (!musicPtr) { return; }
	if (paused) { musicPtr->pause(); }
	else { musicPtr->play(); }
}

void MusicPlayer::setVolume(float volume)
{
	if (!musicPtr) { return; }
	musicPtr->setVolume(volume);
}