#ifndef SPRITE_FACTORY_H
#define SPRITE_FACTORY_H

#include "SFML/Graphics.hpp"
#include "ResourceManager.h"
#include <string>
#include <map>
#include <vector>
#include <cstdlib>
#include <ctime>

using std::string;
using std::map;
using std::vector;

namespace Engine
{
	namespace Sprite
	{
		enum class ID
		{
			AnimatedPotion,
			AntimagePotion,
			Blast,
			Bloodyhands,
			BookButton,
			Boy,
			Brain,
			BrainIcon,
			Bullet,
			EasyButton,
			Gameover,
			GameoverBack,
			Girl,
			Guardian,
			InsaneButton,
			Mage,
			Mageblast,
			Man,
			MenuBackground,
			MenuEasy,
			MenuEscape,
			MenuInsane,
			MenuNormal,
			MenuScore,
			MenuTutorial,
			NormalButton,
			Oldman,
			Oldwoman,
			Scoreboard,
			Story,
			SuperZombieBlast,
			Tileset,
			Tutorial,
			Woman,
			XButton,
			Zombie
		};
	}

	class SpriteFactory
	{
	public:
		static sf::Sprite generateSprite(string textureFilename)
		{
			sf::Texture* texturePtr = ResourceManager<sf::Texture>::GetResource(textureFilename);
			sf::Sprite sprite;
			sprite.setTexture(*texturePtr);
			return sprite;
		}

		static sf::Sprite generateSprite(Sprite::ID id)
		{
			initializeMap();
			map<Sprite::ID, string>& idMap = getIDMap();
			auto pair = idMap.find(id);
			if (pair == idMap.end()) { return sf::Sprite(); }
			return generateSprite((*pair).second);
		}

	private:
		static void initializeMap()
		{
			static bool mapInitialized = false;
			if (mapInitialized) { return; }
			map<Sprite::ID, string>& idMap = getIDMap();
			idMap[Sprite::ID::AnimatedPotion] = "animated_potion.png";
			idMap[Sprite::ID::AntimagePotion] = "antimage_potion.png";
			idMap[Sprite::ID::Blast] = "blast.png";
			idMap[Sprite::ID::Bloodyhands] = "bloodyhands.png";
			idMap[Sprite::ID::BookButton] = "book_button.png";
			idMap[Sprite::ID::Boy] = "boy.png";
			idMap[Sprite::ID::Brain] = "brain.png";
			idMap[Sprite::ID::BrainIcon] = "brain_icon.png";
			idMap[Sprite::ID::Bullet] = "bullet.png";
			idMap[Sprite::ID::EasyButton] = "easy_button.png";
			idMap[Sprite::ID::Gameover] = "gameover.png";
			idMap[Sprite::ID::GameoverBack] = "gameover_back.png";
			idMap[Sprite::ID::Girl] = "girl.png";
			idMap[Sprite::ID::Guardian] = "guardian.png";
			idMap[Sprite::ID::InsaneButton] = "insane_button.png";
			idMap[Sprite::ID::Mage] = "mage.png";
			idMap[Sprite::ID::Mageblast] = "mageblast.png";
			idMap[Sprite::ID::Man] = "man.png";
			idMap[Sprite::ID::MenuBackground] = "menu_background.png";
			idMap[Sprite::ID::MenuEasy] = "menu_easy.png";
			idMap[Sprite::ID::MenuEscape] = "menu_escape.png";
			idMap[Sprite::ID::MenuInsane] = "menu_insane.png";
			idMap[Sprite::ID::MenuNormal] = "menu_normal.png";
			idMap[Sprite::ID::MenuScore] = "menu_score.png";
			idMap[Sprite::ID::MenuTutorial] = "menu_tutorial.png";
			idMap[Sprite::ID::NormalButton] = "normal_button.png";
			idMap[Sprite::ID::Oldman] = "oldman.png";
			idMap[Sprite::ID::Oldwoman] = "oldwoman.png";
			idMap[Sprite::ID::Scoreboard] = "scoreboard.png";
			idMap[Sprite::ID::Story] = "story.png";
			idMap[Sprite::ID::SuperZombieBlast] = "super_zombie_blast.png";
			idMap[Sprite::ID::Tileset] = "tileset.png";
			idMap[Sprite::ID::Tutorial] = "tutorial.png";
			idMap[Sprite::ID::Woman] = "woman.png";
			idMap[Sprite::ID::XButton] = "x_button.png";
			idMap[Sprite::ID::Zombie] = "zombie.png";
			mapInitialized = true;
		}

		static map<Sprite::ID, string>& getIDMap()
		{
			static map<Sprite::ID, string> idMap;
			return idMap;
		}
	};
}
#endif
