#include "SFML/Graphics.hpp"
#include "Screen.h"
#include "GameObject.h"
#include <memory>
#include <functional>
#include <mutex>
#include <iostream>
#include <cmath>
#include <queue>

static bool renderStarted = false;
static int currentFPS;
static sf::RenderWindow* windowPtr = nullptr;
static std::queue<Engine::GameObject*> removeQueue;

namespace Engine
{
	unsigned int Screen::windowWidth = 0;
	unsigned int Screen::windowHeight = 0;
	const char* Screen::windowTitle = nullptr;
	static Screen* currentScreen;
	bool running = true;

	void Screen::addMap(TileMap* map)
	{
		this->map = map;
	}

	void Screen::addMainCharacter(GameObject* mainCharacter)
	{
		this->add(mainCharacter);
		this->mainCharacter = mainCharacter;
	}

	sf::Vector2f Screen::getMainCharacterPosition() const
	{
		if (GraphicalGameObject* mc = dynamic_cast<GraphicalGameObject*>(this->mainCharacter)) { return dynamic_cast<const sf::Transformable*>(mc->getGraphic())->getPosition(); }
		return sf::Vector2f(0.f, 0.f);
	}

	void Screen::add(GameObject* gameObject)
	{
		GameObjectMap& map = (dynamic_cast<GraphicalGameObject*>(gameObject)) ? this->g_objects : this->objects;
		map[gameObject->getID()] = gameObject;
		gameObject->screen = this;
	}

	void Screen::addUIObject(GameObject* uiObj)
	{
		if (!(dynamic_cast<GraphicalGameObject*>(uiObj))) { return; }
		GameObjectMap& map = this->ui_objects;
		map[uiObj->getID()] = uiObj;
		uiObj->screen = this;
	}

	void Screen::remove(GameObject* gameObject)
	{
		removeQueue.push(gameObject);
	}

	sf::Vector2i Screen::getMousePosition() const
	{
		if(!windowPtr) { return sf::Vector2i(0, 0); }
		sf::Vector2i pixelPos = sf::Mouse::getPosition(*windowPtr);
		sf::Vector2f worldPos = windowPtr->mapPixelToCoords(pixelPos, windowPtr->getView());
		return sf::Vector2i(worldPos.x, worldPos.y);
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
		currentScreen = this;
		currentFPS = fps;

		if (renderStarted) { return; }
		renderStarted = true;

		static std::function<void(GameObjectMap&, sf::Event)> handleEvents = [](GameObjectMap& objects, sf::Event event)
		{
			for (auto const& pair : objects)
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

		unsigned int width = (Screen::windowWidth) ? Screen::windowWidth : 500;
		unsigned int height = (Screen::windowHeight) ? Screen::windowHeight : 500;
		const char* title = (Screen::windowTitle) ? Screen::windowTitle : "<no title>";
		sf::RenderWindow window(sf::VideoMode(width, height), title);
		windowPtr = &window;
		sf::View view(sf::Vector2f(width / 2, height / 2), sf::Vector2f(width, height));
		window.setView(view);
		sf::Clock clock;
		uint64_t frameCount = 0;

		while (window.isOpen())
		{
			clock.restart();
			Screen* cs = currentScreen;
			
			while (!removeQueue.empty())
			{
				GameObject* toRemove = removeQueue.front();
				removeQueue.pop();
				GameObjectMap& map = (dynamic_cast<GraphicalGameObject*>(toRemove)) ? this->g_objects : this->objects;
				if (map.find(toRemove->getID()) == map.end()) { continue; }
				GameObjectID id = toRemove->getID();
				map.erase(id);
				delete toRemove;
			}

			for (auto const& pair : cs->g_objects)
			{
				GameObject* obj = pair.second;
				obj->EveryFrame(frameCount);
			}

			for (auto const& pair : cs->objects)
			{
				GameObject* obj = pair.second;
				obj->EveryFrame(frameCount);
			}

			sf::Event event;
			while (window.pollEvent(event))
			{
				if (event.type == sf::Event::Closed || !running)
				{
					window.close();
					return;
				}
				handleEvents(cs->g_objects, event);
				handleEvents(cs->objects, event);
			}

			window.clear();

			//draw the map
			if (cs->map) { window.draw(*cs->map); }

			unsigned int MAP_WIDTH = cs->map->width() * this->map->tileSize().x;
			unsigned int MAP_HEIGHT = cs->map->height() * this->map->tileSize().y;

			//draw the objects
			for (auto const& pair : cs->g_objects)
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
						if (X < 0) { transformable->setPosition(0.f, Y); }
						if (Y < 0) { transformable->setPosition(X, 0.f); }
						if (Y + size.y > MAP_HEIGHT) { transformable->setPosition(X, MAP_HEIGHT - size.y); }
						if (X + size.x > MAP_WIDTH) { transformable->setPosition(MAP_WIDTH - size.x, Y); }
						#undef X
						#undef Y					
					}
				}
				obj->draw(window);
			}

			//draw the UI objects
			for (auto const& pair : cs->ui_objects)
			{
				GraphicalGameObject* obj = dynamic_cast<GraphicalGameObject*>(pair.second);
				sf::Transformable* transformable = dynamic_cast<sf::Transformable*>(obj->getGraphic());
				if (!transformable) { continue; }
				sf::Vector2f viewPos = window.getView().getCenter();
				sf::Vector2f screenPosition = transformable->getPosition();
				transformable->setPosition(viewPos - sf::Vector2f(cs->windowWidth / 2, cs->windowHeight / 2) + screenPosition);
				obj->draw(window);
				transformable->setPosition(screenPosition);
			}

