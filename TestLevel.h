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
#include "Soldier.h"
#include "Citizen.h"
#include "SampleUIObject.h"
#include "SampleUIText.h"
#include "getMap.h"
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

		map.load("map.png", "map1.txt", sf::Vector2u(32, 32));
		levelScreen.addMap(&map);

		sf::Sprite m_Sprite;
		m_Sprite.setPosition(Screen::windowWidth / 2, Screen::windowHeight / 2);
		static sf::Texture m_Texture;
		m_Texture.loadFromFile("zombie.png");
		m_Sprite.setTexture(m_Texture);
		MainCharacter* mc_ptr = new MainCharacter(m_Sprite);
		levelScreen.addMainCharacter(mc_ptr);

		std::vector<sf::Vector2f> respawnPositions = { {140, 180}, {600, 675}, {425, 1000}, {1100, 900}, {1000, 300} };

		static sf::Texture citizen_boy_texture;
		citizen_boy_texture.loadFromFile("boy.png");
		sf::Sprite boy;
		boy.setTexture(citizen_boy_texture);
		// 4: the max number
		// 200: respawn this object per 200 frames
		static RespawnManager<Citizen> boyMng(boy, respawnPositions, 6, 200);
		levelScreen.add(&boyMng);

		static sf::Texture citizen_girl_texture;
		citizen_girl_texture.loadFromFile("girl.png");
		sf::Sprite girl;
		girl.setTexture(citizen_girl_texture);
		static RespawnManager<Citizen> girlMng(girl, respawnPositions, 6, 200);
		levelScreen.add(&girlMng);

		static sf::Texture citizen_man_texture;
		citizen_man_texture.loadFromFile("man.png");
		sf::Sprite man;
		man.setTexture(citizen_man_texture);
		static RespawnManager<Citizen> manMng(man, respawnPositions, 5, 200);
		levelScreen.add(&manMng);

		static sf::Texture citizen_woman_texture;
		citizen_woman_texture.loadFromFile("woman.png");
		sf::Sprite woman;
		woman.setTexture(citizen_woman_texture);
		static RespawnManager<Citizen> womanMng(woman, respawnPositions, 5, 200);
		levelScreen.add(&womanMng);

		static sf::Texture citizen_oldman_texture;
		citizen_oldman_texture.loadFromFile("oldman.png");
		sf::Sprite oldman;
		oldman.setTexture(citizen_oldman_texture);
		static RespawnManager<Citizen> oldmanMng(oldman, respawnPositions, 3, 200);
		levelScreen.add(&oldmanMng);

		static sf::Texture citizen_oldwoman_texture;
		citizen_oldwoman_texture.loadFromFile("oldwoman.png");
		sf::Sprite oldwoman;
		oldwoman.setTexture(citizen_oldwoman_texture);
		static RespawnManager<Citizen> oldwomanMng(oldwoman, respawnPositions, 3, 200);
		levelScreen.add(&oldwomanMng);

		static sf::Texture soldier_texture;
		soldier_texture.loadFromFile("soldier.png");
		sf::Sprite soldier;
		soldier.setTexture(soldier_texture);
		static RespawnManager<Soldier> soldierMng(soldier, respawnPositions, 12, 175);
		levelScreen.add(&soldierMng);

		static HealthBar healthbar;
		healthbar.setCharacter(mc_ptr);
		levelScreen.addUIObject(&healthbar);

		static Score s(0, sf::Color::Cyan, 32, 1000, 0);
		levelScreen.addUIObject(&s);

		levelScreen.render();
	}
};

#endif
