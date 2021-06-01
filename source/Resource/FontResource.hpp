#pragma once


#include <Resource/IResource.hpp>
#include <SFML/Graphics/Font.hpp>


class FontResource : public IResource
{
private:
	std::unique_ptr<sf::Font> font_ = nullptr;

public:
	explicit FontResource( sf::Font* font ) : font_( font ) {}

	auto Get() -> sf::Font*
	{
		return font_.get();
	}
};
