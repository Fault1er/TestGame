#include "RenderPriority.hpp"


#include <Render/RenderInfo.hpp>
#include <Utils/JsonHelper.hpp>


RenderPriority::RenderPriority( GameObject* owner, const rapidjson::Document::GenericValue& node )
: ObjectComponent( owner, node )
{
	if ( !node.IsObject() ) {
		assert( false );
		return;
	}

	JsonHelper::GetMemberInt( node, "priority", priority_ );
}

void RenderPriority::Update( float dt )
{
	ObjectComponent::Update( dt );

	RenderInfo::Top().priority = priority_;
}
