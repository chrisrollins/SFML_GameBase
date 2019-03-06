#ifndef TEST_LEVEL_HEADER
#define TEST_LEVEL_HEADER

#include <SFML/Graphics.hpp>
#include <iostream>
#include <thread>
#include <vector>
#include <ctime>
#include "Screen.h"
#include "GameObject.h"
#include "MainCharacter.h"
#include "Mage.h"
#include "Citizen.h"
#include "SampleUIObject.h"
#include "SampleUIText.h"
#include "Score.h"
#include "HealthBar.h"
#include "RespawnManager.h"

using namespace Engine;

class TestLevel
{
public:
	TestLevel() {}
	void start()
	{
		static Screen levelScreen;
		static TileMap map;
		
		map.load("map.png", "map.txt");
		levelScreen.addMap(&map);

		sf::Sprite m_Sprite;
		m_Sprite.setPosition(Screen::windowWidth / 2, Screen::windowHeight / 2);
		static sf::Texture m_Texture;
		m_Texture.loadFromFile("zombie.png");
		m_Sprite.setTexture(m_Texture);
		MainCharacter* mc_ptr = new MainCharacter(m_Sprite);
		levelScreen.addMainCharacter(mc_ptr);

		static sf::Texture citizen_boy_texture;
		citizen_boy_texture.loadFromFile("boy.png");
		sf::Sprite boy;
		boy.setTexture(citizen_boy_texture);
		// 3: the max number
		// 200: respawn this object per 200 frames
		static RespawnManager<Citizen> boyMng(boy, 5, 200);
		levelScreen.add(&boyMng);

		static sf::Texture citizen_girl_texture;
		citizen_girl_texture.loadFromFile("girl.png");
		sf::Sprite girl;
		girl.setTexture(citizen_girl_texture);
		static RespawnManager<Citizen> girlMng(girl, 3, 200);
		levelScreen.add(&girlMng);

		static sf::Texture citizen_man_texture;
		citizen_man_texture.loadFromFile("man.png");
		sf::Sprite man;
		man.setTexture(citizen_man_texture);
		static RespawnManager<Citizen> manMng(man, 3, 200);
		levelScreen.add(&manMng);

		static sf::Texture citizen_woman_texture;
		citizen_woman_texture.loadFromFile("woman.png");
		sf::Sprite woman;
		woman.setTexture(citizen_woman_texture);
		static RespawnManager<Citizen> womanMng(woman, 3, 200);
		levelScreen.add(&womanMng);

		static sf::Texture citizen_oldman_texture;
		citizen_oldman_texture.loadFromFile("oldman.png");
		sf::Sprite oldman;
		oldman.setTexture(citizen_oldman_texture);
		static RespawnManager<Citizen> oldmanMng(oldman, 3, 200);
		levelScreen.add(&oldmanMng);

		static sf::Texture citizen_oldwoman_texture;
		citizen_oldwoman_texture.loadFromFile("oldwoman.png");
		sf::Sprite oldwoman;
		oldwoman.setTexture(citizen_oldwoman_texture);
		static RespawnManager<Citizen> oldwomanMng(oldwoman, 3, 200);
		levelScreen.add(&oldwomanMng);

		static sf::Texture mage_texture;
		mage_texture.loadFromFile("mage.png");
		sf::Sprite mage;
		mage.setTexture(mage_texture);
		static RespawnManager<Mage> soldierMng(mage, 12, 175);
		levelScreen.add(&soldierMng);

		static HealthBar healthbar;
		healthbar.setCharacter(mc_ptr);
		levelScreen.addUIObject(&healthbar);
		
		levelScreen.render();
	}
};

#endif
