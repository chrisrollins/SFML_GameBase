#ifndef SOUNDPLAYER_H
#define SOUNDPLAYER_H

#include "SFML/Audio.hpp"
#include "FileLoadException.h"
#include <list>
#include <map>
#include <memory>

namespace SoundEffect
{
	enum class ID {
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

class SoundHolder
{
private:
	std::map<SoundEffect::ID, sf::SoundBuffer> mSoundMap;
public:
	bool loadSoundEffect(SoundEffect::ID id, const std::string& filename)
	{
		this->mSoundMap.emplace(id, sf::SoundBuffer());
		return this->mSoundMap[id].loadFromFile(filename);
	}

	sf::SoundBuffer& getSoundBuffer(SoundEffect::ID id)
	{
		auto found = this->mSoundMap.find(id);
		return found->second;
	}
};

class SoundPlayer : private sf::NonCopyable
{
private:
	SoundHolder mSoundContainer;
	std::list<sf::Sound> mSounds;
public:
	SoundPlayer() :mSoundContainer(), mSounds()
	{
#define LOAD(id, soundFile) if(!this->mSoundContainer.loadSoundEffect(id, soundFile)) { throw GameException::SoundFileLoadException(soundFile); }
		LOAD(SoundEffect::ID::ZombieEat1, "zombie_eat1.ogg");
		LOAD(SoundEffect::ID::ZombieEat2, "zombie_eat2.ogg");
		LOAD(SoundEffect::ID::ZombieEat3, "zombie_eat3.ogg");
		LOAD(SoundEffect::ID::ZombieBurp1, "zombie_burp1.ogg");
		LOAD(SoundEffect::ID::ZombieBurp2, "zombie_burp2.ogg");
		LOAD(SoundEffect::ID::ZombieBurp3, "zombie_burp3.ogg");
		LOAD(SoundEffect::ID::ZombieBurp4, "zombie_burp4.ogg");
		LOAD(SoundEffect::ID::ZombieAttack, "zombie_attack.ogg");
		LOAD(SoundEffect::ID::ZombieGroan, "zombie_hurt.ogg");
		LOAD(SoundEffect::ID::ZombieDeath, "zombie_death.ogg");
		LOAD(SoundEffect::ID::MageDeath, "mage_death.ogg");
		LOAD(SoundEffect::ID::Potion, "potion.ogg");
		LOAD(SoundEffect::ID::Trap, "trap.ogg");
		LOAD(SoundEffect::ID::Alarm, "alarm.ogg");
		LOAD(SoundEffect::ID::MenuClick, "menu_buttonclick.ogg");
#undef LOAD
	}

	void play(SoundEffect::ID effect, float volume)
	{
		this->mSounds.push_back(sf::Sound(this->mSoundContainer.getSoundBuffer(effect)));
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
