#include "ObjectComponent.hpp"
#include "RenderPriority.hpp"
#include "SpriteComponent.hpp"
#include "ClickComponent.hpp"
#include "ButtonComponent.hpp"
#include "TextComponent.hpp"


#include <Game/Components/CellComponent.hpp>


#include <Scene/GameObject.hpp>
#include <Utils/JsonHelper.hpp>
#include <Game/Components/FieldManager.hpp>


void ObjectComponent::Deleter( ObjectComponent* component )
{
	component->OnDisable();
	component->OnDestroy();
	delete component;
}

auto ObjectComponent::GetOwner() const -> GameObject*
{
	return owner_;
}

auto ObjectComponent::CreateComponent( GameObject* owner, const rapidjson::Document::GenericValue &node ) -> ObjectComponent::Ptr
{
	ObjectComponent::Ptr component;
	std::string component_name;

	JsonHelper::GetMemberString( node, "id", component_name );

	if ( component_name == "priority" ) {
		component = ObjectComponent::Ptr( new RenderPriority( owner, node.FindMember( "data" )->value ), Deleter );
	}
	else if ( component_name == "sprite" ) {
		component = ObjectComponent::Ptr( new SpriteComponent( owner, node.FindMember( "data" )->value ), Deleter );
	}
	else if ( component_name == "text" ) {
		component = ObjectComponent::Ptr( new TextComponent( owner, node.FindMember( "data" )->value ), Deleter );
	}
	else if ( component_name == "click" ) {
		component = ObjectComponent::Ptr( new ClickComponent( owner, node.FindMember( "data" )->value ), Deleter );
	}
	else if ( component_name == "button" ) {
		component = ObjectComponent::Ptr( new ButtonComponent( owner, node.FindMember( "data" )->value ), Deleter );
	}
	else if ( component_name == "cell" ) {
		component = ObjectComponent::Ptr( new Game::CellComponent( owner, node.FindMember( "data" )->value ), Deleter );
	}
	else if ( component_name == "field_manager" ) {
		component = ObjectComponent::Ptr( new Game::FieldManager( owner, node.FindMember( "data" )->value ), Deleter );
	}

	assert( component != nullptr );
	return component;
}
