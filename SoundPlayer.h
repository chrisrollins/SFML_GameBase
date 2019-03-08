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
		ZombieAttack,
		ZombieGroan,
		ZombieDeath,
		MageDeath,
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
		mSoundContainer.loadSoundEffect(SoundEffect::ZombieEat1, "zombie_eat1.ogg");
		mSoundContainer.loadSoundEffect(SoundEffect::ZombieEat2, "zombie_eat2.ogg");
		mSoundContainer.loadSoundEffect(SoundEffect::ZombieEat3, "zombie_eat3.ogg");
		mSoundContainer.loadSoundEffect(SoundEffect::ZombieAttack, "zombie_attack.ogg");
		mSoundContainer.loadSoundEffect(SoundEffect::ZombieGroan, "zombie_hurt.ogg");
		mSoundContainer.loadSoundEffect(SoundEffect::ZombieDeath, "zombie_death.ogg");
		mSoundContainer.loadSoundEffect(SoundEffect::MageDeath, "mage_death.ogg");
		mSoundContainer.loadSoundEffect(SoundEffect::Alarm, "alarm.ogg");
		mSoundContainer.loadSoundEffect(SoundEffect::MenuClick, "menu_buttonclick.ogg");
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
