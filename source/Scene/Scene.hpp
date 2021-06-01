#pragma once


#include <Scene/GameObject.hpp>


class GameObject;

class Scene
{
private:
    GameObjects objects_{};
    static std::unique_ptr<Scene> active_scene_;

    void Load( const std::string& file_src );

public:
    void Update( float dt );

    [[nodiscard]]
    auto GetObject( const std::string& name ) -> GameObject*;

	static void ReadFromFile( const std::string& file_name );

    static auto GetCurrentScene() -> Scene*;
};
