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
		
		levelScreen->schedule([=]()
		{
			std::vector<sf::Vector2f> spawnPositions = map.getSafeSpawnPositions();
			size_t randIndex = rand() % spawnPositions.size();
			sf::Vector2f position = spawnPositions[randIndex];
			sf::Sprite s = SpriteFactory::generateSprite(Sprite::ID::MageRed);
			s.setPosition(position);
			levelScreen->add(new StrongMage(s));
		}, TimeUnit::Seconds(60), 0);

		//set up the score object
		Score* score = new Score();
		scorePtr = score;
		levelScreen->addUIObject(score);

		levelScreen->addUIObject(new TimerUI());

		if (oldScreen)
		{
			delete oldScreen;
			oldScreen = levelScreen;
		}

		numMagesAlive = 0;

		levelScreen->render();
	}
};

#endif
