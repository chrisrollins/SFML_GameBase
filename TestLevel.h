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

		sf::Sprite m_Sprite;
		m_Sprite.setPosition(static_cast<float>(map.width() * map.tileSize().x / 2),
			static_cast<float>(map.height() * map.tileSize().y / 2));
		static sf::Texture m_Texture;
		if (!m_Texture.loadFromFile("zombie.png")) { throw GameException::ImageFileLoadException("zombie.png"); }
		m_Sprite.setTexture(m_Texture);

		MainCharacter* mc_ptr = new MainCharacter(m_Sprite, playerName);
		levelScreen->addMainCharacter(mc_ptr);

		static sf::Texture potionUI_texture;
		if (!potionUI_texture.loadFromFile("brain_icon.png")) { throw GameException::ImageFileLoadException("brain_icon.png"); }
		sf::Sprite potion_icon;
		potion_icon.setTexture(potionUI_texture);
		static PotionUI potionUI(potion_icon);
		potionUI.setCharacter(mc_ptr);
		levelScreen->addUIObject(&potionUI);

		static sf::Texture citizen_boy_texture;
		if (!citizen_boy_texture.loadFromFile("boy.png")) { throw GameException::ImageFileLoadException("boy.png"); }
		sf::Sprite boy;
		boy.setTexture(citizen_boy_texture);
		// 5: the max number
		// 200: respawn this object per 200 frames
		static RespawnManager<Citizen> boyMng(boy, 5, 200);
		boyMng.clear();
		levelScreen->add(&boyMng);

		static sf::Texture citizen_girl_texture;
		if (!citizen_girl_texture.loadFromFile("girl.png")) { throw GameException::ImageFileLoadException("girl.png"); }
		sf::Sprite girl;
		girl.setTexture(citizen_girl_texture);
		static RespawnManager<Citizen> girlMng(girl, 3, 200);
		girlMng.clear();
		levelScreen->add(&girlMng);

		static sf::Texture citizen_man_texture;
		if (!citizen_man_texture.loadFromFile("man.png")) { throw GameException::ImageFileLoadException("man.png"); }
		sf::Sprite man;
		man.setTexture(citizen_man_texture);
		static RespawnManager<Citizen> manMng(man, 3, 200);
		manMng.clear();
		levelScreen->add(&manMng);

		static sf::Texture citizen_woman_texture;
		if (!citizen_woman_texture.loadFromFile("woman.png")) { throw GameException::ImageFileLoadException("woman.png"); }
		sf::Sprite woman;
		woman.setTexture(citizen_woman_texture);
		static RespawnManager<Citizen> womanMng(woman, 3, 200);
		womanMng.clear();
		levelScreen->add(&womanMng);

		static sf::Texture citizen_oldman_texture;
		if (!citizen_oldman_texture.loadFromFile("oldman.png")) { throw GameException::ImageFileLoadException("oldman.png"); }
		sf::Sprite oldman;
		oldman.setTexture(citizen_oldman_texture);
		static RespawnManager<Citizen> oldmanMng(oldman, 3, 200);
		oldmanMng.clear();
		levelScreen->add(&oldmanMng);

		static sf::Texture citizen_oldwoman_texture;
		if (!citizen_oldwoman_texture.loadFromFile("oldwoman.png")) { throw GameException::ImageFileLoadException("oldwoman.png"); }
		sf::Sprite oldwoman;
		oldwoman.setTexture(citizen_oldwoman_texture);
		static RespawnManager<Citizen> oldwomanMng(oldwoman, 3, 200);
		oldwomanMng.clear();
		levelScreen->add(&oldwomanMng);

		static sf::Texture mage_texture;
		if (!mage_texture.loadFromFile("mage.png")) { throw GameException::ImageFileLoadException("mage.png"); }
		sf::Sprite mage;
		mage.setTexture(mage_texture);
		static RespawnManager<Mage> soldierMng(mage, 12, 175);
		soldierMng.clear();
		levelScreen->add(&soldierMng);

		static HealthBar healthbar;
		healthbar.setCharacter(mc_ptr);
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
		timer.setCharacter(mc_ptr);
		levelScreen->addUIObject(&timer);

		if (oldScreen)
		{
			if (GameObject* oldMC = oldScreen->getMainCharacter()) { delete oldMC; }
			delete oldScreen;
			oldScreen = levelScreen;
		}

		levelScreen->render();
	}
};

#endif
