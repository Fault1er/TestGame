#pragma once


#include <SFML/Graphics/RenderStates.hpp>


#include <stack>
#include <vector>


namespace sf
{
	class Drawable;
}

class RenderInfo
{
private:
	static auto GetStack() -> std::stack<RenderInfo>&;

public:
	std::vector<sf::Drawable*> drawable_objects;
	sf::RenderStates state = sf::RenderStates::Default;
	int priority = 0;

	static void Push( const RenderInfo& settings );
	static auto Pop() -> RenderInfo;
	static auto Top() -> RenderInfo&;
	static void Clear();
};
