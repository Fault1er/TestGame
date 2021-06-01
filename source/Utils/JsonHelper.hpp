#pragma once


#include <Utils/rapidjson/document.h>


#include <string>


namespace JsonHelper
{
	auto LoadFromFile( const std::string& file_name ) -> std::string;

	void GetMemberString( const rapidjson::Document::GenericValue& node, const std::string& id, std::string& out, const std::string& def = "" );

	void GetMemberInt( const rapidjson::Document::GenericValue& node, const std::string& id, int& out, int def = 0 );

	void GetMemberFloat( const rapidjson::Document::GenericValue& node, const std::string& id, float& out, float def = 0 );

	void GetMemberBool( const rapidjson::Document::GenericValue& node, const std::string& id, bool& out, bool def = false );
}