			//trigger collision events
			for (auto const& p1 : cs->g_objects)
			{
				GraphicalGameObject* eventReciever = dynamic_cast<GraphicalGameObject*>(p1.second);
				sf::Sprite* receiverSprite = dynamic_cast<sf::Sprite*>(eventReciever->getGraphic());
				if (!receiverSprite) { continue; }
				for (auto const& p2 : cs->g_objects)
				{
					GraphicalGameObject* eventArg = dynamic_cast<GraphicalGameObject*>(p2.second);
					if (eventArg == eventReciever || !eventArg->collision || !eventReciever->collision) { continue; }
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
					if (graphicAsTransformable->getPosition().x > windowWidth / 2 && graphicAsTransformable->getPosition().x < MAP_WIDTH - windowWidth / 2
						&& graphicAsTransformable->getPosition().y > windowHeight / 2 && graphicAsTransformable->getPosition().y < MAP_HEIGHT - windowHeight / 2)
					{
						view.setCenter(graphicAsTransformable->getPosition());
					}
					else if (graphicAsTransformable->getPosition().x >= 0 && graphicAsTransformable->getPosition().x <= windowWidth / 2 &&
						graphicAsTransformable->getPosition().y >= 0 && graphicAsTransformable->getPosition().y <= windowHeight / 2)

					{
						view.setCenter(windowWidth / 2, windowHeight / 2);
					}
					else if (graphicAsTransformable->getPosition().x >= 0 && graphicAsTransformable->getPosition().x <= windowWidth / 2 &&
						graphicAsTransformable->getPosition().y >= MAP_HEIGHT - windowHeight / 2 && graphicAsTransformable->getPosition().y <= MAP_HEIGHT)
					{
						view.setCenter(windowWidth / 2, MAP_HEIGHT - windowHeight / 2);
					}
					else if (graphicAsTransformable->getPosition().x >= MAP_WIDTH - windowWidth / 2 && graphicAsTransformable->getPosition().x <= MAP_WIDTH &&
						graphicAsTransformable->getPosition().y >= 0 && graphicAsTransformable->getPosition().y <= windowHeight / 2)
					{
						view.setCenter(MAP_WIDTH - windowWidth / 2, windowHeight / 2);
					}
					else if (graphicAsTransformable->getPosition().x >= MAP_WIDTH - windowWidth / 2 && graphicAsTransformable->getPosition().x <= MAP_WIDTH &&
						graphicAsTransformable->getPosition().y >= MAP_HEIGHT - windowHeight / 2 && graphicAsTransformable->getPosition().y <= MAP_HEIGHT)
					{
						view.setCenter(MAP_WIDTH - windowWidth / 2, MAP_HEIGHT - windowHeight / 2);
					}
					else if (graphicAsTransformable->getPosition().x > windowWidth / 2 && graphicAsTransformable->getPosition().x < MAP_WIDTH - windowWidth / 2 &&
						graphicAsTransformable->getPosition().y >= 0 && graphicAsTransformable->getPosition().y <= windowHeight / 2)
					{
						view.setCenter(graphicAsTransformable->getPosition().x, windowHeight / 2);
					}
					else if (graphicAsTransformable->getPosition().x > windowWidth / 2 && graphicAsTransformable->getPosition().x < MAP_WIDTH - windowWidth / 2 &&
						graphicAsTransformable->getPosition().y >= MAP_HEIGHT - windowHeight / 2 && graphicAsTransformable->getPosition().y <= MAP_HEIGHT)
					{
						view.setCenter(graphicAsTransformable->getPosition().x, MAP_HEIGHT - windowHeight / 2);
					}
					else if (graphicAsTransformable->getPosition().x >= 0 && graphicAsTransformable->getPosition().x <= windowWidth / 2 &&
						graphicAsTransformable->getPosition().y > windowHeight / 2 && graphicAsTransformable->getPosition().y < MAP_HEIGHT - windowHeight / 2)
					{
						view.setCenter(windowWidth / 2, graphicAsTransformable->getPosition().y);
					}
					else if (graphicAsTransformable->getPosition().x >= MAP_WIDTH - windowWidth / 2 && graphicAsTransformable->getPosition().x <= MAP_WIDTH &&
						graphicAsTransformable->getPosition().y > windowHeight / 2 && graphicAsTransformable->getPosition().y < MAP_HEIGHT - windowHeight / 2)
					{
						view.setCenter(MAP_WIDTH - windowWidth / 2, graphicAsTransformable->getPosition().y);
					}
				}
			}
			window.setView(view);
			window.display();
			frameCount++;
			while (clock.getElapsedTime().asMicroseconds() < (1000000 / currentFPS)) {}
		}
	}

	Screen::~Screen()
	{
		for (auto const& pair : this->objects)
		{
			auto obj = pair.second;
			if (obj == this->mainCharacter) { continue; }
			delete obj;
		}

		for (auto const& pair : this->g_objects)
		{
			auto obj = pair.second;
			if (obj == this->mainCharacter) { continue; }
			delete obj;
		}
	}
}