#pragma once


#include <SFML/Graphics/Sprite.hpp>
#include <Scene/Components/ObjectComponent.hpp>


class SpriteComponent : public ObjectComponent
{
private:
	sf::Sprite  sprite_;

public:
	explicit SpriteComponent( GameObject* owner, const rapidjson::Document::GenericValue& node );

	void Update( float dt ) override;
};
