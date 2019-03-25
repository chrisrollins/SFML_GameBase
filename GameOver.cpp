#include "GameOver.h"
#include "ScoreBoard.h"
#include "FileLoadException.h"
#include "ResourceManager.h"
#include "SpriteFactory.h"

GameOver::GameOver(int finalScore, DifficultySettings::DIFFICULTY difficulty) : GraphicalGameObject(SpriteFactory::generateSprite(Sprite::ID::Gameover)), finalScore(finalScore), difficulty(difficulty)
{
	this->spritePtr()->setColor({ 255, 255, 255, 0 });
	this->backSprite = SpriteFactory::generateSprite(Sprite::ID::GameoverBack);
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