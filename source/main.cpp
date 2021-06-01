#include <SFML/OpenGL.hpp>
#include <SFML/Window.hpp>


#include <Render/Renderer.hpp>
#include <Resource/ResourceManager.hpp>
#include <Scene/Scene.hpp>


#include <Scene/InputManager.hpp>


int main()
{
    sf::RenderWindow window( sf::VideoMode( 512, 512 ), "TestGame", sf::Style::Close );

	/// Загружаем ресурсы
	ResourceManager::Instance().LoadResources( "Resources.json" );

	/// Настройка отрисовки
	Renderer::Instance().Prepare();

	/// Загружаем сцену
    Scene::ReadFromFile( "GameField.json" );

	sf::Clock deltaClock;

	/// Обработка событий, отображение и логика игры
    while ( window.isOpen() )
    {
	    Renderer::Instance().Clear();

        sf::Event event{};
        while ( window.pollEvent( event ) )
        {
            switch ( event.type )
            {
                case sf::Event::Closed:
                {
                    window.close();
                }   break;
                case sf::Event::MouseButtonPressed:
                {
                	InputManager::Instance().OnPressed( { event.mouseButton.x, event.mouseButton.y } );
                }   break;
                case sf::Event::MouseButtonReleased:
                {
	                InputManager::Instance().OnReleased( { event.mouseButton.x, event.mouseButton.y } );
                }   break;
                default:
                    break;
            }
        }

	    sf::Time dt = deltaClock.restart();
	    if ( auto scene = Scene::GetCurrentScene() ) {
		    scene->Update( dt.asSeconds() );
	    }

	    Renderer::Instance().RenderTo( window );
    }

    /// Освобождаем ресурсы
	ResourceManager::Instance().FreeResources();

    return 0;
}
