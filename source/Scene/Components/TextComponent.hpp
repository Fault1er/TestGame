#pragma once


#include <SFML/Graphics/Text.hpp>
#include <Scene/Components/ObjectComponent.hpp>


class TextComponent : public ObjectComponent
{
private:
	sf::Text text_;

public:
	TextComponent( GameObject* owner, const rapidjson::Document::GenericValue& node );

	void Update( float dt ) override;
};
