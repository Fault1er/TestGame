#include "TextComponent.hpp"


#include <Render/RenderInfo.hpp>
#include <Utils/JsonHelper.hpp>
#include <Resource/ResourceManager.hpp>
#include <Resource/FontResource.hpp>


TextComponent::TextComponent( GameObject* owner, const rapidjson::Document::GenericValue& node )
: ObjectComponent( owner, node )
{
	if ( !node.IsObject() ) {
		assert( false );
		return;
	}

	std::string font_id;
	JsonHelper::GetMemberString( node, "font", font_id );

	auto* font = ResourceManager::Instance().RequireResource<FontResource>( font_id );
	if ( font != nullptr ) {
		text_.setFont( *font->Get() );
	}

	std::string source;
	JsonHelper::GetMemberString( node, "source", source );
	text_.setString( source );

	if ( node.HasMember( "pos" ) )
	{
		const auto& pos_member = node.FindMember( "pos" )->value;
		sf::Vector2f pos;
		JsonHelper::GetMemberFloat( pos_member, "x", pos.x );
		JsonHelper::GetMemberFloat( pos_member, "y", pos.y );
		text_.setPosition( pos );
	}

	text_.setCharacterSize( 16 );
	text_.setFillColor( sf::Color::Black );
}

void TextComponent::Update( float dt )
{
	ObjectComponent::Update( dt );

	auto& info = RenderInfo::Top();
	info.drawable_objects.push_back( &text_ );
}
