#ifndef GAME_OBJECT_HEADER
#define GAME_OBJECT_HEADER

#include "SFML/Graphics.hpp"
#include <cstdint>
#include <map>

namespace Engine
{
	typedef uint64_t GameObjectID;
    class Screen;
	class GameObject
	{
	public:
		GameObject();
		virtual void Resized(sf::Event event);                ///< The window was resized (data in event.size)
		virtual void LostFocus(sf::Event event);              ///< The window lost the focus (no data)
		virtual void GainedFocus(sf::Event event);            ///< The window gained the focus (no data)
		virtual void TextEntered(sf::Event event);            ///< A character was entered (data in event.text)
		virtual void KeyPressed(sf::Event event);             ///< A key was pressed (data in event.key)
		virtual void KeyReleased(sf::Event event);            ///< A key was released (data in event.key)
		virtual void MouseWheelMoved(sf::Event event);        ///< The mouse wheel was scrolled (data in event.mouseWheel) (deprecated)
		virtual void MouseWheelScrolled(sf::Event event);     ///< The mouse wheel was scrolled (data in event.mouseWheelScroll)
		virtual void MouseButtonPressed(sf::Event event);     ///< A mouse button was pressed (data in event.mouseButton)
		virtual void MouseButtonReleased(sf::Event event);    ///< A mouse button was released (data in event.mouseButton)
		virtual void MouseMoved(sf::Event event);             ///< The mouse cursor moved (data in event.mouseMove)
		virtual void MouseEntered(sf::Event event);           ///< The mouse cursor entered the area of the window (no data)
		virtual void MouseLeft(sf::Event event);              ///< The mouse cursor left the area of the window (no data)
		virtual void JoystickButtonPressed(sf::Event event);  ///< A joystick button was pressed (data in event.joystickButton)
		virtual void JoystickButtonReleased(sf::Event event); ///< A joystick button was released (data in event.joystickButton)
		virtual void JoystickMoved(sf::Event event);          ///< The joystick moved along an axis (data in event.joystickMove)
		virtual void JoystickConnected(sf::Event event);      ///< A joystick was connected (data in event.joystickConnect)
		virtual void JoystickDisconnected(sf::Event event);   ///< A joystick was disconnected (data in event.joystickConnect)
		virtual void TouchBegan(sf::Event event);             ///< A touch event began (data in event.touch)
		virtual void TouchMoved(sf::Event event);             ///< A touch moved (data in event.touch)
		virtual void TouchEnded(sf::Event event);             ///< A touch event ended (data in event.touch)
		virtual void SensorChanged(sf::Event event);          ///< A sensor value changed (data in event.sensor)
		virtual void EveryFrame(uint64_t frameNumber); //runs every frame if implemented
		virtual void AddedToScreen(); //triggers when the object is added to the screen
		virtual void RemovedFromScreen(); //triggers when the object is removed from the screen
		GameObjectID getID() { return this->id; }
	private:
		GameObject(GameObjectID id);
	protected:
		friend class Screen;
		GameObjectID id;
		Screen* screen = nullptr;
	};

	class GraphicalGameObject : public GameObject
	{
	public:
		GraphicalGameObject() = delete;
		GraphicalGameObject(sf::Sprite);
		GraphicalGameObject(sf::CircleShape);
		GraphicalGameObject(sf::ConvexShape);
		GraphicalGameObject(sf::RectangleShape);
		GraphicalGameObject(sf::Text);
		GraphicalGameObject(sf::VertexArray);
		GraphicalGameObject(sf::VertexBuffer);
		virtual void draw(sf::RenderWindow& win);
		virtual void Collision(GraphicalGameObject& otherObject);
		~GraphicalGameObject();
		sf::Drawable* getGraphic();
		bool triggerCollisionEvents = true;
		bool blockingCollision = true;
		bool ignoreObstacles = false;
		sf::FloatRect obstacleCollisionSize;
		sf::FloatRect objectCollisionSize;
	protected:
		sf::Drawable* graphic;
	private:
		friend class Screen;
		sf::Vector2f lastPos;
	};
}

#endif
