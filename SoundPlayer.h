#ifndef SOUNDPLAYER_H
#define SOUNDPLAYER_H

#include "SFML/Audio.hpp"
#include "FileLoadException.h"
#include "Screen.h"
#include <list>
#include <map>
#include <thread>

using std::map;
using std::list;
using std::string;

namespace SoundEffect
{
	enum class ID
	{	
		ZombieEat1,
		ZombieEat2,
		ZombieEat3,
		ZombieBurp1,
		ZombieBurp2,
		ZombieBurp3,
		ZombieBurp4,
		ZombieAttack,
		ZombieGroan,
		ZombieDeath,
		MageDeath,
		Potion,
		Trap,
		Alarm,
		MenuClick
	};
}

class SoundPlayer : private sf::NonCopyable
{
private:
	map<SoundEffect::ID, string> resourceMap;
	list<sf::Sound> mSounds;
public:
	SoundPlayer()
	{
		this->resourceMap[SoundEffect::ID::ZombieEat1] = "zombie_eat1.ogg";
		this->resourceMap[SoundEffect::ID::ZombieEat2] = "zombie_eat2.ogg";
		this->resourceMap[SoundEffect::ID::ZombieEat3] = "zombie_eat3.ogg";
		this->resourceMap[SoundEffect::ID::ZombieBurp1] = "zombie_burp1.ogg";
		this->resourceMap[SoundEffect::ID::ZombieBurp2] = "zombie_burp2.ogg";
		this->resourceMap[SoundEffect::ID::ZombieBurp3] = "zombie_burp3.ogg";
		this->resourceMap[SoundEffect::ID::ZombieBurp4] = "zombie_burp4.ogg";
		this->resourceMap[SoundEffect::ID::ZombieAttack] = "zombie_attack.ogg";
		this->resourceMap[SoundEffect::ID::ZombieGroan] = "zombie_hurt.ogg";
		this->resourceMap[SoundEffect::ID::ZombieDeath] = "zombie_death.ogg";
		this->resourceMap[SoundEffect::ID::MageDeath] = "mage_death.ogg";
		this->resourceMap[SoundEffect::ID::Potion] = "potion.ogg";
		this->resourceMap[SoundEffect::ID::Trap] = "trap.ogg";
		this->resourceMap[SoundEffect::ID::Alarm] = "alarm.ogg";
		this->resourceMap[SoundEffect::ID::MenuClick] = "menu_buttonclick.ogg";
	}

	void preloadSounds()
	{
		for (auto iter : this->resourceMap)
		{
			ResourceManager<sf::SoundBuffer>::GetResource(iter.second);	
		}
	}

	void play(SoundEffect::ID effect, float volume)
	{		
		string soundFile = this->resourceMap[effect];
		sf::Sound sound = sf::Sound(*ResourceManager<sf::SoundBuffer>::GetResource(soundFile));
		this->mSounds.push_back(sound);
		this->mSounds.back().setVolume(volume);
		this->mSounds.back().play();
		removeStoppedSounds();
	}

	void removeStoppedSounds()
	{
		this->mSounds.remove_if([](const sf::Sound& s) { return s.getStatus() == sf::Sound::Stopped; });
	}
};

#endif
