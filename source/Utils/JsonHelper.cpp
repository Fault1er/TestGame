#include "JsonHelper.hpp"


#include <sstream>
#include <fstream>
#include <cassert>


namespace JsonHelper
{
	auto LoadFromFile( const std::string& file_name ) -> std::string
	{
		std::ifstream file;

		file.open( file_name );
		if ( !file.is_open() ) {
			assert( false );
			return std::string();
		}

		std::stringstream file_src;
		std::string line;
		while ( !file.eof() ) {
			std::getline( file, line );
			file_src << line;
		}

		file.close();

		return file_src.str();
	}

	void GetMemberString( const rapidjson::Document::GenericValue& node, const std::string& id, std::string& out, const std::string& def )
	{
		if ( node.IsObject() && node.HasMember( id.c_str() ) ) {
			const auto& member_node = node.FindMember( id.c_str() )->value;
			if ( member_node.IsString() ) {
				out = member_node.GetString();
				return;
			}
		}
		out = def;
	}

	void GetMemberInt( const rapidjson::Document::GenericValue& node, const std::string& id, int& out, int def )
	{
		if ( node.IsObject() && node.HasMember( id.c_str() ) ) {
			const auto& member_node = node.FindMember( id.c_str() )->value;
			if ( member_node.IsInt() ) {
				out = member_node.GetInt();
				return;
			}
		}
		out = def;
	}

	void GetMemberFloat( const rapidjson::Document::GenericValue& node, const std::string& id, float& out, float def )
	{
		if ( node.IsObject() && node.HasMember( id.c_str() ) ) {
			const auto& member_node = node.FindMember( id.c_str() )->value;
			if ( member_node.IsNumber() ) {
				out = member_node.GetFloat();
				return;
			}
		}
		out = def;
	}

	void GetMemberBool( const rapidjson::Document::GenericValue& node, const std::string& id, bool& out, bool def )
	{
		if ( node.IsObject() && node.HasMember( id.c_str() ) ) {
			const auto& member_node = node.FindMember( id.c_str() )->value;
			if ( member_node.IsBool() ) {
				out = member_node.GetBool();
				return;
			}
		}
		out = def;
	}
}