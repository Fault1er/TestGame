#pragma once


#include <Resource/IResource.hpp>
#include <SFML/Graphics/Texture.hpp>


#include <memory>


class TextureResource : public IResource
{
private:
	std::unique_ptr<sf::Texture> texture_ = nullptr;

public:
	explicit TextureResource( sf::Texture* texture ) : texture_( texture ) {}

	auto Get() -> sf::Texture*
	{
		return texture_.get();
	}
};
