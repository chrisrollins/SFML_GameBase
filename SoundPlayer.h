#ifndef SOUNDPLAYER_H
#define SOUNDPLAYER_H

#include "SFML/Audio.hpp"
#include <list>
#include <map>
#include <thread>

namespace SoundEffect
{
	enum ID {
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
	std::map<SoundEffect::ID, std::unique_ptr<sf::SoundBuffer>> mSoundMap;
public:
	void loadSoundEffect(SoundEffect::ID id, const std::string& filename)
	{
		std::unique_ptr<sf::SoundBuffer> sound(new sf::SoundBuffer);
		sound->loadFromFile(filename);
		mSoundMap.insert(std::make_pair(id, std::move(sound)));
	}
	sf::SoundBuffer& getSoundBuffer(SoundEffect::ID id)
	{
		auto found = mSoundMap.find(id);
		return *(found->second);
	}
};

class SoundPlayer : private sf::NonCopyable
{
public:
	SoundPlayer() :mSoundContainer(), mSounds()
	{
		mSoundContainer.loadSoundEffect(SoundEffect::ID::ZombieEat1, "zombie_eat1.ogg");
		mSoundContainer.loadSoundEffect(SoundEffect::ID::ZombieEat2, "zombie_eat2.ogg");
		mSoundContainer.loadSoundEffect(SoundEffect::ID::ZombieEat3, "zombie_eat3.ogg");
		mSoundContainer.loadSoundEffect(SoundEffect::ID::ZombieBurp1, "zombie_burp1.ogg");
		mSoundContainer.loadSoundEffect(SoundEffect::ID::ZombieBurp2, "zombie_burp2.ogg");
		mSoundContainer.loadSoundEffect(SoundEffect::ID::ZombieBurp3, "zombie_burp3.ogg");
		mSoundContainer.loadSoundEffect(SoundEffect::ID::ZombieBurp4, "zombie_burp4.ogg");
		mSoundContainer.loadSoundEffect(SoundEffect::ID::ZombieAttack, "zombie_attack.ogg");
		mSoundContainer.loadSoundEffect(SoundEffect::ID::ZombieGroan, "zombie_hurt.ogg");
		mSoundContainer.loadSoundEffect(SoundEffect::ID::ZombieDeath, "zombie_death.ogg");
		mSoundContainer.loadSoundEffect(SoundEffect::ID::MageDeath, "mage_death.ogg");
		mSoundContainer.loadSoundEffect(SoundEffect::ID::Potion, "potion.ogg");
		mSoundContainer.loadSoundEffect(SoundEffect::ID::Trap, "trap.ogg");
		mSoundContainer.loadSoundEffect(SoundEffect::ID::Alarm, "alarm.ogg");
		mSoundContainer.loadSoundEffect(SoundEffect::ID::MenuClick, "menu_buttonclick.ogg");
	}
	void play(SoundEffect::ID effect, float volume)
	{
		mSounds.push_back(sf::Sound(mSoundContainer.getSoundBuffer(effect)));
		mSounds.back().setVolume(volume);
		mSounds.back().play();
		removeStoppedSounds();
	}
	void removeStoppedSounds()
	{
		mSounds.remove_if([](const sf::Sound& s)
		{
			return s.getStatus() == sf::Sound::Stopped;
		});
	}

private:
	SoundHolder mSoundContainer;
	std::list<sf::Sound> mSounds;
};
#endif
