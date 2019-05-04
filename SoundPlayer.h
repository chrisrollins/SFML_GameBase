#ifndef SOUNDPLAYER_H
#define SOUNDPLAYER_H

#include "SFML/Audio.hpp"
#include "FileLoadException.h"
#include "ResourceManager.h"
#include "Screen.h"
#include <list>
#include <map>
#include <thread>

using std::map;
using std::list;
using std::string;
using Engine::ResourceManager;

namespace Engine
{

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
			MageAttack1,
			MageAttack2,
			StrongMageAttack,
			StrongMageCharge,
			Potion,
			Trap,
			Alarm,
			MenuClick
		};
	}

	class SoundPlayer : private sf::NonCopyable
	{
	private:
		static list<sf::Sound>& getSoundList()
		{
			static list<sf::Sound> soundList;
			return soundList;
		}

		static map<SoundEffect::ID, string>& getIDMap()
		{
			static map<SoundEffect::ID, string> idMap;
			return idMap;
		}

		static void initializeIDMap()
		{
			static bool initialized = false;
			if (initialized) { return; }
			map<SoundEffect::ID, string>& idMap = getIDMap();
			idMap[SoundEffect::ID::ZombieEat1] = "zombie_eat1.ogg";
			idMap[SoundEffect::ID::ZombieEat2] = "zombie_eat2.ogg";
			idMap[SoundEffect::ID::ZombieEat3] = "zombie_eat3.ogg";
			idMap[SoundEffect::ID::ZombieBurp1] = "zombie_burp1.ogg";
			idMap[SoundEffect::ID::ZombieBurp2] = "zombie_burp2.ogg";
			idMap[SoundEffect::ID::ZombieBurp3] = "zombie_burp3.ogg";
			idMap[SoundEffect::ID::ZombieBurp4] = "zombie_burp4.ogg";
			idMap[SoundEffect::ID::ZombieAttack] = "zombie_attack.ogg";
			idMap[SoundEffect::ID::ZombieGroan] = "zombie_hurt.ogg";
			idMap[SoundEffect::ID::ZombieDeath] = "zombie_death.ogg";
			idMap[SoundEffect::ID::MageDeath] = "mage_death.ogg";
			idMap[SoundEffect::ID::MageAttack1] = "mage_attack.ogg";
			idMap[SoundEffect::ID::MageAttack2] = "mage_attack2.ogg";
			idMap[SoundEffect::ID::StrongMageAttack] = "strong_mage_attack.ogg";
			idMap[SoundEffect::ID::StrongMageCharge] = "strong_mage_charge.ogg";
			idMap[SoundEffect::ID::Potion] = "potion.ogg";
			idMap[SoundEffect::ID::Trap] = "trap.ogg";
			idMap[SoundEffect::ID::Alarm] = "alarm.ogg";
			idMap[SoundEffect::ID::MenuClick] = "menu_buttonclick.ogg";
			initialized = true;
		}
	public:

		static void preloadSounds()
		{
			for (auto iter : getIDMap())
			{
				ResourceManager<sf::SoundBuffer>::GetResource(iter.second);
			}
		}

		static sf::Vector2f calculateSoundPosition(sf::Vector2f listenerPos, sf::Vector2f sourcePos)
		{
			sf::Vector2f soundPos = sourcePos - listenerPos;
			soundPos.x /= 100.f;
			soundPos.y /= 100.f;
			return soundPos;
		}

		static void play(string effectFileName, float volume, sf::Vector2f position = { 0.f, 0.f })
		{
			sf::Sound sound = sf::Sound(*ResourceManager<sf::SoundBuffer>::GetResource(effectFileName));
			list<sf::Sound>& soundList = getSoundList();
			soundList.push_back(sound);
			soundList.back().setVolume(volume);
			soundList.back().play();
			soundList.back().setPosition({ position.x, position.y, 0.f });
			soundList.remove_if([](const sf::Sound& s) { return s.getStatus() == sf::Sound::Stopped; });
		}

		static void play(SoundEffect::ID effect, float volume, sf::Vector2f position = { 0.f, 0.f })
		{
			initializeIDMap();
			map<SoundEffect::ID, string>& idMap = getIDMap();
			auto iter = idMap.find(effect);
			if (iter == idMap.end()) { return; }
			play((*iter).second, volume, position);
		}
	};
}

#endif