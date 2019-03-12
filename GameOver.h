#ifndef GAMEOVER_HEADER
#define GAMEOVER_HEADER

#include "SFML/Graphics.hpp"
#include "GameObject.h"
#include "DifficultySettings.h"

using namespace Engine;

class GameOver : public GraphicalGameObject
{
public:
	GameOver(int finalScore, DifficultySettings::DIFFICULTY difficulty);
	void AddedToScreen();
	void EveryFrame(uint64_t f);
	void MouseButtonReleased(sf::Event e);
	void draw(sf::RenderWindow& win);
private:
	DifficultySettings::DIFFICULTY difficulty;
	int finalScore = 0;
	int internalClock = 0;
	sf::Texture mainTexture;
	sf::Texture backTexture;
	sf::Sprite backSprite;
	sf::Sprite* spritePtr() { return dynamic_cast<sf::Sprite*>(this->graphic); }
};

#endif // !GAMEOVER_HEADER
