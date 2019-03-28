#ifndef POTIONUI_H
#define POTIONUI_H

#include "GameObject.h"
#include "MainCharacter.h"
#include "ResourceManager.h"
#include <string>

using namespace Engine;

class PotionUI : public GraphicalGameObject
{
private:
	MainCharacter* character = nullptr;
	sf::Text text;
	sf::Sprite* spritePtr() { return dynamic_cast<sf::Sprite*>(this->getGraphic()); }
public:
	PotionUI(sf::Sprite s) : GraphicalGameObject(s)
	{
		this->spritePtr()->setPosition(50.f, 70.f);
		sf::Font* font = ResourceManager<sf::Font>::GetResource("zombie.ttf");
		this->text.setFont(*font);
		this->text.setStyle(sf::Text::Bold);
		this->text.setLetterSpacing(3.f);
		this->text.setOutlineColor({ 163, 19, 88 });
		this->text.setOutlineThickness(2.f);
		this->text.setFillColor(sf::Color::Black);
	}

	void setCharacter(MainCharacter * mc)
	{
		this->character = mc;
	}

	void EveryFrame(uint64_t f)
	{
		std::string currNum = std::to_string(this->character->getPotionNum()) + "/" + std::to_string(this->character->getMaxPotionNum());
		this->text.setString(currNum);
	}

	void draw(sf::RenderWindow& win)
	{
		win.draw(*this->spritePtr());
		this->text.setPosition(this->spritePtr()->getPosition().x + 50, this->spritePtr()->getPosition().y - 5);
		win.draw(this->text);
	}
};

#endif
