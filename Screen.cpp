#include "SFML/Graphics.hpp"
#include "Screen.h"
#include "GameObject.h"
#include <memory>
#include <functional>
#include <mutex>
#include <iostream>

static bool renderStarted = false;
static int currentFPS;

namespace Engine
{
	unsigned int Screen::windowWidth = 0;
	unsigned int Screen::windowHeight = 0;
	const char* Screen::windowTitle = nullptr;
	static Screen* currentScreen;

	void Screen::addMap(sf::Drawable* map)
	{
		this->map = map;
	}

	void Screen::addMainCharacter(GameObject* mainCharacter)
	{
		this->add(mainCharacter);
		this->mainCharacter = mainCharacter;
	}

	void Screen::add(GameObject* gameObject)
	{
		GameObjectMap& map = (dynamic_cast<GraphicalGameObject*>(gameObject)) ? this->g_objects : this->objects;
		map[gameObject->getID()] = gameObject;
	}

	void Screen::remove(GameObject* gameObject)
	{
		GameObjectMap& map = (dynamic_cast<GraphicalGameObject*>(gameObject)) ? this->g_objects : this->objects;
		if (map.find(gameObject->getID()) == map.end()) { return; }
		GameObjectID id = gameObject->getID();
		GameObject* obj = map[id];
		map.erase(id);
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
				}
			}
		};

		unsigned int width = (Screen::windowWidth) ? Screen::windowWidth : 500;
		unsigned int height = (Screen::windowHeight) ? Screen::windowHeight : 500;
		const char* title = (Screen::windowTitle) ? Screen::windowTitle : "<no title>";
		sf::RenderWindow window(sf::VideoMode(width, height), title);
		sf::View view(sf::Vector2f(width/2, height/2), sf::Vector2f(width, height));
		window.setView(view);
		sf::Clock clock;
		uint64_t frameCount = 0;

		while (window.isOpen())
		{
			clock.restart();
			Screen* cs = currentScreen;

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
				handleEvents(cs->g_objects, event);
				handleEvents(cs->objects, event);
				if (event.type == sf::Event::Closed)
				{
					window.close();
				}
			}

			window.clear();
			for (auto const& pair : cs->g_objects)
			{
				GraphicalGameObject* obj = dynamic_cast<GraphicalGameObject*>(pair.second); //does not need to be checked, they are checked on insertion into the maps
				obj->draw(window);
			}

			//draw the map
			if (cs->map) { window.draw(*cs->map); }

			//view moves with character
			if (GraphicalGameObject* mainCharGraphical = dynamic_cast<GraphicalGameObject*>(mainCharacter))
			{
				if (const sf::Transformable* graphicAsTransformable = dynamic_cast<const sf::Transformable*>(mainCharGraphical->getGraphic()))
				{
					view.setCenter(graphicAsTransformable->getPosition());
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
			delete obj;
		}

		for (auto const& pair : this->g_objects)
		{
			auto obj = pair.second;
			delete obj;
		}
	}
}