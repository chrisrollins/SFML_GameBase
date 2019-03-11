#include "SFML/Graphics.hpp"
#include "Screen.h"
#include "GameObject.h"
#include <memory>
#include <functional>
#include <iostream>
#include <cmath>
#include <queue>

static bool renderStarted = false;
static int currentFPS;
static sf::RenderWindow* windowPtr = nullptr;
static std::queue<Engine::GameObject*> removeQueue;

namespace Engine
{
	typedef std::map<GameObjectID, GameObject*> GameObjectMap;
	unsigned int Screen::windowWidth = 0;
	unsigned int Screen::windowHeight = 0;
	const char* Screen::windowTitle = nullptr;
	static Screen* currentScreen;
	static Screen* pendingSwitch;
	bool running = true;
	bool windowInitialized = false;

	Screen::Screen() {}

	void Screen::addMap(TileMap* map)
	{
		this->map = map;
	}

	void Screen::addMainCharacter(GameObject* mainCharacter)
	{
		this->add(mainCharacter);
		this->mainCharacter = mainCharacter;
		mainCharacter->AddedToScreen();
	}

	GameObject* Screen::getMainCharacter() const
	{
		return this->mainCharacter;
	}

	void Screen::add(GameObject* gameObject)
	{
		GameObjectMap& map = (dynamic_cast<GraphicalGameObject*>(gameObject)) ? this->g_objects : this->objects;
		map[gameObject->getID()] = gameObject;
		gameObject->screen = this;
		gameObject->AddedToScreen();
	}

	void Screen::addUIObject(GameObject* uiObj)
	{
		this->ui_objects[uiObj->getID()] = uiObj;
		uiObj->screen = this;
		uiObj->AddedToScreen();
	}

