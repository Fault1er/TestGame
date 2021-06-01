#include "ResourceManager.hpp"
#include "TextureResource.hpp"
#include "FontResource.hpp"


#include <Utils/JsonHelper.hpp>
#include <SFML/Graphics/Font.hpp>


auto ResourceManager::Instance() -> ResourceManager &
{
	static ResourceManager manager;
	return manager;
}

void ResourceManager::LoadResources( const std::string& file_name )
{
	std::string file_src = JsonHelper::LoadFromFile( file_name );

	rapidjson::Document doc;
	doc.Parse( file_src.c_str() );

	if ( doc.HasParseError() ) {
		assert( false );
		return;
	}

	if ( doc.HasMember( "textures" ) )
	{
		const auto& textures_node = doc.FindMember( "textures" )->value;
		if ( textures_node.IsArray() ) {
			for ( const auto& texture_node : textures_node.GetArray() )
			{
				std::string id, path;
				JsonHelper::GetMemberString( texture_node, "id", id );
				JsonHelper::GetMemberString( texture_node, "path", path );

				assert( !id.empty() && !path.empty() );
				if ( !id.empty() && !path.empty() )
				{
					auto* tex = new sf::Texture;
					tex->loadFromFile( path );
					resources_[id] = ResPtr( new TextureResource( tex ) );
				}
			}
		}
	}

	if ( doc.HasMember( "fonts" ) )
	{
		const auto& fonts_node = doc.FindMember( "fonts" )->value;
		if ( fonts_node.IsArray() ) {
			for ( const auto& font_node : fonts_node.GetArray() )
			{
				std::string id, path;
				JsonHelper::GetMemberString( font_node, "id", id );
				JsonHelper::GetMemberString( font_node, "path", path );

				assert( !id.empty() && !path.empty() );
				if ( !id.empty() && !path.empty() )
				{
					auto* font = new sf::Font;
					font->loadFromFile( path );
					resources_[id] = ResPtr( new FontResource( font ) );
				}
			}
		}
	}
}

void ResourceManager::FreeResources()
{
	resources_.clear();
}
