#include "ButtonComponent.hpp"


#include <Resource/ResourceManager.hpp>
#include <Resource/TextureResource.hpp>
#include <Render/RenderInfo.hpp>
#include <Utils/JsonHelper.hpp>


ButtonComponent::ButtonComponent( GameObject* owner, const rapidjson::Document::GenericValue& node )
: ClickComponent( owner, node )
{
	if ( !node.IsObject() ) {
		assert( false );
		return;
	}

	std::string pressed_tex, released_tex;

	JsonHelper::GetMemberString( node, "pressed_tex", pressed_tex );
	auto* p_texture = ResourceManager::Instance().RequireResource<TextureResource>( pressed_tex );
	if ( p_texture != nullptr ) {
		pressed_.setTexture( *p_texture->Get() );
	}

	JsonHelper::GetMemberString( node, "released_tex", released_tex );
	auto* r_texture = ResourceManager::Instance().RequireResource<TextureResource>( released_tex );
	if ( r_texture != nullptr ) {
		released_.setTexture( *r_texture->Get() );
	}

	if ( node.HasMember( "p_rect" ) )
	{
		const auto& rect_node = node.FindMember( "p_rect" )->value;
		if ( rect_node.IsObject() )
		{
			sf::IntRect rect;
			JsonHelper::GetMemberInt( rect_node, "x", rect.left );
			JsonHelper::GetMemberInt( rect_node, "y", rect.top );
			JsonHelper::GetMemberInt( rect_node, "width", rect.width );
			JsonHelper::GetMemberInt( rect_node, "height", rect.height );

			pressed_.setTextureRect( rect );
		}
	}

	if ( node.HasMember( "r_rect" ) )
	{
		const auto& rect_node = node.FindMember( "r_rect" )->value;
		if ( rect_node.IsObject() )
		{
			sf::IntRect rect;
			JsonHelper::GetMemberInt( rect_node, "x", rect.left );
			JsonHelper::GetMemberInt( rect_node, "y", rect.top );
			JsonHelper::GetMemberInt( rect_node, "width", rect.width );
			JsonHelper::GetMemberInt( rect_node, "height", rect.height );

			released_.setTextureRect( rect );
		}
	}
}

void ButtonComponent::Update( float dt )
{
	ObjectComponent::Update( dt );

	auto& info = RenderInfo::Top();
	info.drawable_objects.push_back( is_pressed ? &pressed_ : &released_ );
}
