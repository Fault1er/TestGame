#include "Renderer.hpp"


#include <SFML/Window.hpp>
#include <SFML/OpenGL.hpp>


auto Renderer::Instance() -> Renderer &
{
	static Renderer renderer;
	return renderer;
}

void Renderer::Prepare()
{
	glDisable( GL_DEPTH_TEST );

	RenderInfo default_settings;
	default_settings.priority = 0;
	RenderInfo::Push( default_settings );
}

void Renderer::ApplyRenderInfo( RenderInfo &info )
{
	if ( !info.drawable_objects.empty() ) {
		drawable_info_[info.priority].push_back( std::move( info ) );
	}
}

void Renderer::Clear()
{
	drawable_info_.clear();
}

void Renderer::RenderTo( sf::RenderWindow& window ) const
{
	for ( const auto& order : drawable_info_ ) {
		for ( const auto& drawable : order.second ) {
			/// TODO : тут используем шейдер
			for ( const auto& object : drawable.drawable_objects ) {
				if ( object != nullptr ) {
					window.draw( *object, drawable.state );
				}
			}
		}
	}

	window.display();
}
