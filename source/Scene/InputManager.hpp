#pragma once


#include <SFML/System/Vector2.hpp>


#include <vector>


class InputManager;

class IInputListener
{
	friend class InputManager;
protected:
	virtual void OnPressed( const sf::Vector2i& point ) = 0;
	virtual void OnReleased( const sf::Vector2i& point ) = 0;
};


class InputManager
{
private:
	std::vector<IInputListener*> listeners_;

public:
	static auto Instance() -> InputManager&;

	void OnPressed( const sf::Vector2i& point );

	void OnReleased( const sf::Vector2i& point );

	void Subscribe( IInputListener* listener );

	void Unsubscribe( IInputListener* listener );
};
