#include "SFML\Graphics.hpp"
#include "Screen.h"
#include "GameObject.h"
#include <thread>
#include <memory>
#include <atomic>
#include <functional>
#include <mutex>

#include <iostream>

unsigned int Screen::windowWidth = 0;
unsigned int Screen::windowHeight = 0;
const char* Screen::windowTitle = nullptr;

static std::mutex l;
static std::atomic<Screen*> currentScreen;
static GameObject* objectsToAdd[UINT16_MAX];
static GameObject* objectsToRemove[UINT16_MAX];
static std::atomic<int> addCount = 0;
static std::atomic<int> removeCount = 0;
static std::thread::id renderThreadID;

bool Screen::_add(GameObject* gameObject, GameObjectMap& map)
{
	bool result = true;
	l.lock();
	try
	{
		if (std::this_thread::get_id() != renderThreadID && currentScreen.load() == this)
		{
			int index = addCount.fetch_add(1);
			if (index >= UINT16_MAX)
			{
				addCount.fetch_sub(1);
				result = false;
			}
			else
			{
				objectsToAdd[index] = gameObject;
			}
		}
		else
		{
			map[gameObject->getID()] = gameObject;
		}
	}
	catch (...) {}
	l.unlock();
	return result;
}

bool Screen::_remove(GameObject* gameObject, GameObjectMap& map)
{
	bool result = true;
	l.lock();
	try
	{
		if (std::this_thread::get_id() != renderThreadID && currentScreen.load() == this)
		{
			int index = removeCount.fetch_add(1);
			objectsToRemove[index] = gameObject;			
		}
		else
		{
			GameObjectID id = gameObject->getID();
			GameObject* obj = map[id];
			map.erase(id);
		}
	}
	catch (...) {}
	l.unlock();
	return result;
}

void Screen::add(GameObject* gameObject)
{
	GameObjectMap& map = (dynamic_cast<GraphicalGameObject*>(gameObject)) ? this->g_objects : this->objects;
	while(!this->_add(gameObject, map)) { }
}

void Screen::remove(GameObject* gameObject)
{
	GameObjectMap& map = (dynamic_cast<GraphicalGameObject*>(gameObject)) ? this->g_objects : this->objects;
	if (map.find(gameObject->getID()) == map.end()) { return; }
	while (!this->_remove(gameObject, map)) { }
}

std::thread* Screen::render(int fps)
{
	if (fps < 1) { fps = 1; }
	else if (fps > 1000) { fps = 1000; }
	
	static std::atomic<int> _fps;
	static bool renderThreadStarted = false;
	static std::thread* returnPtr;

	l.lock();
	currentScreen = this;
	l.unlock();

	_fps = fps;
	
	if (renderThreadStarted) { return returnPtr; }
	renderThreadStarted = true;
	
	static std::function<void (GameObjectMap&, sf::Event)> handleEvents = [](GameObjectMap& objects, sf::Event event)
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

	static std::thread renderThread([]()
	{
		unsigned int width = (Screen::windowWidth) ? Screen::windowWidth : 500;
		unsigned int height = (Screen::windowHeight) ? Screen::windowHeight : 500;
		const char* title = (Screen::windowTitle) ? Screen::windowTitle : "<no title>";
		sf::RenderWindow window(sf::VideoMode(width, height), title);
		sf::Clock clock;
		uint64_t frameCount = 0;
			   
		while (window.isOpen())
		{
			clock.restart();
			Screen* cs = currentScreen.load();
			int addIndex = addCount.load();

			while (addIndex > 0)
			{
				l.lock();
				cs->add(objectsToAdd[addIndex]);
				addCount--;
				l.unlock();
			} 

			int removeIndex = removeCount.load();
			while (removeIndex > 0)
			{
				l.lock();
				cs->remove(objectsToAdd[removeIndex]);
				removeIndex = --removeCount;
				l.unlock();
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
			window.display();
			frameCount++;
			int __fps = _fps.load();
			while (clock.getElapsedTime().asMicroseconds() < (1000000 / __fps)) {}
		}

	});
	returnPtr = &renderThread;
	renderThreadID = renderThread.get_id();
	return returnPtr;
}