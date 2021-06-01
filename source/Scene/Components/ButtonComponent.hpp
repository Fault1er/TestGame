#pragma once


#include <Scene/Components/ClickComponent.hpp>
#include <SFML/Graphics/Sprite.hpp>


class ButtonComponent : public ClickComponent
{
private:
	sf::Sprite pressed_, released_;

public:
	ButtonComponent( GameObject* owner, const rapidjson::Document::GenericValue& node );

	void Update( float dt ) override;
};

