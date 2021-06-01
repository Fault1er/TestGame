#pragma once


#include <Scene/GameObject.hpp>
#include <Game/Components/CellComponent.hpp>


namespace Game
{
	struct AITarget
	{
		sf::Vector2i pawn_pos;
		sf::Vector2i target_pos;

		AITarget( const sf::Vector2i& pawn, const sf::Vector2i& target )
		: pawn_pos( pawn )
		, target_pos( target ) {}
	};

	using Targets = std::vector<sf::Vector2i>;

	class FieldManager : public ObjectComponent
	{
	private:
		std::vector<sf::Vector2i>           player_pawn_pos_,
											player_target_pos_;
		std::vector<AITarget>               ai_targets_;
		std::vector<Game::CellComponent*>   cells_;
		sf::Vector2i                        selected_cell_pos_{};
		Pawn                                player_color_ = Pawn::NONE;
		Pawn                                pawn_color_move_ = Pawn::WHITE;
		bool                                cell_selected_ = false;

	private:
		static bool CheckPos( const sf::Vector2i& pos );

		void ClickOnCell( int x, int y );

		void SwitchMoveChoice();

		void StartGame( Pawn player_color );

		[[nodiscard]]
		bool CheckFinish() const;

		void UpdateAI();

	public:
		explicit FieldManager( GameObject* owner, const rapidjson::Document::GenericValue& node );

		void OnCreate() override;

		void Update( float dt ) override;

		void FinishGame();

		[[nodiscard]]
		bool CanMove( const sf::Vector2i& pos, const sf::Vector2i& new_pos ) const;

		bool MakeMove( Pawn pawn, const sf::Vector2i& pos, const sf::Vector2i& new_pos );
	};
}