	void Screen::remove(GameObject* gameObject)
	{
		if (this != currentScreen)
		{
			for (auto map : { &this->objects, &this->g_objects, &this->ui_objects })
			{
				if (map->find(gameObject->getID()) != map->end())
				{
					GameObjectID id = gameObject->getID();
					map->erase(id);
					delete gameObject;
					break;
				}
			}
		}
		else
		{		
			removeQueue.push(gameObject);
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
		return this->map;
	}

	void Screen::close()
	{
		running = false;
	}

	void Screen::render(int fps)
	{
		if (fps < 1) { fps = 1; }
		else if (fps > 1000) { fps = 1000; }
		currentFPS = fps;

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
		else
		{
			pendingSwitch = nullptr;
		}
		currentScreen = this;
		renderStarted = true;

		//local function used by game loop to iterate over each map of objects for the events
		static std::function<void(GameObjectMap*, sf::Event)> handleEvents = [](GameObjectMap* objects, sf::Event event)
		{
			for (auto const& pair : *objects)
			{
				GameObject* obj = pair.second;
				switch (event.type)
				{
				case sf::Event::Resized:
					obj->Resized(event);
					break;
				case sf::Event::LostFocus:
					obj->LostFocus(event);
					break;
				case sf::Event::GainedFocus:
					obj->GainedFocus(event);
					break;
				case sf::Event::TextEntered:
					obj->TextEntered(event);
					break;
				case sf::Event::KeyPressed:
					obj->KeyPressed(event);
					break;
				case sf::Event::KeyReleased:
					obj->KeyReleased(event);
					break;
				case sf::Event::MouseWheelMoved:
					obj->MouseWheelMoved(event);
					break;
				case sf::Event::MouseWheelScrolled:
					obj->MouseWheelScrolled(event);
					break;
				case sf::Event::MouseButtonPressed:
					obj->MouseButtonPressed(event);
					break;
				case sf::Event::MouseButtonReleased:
					obj->MouseButtonReleased(event);
					break;
				case sf::Event::MouseMoved:
					obj->MouseMoved(event);
					break;
				case sf::Event::MouseEntered:
					obj->MouseEntered(event);
					break;
				case sf::Event::MouseLeft:
					obj->MouseLeft(event);
					break;
				case sf::Event::JoystickButtonPressed:
					obj->JoystickButtonPressed(event);
					break;
				case sf::Event::JoystickButtonReleased:
					obj->JoystickButtonReleased(event);
					break;
				case sf::Event::JoystickMoved:
					obj->JoystickMoved(event);
					break;
				case sf::Event::JoystickConnected:
					obj->JoystickConnected(event);
					break;
				case sf::Event::JoystickDisconnected:
					obj->JoystickDisconnected(event);
					break;
				case sf::Event::TouchBegan:
					obj->TouchBegan(event);
					break;
				case sf::Event::TouchMoved:
					obj->TouchMoved(event);
					break;
				case sf::Event::TouchEnded:
					obj->TouchEnded(event);
					break;
				case sf::Event::SensorChanged:
					obj->SensorChanged(event);
					break;
				default:
					break;
				}
			}
		};
		
		while (window.isOpen() && !pendingSwitch)
		{
			clock.restart();
			/*
			//remove objects that are pending to be removed
			while (!removeQueue.empty())
			{
				GameObject* toRemove = removeQueue.front();
				removeQueue.pop();
				for (auto map : { &currentScreen->objects, &currentScreen->g_objects, &currentScreen->ui_objects })
				{
					if (map->find(toRemove->getID()) != map->end())
					{
						GameObjectID id = toRemove->getID();
						toRemove->RemovedFromScreen();
						map->erase(id);
						delete toRemove;
						break;
					}
				}
			}*/

			//run the EveryFrame event on all objects
			for (auto map : { &currentScreen->objects, &currentScreen->g_objects, &currentScreen->ui_objects })
			{
				for (auto const& pair : *map)
				{
					GameObject* obj = pair.second;
					obj->EveryFrame(frameCount);
				}
			}

			sf::Event event;
			while (window.pollEvent(event))
			{
				if (event.type == sf::Event::Closed || !running)
				{
					window.close();
					return;
				}
				if (event.type == sf::Event::Resized)
				{
					// update the view to the new size of the window
					sf::FloatRect visibleArea(0.f, 0.f, static_cast<float>(event.size.width), static_cast<float>(event.size.height));
					view = sf::View(visibleArea);
				}
				//handle events on each object
				for (auto map : { &currentScreen->objects, &currentScreen->g_objects, &currentScreen->ui_objects })
				{
					handleEvents(map, event);
				}
			}

			window.clear();

			//draw the map

			unsigned int MAP_WIDTH = 0;
			unsigned int MAP_HEIGHT = 0;

			if (currentScreen->map)
			{
				window.draw(*currentScreen->map);
				MAP_WIDTH = currentScreen->map->width() * currentScreen->map->tileSize().x;
				MAP_HEIGHT = currentScreen->map->height() * currentScreen->map->tileSize().y;
			}

			//draw the objects
			for (auto const& pair : currentScreen->g_objects)
			{
				GraphicalGameObject* obj = dynamic_cast<GraphicalGameObject*>(pair.second); //does not need to be checked, they are checked on insertion into the maps

				//prevent objects from leaving the map
				if (!obj->ignoreObstacles)
				{
					if (sf::Transformable* transformable = dynamic_cast<sf::Transformable*>(obj->getGraphic()))
					{
						sf::Vector2u size(0, 0);
						if (sf::Sprite* sprite = dynamic_cast<sf::Sprite*>(obj->getGraphic())) { size = sf::Vector2u(sprite->getTextureRect().width, sprite->getTextureRect().height); }
#define X (transformable->getPosition().x)
#define Y (transformable->getPosition().y)
						if (X < 0.f) { transformable->setPosition(0.f, Y); }
						if (Y < 0.f) { transformable->setPosition(X, 0.f); }
						if (Y + size.y > MAP_HEIGHT) { transformable->setPosition(X, static_cast<float>(MAP_HEIGHT - size.y)); }
						if (X + size.x > MAP_WIDTH) { transformable->setPosition(static_cast<float>(MAP_WIDTH - size.x), Y); }

						sf::Vector2f offsets(0.f, 0.f);
						if (obj->obstacleCollisionSize.width > 0.f && obj->obstacleCollisionSize.height > 0.f)
						{
							offsets.x = obj->obstacleCollisionSize.left;
							offsets.y = obj->obstacleCollisionSize.top;
							size.x = static_cast<unsigned int>(obj->obstacleCollisionSize.width);
							size.y = static_cast<unsigned int>(obj->obstacleCollisionSize.height);
						}

						do
						{
							sf::Vector2f corners[4] = {
								{X + static_cast<float>(offsets.x)          , Y + static_cast<float>(offsets.y)          },
								{X + static_cast<float>(size.x + offsets.x) , Y + static_cast<float>(offsets.y)          },
								{X + static_cast<float>(offsets.x)          , Y + static_cast<float>(size.y + offsets.y) },
								{X + static_cast<float>(size.x + offsets.x) , Y + static_cast<float>(size.y + offsets.y) }
							};

							bool collision = false;
							for (auto corner : corners)
							{
								if (currentScreen->map->isObstacle(corner))
								{
									collision = true;
									break;
								}
							}
							if (collision)
							{
								if (obj->spawnCollisionsResolved) { transformable->setPosition(obj->lastPos); }
								else
								{
									auto positions = this->map->getSafeSpawnPositions();
									transformable->setPosition(positions[rand() % positions.size()]);
								}
							}
							else { obj->spawnCollisionsResolved = true; }
						} while (!obj->spawnCollisionsResolved);

						obj->lastPos = { X , Y };
#undef X
#undef Y
					}
				}
				obj->draw(window);
			}

			//draw the UI objects
			for (auto const& pair : currentScreen->ui_objects)
			{
				GraphicalGameObject* obj = dynamic_cast<GraphicalGameObject*>(pair.second);
				sf::Transformable* transformable = dynamic_cast<sf::Transformable*>(obj->getGraphic());
				if (!transformable) { continue; }
				sf::Vector2f viewPos = window.getView().getCenter();
				sf::Vector2f screenPosition = transformable->getPosition();
				transformable->setPosition(viewPos - sf::Vector2f(static_cast<float>(currentScreen->windowWidth / 2), static_cast<float>(currentScreen->windowHeight / 2)) + screenPosition);
				obj->draw(window);
				transformable->setPosition(screenPosition);
			}

			//trigger collision events
			for (auto const& p1 : currentScreen->g_objects)
			{
				GraphicalGameObject* eventReciever = dynamic_cast<GraphicalGameObject*>(p1.second);
				sf::Sprite* receiverSprite = dynamic_cast<sf::Sprite*>(eventReciever->getGraphic());
				if (!receiverSprite) { continue; }
				for (auto const& p2 : currentScreen->g_objects)
				{
					GraphicalGameObject* eventArg = dynamic_cast<GraphicalGameObject*>(p2.second);
					if (eventArg == eventReciever || !eventArg->triggerCollisionEvents || !eventReciever->triggerCollisionEvents) { continue; }
					sf::Sprite* argSprite = dynamic_cast<sf::Sprite*>(eventArg->getGraphic());
					if (!argSprite) { continue; }
					sf::FloatRect r1 = receiverSprite->getGlobalBounds();
					sf::FloatRect r2 = argSprite->getGlobalBounds();
					if (r1.intersects(r2))
					{
						eventReciever->Collision(*eventArg);
					}
				}
			}

			//view moves with character
			if (GraphicalGameObject* mainCharGraphical = dynamic_cast<GraphicalGameObject*>(mainCharacter))
			{
				if (const sf::Transformable* graphicAsTransformable = dynamic_cast<const sf::Transformable*>(mainCharGraphical->getGraphic()))
				{
					sf::Vector2f pos = graphicAsTransformable->getPosition();
					float x = pos.x;
					float y = pos.y;
					float fWidth = static_cast<float>(MAP_WIDTH);
					float fHeight = static_cast<float>(MAP_HEIGHT);
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
						y > halfHeight && y < MAP_HEIGHT - halfHeight)
					{
						view.setCenter(fWidth - halfWidth, y);
					}
				}
			}
			window.setView(view);
			window.display();

			//remove objects that are pending to be removed
			while (!removeQueue.empty())
			{
				GameObject* toRemove = removeQueue.front();
				removeQueue.pop();
				for (auto map : { &currentScreen->objects, &currentScreen->g_objects, &currentScreen->ui_objects })
				{
					if (map->find(toRemove->getID()) != map->end())
					{
						GameObjectID id = toRemove->getID();
						toRemove->RemovedFromScreen();
						map->erase(id);
						delete toRemove;
						break;
					}
				}
			}

			frameCount++;
			while (clock.getElapsedTime().asMicroseconds() < (1000000 / currentFPS)) {}
		}

		if (pendingSwitch)
		{
			renderStarted = false;
			pendingSwitch->render();
		}
	}

	Screen::~Screen()
	{
	}
}