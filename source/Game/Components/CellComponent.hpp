#pragma once


#include <Scene/Components/ObjectComponent.hpp>
#include <SFML/System/Vector2.hpp>


namespace Game
{
	enum class Pawn : uint8_t
	{
		NONE,
		BLACK,
		WHITE
	};


	class CellComponent : public ObjectComponent
	{
	private:
		sf::Vector2i pos_;
		Pawn pawn_ = Pawn::NONE;

	public:
		CellComponent( GameObject* owner, const rapidjson::Document::ValueType& node );

		[[nodiscard]]
		auto GetPos() const -> const sf::Vector2i&;

		void SetPos( const sf::Vector2i& pos );

		[[nodiscard]]
		bool HasPawn( Pawn pawn ) const;

		void MoveToCell( CellComponent* new_cell );
	};
}
