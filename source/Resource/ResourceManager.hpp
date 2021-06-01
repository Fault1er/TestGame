#pragma once


#include <SFML/Window/GlResource.hpp>
#include <Resource/IResource.hpp>


#include <unordered_map>
#include <memory>
#include <cassert>


class ResourceManager
{
private:
	using ResPtr = std::unique_ptr<IResource>;

	std::unordered_map<std::string, ResPtr> resources_;

public:
	static auto Instance() -> ResourceManager&;

	void LoadResources( const std::string& file_name );

	template<class Resource>
	auto RequireResource( const std::string& resource_id ) -> Resource*
	{
		assert( !resource_id.empty() );

		Resource* resource = nullptr;
		auto res_it = resources_.find( resource_id );

		if ( res_it != resources_.end() ) {
			resource = dynamic_cast<Resource*>( res_it->second.get() );
			assert( resource != nullptr );
		}

		return resource;
	}

	void FreeResources();
};
