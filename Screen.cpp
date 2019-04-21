#include "SFML/Graphics.hpp"
#include "Screen.h"
#include "GameObject.h"
#include "GameObjectAttribute.h"
#include "FileLoadException.h"
#include "DebugManager.h"
#include <utility>
#include <functional>

using namespace Engine;

static bool renderStarted = false;
static int currentFPS;
static sf::RenderWindow* windowPtr = nullptr;
static std::queue<std::pair<GameObject*, bool>> removeQueue;
unsigned int Screen::windowWidth = 0;
unsigned int Screen::windowHeight = 0;
const char* Screen::windowTitle = nullptr;
static Screen* currentScreen;
static Screen* pendingSwitch;
bool running = true;
bool windowInitialized = false;

namespace Engine
{	
	Screen::Screen() {}

	void Screen::addMap(TileMap* map)
	{
		this->tMap = map;
	}

	void Screen::addMainCharacter(GraphicalGameObject* mainCharacter)
	{
		this->add(mainCharacter);
		this->mainCharacter = mainCharacter;
		mainCharacter->AddedToScreen();
	}

	void Screen::add(GameObject* gameObject)
	{
		if (gameObject == nullptr) { return; }
		GameObjectID id = gameObject->getID();
		this->allObjects[id] = gameObject;
		if (GraphicalGameObject* ggo = dynamic_cast<GraphicalGameObject*>(gameObject)) { this->renderObjects[id] = ggo; }
		if (GameObjectAttribute::Collision* collisionObject = dynamic_cast<GameObjectAttribute::Collision*>(gameObject)) { this->collisionObjects[id] = collisionObject; }
		if (GameObjectAttribute::Movement* movingObject = dynamic_cast<GameObjectAttribute::Movement*>(gameObject))
		{
			if (dynamic_cast<GameObjectAttribute::TerrainCollision*>(gameObject)) { this->movingObjectsWithTerrainCollision[id] = movingObject; }
			else { this->movingObjects[id] = movingObject; }
		}
		gameObject->screen = this;
		gameObject->AddedToScreen();
	}

	void Screen::addUIObject(GameObject* uiObj)
	{
		#ifdef _DEBUG
		if (GameObjectAttribute::Collision* collisionObject = dynamic_cast<GameObjectAttribute::Collision*>(uiObj))
		{ DebugManager::PrintMessage(DebugManager::MessageType::ERROR_REPORTING, "Argument to Screen::addUIObject should not inherit from GameObjectAttribute::Collision. It will not be added to list of collision objects."); }
		if (GameObjectAttribute::Movement* movingObject = dynamic_cast<GameObjectAttribute::Movement*>(uiObj))
		{ DebugManager::PrintMessage(DebugManager::MessageType::ERROR_REPORTING, "Argument to Screen::addUIObject should not inherit from GameObjectAttribute::Movement. It will not be added to list of moving objects."); }
		#endif
		GameObjectID id = uiObj->getID();
		this->allObjects[id] = uiObj;
		if (GraphicalGameObject* ggo = dynamic_cast<GraphicalGameObject*>(uiObj)) { this->uiObjects[id] = ggo; }
		uiObj->screen = this;
		uiObj->AddedToScreen();		
	}

	GraphicalGameObject* Screen::getMainCharacter() const
	{
		return this->mainCharacter;
	}

	bool Screen::find(GameObject* gameObject)
	{
		GameObjectID id = gameObject->getID();
		return (gameObject == this->mainCharacter) || (this->allObjects.find(id) != this->allObjects.end());
	}

	void Screen::remove(GameObject* gameObject, bool autoDelete)
	{
		if (this != currentScreen)
		{	
			GameObjectID id = gameObject->getID();
			if (this->allObjects.erase(id) |
				this->renderObjects.erase(id) |
				this->uiObjects.erase(id) |
				this->collisionObjects.erase(id) |
				this->movingObjects.erase(id) |
				this->movingObjectsWithTerrainCollision.erase(id))
			{
				if (autoDelete) { delete gameObject; }
			}
		}
		else
		{
			removeQueue.push({ gameObject, autoDelete });
		}
	}

	sf::Vector2i Screen::getMousePosition() const
	{
		if (!windowPtr) { return sf::Vector2i(0, 0); }
		sf::Vector2i pixelPos = sf::Mouse::getPosition(*windowPtr);
		sf::Vector2f worldPos = windowPtr->mapPixelToCoords(pixelPos, windowPtr->getView());
		return sf::Vector2i(static_cast<int>(worldPos.x), static_cast<int>(worldPos.y));
	}

