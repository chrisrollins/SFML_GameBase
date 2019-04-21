#ifndef TESTLEVEL_H
#define TESTLEVEL_H

#include "MainCharacter.h"
#include "HealthBar.h"
#include "PotionUI.h"
#include "TimerUI.h"
#include "FileLoadException.h"
#include "ResourceManager.h"
#include "SpriteFactory.h"

using namespace Engine;

class TestLevel
{
public:
	TestLevel() {}

	void start(std::string playerName)
	{
		static Screen* oldScreen = nullptr;

		Screen* levelScreen = new Screen();
		static TileMap map;

		map.load(DifficultySettings::Map::picture, DifficultySettings::Map::fileName);
		levelScreen->addMap(&map);

		MainCharacter* mcPtr = new MainCharacter(playerName);
		mcPtr->getDrawablePtr()->setPosition(static_cast<float>(map.width() * map.tileSize().x / 2),
			static_cast<float>(map.height() * map.tileSize().y / 2));
		levelScreen->addMainCharacter(mcPtr);

		sf::Sprite potionIcon = SpriteFactory::generateSprite(Sprite::ID::BrainIcon);
		static PotionUI potionUI(potionIcon);
		potionUI.setCharacter(mcPtr);
		levelScreen->addUIObject(&potionUI);

		// 5: the max number
		// 200: respawn this object per 200 frames
		static RespawnManager<Citizen> boyMng(Sprite::ID::Boy, 5, 200);
		boyMng.clear();
		levelScreen->add(&boyMng);

		static RespawnManager<Citizen> girlMng(Sprite::ID::Girl, 3, 200);
		girlMng.clear();
		levelScreen->add(&girlMng);

		static RespawnManager<Citizen> manMng(Sprite::ID::Man, 3, 200);
		manMng.clear();
		levelScreen->add(&manMng);

		static RespawnManager<Citizen> womanMng(Sprite::ID::Woman, 3, 200);
		womanMng.clear();
		levelScreen->add(&womanMng);

		static RespawnManager<Citizen> oldmanMng(Sprite::ID::Oldman, 3, 200);
		oldmanMng.clear();
		levelScreen->add(&oldmanMng);

		static RespawnManager<Citizen> oldwomanMng(Sprite::ID::Oldwoman, 3, 200);
		oldwomanMng.clear();
		levelScreen->add(&oldwomanMng);

		static RespawnManager<Mage> mageMng(Sprite::ID::Mage, 12, 175);
		mageMng.clear();
		levelScreen->add(&mageMng);

		static HealthBar healthbar;
		healthbar.setCharacter(mcPtr);
		levelScreen->addUIObject(&healthbar);

		//set up the score object
		static sf::Text s;
		static Score score(s);
		score.unfreeze();
		score.set(0);
		scorePtr = &score;
		levelScreen->addUIObject(&score);

		static sf::Text t;
		static TimerUI timer(t);
		timer.setCharacter(mcPtr);
		levelScreen->addUIObject(&timer);

		if (oldScreen)
		{
			//if (GameObject* oldMC = oldScreen->getMainCharacter()) { delete oldMC; }
			delete oldScreen;
			oldScreen = levelScreen;
		}

		numMagesAlive = 0;

		levelScreen->render();
	}
};

#endif
