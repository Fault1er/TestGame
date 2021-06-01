#include "CellComponent.hpp"

#include <Scene/GameObject.hpp>
#include <Utils/JsonHelper.hpp>


namespace Game
{
	CellComponent::CellComponent( GameObject* owner, const rapidjson::Document::GenericValue & node)
	: ObjectComponent( owner, node )
	{
		if ( !node.IsObject() ) {
			return;
		}

		if ( node.HasMember( "pos" ) ) {
			const auto& pos_node = node.FindMember( "pos" )->value;
			JsonHelper::GetMemberInt( node, "x", pos_.x );
			JsonHelper::GetMemberInt( node, "y", pos_.y );
		}

		std::string pawn_str;
		JsonHelper::GetMemberString( node, "pawn", pawn_str );

		if ( pawn_str == "black" ) {
			pawn_ = Pawn::BLACK;
		} else if ( pawn_str == "white" ) {
			pawn_ = Pawn::WHITE;
		} else {
			pawn_ = Pawn::NONE;
		}
	}

	auto CellComponent::GetPos() const -> const sf::Vector2i &
	{
		return pos_;
	}

	void CellComponent::SetPos( const sf::Vector2i& pos )
	{
		pos_ = pos;
	}

	bool CellComponent::HasPawn( Pawn pawn ) const
	{
		return pawn == pawn_;
	}

	void CellComponent::MoveToCell( CellComponent* new_cell )
	{
		if ( new_cell == nullptr || !new_cell->HasPawn( Pawn::NONE ) ) {
			assert( false );
			return;
		}

		GameObject* pawn_obj = nullptr;
		if ( owner_ != nullptr ) {
			pawn_obj = owner_->GetChild( "pawn" );
		}

		auto* new_owner = new_cell->owner_;
		if ( new_owner != nullptr && pawn_obj != nullptr ) {
			new_owner->MoveToChildren( owner_, "pawn" );
			new_cell->pawn_ = pawn_;
			pawn_ = Pawn::NONE;
		}
	}
}
