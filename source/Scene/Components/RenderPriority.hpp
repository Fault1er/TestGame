#pragma once


#include <Scene/Components/ObjectComponent.hpp>


class RenderPriority : public ObjectComponent
{
private:
	int priority_ = 0;

public:
	explicit RenderPriority( GameObject* owner, const rapidjson::Document::GenericValue& node );

	void Update( float dt ) override;
};