	const TileMap* Screen::getMap() const
	{
		return this->tMap;
	}

	void Screen::close()
	{
		running = false;
	}

	void Screen::render()
	{
		constexpr int fps = 60;
		constexpr int frameDurationMicroseconds = 1000000 / fps;
		unsigned int width = (Screen::windowWidth) ? Screen::windowWidth : 500;
		unsigned int height = (Screen::windowHeight) ? Screen::windowHeight : 500;
		const char* title = (Screen::windowTitle) ? Screen::windowTitle : "<no title>";
		static sf::RenderWindow window(sf::VideoMode(width, height), title, sf::Style::Close);
		static sf::Clock clock;
		static uint64_t frameCount = 0;
		sf::View view(sf::Vector2f(static_cast<float>(width / 2), static_cast<float>(height / 2)), sf::Vector2f(static_cast<float>(width), static_cast<float>(height)));
		windowPtr = &window;
		window.setView(view);
		if (renderStarted)
		{
			pendingSwitch = this;
			return;
		}
		else { pendingSwitch = nullptr; }
		currentScreen = this;
		renderStarted = true;
		#ifdef _DEBUG
		sf::Clock eventClock;
		sf::Clock collisionClock;
		sf::Clock movementClock;
		sf::Clock drawClock;
		sf::Int64 eventDurationSum = 0;
		sf::Int64 collisionDurationSum = 0;
		sf::Int64 movementDurationSum = 0;
		sf::Int64 drawDurationSum = 0;
		sf::Int64 frameDurationSum = 0;
		#endif
		
		//game loop
		while (window.isOpen() && !pendingSwitch)
		{
			clock.restart();
			#ifdef _DEBUG
			eventClock.restart();
			#endif

			for (auto const & pair : this->allObjects)
			{
				pair.second->EveryFrame(frameCount);
			}

			sf::Event ev;
			while (window.pollEvent(ev))
			{
				if (ev.type == sf::Event::Closed || !running)
				{
					window.close();
					return;
				}
				else if (ev.type == sf::Event::Resized)
				{
					// update the view to the new size of the window
					sf::FloatRect visibleArea(0.f, 0.f, static_cast<float>(ev.size.width), static_cast<float>(ev.size.height));
					view = sf::View(visibleArea);
				}
				//handle events on each object
				for (auto & pair : this->allObjects)
				{
					GameObject* obj = pair.second;
					if (!obj->eventsDisabled) { obj->dispatchEvent(ev); }
				}
			}
			#ifdef _DEBUG
			eventDurationSum += eventClock.getElapsedTime().asMicroseconds();
			movementClock.restart();
			#endif

			//handle movement and terrain collision
			for (auto const & pair : this->movingObjectsWithTerrainCollision)
			{
				GameObjectAttribute::Movement* obj = pair.second;
				if (obj->xVelocity == 0.0 && obj->yVelocity == 0.0) { continue; }
				sf::Sprite* spr = obj->getDrawablePtr();
				sf::Vector2f position = spr->getPosition();
				sf::Vector2f velocity = obj->getVelocity();
				obj->xVelocity = 0.0;
				obj->yVelocity = 0.0;
				sf::FloatRect collisionSize = dynamic_cast<TerrainCollision*>(obj)->getObstacleCollisionSize();
				sf::IntRect tRect = obj->getDrawablePtr()->getTextureRect();
				auto tryMove = [&](float vx, float vy)
				{
					sf::Vector2f destination(position.x + vx, position.y + vy);
					float x = destination.x + collisionSize.left;
					float y = destination.y + collisionSize.top;
					sf::Vector2f mapBoundsCollisionCorners[4] = {
						{x, y},
						{x + static_cast<float>(tRect.width), y},
						{x + static_cast<float>(tRect.width), y + static_cast<float>(tRect.height)},
						{x, y + static_cast<float>(tRect.height)}
					};
					for (auto const & corner : mapBoundsCollisionCorners)
					{
						if (this->tMap->isOutOfBounds(corner)) { return false; }
					}
					sf::Vector2f terrainCollisionCorners[4] = {
						{x, y},
						{x + collisionSize.width, y},
						{x + collisionSize.width, y + collisionSize.height},
						{x, y + collisionSize.height}
					};
					for (auto const & corner : terrainCollisionCorners)
					{
						if (this->tMap->isObstacle(corner)) { return false; }
					}
					spr->setPosition(destination);
					return true;
				};
				if (tryMove(velocity.x, velocity.y)) { continue; }
				if (tryMove(velocity.x, 0.f)) { continue; }
				if (tryMove(0.f, velocity.y)) { continue; }
			}

			//handle movement of objects that ignore terrain
			for (auto const & pair : this->movingObjects)
			{
				GameObjectAttribute::Movement* obj = pair.second;
				if (obj->xVelocity == 0.0 && obj->yVelocity == 0.0) { continue; }
				sf::Sprite* spr = obj->getDrawablePtr();
				spr->move(obj->getVelocity());
				obj->xVelocity = 0.0;
				obj->yVelocity = 0.0;
			}
			#ifdef _DEBUG
			movementDurationSum += movementClock.getElapsedTime().asMicroseconds();
			collisionClock.restart();
			#endif

			//object collision
			for (auto & p1 : this->collisionObjects)
			{
				GameObjectAttribute::Collision* eventReceiver = p1.second;
				for (auto & p2 : this->collisionObjects)
				{
					GameObjectAttribute::Collision* eventArg = p2.second;
					if (eventReceiver != eventArg && eventReceiver->CheckCollision(eventArg)) { eventReceiver->Collided(eventArg); }
				}
			}
			#ifdef _DEBUG
			collisionDurationSum += collisionClock.getElapsedTime().asMicroseconds();
			drawClock.restart();
			#endif

			//draw
			window.clear();

			//draw the map
			if (this->tMap) { window.draw(*this->tMap); }

			//draw the objects
			for (auto & pair : this->renderObjects)
			{
				GraphicalGameObject* obj = pair.second;
				obj->draw(window);
			}

			//draw the UI objects
			for (auto const & pair : this->uiObjects)
			{
				GraphicalGameObject* obj = pair.second;
				sf::Transformable* transformable = dynamic_cast<sf::Transformable*>(obj->getGraphic());
				if (!transformable) { continue; }
				sf::Vector2f viewPos = window.getView().getCenter();
				sf::Vector2f screenPosition = transformable->getPosition();
				transformable->setPosition(viewPos - sf::Vector2f(static_cast<float>(this->windowWidth / 2), static_cast<float>(this->windowHeight / 2)) + screenPosition);
				obj->draw(window);
				transformable->setPosition(screenPosition);
			}

			//view moves with character
			sf::Sprite* mainCharacterSprite = (this->mainCharacter != nullptr) ? dynamic_cast<sf::Sprite*>(this->mainCharacter->graphic) : nullptr;
			if (mainCharacterSprite != nullptr)
			{
				unsigned int mapWidth = 0;
				unsigned int mapHeight = 0;
				if (this->tMap != nullptr)
				{
					mapWidth = this->tMap->width() * this->tMap->tileSize().x;
					mapHeight = this->tMap->height() * this->tMap->tileSize().y;
				}
				sf::Vector2f pos = mainCharacterSprite->getPosition();
				sf::Vector2f origin = mainCharacterSprite->getOrigin();
				float x = pos.x + origin.x;
				float y = pos.y + origin.y;
				float fWidth = static_cast<float>(mapWidth);
				float fHeight = static_cast<float>(mapHeight);
				float halfWidth = static_cast<float>(windowWidth / 2);
				float halfHeight = static_cast<float>(windowHeight / 2);
				if (x > halfWidth && x < (fWidth - halfWidth)
					&& y > halfHeight && y < (fHeight - halfHeight))
				{
					view.setCenter(pos);
				}
				else if (x >= 0.f && x <= halfWidth &&
					y >= 0.f && y <= halfHeight)
				{
					view.setCenter(halfWidth, halfHeight);
				}
				else if (x >= 0.f && x <= halfWidth &&
					y >= fHeight - halfHeight && y <= fHeight)
				{
					view.setCenter(halfWidth, fHeight - halfHeight);
				}
				else if (x >= fWidth - halfWidth && x <= fWidth &&
					y >= 0.f && y <= halfHeight)
				{
					view.setCenter(fWidth - halfWidth, halfHeight);
				}
				else if (x >= fWidth - halfWidth && x <= fWidth &&
					y >= fHeight - halfHeight && y <= fHeight)
				{
					view.setCenter(fWidth - halfWidth, fHeight - halfHeight);
				}
				else if (x > halfWidth && x < fWidth - halfWidth &&
					y >= 0.f && y <= halfHeight)
				{
					view.setCenter(x, halfHeight);
				}
				else if (x > halfWidth && x < fWidth - halfWidth &&
					y >= fHeight - halfHeight && y <= fHeight)
				{
					view.setCenter(x, fHeight - halfHeight);
				}
				else if (x >= 0.f && x <= halfWidth &&
					y > halfHeight && y < fHeight - halfHeight)
				{
					view.setCenter(halfWidth, y);
				}
				else if (x >= fWidth - halfWidth && x <= fWidth &&
					y > halfHeight && y < mapHeight - halfHeight)
				{
					view.setCenter(fWidth - halfWidth, y);
				}
			}
			#ifdef _DEBUG
			drawDurationSum += drawClock.getElapsedTime().asMicroseconds();
			#endif

			window.setView(view);
			window.display();

			//remove objects that are pending to be removed
			while (!removeQueue.empty())
			{
				std::pair<GameObject*, bool> pRemove = removeQueue.front();
				GameObject* toRemove = pRemove.first;
				bool autoDelete = pRemove.second;
				bool removed = false;
				removeQueue.pop();
				GameObjectID id = toRemove->getID();
				if(	this->allObjects.erase(id) |
					this->renderObjects.erase(id) |
					this->uiObjects.erase(id) |
					this->collisionObjects.erase(id) |
					this->movingObjects.erase(id) |
					this->movingObjectsWithTerrainCollision.erase(id))
				{
					toRemove->RemovedFromScreen();
					if (autoDelete) { delete toRemove; }
				}
			}

			#ifdef _DEBUG
			frameDurationSum += clock.getElapsedTime().asMicroseconds();
			int avgFrameReportFrequency = 60;
			if (frameCount % avgFrameReportFrequency == 0)
			{
				DebugManager::MessageType msgType = DebugManager::MessageType::PERFORMANCE_REPORTING;
				DebugManager::PrintMessage(msgType, string("\naverage event compute time: ") + std::to_string(eventDurationSum / avgFrameReportFrequency));
				DebugManager::PrintMessage(msgType, string("average movement compute time: ") + std::to_string(movementDurationSum / avgFrameReportFrequency));
				DebugManager::PrintMessage(msgType, string("average collision compute time: ") + std::to_string(collisionDurationSum / avgFrameReportFrequency));
				DebugManager::PrintMessage(msgType, string("average draw compute time: ") + std::to_string(drawDurationSum / avgFrameReportFrequency));
				DebugManager::PrintMessage(msgType, string("average total compute time: ") + std::to_string(frameDurationSum / avgFrameReportFrequency));
				DebugManager::PrintMessage(msgType, string("max total before slowdown: ") + std::to_string(frameDurationMicroseconds));
				frameDurationSum = 0;
				eventDurationSum = 0;
				movementDurationSum = 0;
				collisionDurationSum = 0;
				drawDurationSum = 0;
			}
			#endif
			frameCount++;
			while (clock.getElapsedTime().asMicroseconds() < frameDurationMicroseconds) {}
		}
		//end game loop

		if (pendingSwitch)
		{
			renderStarted = false;
			pendingSwitch->render();
		}
	}

	Screen::~Screen()
	{
		vector<GameObject*> objs;
		for (auto const & iter : this->allObjects)
		{
			objs.push_back(iter.second);
		}
		for (auto const & obj : objs)
		{
			this->remove(obj);
		}
	}
	
	class Scheduler : public GameObject
	{
	private:
		uint64_t delay;
		uint64_t countdown;
		uint16_t repeatsRemaining;
		bool infinite;
		function<void()> func;
	public:
		Scheduler( function<void()> func, uint64_t delay, uint16_t repeatCount) : delay(delay), countdown(delay), repeatsRemaining(repeatCount), infinite(repeatCount == 0), func(func) {}
		void EveryFrame(uint64_t f)
		{
			if (this->countdown == 0)
			{
				func();
				if (repeatsRemaining > 0)
				{
					repeatsRemaining--;
					this->countdown = this->delay;
				}
				else if(!this->infinite) { this->screen->remove(this); }				
			}
			else { this->countdown--; }
		}
	};

	void Screen::schedule(function<void()> func, TimeUnit::Time delay, uint16_t repeatCount)
	{
		this->add(new Scheduler(func, delay, repeatCount));
	}
}