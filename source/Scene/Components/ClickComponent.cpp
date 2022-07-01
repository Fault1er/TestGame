#include "ClickComponent.hpp"


#include <Scene/GameObject.hpp>
#include <Utils/JsonHelper.hpp>
#include <SFML/Graphics/Rect.hpp>

#include <cmath>


bool ClickComponent::Contains( const sf::Vector2i& point ) const
{
	sf::Vector2f pos;

	GameObject* owner = owner_;
	while ( owner != nullptr ) {
		pos += owner->GetPosition();
		owner = owner->GetParent();
	}

	sf::IntRect rect{ sf::Vector2i{ int( std::round( pos.x ) ), int( std::round( pos.y ) ) }, sf::Vector2i{ width_, height_ } };

	return rect.contains( point );
}


ClickComponent::ClickComponent( GameObject* owner, const rapidjson::Document::GenericValue& node )
: ObjectComponent( owner, node )
{
	if ( !node.IsObject() ) {
		return;
	}

	JsonHelper::GetMemberInt( node, "width", width_ );
	JsonHelper::GetMemberInt( node, "height", height_ );
}

void ClickComponent::OnPressed( const sf::Vector2i& point )
{
	is_pressed = Contains( point );
}

void ClickComponent::OnReleased( const sf::Vector2i& point )
{
	if ( action_ != nullptr && is_pressed && Contains( point ) ) {
		action_();
	}
	is_pressed = false;
}

void ClickComponent::OnEnable()
{
	ObjectComponent::OnEnable();

	InputManager::Instance().Subscribe( this );
}

void ClickComponent::OnDisable()
{
	ObjectComponent::OnDisable();

	is_pressed = false;
	InputManager::Instance().Unsubscribe( this );
}

void ClickComponent::SetAction( const Action& action )
{
	action_ = action;
}
