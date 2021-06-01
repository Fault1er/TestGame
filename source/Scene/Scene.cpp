#include "Scene.hpp"


#include <Utils/JsonHelper.hpp>


#include <memory>
#include <stdexcept>


std::unique_ptr<Scene> Scene::active_scene_ = nullptr;


void Scene::Load( const std::string &file_src )
{
    rapidjson::Document doc;
    doc.Parse( file_src.c_str() );

    if ( doc.HasParseError() || !doc.IsObject() || !doc.HasMember( "objects" ) ) {
        return;
    }

    auto& children_member = doc.FindMember("objects" )->value;
    if ( !children_member.IsArray() ) {
        assert( false );
        return;
    }

    const auto& children_array = children_member.GetArray();
    objects_.reserve( children_array.Size() );

    for ( const auto& child_node : children_array ) {
        objects_.emplace_back( new GameObject( nullptr, child_node ) );
    }
}

void Scene::Update( float dt )
{
	for ( auto& object : objects_ ) {
		if ( object != nullptr ) {
			object->Update( dt );
		}
	}
}

auto Scene::GetObject( const std::string& name ) -> GameObject *
{
	for ( auto& object : objects_ ) {
		if ( object != nullptr && object->name_ == name ) {
			return object.get();
		}
	}

	return nullptr;
}

void Scene::ReadFromFile( const std::string& file_name )
{
	active_scene_ = std::make_unique<Scene>();
	active_scene_->Load( JsonHelper::LoadFromFile( file_name ) );
}

auto Scene::GetCurrentScene() -> Scene*
{
	return active_scene_.get();
}
