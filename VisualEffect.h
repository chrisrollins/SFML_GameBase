#ifndef VISUAL_EFFECT_H
#define VISUAL_EFFECT_H

#include <functional>
#include "GameObject.h"
#include "Screen.h"
#include "SFML/Graphics.hpp"

namespace Engine
{
	class VisualEffect : public GraphicalGameObject
	{
	private:
		sf::Sprite* spritePtr;
		uint64_t remainingFrames;
		const sf::Sprite* attachedSprite;
		GraphicalGameObject* attachedGameObject;
		sf::Vector2f offset = {0, 0};
		std::function<void(VisualEffect*, uint64_t)> everyFrameAdditionalAction;

		void updatePosition()
		{
			if (this->attachedSprite != nullptr) { this->spritePtr->setPosition(attachedSprite->getPosition() + this->offset); }
		}

		void EveryFrame(uint64_t f)
		{
			this->remainingFrames--;
			this->updatePosition();
			this->everyFrameAdditionalAction(this, this->remainingFrames);
			if (this->remainingFrames == 0) { this->screen->remove(this); }
		}

		void RemovedFromScreen()
		{
			if (this->attachedGameObject == nullptr) { return; }
			this->attachedGameObject->preserved = false;
			if (attachedGameObject->removed) { this->screen->remove(this->attachedGameObject); }
		}

	public:

		VisualEffect(sf::Sprite sprite, sf::Vector2f position, TimeUnit::Time duration) :
			GraphicalGameObject(sprite),
			spritePtr(dynamic_cast<sf::Sprite*>(this->graphic)),
			remainingFrames(duration),
			attachedSprite(nullptr)
		{
			this->spritePtr->setPosition(position);
			sf::Vector2u size = this->spritePtr->getTexture()->getSize();
			this->spritePtr->setOrigin(static_cast<float>(size.x) / 2.f, static_cast<float>(size.y) / 2.f);
		}

		VisualEffect(sf::Sprite sprite, GraphicalGameObject* attachTo, TimeUnit::Time duration) :
			VisualEffect(sprite, { 0.f, 0.f }, duration)
		{
			this->attachTo(attachTo);
		}

		VisualEffect(sf::Sprite sprite, TimeUnit::Time duration) :
			VisualEffect(sprite, { 0.f, 0.f }, duration) { }
		
		void attachTo(GraphicalGameObject* target)
		{
			this->attachedSprite = dynamic_cast<sf::Sprite*>(target->getGraphic());
			if (!attachedSprite) { return; }
			this->attachedGameObject = target;
			this->attachedGameObject->preserved = true;
			this->updatePosition();
		}

		void setEveryFrameAdditionalAction(std::function<void(VisualEffect* self, uint64_t remainingFrames)> action) { this->everyFrameAdditionalAction = action; }

		void setPosition(sf::Vector2f position) { this->spritePtr->setPosition(position + this->offset); }

		void setOffset(sf::Vector2f offset) { this->offset = offset; }

		sf::Sprite* getSprite() const { return this->spritePtr; }
	};
}

#endif