#include "SpriteComponent.hpp"


#include <Render/RenderInfo.hpp>
#include <Utils/JsonHelper.hpp>
#include <Resource/ResourceManager.hpp>
#include <Resource/TextureResource.hpp>


SpriteComponent::SpriteComponent( GameObject* owner, const rapidjson::Document::GenericValue& node )
: ObjectComponent( owner, node )
{
	if ( !node.IsObject() ) {
		assert( false );
		return;
	}

	if ( node.HasMember( "texture" ) )
	{
		std::string texture_id;
		JsonHelper::GetMemberString( node, "texture", texture_id );

		auto* texture = ResourceManager::Instance().RequireResource<TextureResource>( texture_id );
		if ( texture != nullptr ) {
			sprite_.setTexture( *texture->Get() );
		}
	}

	if ( node.HasMember( "rect" ) )
	{
		const auto& rect_node = node.FindMember( "rect" )->value;
		if ( rect_node.IsObject() )
		{
			sf::IntRect rect;
			JsonHelper::GetMemberInt( rect_node, "x", rect.left );
			JsonHelper::GetMemberInt( rect_node, "y", rect.top );
			JsonHelper::GetMemberInt( rect_node, "width", rect.width );
			JsonHelper::GetMemberInt( rect_node, "height", rect.height );

			sprite_.setTextureRect( rect );
		}
	}
}

void SpriteComponent::Update( float dt )
{
	ObjectComponent::Update( dt );

	auto& info = RenderInfo::Top();
	info.drawable_objects.push_back( &sprite_ );
}
