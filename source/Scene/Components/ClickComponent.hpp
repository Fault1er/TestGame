#pragma once


#include <Scene/Components/ObjectComponent.hpp>
#include <Scene/InputManager.hpp>


class ClickComponent : public ObjectComponent, public IInputListener
{
public:
	using Action = std::function<void()>;

protected:
	Action action_ = nullptr;
	int width_ = 0, height_ = 0;
	bool is_pressed = false;

protected:
	[[nodiscard]]
	bool Contains( const sf::Vector2i& point ) const;

public:
	ClickComponent( GameObject* owner, const rapidjson::Document::GenericValue& node );


	void OnPressed( const sf::Vector2i& point ) override;

	void OnReleased( const sf::Vector2i& point ) override;

	void OnEnable() override;

	void OnDisable() override;

	void SetAction( const Action& action );
};
