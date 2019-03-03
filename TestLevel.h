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

		static sf::Texture citizen_boy_texture;
		citizen_boy_texture.loadFromFile("boy.png");
		Citizen* boy_ptr = nullptr;
		sf::Sprite boy;
		boy.setTexture(citizen_boy_texture);
		// 2: the initial number
		// 300: respawn this object per 300 frames
		static RespawnManager<Citizen> boyMng(boy, boy_ptr, 2, 300, &levelScreen, &map);
		levelScreen.add(&boyMng);

		static sf::Texture citizen_girl_texture;
		citizen_girl_texture.loadFromFile("girl.png");
		Citizen* girl_ptr = nullptr;
		sf::Sprite girl;
		girl.setTexture(citizen_girl_texture);
		static RespawnManager<Citizen> girlMng(girl, girl_ptr, 2, 300, &levelScreen, &map);
		levelScreen.add(&girlMng);

		static sf::Texture citizen_man_texture;
		citizen_man_texture.loadFromFile("man.png");
		Citizen* man_ptr = nullptr;
		sf::Sprite man;
		man.setTexture(citizen_man_texture);
		static RespawnManager<Citizen> manMng(man, man_ptr, 2, 300, &levelScreen, &map);
		levelScreen.add(&manMng);

		static sf::Texture citizen_woman_texture;
		citizen_woman_texture.loadFromFile("woman.png");
		Citizen* woman_ptr = nullptr;
		sf::Sprite woman;
		woman.setTexture(citizen_woman_texture);
		static RespawnManager<Citizen> womanMng(woman, woman_ptr, 2, 300, &levelScreen, &map);
		levelScreen.add(&womanMng);

		static sf::Texture citizen_oldman_texture;
		citizen_oldman_texture.loadFromFile("oldman.png");
		Citizen* oldman_ptr = nullptr;
		sf::Sprite oldman;
		oldman.setTexture(citizen_oldman_texture);
		static RespawnManager<Citizen> oldmanMng(oldman, oldman_ptr, 2, 300, &levelScreen, &map);
		levelScreen.add(&oldmanMng);

		static sf::Texture citizen_oldwoman_texture;
		citizen_oldwoman_texture.loadFromFile("oldwoman.png");
		Citizen* oldwoman_ptr = nullptr;
		sf::Sprite oldwoman;
		oldwoman.setTexture(citizen_oldwoman_texture);
		static RespawnManager<Citizen> oldwomanMng(oldwoman, oldwoman_ptr, 2, 300, &levelScreen, &map);
		levelScreen.add(&oldwomanMng);

		static sf::Texture soldier_texture;
		soldier_texture.loadFromFile("soldier.png");
		Soldier* soldier_ptr = nullptr;
		sf::Sprite soldier;
		soldier.setTexture(soldier_texture);
		static RespawnManager<Soldier> soldierMng(soldier, soldier_ptr, 10, 200, &levelScreen, &map);
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
