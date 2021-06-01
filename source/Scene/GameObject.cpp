#include "GameObject.hpp"


#include <Render/Renderer.hpp>
#include <Utils/JsonHelper.hpp>


GameObject::GameObject( GameObject* parent, const rapidjson::Document::GenericValue& node )
: parent_( parent )
{
	Load( node );
}


void GameObject::Load( const rapidjson::Document::GenericValue& node )
{
	if ( !node.IsObject() ) {
		assert( false );
		return;
	}

	/// Загружаем информацию об объекте
	JsonHelper::GetMemberString( node, "name", name_ );

	if ( node.HasMember( "pos" ) )
	{
		const auto& pos_member = node.FindMember( "pos" )->value;
		JsonHelper::GetMemberFloat( pos_member, "x", position_.x );
		JsonHelper::GetMemberFloat( pos_member, "y", position_.y );
	}

	JsonHelper::GetMemberBool( node, "active", is_active_, true );

	/// Создаём объекты, потом компоненты
	if ( node.HasMember( "children" ) ) {
		const auto& children_member = node.FindMember( "children" )->value;
		if ( children_member.IsArray() )
		{
			const auto& children_array = children_member.GetArray();
			children_.clear();
			children_.reserve( children_array.Size() );
			for ( const auto& child_member : children_array ) {
				children_.emplace_back( new GameObject( this, child_member ) );
			}
		}
	}

	/// При добавлении компоненты, можно смотреть на наличие компонентов у потомков
	if ( node.HasMember( "components" ) ) {
		const auto& components_member = node.FindMember( "components" )->value;
		if ( components_member.IsArray() )
		{
			const auto& components_array = components_member.GetArray();
			components_.clear();
			components_.reserve( components_array.Size() );
			for ( const auto& component_node : components_array ) {
				if ( auto& component = components_.emplace_back( ObjectComponent::CreateComponent( this, component_node ) ) ) {
					component->OnCreate();
					component->OnEnable();
				}
			}
		}
	}
}

auto GameObject::GetName() const -> const std::string&
{
    return name_;
}

auto GameObject::GetPosition() const -> const sf::Vector2f &
{
	return position_;
}

auto GameObject::GetParent() -> GameObject*
{
	return parent_;
}

auto GameObject::GetChildren() -> GameObjects&
{
	return children_;
}

auto GameObject::GetChild(const std::string& name) -> GameObject*
{
	for ( auto& child : children_ ) {
		if ( child != nullptr && child->name_ == name ) {
			return child.get();
		}
	}
	return nullptr;
}

auto GameObject::FindChild( const std::string& name ) -> GameObject*
{
	if ( auto* child = GetChild( name ) ) {
		return child;
	}
	for ( auto& child : children_ ) {
		GameObject* object = child != nullptr ? child->FindChild( name ) : nullptr;
		if ( object != nullptr && object->name_ == name ) {
			return object;
		}
	}

	return nullptr;
}

void GameObject::Update( float dt )
{
	if ( !is_active_ ) {
		return;
	}

	RenderInfo info;
	const auto& last_info = RenderInfo::Top();
	info.priority = last_info.priority;
	info.state.transform = last_info.state.transform;
	info.state.transform.translate( position_ );
	RenderInfo::Push( info );

    for ( auto& component : components_ ) {
        if ( component != nullptr ) {
            component->Update( dt );
        }
    }

    for ( auto& child : children_ ) {
    	if ( child != nullptr ) {
	    	child->Update( dt );
    	}
    }

	info = RenderInfo::Pop();
    Renderer::Instance().ApplyRenderInfo( info );
}

bool GameObject::IsActive() const
{
	return is_active_;
}

void GameObject::SetActive( bool active )
{
	if ( is_active_ && !active ) {
		for ( auto& component : components_ ) {
			if ( component != nullptr ) {
				component->OnDisable();
			}
		}
	} else if ( !is_active_ && active ) {
		for ( auto& component : components_ ) {
			if ( component != nullptr ) {
				component->OnEnable();
			}
		}
	}

	is_active_ = active;
}

void GameObject::MoveToChildren( GameObject* old_owner, const std::string& name )
{
	assert( old_owner != nullptr && !name.empty() );

	for ( auto& child : old_owner->children_ ) {
		if ( child != nullptr && child->name_ == name )
		{
			std::swap( child, old_owner->children_.back() );
			children_.push_back( std::move( old_owner->children_.back() ) );
			old_owner->children_.pop_back();
			return;
		}
	}
}
