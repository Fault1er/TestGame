#include "InputManager.hpp"


auto InputManager::Instance() -> InputManager &
{
	static InputManager manager;
	return manager;
}

void InputManager::OnPressed( const sf::Vector2i& point )
{
	for ( auto& listener : listeners_ ) {
		listener->OnPressed( point );
	}
}

void InputManager::OnReleased( const sf::Vector2i& point )
{
	for ( auto& listener : listeners_ ) {
		listener->OnReleased( point );
	}
}

void InputManager::Subscribe( IInputListener* listener )
{
	if ( listener != nullptr ) {
		listeners_.push_back( listener );
	}
}

void InputManager::Unsubscribe( IInputListener* listener )
{
	auto it = std::find( listeners_.begin(), listeners_.end(), listener );
	if ( it != listeners_.end() ) {
		std::swap( *it, listeners_.back() );
		listeners_.pop_back();
	}
}
