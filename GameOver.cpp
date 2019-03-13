#include "GameOver.h"
#include "ScoreBoard.h"

GameOver::GameOver(int finalScore, DifficultySettings::DIFFICULTY difficulty) : GraphicalGameObject(sf::Sprite()), finalScore(finalScore), difficulty(difficulty)
{
	this->mainTexture.loadFromFile("gameover.png");
	this->spritePtr()->setTexture(this->mainTexture);
	this->spritePtr()->setColor({ 255, 255, 255, 0 });
	this->backTexture.loadFromFile("gameover_back.png");
	this->backSprite.setTexture(this->backTexture);
	this->backSprite.setColor({ 255, 255, 255, 255 });
}

void GameOver::AddedToScreen()
{
	sf::IntRect size = this->spritePtr()->getTextureRect();
	sf::Vector2f pos;
	pos.x = static_cast<float>(Screen::windowWidth / 2 - size.width / 2);
	pos.y = static_cast<float>(Screen::windowHeight / 2 - size.height / 2);
	this->spritePtr()->setPosition(pos);
}

void GameOver::EveryFrame(uint64_t f)
{
	if (this->internalClock++ < 255)
	{
		sf::Color color = this->spritePtr()->getColor();
		color.a++;
		this->spritePtr()->setColor(color);
	}
}

void GameOver::MouseButtonReleased(sf::Event e)
{
	if (e.mouseButton.button != sf::Mouse::Button::Left) { return; }
	sf::Vector2i mouse = this->screen->getMousePosition();
	if (this->backSprite.getGlobalBounds().contains(static_cast<float>(mouse.x), static_cast<float>(mouse.y)))
	{
		MainCharacter* mc = dynamic_cast<MainCharacter*>(this->screen->getMainCharacter());
		ScoreBoard scoreBoard;
		scoreBoard.add(ScoreEntry(mc->getName(), this->finalScore, static_cast<int>(mc->getTotalAliveTime())), this->difficulty);
		this->screen->remove(this);
		Menu* menu = new Menu();
		menu->start();
	}
}

void GameOver::draw(sf::RenderWindow& win)
{
	win.draw(*this->spritePtr());
	if (this->internalClock > 255)
	{
		sf::Vector2f pos = this->spritePtr()->getPosition();
		sf::IntRect backSize = this->backSprite.getTextureRect();
		pos.x += static_cast<float>(backSize.width) * 1.5f;
		pos.y += 150.f;
		this->backSprite.setPosition(pos);
		win.draw(this->backSprite);
	}
}