#ifndef TESTLEVEL_H
#define TESTLEVEL_H

#include "MainCharacter.h"
#include "HealthBar.h"
#include "PotionUI.h"
#include "TimerUI.h"
#include "FileLoadException.h"

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

		if (!map.load(DifficultySettings::Map::picture, DifficultySettings::Map::fileName)) { throw GameException::ImageFileLoadException(DifficultySettings::Map::fileName); }
		levelScreen->addMap(&map);

		static sf::Texture mTexture;
		if (!mTexture.loadFromFile("zombie.png")) { throw GameException::ImageFileLoadException("zombie.png"); }
		sf::Sprite mCharacter;
		mCharacter.setTexture(mTexture);
		mCharacter.setPosition(static_cast<float>(map.width() * map.tileSize().x / 2),
			static_cast<float>(map.height() * map.tileSize().y / 2));
		MainCharacter* mcPtr = new MainCharacter(mCharacter, playerName);
		levelScreen->addMainCharacter(mcPtr);

		static sf::Texture potionUITexture;
		if (!potionUITexture.loadFromFile("brain_icon.png")) { throw GameException::ImageFileLoadException("brain_icon.png"); }
		sf::Sprite potionIcon;
		potionIcon.setTexture(potionUITexture);
		static PotionUI potionUI(potionIcon);
		potionUI.setCharacter(mcPtr);
		levelScreen->addUIObject(&potionUI);

		static sf::Texture boyTexture;
		if (!boyTexture.loadFromFile("boy.png")) { throw GameException::ImageFileLoadException("boy.png"); }
		sf::Sprite boy;
		boy.setTexture(boyTexture);
		// 5: the max number
		// 200: respawn this object per 200 frames
		static RespawnManager<Citizen> boyMng(boy, 5, 200);
		boyMng.clear();
		levelScreen->add(&boyMng);

		static sf::Texture girlTexture;
		if (!girlTexture.loadFromFile("girl.png")) { throw GameException::ImageFileLoadException("girl.png"); }
		sf::Sprite girl;
		girl.setTexture(girlTexture);
		static RespawnManager<Citizen> girlMng(girl, 3, 200);
		girlMng.clear();
		levelScreen->add(&girlMng);

		static sf::Texture manTexture;
		if (!manTexture.loadFromFile("man.png")) { throw GameException::ImageFileLoadException("man.png"); }
		sf::Sprite man;
		man.setTexture(manTexture);
		static RespawnManager<Citizen> manMng(man, 3, 200);
		manMng.clear();
		levelScreen->add(&manMng);

		static sf::Texture womanTexture;
		if (!womanTexture.loadFromFile("woman.png")) { throw GameException::ImageFileLoadException("woman.png"); }
		sf::Sprite woman;
		woman.setTexture(womanTexture);
		static RespawnManager<Citizen> womanMng(woman, 3, 200);
		womanMng.clear();
		levelScreen->add(&womanMng);

		static sf::Texture oldmanTexture;
		if (!oldmanTexture.loadFromFile("oldman.png")) { throw GameException::ImageFileLoadException("oldman.png"); }
		sf::Sprite oldman;
		oldman.setTexture(oldmanTexture);
		static RespawnManager<Citizen> oldmanMng(oldman, 3, 200);
		oldmanMng.clear();
		levelScreen->add(&oldmanMng);

		static sf::Texture oldwomanTexture;
		if (!oldwomanTexture.loadFromFile("oldwoman.png")) { throw GameException::ImageFileLoadException("oldwoman.png"); }
		sf::Sprite oldwoman;
		oldwoman.setTexture(oldwomanTexture);
		static RespawnManager<Citizen> oldwomanMng(oldwoman, 3, 200);
		oldwomanMng.clear();
		levelScreen->add(&oldwomanMng);

		static sf::Texture mageTexture;
		if (!mageTexture.loadFromFile("mage.png")) { throw GameException::ImageFileLoadException("mage.png"); }
		sf::Sprite mage;
		mage.setTexture(mageTexture);
		static RespawnManager<Mage> soldierMng(mage, 12, 175);
		soldierMng.clear();
		levelScreen->add(&soldierMng);

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
			if (GameObject* oldMC = oldScreen->getMainCharacter()) { delete oldMC; }
			delete oldScreen;
			oldScreen = levelScreen;
		}

		numMagesAlive = 0;

		levelScreen->render();
	}
};

#endif
