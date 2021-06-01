#pragma once


#include <SFML/Graphics/RenderWindow.hpp>
#include <Render/RenderInfo.hpp>


#include <map>
#include <vector>


class Renderer
{
	using Collection = std::map<int, std::vector<RenderInfo>>;

private:
	Collection drawable_info_;


public:
	static auto Instance() -> Renderer&;

	void Prepare();

	void ApplyRenderInfo( RenderInfo& info );

	void Clear();

	void RenderTo( sf::RenderWindow& window ) const;
};
