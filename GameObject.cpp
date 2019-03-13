#include "GameObject.h"

namespace Engine
{
	GameObjectID generateID()
	{
		static GameObjectID count;
		count++;
		return count;
	}

	GameObject::GameObject()
	{
		this->id = generateID();
	}

	GraphicalGameObject::GraphicalGameObject(sf::Sprite s)
	{
		this->graphic = new sf::Sprite(s);
	}

	GraphicalGameObject::GraphicalGameObject(sf::CircleShape cs)
	{
		this->graphic = new sf::CircleShape(cs);
	}

	GraphicalGameObject::GraphicalGameObject(sf::ConvexShape cx)
	{
		this->graphic = new sf::ConvexShape(cx);
	}

	GraphicalGameObject::GraphicalGameObject(sf::RectangleShape r)
	{
		this->graphic = new sf::RectangleShape(r);
	}

	GraphicalGameObject::GraphicalGameObject(sf::Text t)
	{
		this->graphic = new sf::Text(t);
	}

	GraphicalGameObject::GraphicalGameObject(sf::VertexArray va)
	{
		this->graphic = new sf::VertexArray(va);
	}

	GraphicalGameObject::GraphicalGameObject(sf::VertexBuffer vb)
	{
		this->graphic = new sf::VertexBuffer(vb);
	}

	void GraphicalGameObject::draw(sf::RenderWindow& win)
	{
		win.draw(*this->graphic);
	}

	void GraphicalGameObject::Collision(GraphicalGameObject& otherObject) 
	{

	}

	GraphicalGameObject::~GraphicalGameObject()
	{
		delete this->graphic;
	}

	sf::Drawable* GraphicalGameObject::getGraphic()
	{
		return this->graphic;
	}

	void GameObject::EveryFrame(uint64_t frameNumber) 
	{

	}

	void GameObject::Resized(sf::Event event)
	{

	}

	void GameObject::LostFocus(sf::Event event)
	{

	}

	void GameObject::GainedFocus(sf::Event event)
	{

	}

	void GameObject::TextEntered(sf::Event event)
	{

	}

	void GameObject::KeyPressed(sf::Event event)
	{

	}

	void GameObject::KeyReleased(sf::Event event)
	{

	}

	void GameObject::MouseWheelMoved(sf::Event event)
	{

	}

	void GameObject::MouseWheelScrolled(sf::Event event)
	{

	}

	void GameObject::MouseButtonPressed(sf::Event event)
	{

	}

	void GameObject::MouseButtonReleased(sf::Event event)
	{

	}

	void GameObject::MouseMoved(sf::Event event)
	{

	}

	void GameObject::MouseEntered(sf::Event event)
	{

	}

	void GameObject::MouseLeft(sf::Event event)
	{

	}

	void GameObject::JoystickButtonPressed(sf::Event event)
	{

	}

	void GameObject::JoystickButtonReleased(sf::Event event)
	{

	}

	void GameObject::JoystickMoved(sf::Event event)
	{

	}

	void GameObject::JoystickConnected(sf::Event event)
	{

	}

	void GameObject::JoystickDisconnected(sf::Event event)
	{

	}

	void GameObject::TouchBegan(sf::Event event)
	{

	}

	void GameObject::TouchMoved(sf::Event event)
	{

	}

	void GameObject::TouchEnded(sf::Event event)
	{

	}

	void GameObject::SensorChanged(sf::Event event)
	{

	}

	void GameObject::AddedToScreen()
	{

	}

	void GameObject::RemovedFromScreen()
	{

	}
}