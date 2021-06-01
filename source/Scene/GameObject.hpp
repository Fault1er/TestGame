#pragma once


#include <SFML/System/Vector2.hpp>
#include <Utils/rapidjson/document.h>
#include <Scene/Components/ObjectComponent.hpp>

#include <vector>
#include <string>
#include <memory>


class GameObject;

using GameObjects = std::vector<std::unique_ptr<GameObject>>;
using ObjectComponents = std::vector<ObjectComponent::Ptr>;

class GameObject
{
	friend class Scene;

private:
    std::string         name_{};
    GameObjects         children_{};
    ObjectComponents    components_{};
    sf::Vector2f        position_{};
    GameObject*         parent_{};
    bool                is_active_ = true;

private:
	void Load( const rapidjson::Document::GenericValue& node );

public:
	GameObject() = default;
    GameObject( GameObject* parent, const rapidjson::Document::GenericValue& node );

    [[nodiscard]]
    auto GetName() const -> const std::string&;

    [[nodiscard]]
	auto GetPosition() const -> const sf::Vector2f&;

    [[nodiscard]]
	auto GetParent() -> GameObject*;

    [[nodiscard]]
    auto GetChildren() -> GameObjects&;

    [[nodiscard]]
    auto GetChild( const std::string& name ) -> GameObject*;

    [[nodiscard]]
    auto FindChild( const std::string& name ) -> GameObject*;

	template<class T>
    auto GetComponent() -> T*
	{
    	for ( auto& component : components_ ) {
    		if ( auto component_ptr = dynamic_cast<T*>( component.get() ) ) {
			    return component_ptr;
    		}
    	}
		return nullptr;
	}

    void Update( float dt );

    [[nodiscard]]
    bool IsActive() const;

    void SetActive( bool active );

    void MoveToChildren( GameObject* old_owner, const std::string& name );
};

